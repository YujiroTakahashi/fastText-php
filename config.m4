dnl $Id$
dnl config.m4 for extension fasttext

PHP_ARG_ENABLE(fasttext, whether to enable fasttext support,
dnl Make sure that the comment is aligned:
[  --enable-fasttext           Enable fasttext support])

if test "$PHP_FASTTEXT" != "no"; then
  PHP_REQUIRE_CXX()

  # --with-fasttext -> check with-path
  SEARCH_PATH="/usr/local /usr"
  SEARCH_FOR="/include/fasttext/fasttext.h"
  if test -r $PHP_FASTTEXT/$SEARCH_FOR; then # path given as parameter
    FASTTEXT_DIR=$PHP_FASTTEXT
  else # search default path list
    AC_MSG_CHECKING([for fasttext files in default path])
    for i in $SEARCH_PATH ; do
      if test -r $i/$SEARCH_FOR; then
        FASTTEXT_DIR=$i
        AC_MSG_RESULT(found in $i)
      fi
    done
  fi

  if test -z "$FASTTEXT_DIR"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please reinstall the fasttext distribution, "$PHP_FASTTEXT"])
  fi

  # --with-fasttext -> add include path
  PHP_ADD_INCLUDE($FASTTEXT_DIR/include/fasttext)

  # --with-fasttext -> check for lib and symbol presence
  LIBNAME="fasttext"
  LIBSYMBOL="FASTTEXT_VERSION"

  PHP_SUBST(FASTTEXT_SHARED_LIBADD)

  PHP_ADD_LIBRARY(stdc++, 1, FASTTEXT_SHARED_LIBADD)
  PHP_ADD_LIBRARY(fasttext, 1, FASTTEXT_SHARED_LIBADD)
  CFLAGS="-O3 -funroll-loops"
  CXXFLAGS="-pthread -std=c++11 -O3 -funroll-loops"

  PHP_NEW_EXTENSION(fasttext, fasttext_api.cc fasttext.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
