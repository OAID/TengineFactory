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

#ifndef TFACTORYPROCESS_HPP
#define TFACTORYPROCESS_HPP
#include <iostream>
#include <vector>
#include "TFactoryComponent.hpp"

namespace TFactory {
class TFactoryProcess
{
public:
    /**
     * @brief Create Tengine Factory handler.
     * @return handler.
     */
    static TFactoryProcess* create();

    /**
     * @brief Initialize Tengine Factory.
     * @param jsonPath json config file path.
     * @return None.
     */
    void init(const char* jsonPath);

    /**
     * @brief Run Tengine Factory by read json file sources.
     * @param index image file index, if is video you should not set the index.
     * @return None.
     */
    void run(int index = -1);

    /**
     * @brief Run Tengine Factory by bytes.
     * @param input_data image or video bytes.
     * @param input_data image or video width.
     * @param input_data image or video height.
     * @return None.
     */
    void runWithData(uint8_t* input_data, int width, int height);

    /**
     * @brief Get Tengine Factory Output.
     * @return memory of TFactoryComponent data.
     */
    TFactoryComponent* getComponents();

    /**
     * @brief Release Tengine Factory.
     * @return None.
     */
    void release();

    /**
     * @brief Get image count in folder.
     * @return image count.
     */
    int imageCount();

private:

};
}
#endif