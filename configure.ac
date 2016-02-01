#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([Octave-Forge sqlite package], [0.0.1], [andy.weber.aw@gmail.com])

# Checks for programs.
AC_PROG_CXX
AC_LANG(C++)

AC_CHECK_PROG([HAVE_MKOCTFILE], [mkoctfile], [yes], [no])
if [test $HAVE_MKOCTFILE = "no"]; then
  AC_MSG_ERROR([mkoctfile required to install $PACKAGE_NAME])
fi

# Checks for libraries.
AC_CHECK_LIB(sqlite3, sqlite3_open, [], [
  AC_MSG_ERROR([unable to find the sqlite3_open() function])
])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

AC_MSG_NOTICE([

$PACKAGE_NAME is now configured with

   LIBS:   $LIBS
])
