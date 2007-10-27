dnl -------------------------------------------------------- -*- autoconf -*-
dnl Copyright 2006 The Apache Software Foundation or its licensors, as
dnl applicable.
dnl
dnl Licensed under the Apache License, Version 2.0 (the "License");
dnl you may not use this file except in compliance with the License.
dnl You may obtain a copy of the License at
dnl
dnl     http://www.apache.org/licenses/LICENSE-2.0
dnl
dnl Unless required by applicable law or agreed to in writing, software
dnl distributed under the License is distributed on an "AS IS" BASIS,
dnl WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
dnl See the License for the specific language governing permissions and
dnl limitations under the License.

dnl
dnl SSL module
dnl

dnl
dnl AMQ_FIND_SSL: look for ssl libraries and headers
dnl
AC_DEFUN([AMQ_FIND_SSL], [
  amq_have_ssl=0

  AC_ARG_WITH([ssl], [AC_HELP_STRING([--with-ssl], [enable SSL support])],
  [
    if test "$withval" = "yes"; then
      AMQ_CHECK_OPENSSL
      dnl add checks for other varieties of ssl here
    fi
  ], [
      amq_have_ssl=0
  ])

  if test "$amq_have_ssl" = "1"; then
    AC_DEFINE([AMQ_HAVE_SSL], 1, [Define that we have SSL capability])
  fi

])
dnl

AC_DEFUN([AMQ_CHECK_OPENSSL], [
  amq_have_openssl=0
  openssl_have_headers=0
  openssl_have_libs=0

  AC_ARG_WITH([openssl], 
  [AC_HELP_STRING([--with-openssl=DIR], [specify location of OpenSSL])],
  [
    if test "$withval" = "yes"; then
      AC_CHECK_HEADERS(openssl/x509.h, [openssl_have_headers=1])
      AC_CHECK_LIB(crypto, BN_init, AC_CHECK_LIB(ssl, SSL_connect, [openssl_have_libs=1],,-lcrypto))
      if test "$openssl_have_headers" != "0" && test "$openssl_have_libs" != "0"; then
        amq_have_openssl=1
      fi
    elif test "$withval" = "no"; then
      amq_have_openssl=0
    else
      old_cppflags="$CPPFLAGS"
      old_ldflags="$LDFLAGS"

      openssl_CPPFLAGS="-I$withval/include"
      openssl_LDFLAGS="-L$withval/lib "

      AMQ_ADDTO(CPPFLAGS, [$openssl_CPPFLAGS])
      AMQ_ADDTO(LDFLAGS, [$openssl_LDFLAGS])

      AC_MSG_NOTICE(checking for openssl in $withval)
      AC_CHECK_HEADERS(openssl/x509.h, [openssl_have_headers=1])
      AC_CHECK_LIB(crypto, BN_init, AC_CHECK_LIB(ssl, SSL_connect, [openssl_have_libs=1],,-lcrypto))
      if test "$openssl_have_headers" != "0" && test "$openssl_have_libs" != "0"; then
        amq_have_openssl=1
        AMQ_ADDTO(LDFLAGS, [-L$withval/lib])
        AMQ_ADDTO(CXXFLAGS, [-I$withval/include])
        AMQ_ADDTO(CFLAGS, [-I$withval/include])
      fi

      if test "$amq_have_openssl" != "1"; then
        AC_CHECK_HEADERS(openssl/x509.h, [openssl_have_headers=1])
        AC_CHECK_LIB(crypto, BN_init, AC_CHECK_LIB(ssl, SSL_connect, [openssl_have_libs=1],,-lcrypto))
        if test "$openssl_have_headers" != "0" && test "$openssl_have_libs" != "0"; then
          amq_have_openssl=1
          AMQ_ADDTO(LDFLAGS, [-L$withval/lib])
          AMQ_ADDTO(CXXFLAGS, [-I$withval/include])
          AMQ_ADDTO(CFLAGS, [-I$withval/include])
        fi
      fi

      CPPFLAGS="$old_cppflags"
      LDFLAGS="$old_ldflags"
    fi
  ], [
    AC_CHECK_HEADERS(openssl/x509.h, [openssl_have_headers=1])
    AC_CHECK_LIB(crypto, BN_init, AC_CHECK_LIB(ssl, SSL_connect, [openssl_have_libs=1],,-lcrypto))
    if test "$openssl_have_headers" != "0" && test "$openssl_have_libs" != "0"; then
      amq_have_openssl=1
    fi
  ])


  AC_SUBST(amq_have_openssl)

  dnl Add the libraries we will need now that we have set amq_have_openssl correctly
  if test "$amq_have_openssl" = "1"; then
    AC_DEFINE([AMQ_HAVE_OPENSSL], 1, [Define that we have OpenSSL available])
    AMQ_ADDTO(AMQ_LIBS,[-lssl -lcrypto])
    AMQ_ADDTO(LIBS,[-lssl -lcrypto])
    amq_have_ssl=1
  fi
])

dnl
dnl AMQ_ADDTO(variable, value)
dnl
dnl  Add value to variable
dnl
AC_DEFUN([AMQ_ADDTO], [
  if test "x$$1" = "x"; then
    test "x$silent" != "xyes" && echo "  setting $1 to \"$2\""
    $1="$2"
  else
    amq_addto_bugger="$2"
    for i in $amq_addto_bugger; do
      amq_addto_duplicate="0"
      for j in $$1; do
        if test "x$i" = "x$j"; then
          amq_addto_duplicate="1"
          break
        fi
      done
      if test $amq_addto_duplicate = "0"; then
        test "x$silent" != "xyes" && echo "  adding \"$i\" to $1"
        $1="$$1 $i"
      fi
    done
  fi
])dnl

