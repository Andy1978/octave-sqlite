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

#ifndef _CL_SQLITE3_HANDLER_
#define _CL_SQLITE3_HANDLER_

#include <octave/oct.h>
#include <sqlite3.h>
#include <iostream>

using namespace std;

class sqlite3_handler: public octave_base_value
{
public:

  sqlite3_handler ();

  octave_base_value *clone (void) const
  {
    octave_stdout << "sqlite3_handler clone" << endl;
    return new sqlite3_handler (*this);
  }

  octave_base_value *empty_clone (void) const
  {
    octave_stdout << "sqlite3_handler empty_clone" << endl;
    return new sqlite3_handler ();
  }

  ~sqlite3_handler (void);

  void print (std::ostream& os, bool pr_as_read_syntax);  //!< print itself on ostream
  void open (string fn, bool create);
  void close ();
  // FIMXE: return cell
  void exec_sql (string sql, Cell bind);
  void testme ();

private:
  sqlite3_handler (const sqlite3_handler& m);

  sqlite3 *db;
  string fn;

  bool is_constant (void) const
  {
    return true;
  }
  bool is_defined (void) const
  {
    return true;
  }

  DECLARE_OV_TYPEID_FUNCTIONS_AND_DATA
};

sqlite3_handler* get_sqlite3_handler_from_ov (octave_value ov);

#endif
