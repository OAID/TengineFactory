# 执行算法

```c++
    /**
     * @brief Create Tengine Factory handler.
     * @return handler.
     */
    static TFactoryProcess* create();
```
!> 必须先进行create操作，api都是基于这个handler的。建议只创建一次多次使用。

## 初始化
```c++
    /**
     * @brief Initialize Tengine Factory.
     * @param jsonPath json config file path.
     * @return None.
     */
    void init(const char* jsonPath);
```

## 运行
有两种方式进行输入的传入：
- 通过json配置，配置文件路径或者视频流。参数为第几张图片。      

```c++
    /**
     * @brief Run Tengine Factory by read json file sources.
     * @param index image file index, if is video you should not set the index.
     * @return None.
     */
    void run(int index = -1);
```

- 通过传入输入参数的方式。      

```c++
    /**
     * @brief Run Tengine Factory by bytes.
     * @param input_data image or video bytes.
     * @param input_data image or video width.
     * @param input_data image or video height.
     * @return None.
     */
    void runWithData(uint8_t* input_data, int width, int height);
```

## 获取输出
```c++
    /**
     * @brief Get Tengine Factory Output.
     * @return memory of TFactoryComponent data.
     */
    TFactoryComponent* getComponents();
```
> 对于每个图像的输入，都会输出一个`TFactoryComponent`。

通过下面的方式获取输入图像的宽、高、通道、以及数据。
```c++
    TFactory::TFactoryComponent *com = interProcess->getComponents();
    int image_w = com->width();
    int image_h = com->height();
    int channel = com->channel();
    uint8_t *input_data = com->buffer();
```

### 获取每个算法的输出：
有两种方式：
- 传入算法的名称，也就是json文件里面配置的。        

```c++
    /**
     * @brief get function component by functionName ,function name is configured in the json file.
     * @param functionName function name.
     * @return function output.
     */
    FunctionComponent* componentOutput(std::string functionName);
```

- 获取所有的算法输出。

```c++
    /**
     * @brief get function components.
     * @return function outputs.
     */
    std::vector<FunctionComponent*> getComponentsOutput();
```

> 输出为`FunctionComponent`为一个数据结构。


```c++
    struct FunctionComponent
    {
        // function Name
        std::string functionName;
        // how many outputs in function
        int function_output_count;
        // outputs buffer
        std::vector<float*> output_buffers;
    };
```

## 释放
```c++
    /**
     * @brief Release Tengine Factory.
     * @return None.
     */
    void release();
```
!> 记得释放，否则会造成内存泄漏。

## 获取图像数量
```c++
    /**
     * @brief Get image count in folder.
     * @return image count.
     */
    int imageCount();
```