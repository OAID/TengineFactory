# 配置

## 参数表格
| 参数名 | 参数类型 | 参数功能 | 注意事项 |
| :---: | :---: | :---: | :---: |
| Function | StringArray | 算法名称  | - | 
| Thread | Integer | 线程数 | <=4 | 
| Sources | String | 资源目录 | 可填单个图片，文件夹。如果为视频流则不填 | 
| pipelineMode | String | 算法模式 | - | 
| ModelPath | String | 模型路径 | - | 
| mean | FloatArray | 均值 | - | 
| normal | FloatArray | 方差 | - | 
| min_sizes | FloatArrayArray | 最小尺寸 | 可不填，不填则需要配置base_sizes，ratios参数 | 
| scales | FloatArrayArray | 缩放系数  | - | 
| base_sizes | FloatArrayArray | 基础尺寸 | - | 
| ratios | FloatArray | anchor比率 | - | 
| clip | Boolean | 是否裁剪 | - | 
| input_w | Integer | 模型输入宽 | - | 
| input_h | Integer | 模型输入高 | - | 
| input_type | String | 模型输入类型 | - | 
| strides | FloatArray | 跨度 | - | 
| score_threshold | Float | 分数阈值 | - | 
| iou_threshold | Float | IOU阈值 | - | 
| variance | FloatArray | 方差 | 生成检测框时使用 | 
| preprocess | String | 前处理模式 | - | 
| postprocess | String | 后处理模型  | - | 
| TensorOuputString | StringArray | 算法输出字符串 | “含义”:“字符串名” | 
| input_stream | StringArray | 取某算法的结果作为输入流 | “算法名”:“流的含义” | 
| output_stream | StringArray | 只取输出的一部分 | “含义”:“取的部分” | 

## 示例配置
```json
{
    "Function" : ["FaceDetect", "FaceLandmark"],
    "Thread" : 2,
    "Sources" : "./images/",
    "FaceDetect" : {
        "pipelineMode" : "Standard",
        "ModelPath" : "",
        "mean" : [127.0, 127.0, 127.0],
        "normal" : [0.0078125, 0.0078125, 0.0078125],
        "min_sizes" : [
            [10.0, 16.0, 24.0],
            [32.0, 48.0],
            [64.0, 96.0],
            [128.0, 192.0, 256.0]],
        "scales" : [
            [8.0, 4.0, 2.0, 1.0],
            [8.0, 4.0, 2.0, 1.0],
            [32.0, 16.0, 8.0, 4.0]],
        "base_sizes" : [[16], [16], [16]],
        "ratios" : [2.5], 
        "clip" : false,
        "input_w" : 160,
        "input_h" : 120,
        "input_type" : "RGB",
        "strides" : [8.0, 16.0, 32.0, 64.0],
        "score_threshold" : 0.6,
        "iou_threshold" : 0.3,
        "variance" : [0.1, 0.2],
        "preprocess" : "CreateAnchor",
        "postprocess" : "NMS",
        "TensorOuputString" : ["scores:score", "boxes:box"]
    },
    "FaceLandmark" : {
        "ModelPath" : "./model/landmark.tmfile",
        "pipelineMode" : "MutiInput",
        "mean" : [127.0, 127.0, 127.0],
        "normal" : [0.007874, 0.007874, 0.007874],
        "input_w" : 160,
        "input_h" : 160,
        "input_type" : "RGB",
        "input_stream" : ["FaceDetect:rect"],
        "output_stream" : ["points:212"]
    }
}
```