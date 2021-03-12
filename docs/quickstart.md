# Quick Start

## Environmental requirements
- opencv >= 3.4
- cmake >= 3.10
- protobuf >= 3.0
- gcc >= 4.9
- ndk >= 15c

?> The ndk environment is only needed if you compile `Android` related applications.

## Compile Tengine-Lite
### Download Tengine-Lite source code
```bash
git clone https://github.com/OAID/Tengine.git
```

### Compile
```bash
cd Tengine
mkdir build && cd build
cmake .. && make -j4
make install
```
After the compilation is complete, the build/install/lib directory will generate a `libtengine-lite.so` file, as shown below:
```Text
install
├── bin
│ ├── tm_benchmark
│ ├── tm_classification
│ └── tm_mobilenet_ssd
├── include
│ └── tengine_c_api.h
└── lib
    └── libtengine-lite.so
```

### Compile Androidv7/v8 version
Download ndk, [http://developer.android.com/ndk/downloads/index.html](http://developer.android.com/ndk/downloads/index.html)
(Optional) Delete the debug parameter to reduce the binary size. The file android.toolchain.cmake can be found from $ANDROID_NDK/build/cmake
```bash
# vim $ANDROID_NDK/build/cmake/android.toolchain.cmake
# Delete the "-g" line
list(APPEND ANDROID_COMPILER_FLAGS
  -g
  -DANDROID
```

Configuration Environment:
```bash
export $ANDROID_NDK=<NDK-path>
```

Android v7 compilation instructions:
```bash
mkdir build-android-armv7
cd build-android-armv7
cmake -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake -DANDROID_ABI="armeabi-v7a" -DANDROID_ARM_NEON=ON -DANDROID_PLATFORM=android-19 ..
make
make install
```

Android v8 compilation instructions:
```bash
mkdir build-android-aarch64
cd build-android-aarch64
cmake -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build-android-aarch64/cmake/android.toolchain.cmake -DANDROID_ABI="arm64-v8a" -DANDROID_PLATFORM=android-21 ..
make
make install
```

!> After successful compilation, copy libtengine-lite.so to `<Tengine-Factory-path>/libs`

## Compile Tengine-Factory
### Download Tengine-Factory source code
```bash
git clone https://github.com/OAID/TengineFactory.git
```

Compile Tengine-Factory:
```bash
./build.sh
```
You need the file directory structure as follows:
```Text
TengineFactory
├── build
│ └── libTFactory.so
└── include
    └── TFactoryComponent.hpp
    └── TFactoryProcess.hpp
```
> Introduce `libTFactory.so`, and include `TFactoryComponent.hpp`&`TFactoryProcess.hpp`.