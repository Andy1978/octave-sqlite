// Copyright (C) 2016 Andreas Weber <andy.weber.aw@gmail.com>
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 3 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, see <http://www.gnu.org/licenses/>.

#include "cl_sqlite_handler.h"


DEFINE_OV_TYPEID_FUNCTIONS_AND_DATA(sqlite_handler, "sqlite_handler", "sqlite_handler");

sqlite_handler::sqlite_handler ()
  : octave_base_value()
{
  octave_stdout << "sqlite_handler C'Tor " << endl;
}

sqlite_handler::sqlite_handler (const sqlite_handler& m)
  : octave_base_value()
{
  octave_stdout << "sqlite_handler: the copy constructor shouldn't be called" << std::endl;
}

sqlite_handler::~sqlite_handler ()
{
  octave_stdout << "sqlite_handler D'Tor " << endl;
  if (db)
    close ();
}

void
sqlite_handler::open (string fn, bool create)
{

  cout << "sqlite_handler::open " << fn << " create=" << create << endl;

  int rc = sqlite3_open_v2 (fn.c_str (), &db,
                            SQLITE_OPEN_READWRITE
                            | ((create)? SQLITE_OPEN_CREATE : 0), NULL);
  if( rc )
    error ("Can't open sqlite database '%s':'%s'", fn.c_str (), sqlite3_errmsg (db));
  this->fn = fn;
  sqlite3_exec (db, "PRAGMA foreign_keys = ON;", 0, 0, 0);
}

void
sqlite_handler::close ()
{
  cout << "sqlite_handler::close" << endl;
  sqlite3_close (db);
  db = NULL;
  fn.clear ();
}

void
sqlite_handler::exec_sql (string sql, Cell bind)
{
  cout << "sqlite_handler::exec_sql sql=" << sql << endl;
  Matrix s;
  for (octave_idx_type i = 0; i < bind.numel (); i++)
    {
      //bind(i).print (cout);
      //cout << bind(i).size () << endl;

      // check if all values in bind have the same size
      if (s.is_empty ())
        s = bind(i).size ();
      else if (bind(i).size () != s)
        error ("All bind params have to have the same size");
    }

  int num_values = s.prod()(1);
  cout << "numel per bind param = " << num_values << endl;

  sqlite3_stmt *pStmt;
  sqlite3_exec (db, "BEGIN;", 0, 0, 0);

  int rc = sqlite3_prepare_v2 (db, sql.c_str (), -1, &pStmt, NULL);
  if (rc == SQLITE_OK)
    {
      cout << "sqlite3_prepare_v2 returned SQLITE_OK" << endl;

      // loop over all values per cell
      octave_idx_type val_idx = 0;
      do
        {

          for (octave_idx_type bind_idx = 0; bind_idx < bind.numel (); ++bind_idx)
            {
              octave_value tmp = bind (bind_idx);
              //~ tmp.print (cout);
              //~ cout << "is_integer_type = " << tmp.is_integer_type () << endl;
              //~ cout << "is_real_scalar = " << tmp.is_real_scalar () << endl;
              //~ cout << "is_real_matrix = " << tmp.is_real_matrix () << endl;
              //~ cout << "is_string = " << tmp.is_string () << endl;
              //~ cout << "is_cell = " << tmp.is_cell () << endl;

              //cout << "val_idx=" << val_idx << " bind_idx=" << bind_idx << endl;
              if (tmp.is_real_scalar () || tmp.is_real_matrix ())
                {
                  double v = tmp.matrix_value ()(val_idx);
                  cout << "sqlite3_bind_double " << bind_idx + 1 << " to " << v << endl;
                  rc = sqlite3_bind_double (pStmt, bind_idx + 1, v);
                  if (rc != SQLITE_OK)
                    error ("sqlite_handler::exec_sql sqlite3_bind_double failed: %i = %s", rc, sqlite3_errmsg (db));
                }
              else if(tmp.is_cell ())
                {
                  Cell c = tmp.cell_value ();
                  if (c(val_idx).is_string ())
                    {
                      string s = c(val_idx).string_value ();
                      cout << "sqlite3_bind_text " << bind_idx + 1 << " to '" << s << "'" << endl;
                      rc = sqlite3_bind_text (pStmt, bind_idx + 1, s.c_str (), -1, SQLITE_STATIC);
                      if (rc != SQLITE_OK)
                        error ("sqlite_handler::exec_sql sqlite3_bind_text failed: %i = %s", rc, sqlite3_errmsg (db));
                    }
                  else
                    error ("Nested non string cells are not allowed");
                }
            }

          rc = sqlite3_step (pStmt);
          cout << "sqlite3_step returned " << rc << endl;
          if (rc == SQLITE_BUSY)
            error ("The database is busy... (locked by another app)");
          else if (rc == SQLITE_DONE)
            // sqlite3_step() has finished executing
            cout << "SQLITE_DONE" << endl;
          else if (rc == SQLITE_ROW)
            // sqlite3_step() has another row ready
            cout << "SQLITE_ROW" << endl;
          else
            error ("sqlite_handler::exec_sql sqlite3_step failed: %i = %s", rc, sqlite3_errmsg (db));

          sqlite3_reset (pStmt);

          val_idx++;

        }
      while (val_idx < num_values || (rc == SQLITE_ROW && val_idx<10));

      sqlite3_finalize(pStmt);
    }
  else
    error ("sqlite_handler::exec_sql sqlite3_prepare_v2 failed: %s", sqlite3_errmsg (db));

  sqlite3_exec(db, "COMMIT;", 0, 0, 0);
}

void
sqlite_handler::print (std::ostream& os, bool pr_as_read_syntax = false)
{
  os << "This is class sqlite_handler" << endl;
  os << "  database = '" << fn << "'" << endl;

}

sqlite_handler*
get_sqlite_handler_from_ov (octave_value ov)
{
  static bool type_loaded = false;
  if (!type_loaded)
    {
      sqlite_handler::register_type();
      type_loaded = true;
    }

  if (ov.type_id() != sqlite_handler::static_type_id())
    {
      error("get_sqlite_handler_from_ov: Not a valid sqlite_handler");
      return 0;
    }

  sqlite_handler* sqh = 0;
  const octave_base_value& rep = ov.get_rep();
  sqh = &((sqlite_handler &)rep);
  return sqh;
}
