#!/bin/bash
if [ "$1" == "" ]; then
	echo "Podaj scieżkę do źrodeł boosta \"./build-boost.sh <sciezka>\""
	exit 1
fi
boostdir="$1"
p2pdir=`pwd`
echo -e "boost dir: \t\t$1"
echo -e "p2p-network dir: \t$p2pdir"

cd $boostdir
boostbuilddir=$p2pdir"/boost-built-1_73_0"
echo -e "building to: \t\t"$boostbuilddir
./bootstrap.sh --prefix=$boostbuilddir
./b2 install
echo "Done!"

