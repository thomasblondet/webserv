#!/bin/bash

PROJECT_DIR=`pwd`
echo $PROJECT_DIR

################################################################################
# == 1. Install build dependencies ==
################################################################################


################################################################################
# == 2. Download the source ==
################################################################################

# https://nginx.org/en/download.html
# Stable version is nginx-1.28.0
rm -rf nginx-1.28.0.tar.gz
wget https://nginx.org/download/nginx-1.28.0.tar.gz
tar -xzf nginx-1.28.0.tar.gz
cd nginx-1.28.0

# Mainline version is nginx-1.29.3
# rm -rf nginx-1.29.3.tar.gz
# wget https://nginx.org/download/nginx-1.29.3.tar.gz
# tar -xzf nginx-1.29.3.tar.gz
# cd nginx-1.29.3

################################################################################
# == 3. Configure the build ==
################################################################################

# https://nginx.org/en/docs/configure.html

# ./configure \
#   --prefix=$PROJECT_DIR/nginx-test \
#   --sbin-path=$PROJECT_DIR/nginx-test/sbin/nginx \
#   --conf-path=$PROJECT_DIR/nginx-test/conf/nginx.conf \
#   --pid-path=$PROJECT_DIR/nginx-test/logs/nginx.pid \
#   --with-http_ssl_module \
#   --with-http_v2_module \
#   --with-http_v3_module \
#   --with-cc-opt='-g -O2'

./configure \
  --prefix=$PROJECT_DIR/nginx-test \
  --sbin-path=$PROJECT_DIR/nginx-test/sbin/nginx \
  --conf-path=$PROJECT_DIR/nginx-test/conf/nginx.conf \
  --pid-path=$PROJECT_DIR/nginx-test/logs/nginx.pid \
  --with-cc-opt='-g -O2'

################################################################################
# == 4. Build and install ==
################################################################################
make -j
make install

################################################################################
# == 5. Add user $USER; in the .conf file ==
################################################################################
LINE="user $USER; # added by bash script"
sed -i "3i $LINE" $PROJECT_DIR/nginx-test/conf/nginx.conf
sed -i "3i $LINE" $PROJECT_DIR/nginx-test/conf/nginx.conf.default

################################################################################
# == IMPORTANT ==
################################################################################
RED="\033[0;31m"
RESET="\033[0m"
echo -e $RED
echo -e "[WARNING]"
echo -e "In your .conf file"
echo -e "add:"
echo -en $RESET
echo -e "\tuser $USER;"
echo -en $RED
echo -e "or replace:"
echo -en $RESET
echo -e "\tuser  nobody;"
echo -en $RED
echo -e "by"
echo -en $RESET
echo -e "\tuser $USER;"
