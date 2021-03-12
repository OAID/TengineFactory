# Introduction
## Tengine-Factory

> An open source framework that accelerates the implementation of algorithms.

With the popularity of artificial intelligence, deep learning algorithms are becoming more and more regular, a set of frameworks that can be low-code and quickly implemented and have customized solutions is a trend. In order to shorten the algorithm landing cycle, lowering the algorithm landing threshold is an inevitable direction.

TengineFactory is a fast and low-code algorithm landing framework independently developed by OPEN AI LAB. We are committed to creating a completely open source and easy-to-use algorithm landing framework. By configuring the json file, you can get the results you need with the most concise interface. In addition, you can also insert your own unique code to complete the implementation of the algorithm.

## Features
### Lightweight
- The only dependent library [Tengine-Lite](https://github.com/OAID/Tengine), Tengine-Lite is the core reasoning framework of the framework.
- x86 platform: the size of so is about 1.5M.

### Easy to use
- For general pre-processing, post-processing is included in the framework, no need to code.
- If the model has unique pre-processing, post-processing, and plug-in methods, you can quickly add your algorithm.

## Architecture Design
![framework](https://openailab.oss-cn-shenzhen.aliyuncs.com/tenginefactory/framework.png)

## Thanks
TengineFactory refers to the project:
- [Tengine](https://github.com/OAID/Tengine)
- [TengineKit](https://github.com/OAID/TengineKit)
- [MNN](https://github.com/alibaba/MNN)
- [protobuffer](https://github.com/protocolbuffers/protobuf)

## License
Apache 2.0
