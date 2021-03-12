# 介绍

> 一个加速算法落地的开源框架。

随着人工智能的普及，深度学习算法的越来越规整，一套可以低代码并且快速落地并且有定制化解决方案的框架就是一种趋势。为了缩短算法落地周期，降低算法落地门槛是一个必然的方向。

TengineFactory是有OPEN AI LAB自主研发的一套快速，低代码的算法落地框架。我们致力于打造一个完全开源的易用的算法落地框架，通过配置json文件的方式，可以以最简洁的接口来获取你所需要的结果。此外，你也可以插入你自己特有的代码，完成算法的落地工程。

## 整体特点
### 轻量级
- 唯一依赖库[Tengine-Lite](https://github.com/OAID/Tengine)，Tengine-Lite是框架的核心推理框架。
- x86平台 ： so大小为1.5M左右。

### 易用性
- 针对通用的前处理，后处理框架内均已包含，无须再code。
- 如模型拥有独有的前处理、后处理，插件化的方式，可以很快的新增你的算法。

## 架构设计
![framework](https://openailab.oss-cn-shenzhen.aliyuncs.com/tenginefactory/framework.png)

## 致谢
Tengine Factory参考一下项目：
- [Tengine](https://github.com/OAID/Tengine)
- [TengineKit](https://github.com/OAID/TengineKit)
- [MNN](https://github.com/alibaba/MNN)
- [protobuffer](https://github.com/protocolbuffers/protobuf)

## License 
Apache 2.0
