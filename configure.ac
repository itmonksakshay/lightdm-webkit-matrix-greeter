AC_INIT([lightdm-webkit-matrix-greeter],[1.0.0],[akshay.bhatt@hotmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
PKG_CHECK_MODULES([GTK], [gtk+-3.0])
PKG_CHECK_MODULES([GMODULE], [gmodule-export-2.0])
PKG_CHECK_MODULES([LIGHTDM], [liblightdm-gobject-1 >= 1.19.2])
PKG_CHECK_MODULES([WEBKIT], [webkitgtk-3.0])
AC_PROG_CC
AM_PROG_CC_C_O
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile data/Makefile themes/Makefile])
AC_OUTPUT
