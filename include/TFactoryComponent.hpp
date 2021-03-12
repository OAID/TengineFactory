/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * License); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * AS IS BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/*
 * Copyright (c) 2021, OPEN AI LAB
 * Author: zhangjun@openailab.com
 */

#ifndef TFACTORYCOMPONENT_HPP
#define TFACTORYCOMPONENT_HPP
#include <iostream>
#include <string>
#include <vector>
namespace TFactory {
class TFactoryComponent
{
public:
    struct ImageComponent
    {
        uint8_t* data;
        int width;
        int height;
        int channel;
    };

    struct FunctionComponent
    {
        // function Name
        std::string functionName;
        // how many outputs in function
        int function_output_count;
        // outputs buffer
        std::vector<float*> output_buffers;
    };

    inline ImageComponent* image() const {
        return mImageComponent;
    }

    inline int width() const {
        return mImageComponent->width;
    }

    inline int height() const {
        return mImageComponent->height;
    }

    inline int channel() const {
        return mImageComponent->channel;
    }

    inline uint8_t* buffer() const {
        return mImageComponent->data;
    }

    /**
     * @brief get function component by functionName ,function name is configured in the json file.
     * @param functionName function name.
     * @return function output.
     */
    inline FunctionComponent* componentOutput(std::string functionName) const {
        for (std::size_t i = 0; i < mFunctionComs.size(); i++)
        {
            if (functionName.compare(mFunctionComs[i]->functionName) == 0)
            {
                return mFunctionComs[i];
            }
        }
        return nullptr;
    }

    /**
     * @brief get function components.
     * @return function outputs.
     */
    inline std::vector<FunctionComponent*> getComponentsOutput() const {
        return mFunctionComs;
    }

    void setImageComponent(uint8_t* data, int w, int h, int channel);
    void setFunctionComponent(struct FunctionComponent* com);
    void clearCached();
    TFactoryComponent();
    ~TFactoryComponent();
private:
    std::vector<struct FunctionComponent*> mFunctionComs;
    struct ImageComponent* mImageComponent; 
};
}
#endif
