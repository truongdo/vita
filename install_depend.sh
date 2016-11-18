#! /bin/bash
#
# install.sh
# Copyright (C) 2015 truong-d <truong-d@truongd-XPS13-9333>
#
# Distributed under terms of the MIT license.
#

root=`pwd`

mkdir -p build
cd build

if [[ ! -d liblbfgs-1.10 ]];then
    wget -O - https://github.com/downloads/chokkan/liblbfgs/liblbfgs-1.10.tar.gz | tar xvpfz -
    (cd liblbfgs-1.10 && ./configure --prefix=`pwd`/installed && make -j 4 && make install)
fi

if [[ ! -d crfsuite-0.12 ]];then
    git clone https://github.com/truongdq54/crfsuite.git
    cd crfsuite
    automake
    ./configure --prefix=`pwd`/installed --with-liblbfgs=$root/build/liblbfgs-1.10/installed
    make -j 4 && make install
fi
