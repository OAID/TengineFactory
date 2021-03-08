#ifndef AutoStorage_h
#define AutoStorage_h

#include <stdint.h>
#include <string.h>
#include "MemoryUtils.h"

namespace Tengine {
template <typename T>

/** self-managed memory storage */
class AutoStorage {
public:
    /**
     * @brief default initializer.
     */
    AutoStorage() {
        mSize = 0;
        mData = NULL;
    }
    /**
     * @brief parameter initializer.
     * @param size  number of elements.
     */
    AutoStorage(int size) {
        mData = (T*)MemoryAllocAlign(sizeof(T) * size, Tengine_MEMORY_ALIGN_DEFAULT);
        mSize = size;
    }
    /**
     * @brief deinitializer.
     */
    ~AutoStorage() {
        if (NULL != mData) {
            MemoryFreeAlign(mData);
        }
    }

    /**
     * @brief get number of elements.
     * @return number of elements..
     */
    inline int size() const {
        return mSize;
    }

    /**
     * @brief set data with number of elements.
     * @param data  data pointer create with `MemoryAllocAlign`.
     * @param size  number of elements.
     * @warning do NOT call `free` or `MemoryFreeAlign` for data pointer passes in.
     */
    void set(T* data, int size) {
        if (NULL != mData && mData != data) {
            MemoryFreeAlign(mData);
        }
        mData = data;
        mSize = size;
    }

    /**
     * @brief reset data size.
     * @param size  number of elements.
     * @warning writed data won't be kept.
     */
    void reset(int size) {
        if (NULL != mData) {
            MemoryFreeAlign(mData);
        }
        mData = (T*)MemoryAllocAlign(sizeof(T) * size, Tengine_MEMORY_ALIGN_DEFAULT);
        mSize = size;
    }

    /**
     * @brief release allocated data.
     */
    void release() {
        if (NULL != mData) {
            MemoryFreeAlign(mData);
            mData = NULL;
            mSize = 0;
        }
    }

    /**
     * @brief set allocated memory data to 0.
     */
    void clear() {
        ::memset(mData, 0, mSize * sizeof(T));
    }

    /**
     * @brief get data pointer.
     * @return data pointer.
     */
    T* get() const {
        return mData;
    }

private:
    T* mData  = NULL;
    int mSize = 0;
};
} // namespace 

#endif /* AutoStorage_h */
