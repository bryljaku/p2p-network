apt-get -y install g++ cmake libboost-test-dev protobuf-compiler libprotobuf-dev
git submodule update --init
cd lib/spdlog
mkdir build
cd build
cmake ..
make -j
