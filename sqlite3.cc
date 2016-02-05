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

#include "cl_sqlite3_handler.h"

static bool type_loaded = false;

// PKG_ADD: autoload ("sqlite3", which ("sqlite3.oct"));
// PKG_DEL: autoload ("sqlite3", which ("sqlite3.oct"), "remove");
DEFUN_DLD(sqlite3, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{sqlite} =} sqlite3 (@var{filename}, [@var{create}])\n\
Open database @var{filename}.\n\
@seealso{foobar}\n\
@end deftypefn")
{
  octave_value_list retval;
  int nargin = args.length ();

  if (nargin < 1 || nargin > 2)
    print_usage();

  if (!type_loaded)
    {
      sqlite3_handler::register_type();
      type_loaded = true;
    }
  string fn = args(0).string_value ();
  bool create = 0;

  if (nargin > 1)
    create = args(1).bool_value ();

  if (! error_state)
    {
      sqlite3_handler *h = new sqlite3_handler ();
      h->open (fn, create);
      retval.append (octave_value (h));
    }
  return retval;
}

// PKG_ADD: autoload ("exec_sql", which ("sqlite3.oct"));
// PKG_DEL: autoload ("exec_sql", which ("sqlite3.oct"), "remove");
DEFUN_DLD(exec_sql, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{result} =} exec_sql (@var{sqlite}, @var{sql}, [@var{bind}])\n\
Execute @var{SQL} on @var{SQLITE} object.\n\
\n\
@var{bind} is a Mx1 cell array with parameters which are bind to the\n\
prepared statement. It has to contain as may rows as the @var{sql} command\n\
has parameters and the size of all parameters have to be equal.\n\
@seealso{foobar}\n\
@end deftypefn")
{
  octave_value_list retval;
  int nargin = args.length ();

  if (nargin < 2 || nargin > 3)
    print_usage();

  sqlite3_handler* h = get_sqlite3_handler_from_ov (args(0));
  if (h)
    {
      string sql = args(1).string_value ();
      Cell bind;

      if (nargin > 2)
        bind = args(2).cell_value ();

      h->exec_sql (sql, bind);
    }

  return retval;
}
