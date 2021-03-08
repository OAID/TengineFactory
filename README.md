# Tengine_Factory

## 简介
随着人工智能的普及，深度学习算法的越来越规整，一套可以低代码并且快速落地并且有定制化解决方案的框架就是一种趋势。为了缩短算法落地周期，降低算法落地门槛是一个必然的方向。

Tengine_Factory是有OPEN AI LAB自主研发的一套快速，低代码的算法落地框架。我们致力于打造一个完全开源的易用的落地解决方案，欢迎提出问题或者提交您的代码。

[![Apache 2.0](https://img.shields.io/crates/l/r)](LICENSE)  

## 快速使用 Quick Start
### 资源
- opencv >= 3.4
- cmake >= 3.10
- protobuf >= 3.0
- libtengine-lite.so, 编译放在```libs```下
```
// Build libtengine-lite.so

git clone https://github.com/OAID/Tengine
cd Tengine
mkdir build && cd build
cmake .. && make -j4
cp ./libtengine-lite.so ${Tengine_Factory}/libs
```

### 功能
Tengine_Factory是通过配置json文件的方式，来配置算法参数和实现的算法功能的。
- [Json参数列表](https://github.com/OAID/TFactory/wiki/Json) : 提供Json文件参数列表，[示例](test/jsonFile/config.json)Json文件
- [API文档](https://github.com/OAID/TFactory/wiki/API) : 快速使用Tengine_Factory，功能API文档
### 使用
```bash
./build.sh
```
使用资源：
> ```TFactory.so```  : Tengine_Factory库   
```include/TFactoryProcess.hpp``` : Tengine_Factory头文件

## 贡献
你如果可以提出问题建议或者提交代码用以下方法：
- Github issues
- Email: [Support@openailab.com](Support@openailab.com)
- QQ Group: 787519516 (TengineFactory)
- 提交PR
    - Fork it!
    - 创建你的分支: git checkout -b my-new-feature
    - 提交你的修改: git add . && git commit -m 'Add some feature'
    - 推送这个分支: git push origin my-new-feature
    - 提交请求
- Thanks a lot!!

## 鸣谢
Tengine_Factory参考项目：
- [Tengine](https://github.com/OAID/Tengine)
- [TengineKit](https://github.com/OAID/TengineKit)
- [MNN](https://github.com/alibaba/MNN)

## License
[Apache License 2.0](https://img.shields.io/crates/l/r)