#!bin/bash

wget http://ftp.gnu.org/gnu/inetutils/inetutils-2.0.tar.xz && \
mv inetutils-2.0.tar.xz /usr/bin && \
cd /usr/bin && \
tar -xvf inetutils-2.0.tar.xz && \
cd inetutils-2.0 || exit 1

./configure --prefix=/usr        \
            --localstatedir=/var \
            --disable-logger     \
            --disable-whois      \
            --disable-rcp        \
            --disable-rexec      \
            --disable-rlogin     \
            --disable-rsh        \
            --disable-servers || exit 1
make && make install && \
mv -v /usr/bin/{hostname,ping,ping6,traceroute} /bin && \
mv -v /usr/bin/ifconfig /sbin || exit 1



