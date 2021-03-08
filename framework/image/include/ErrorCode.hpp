#ifndef ErrorCode_h
#define ErrorCode_h

namespace Tengine {
enum ErrorCode {
#ifdef NO_ERROR
#undef NO_ERROR
#endif //NO_ERROR
    NO_ERROR           = 0,
    OUT_OF_MEMORY      = 1,
    NOT_SUPPORT        = 2,
    COMPUTE_SIZE_ERROR = 3,
    NO_EXECUTION       = 4,

    // User error
    INPUT_DATA_ERROR = 10,
    CALL_BACK_STOP   = 11,

    // Op Resize Error
    TENSOR_NOT_SUPPORT = 20,
    TENSOR_NEED_DIVIDE = 21,
};
}

#endif /* ErrorCode_h */
