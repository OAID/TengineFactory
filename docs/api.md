# Execution algorithm

```c++
    /**
     * @brief Create Tengine Factory handler.
     * @return handler.
     */
    static TFactoryProcess* create();
```
!> The create operation must be performed first, and the api is based on this handler. We recommend you only create it once and use it multiple times.

## Initialization
```c++
    /**
     * @brief Initialize Tengine Factory.
     * @param jsonPath json config file path.
     * @return None.
     */
    void init(const char* jsonPath);
```

## Run
There are two ways to input input:
- Through json configuration, configuration file path or video stream. The parameter is the first picture.

```c++
    /**
     * @brief Run Tengine Factory by read json file sources.
     * @param index image file index, if is video you should not set the index.
     * @return None.
     */
    void run(int index = -1);
```

- By sending input data.

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

## Get outputs
```c++
    /**
     * @brief Get Tengine Factory Output.
     * @return memory of TFactoryComponent data.
     */
    TFactoryComponent* getComponents();
```
> For each image input, a `TFactoryComponent` will be output.

Obtain the width, height, channel, and data of the input image in the following way.
```c++
    TFactory::TFactoryComponent *com = interProcess->getComponents();
    int image_w = com->width();
    int image_h = com->height();
    int channel = com->channel();
    uint8_t *input_data = com->buffer();
```

### Get the outputs of each algorithm:
There are two ways:
- The name of the incoming algorithm, which is configured in the json file.

```c++
    /**
     * @brief get function component by functionName ,function name is configured in the json file.
     * @param functionName function name.
     * @return function output.
     */
    FunctionComponent* componentOutput(std::string functionName);
```

- Get all the algorithm outputs.

```c++
    /**
     * @brief get function components.
     * @return function outputs.
     */
    std::vector<FunctionComponent*> getComponentsOutput();
```

> The output is `FunctionComponent` as a data structure.


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

## Release
```c++
    /**
     * @brief Release Tengine Factory.
     * @return None.
     */
    void release();
```
!> Remember to release, otherwise it will cause a memory leak.

## Images number
```c++
    /**
     * @brief Get image number in folder.
     * @return image number.
     */
    int imageCount();
```