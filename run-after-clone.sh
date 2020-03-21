git submodule update --init
cd lib/spdlog
mkdir build
cd build
cmake ..
make -j
