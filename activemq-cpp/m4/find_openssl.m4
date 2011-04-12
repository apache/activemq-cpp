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
dnl -------------------------------------------------------- -*- autoconf -*-
dnl
dnl DECAF_CONFIGURE_SSL: look for OpenSSL libraries and headers
dnl
AC_DEFUN([DECAF_CONFIGURE_SSL], [
    decaf_have_ssl=0

    AC_MSG_NOTICE([Checking if OpenSSL is supported on this platform...])

    dnl We let the user define a single dir where they think OpenSSL is and we only look there
    dnl but if they don't define one we search in some of the more common location.
    AC_ARG_WITH([openssl],
                AS_HELP_STRING([--with-openssl=PREFIX], [root of the OpenSSL installation directory]),
    [
        AC_MSG_NOTICE([Checking user specified OpenSSL installation directory])
        DECAF_SEARCHFOR_OPENSSL([$withval])
    ],
    [
        AC_MSG_NOTICE([Checking for OpenSSL installation directory in common locations])

        dnl Use pkg_config if its installed and there's a .pc for OpenSSL
        dnl This is our prefered scenario.
        AC_PATH_PROG(PKG_CONFIG, pkg-config)
        if test x"$PKG_CONFIG" != x""; then
            OPENSSL_LDFLAGS=`$PKG_CONFIG openssl --libs-only-L 2>/dev/null`
            if test $? = 0; then
                OPENSSL_LIBS=`$PKG_CONFIG openssl --libs-only-l 2>/dev/null`
                OPENSSL_INCLUDES=`$PKG_CONFIG openssl --cflags-only-I 2>/dev/null`
            fi

            dnl Found it ensure no more tests are run.
            decaf_have_ssl="1"
        fi

        dnl No pkg_config or no .pc file so we look in some common locations.
        if test "$decaf_have_ssl" = "0"; then
            ssldirs="/usr/local/ssl /usr/lib/ssl /usr/ssl /usr/pkg /usr/local /usr"
            DECAF_SEARCHFOR_OPENSSL([/usr/local/ssl /usr/lib/ssl /usr/ssl /usr/pkg /usr/local /usr /opt /opt/local])
        fi
    ])

    dnl its not an error if we can't find it, so we just let the user know to be nice.
    if test "$decaf_have_ssl" = "1"; then
        AC_MSG_NOTICE([OpenSSL library found, SSL is enabled in this build])
        AC_DEFINE([HAVE_OPENSSL], 1, [Define that we have SSL capability])
    else
        AC_MSG_NOTICE([OpenSSL library not found, SSL is disabled in this build])
    fi

    dnl Make the configuration available to our main script.
    AC_SUBST([OPENSSL_LIBS])
    AC_SUBST([OPENSSL_LDFLAGS])
    AC_SUBST([OPENSSL_INCLUDES])
])

dnl DECAF_SEARCHFOR_OPENSSL
dnl
dnl Given a path(s) look for and try to compile and link against the OpenSSL lib.
dnl
AC_DEFUN([DECAF_SEARCHFOR_OPENSSL], [

    dnl try and find the headers, then we can make some assumptions on the libs
    for currentDir in $1; do
        AC_MSG_CHECKING([for openssl/ssl.h in $currentDir/include])
        if test -f "$currentDir/include/openssl/ssl.h"; then
            OPENSSL_INCLUDES="-I$currentDir/include"
            OPENSSL_LDFLAGS="-L$currentDir/lib"
            OPENSSL_LIBS="-lssl -lcrypto"
            decaf_have_ssl="1"
            AC_MSG_RESULT([yes])
            break
        else
            AC_MSG_RESULT([no])
        fi
    done

    dnl Now we can try a compile of something simple and see if it compiles then
    dnl we can try linking to the libs, if all goes well then we are good to go.
    if test "$decaf_have_ssl" = "1"; then
        AC_MSG_CHECKING([if compiling and linking against discovered OpenSSL succeeds])

        original_LIBS="$LIBS"
        original_LDFLAGS="$LDFLAGS"
        original_CPPFLAGS="$CPPFLAGS"

        LDFLAGS="$LDFLAGS $OPENSSL_LDFLAGS"
        LIBS="$OPENSSL_LIBS $LIBS"
        CPPFLAGS="$OPENSSL_INCLUDES $CPPFLAGS"

        AC_LINK_IFELSE(
            AC_LANG_PROGRAM([#include <openssl/ssl.h>], [SSL_new(NULL)]),
            [
                AC_MSG_RESULT([yes])
                decaf_have_ssl="1"
            ], [
                AC_MSG_RESULT([no])
                decaf_have_ssl="0"
            ]
        )

        CPPFLAGS="$original_CPPFLAGS"
        LDFLAGS="$original_LDFLAGS"
        LIBS="$original_LIBS"
    fi
])
