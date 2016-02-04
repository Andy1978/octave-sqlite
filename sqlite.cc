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

static bool type_loaded = false;

// PKG_ADD: autoload ("sqlite", which ("sqlite.oct"));
// PKG_DEL: autoload ("sqlite", which ("sqlite.oct"), "remove");
DEFUN_DLD(sqlite, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{sqlite} =} sqlite (@var{filename}, [@var{create}])\n\
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
      sqlite_handler::register_type();
      type_loaded = true;
    }
  string fn = args(0).string_value ();
  bool create = 0;

  if (nargin > 1)
    create = args(1).bool_value ();

  if (! error_state)
    {
      sqlite_handler *h = new sqlite_handler ();
      h->open (fn, create);
      retval.append (octave_value (h));
    }
  return retval;
}
