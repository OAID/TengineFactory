rm -rf ./build
mkdir build
cd build

build_file="FaceDemo"

cmake ../ \
-DBUILD_FILE=$build_file \
-DCMAKE_BUILD_TYPE=Release

make -j4