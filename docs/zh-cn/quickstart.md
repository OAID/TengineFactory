# 快速开始

## 环境要求
- opencv >= 3.4
- cmake >= 3.10
- protobuf >= 3.0
- gcc >= 4.9
- ndk >= 15c

?> 如果你编译`Android`相关应用，才需要ndk环境。

## 编译Tengine-Lite
### 下载Tengine-Lite源码
```bash
git clone https://github.com/OAID/Tengine.git
```

### 编译
```bash
cd Tengine
mkdir build && cd build
cmake .. && make -j4
make install
```
编译完成后build/install/lib目录会生成`libtengine-lite.so`文件，如下所示：
```Text
install
├── bin
│   ├── tm_benchmark
│   ├── tm_classification
│   └── tm_mobilenet_ssd
├── include
│   └── tengine_c_api.h
└── lib
    └── libtengine-lite.so
```

### 编译Androidv7/v8版本
下载ndk，[http://developer.android.com/ndk/downloads/index.html](http://developer.android.com/ndk/downloads/index.html)     
（可选）删除debug参数，缩小二进制体积。android.toolchain.cmake 这个文件可以从 $ANDROID_NDK/build/cmake 找到
```bash
# vim $ANDROID_NDK/build/cmake/android.toolchain.cmake
# 删除 "-g" 这行
list(APPEND ANDROID_COMPILER_FLAGS
  -g
  -DANDROID
```

配置环境：
```bash
export $ANDROID_NDK=<NDK-path>
```

Android v7编译指令：
```bash
mkdir build-android-armv7
cd build-android-armv7
cmake -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake -DANDROID_ABI="armeabi-v7a" -DANDROID_ARM_NEON=ON -DANDROID_PLATFORM=android-19 ..
make
make install
```

Android v8编译指令：
```bash
mkdir build-android-aarch64
cd build-android-aarch64
cmake -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build-android-aarch64/cmake/android.toolchain.cmake -DANDROID_ABI="arm64-v8a" -DANDROID_PLATFORM=android-21 ..
make
make install
```

!> 编译成功后，把libtengine-lite.so 拷贝到`<Tengine-Factory-path>/libs`下

## 编译Tengine-Factory
### 下载Tengine-Factory源码
```bash
git clone https://github.com/OAID/TengineFactory.git
```

编译Tengine-Factory：
```bash
./build.sh
```
你需要文件目录结构如下如：
```Text
TengineFactory
├── build
│   └── libTFactory.so
└── include
    └── TFactoryComponent.hpp
    └── TFactoryProcess.hpp
```
> 引入`libTFactory.so`，并且include `TFactoryComponent.hpp`&`TFactoryProcess.hpp`即可。

