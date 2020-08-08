#!/usr/bin/env bash

SHELL_HOME=`pwd`
CURL_HOME=$SHELL_HOME/deps/curl
LIBCURL_HOME=$CURL_HOME/lib/.libs

# build configuration.
cd `echo $CURL_HOME`
./buildconf

# configure and compile.
# TODO: make more options for different platfm
$CURL_HOME/configure --with-ssl
make

if [ ! -f $LIBCURL_HOME/libcurl.a ]; then
  echo "libcurl build failure."
  exit 1
fi

cd `echo $SHELL_HOME`
for out in $*
do
  current=`echo $LIBCURL_HOME`"/"`echo $(basename $out)`
  cp $current $out
done