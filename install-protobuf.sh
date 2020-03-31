P_NAME=protobuf-cpp-3.11.4

tar -xvf $P_NAME.tar.gz
sudo apt-get install autoconf automake libtool curl make g++ unzip
cd $P_NAME
./configure
make
make check
sudo make install
sudo ldconfig
cd ..
rm -rf $P_NAME
