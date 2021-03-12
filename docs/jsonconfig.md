# Configuration

## Parameter table
| Parameter name | Parameter type | Parameter function | Notes |
| :---: | :---: | :---: | :---: |
| Function | StringArray | Algorithm name |-|
| Thread | Integer | Number of threads | <=4 |
| Sources | String | Resource Directory | Can fill in a single picture, folder. If it is a video stream, leave it blank |
| pipelineMode | String | Algorithm mode |-|
| ModelPath | String | Model Path |-|
| mean | FloatArray | mean |-|
| normal | FloatArray | Variance |-|
| min_sizes | FloatArrayArray | Minimum size | You can leave it blank, if you don’t fill it, you need to configure base_sizes, ratios parameters |
| scales | FloatArrayArray | Scale factor |-|
| base_sizes | FloatArrayArray | base size |-|
| ratios | FloatArray | anchor ratio |-|
| clip | Boolean | Whether to clip or not |-|
| input_w | Integer | Model input width |-|
| input_h | Integer | Model input high |-|
| input_type | String | Model input type |-|
| strides | FloatArray | Span |-|
| score_threshold | Float | Score threshold |-|
| iou_threshold | Float | IOU threshold |-|
| variance | FloatArray | Variance | Used when generating the detection frame |
| preprocess | String | Pre-processing mode |-|
| postprocess | String | Post-processing model |-|
| TensorOuputString | StringArray | Algorithm output string | "Meaning": "String name" |
| input_stream | StringArray | Take the result of an algorithm | "Algorithm name": "The meaning of the stream" |
| output_stream | StringArray | Take only part of the output | "Meaning": "Part taken" |

## Sample configuration
```json
{
    "Function": ["FaceDetect", "FaceLandmark"],
    "Thread": 2,
    "Sources": "./images/",
    "FaceDetect": {
        "pipelineMode": "Standard",
        "ModelPath": "",
        "mean": [127.0, 127.0, 127.0],
        "normal": [0.0078125, 0.0078125, 0.0078125],
        "min_sizes": [
            [10.0, 16.0, 24.0],
            [32.0, 48.0],
            [64.0, 96.0],
            [128.0, 192.0, 256.0]],
        "scales": [
            [8.0, 4.0, 2.0, 1.0],
            [8.0, 4.0, 2.0, 1.0],
            [32.0, 16.0, 8.0, 4.0]],
        "base_sizes": [[16], [16], [16]],
        "ratios": [2.5],
        "clip": false,
        "input_w": 160,
        "input_h": 120,
        "input_type": "RGB",
        "strides": [8.0, 16.0, 32.0, 64.0],
        "score_threshold": 0.6,
        "iou_threshold": 0.3,
        "variance": [0.1, 0.2],
        "preprocess": "CreateAnchor",
        "postprocess": "NMS",
        "TensorOuputString": ["scores:score", "boxes:box"]
    },
    "FaceLandmark": {
        "ModelPath": "./model/landmark.tmfile",
        "pipelineMode": "MutiInput",
        "mean": [127.0, 127.0, 127.0],
        "normal": [0.007874, 0.007874, 0.007874],
        "input_w": 160,
        "input_h": 160,
        "input_type": "RGB",
        "input_stream": ["FaceDetect:rect"],
        "output_stream": ["points:212"]
    }
}
```