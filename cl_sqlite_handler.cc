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
