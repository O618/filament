/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef TNT_FILAMENT_DRIVER_UNIFORMINTERFACEBLOCK_H
#define TNT_FILAMENT_DRIVER_UNIFORMINTERFACEBLOCK_H

#include <filament/driver/DriverEnums.h>

#include <utils/CString.h>
#include <utils/compiler.h>

#include <math/vec4.h>

#include <assert.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace filament {

class UniformInterfaceBlock {
public:
    UniformInterfaceBlock();
    UniformInterfaceBlock(const UniformInterfaceBlock& rhs);
    UniformInterfaceBlock(UniformInterfaceBlock&& rhs) noexcept;
    UniformInterfaceBlock& operator=(const UniformInterfaceBlock& rhs);
    UniformInterfaceBlock& operator=(UniformInterfaceBlock&& rhs) noexcept;
    ~UniformInterfaceBlock() noexcept;

    using Type = driver::UniformType;
    using Precision = driver::Precision;

    class Builder {
    public:
        // Give a name to this uniform interface block
        Builder& name(const std::string& interfaceBlockName);
        // Add a uniform
        Builder& add(const std::string& uniformName, size_t size,
                Type type, Precision precision = Precision::DEFAULT);
        // build and return the UniformInterfaceBlock
        UniformInterfaceBlock build();
    private:
        friend class UniformInterfaceBlock;
        struct Entry {
            Entry(utils::CString name, uint32_t size, Type type, Precision precision) noexcept
                    : name(std::move(name)), size(size), type(type), precision(precision) { }
            utils::CString name;
            uint32_t size;
            Type type;
            Precision precision;
        };
        utils::CString mName;
        std::vector<Entry> mEntries;
    };

    struct UniformInfo {
        utils::CString name;// name of this uniform
        uint16_t offset;    // offset in "uint32_t" of this uniform in the buffer
        uint8_t stride;     // stride in "uint32_t" to the next element
        Type type;          // type of this uniform
        uint32_t size;      // size of the array in elements, or 1 if not an array
        Precision precision;// precision of this uniform
        // returns offset in bytes of this uniform (at index if an array)
        inline size_t getBufferOffset(size_t index = 0) const {
            assert(index < size);
            return (offset + stride * index) * sizeof(uint32_t);
        }
    private:
        friend std::ostream& operator << (std::ostream& out, const UniformInfo& info);
    };

    // name of this uniform interface block
    const utils::CString& getName() const noexcept { return mName; }

    // size in bytes needed to store the uniforms described by this interface block in a UniformBuffer
    size_t getSize() const noexcept { return mSize; }

    // list of information records for each uniform
    std::vector<UniformInfo> const& getUniformInfoList() const noexcept { return mUniformsInfoList; }

    // negative value if name doesn't exist or Panic if exceptions are enabled
    ssize_t getUniformOffset(const char* name, size_t index) const;

    bool hasUniform(const char* name) const noexcept {
        return mInfoMap.find(name) != mInfoMap.end();
    }

    bool isEmpty() const noexcept { return mUniformsInfoList.empty(); }

private:
    friend class Builder;

    explicit UniformInterfaceBlock(Builder& builder) noexcept;

    static uint8_t baseAlignmentForType(Type type) noexcept;
    static uint8_t strideForType(Type type) noexcept;

    utils::CString mName;
    std::vector<UniformInfo> mUniformsInfoList;
    std::unordered_map<const char*, uint32_t, utils::hashCStrings, utils::equalCStrings> mInfoMap;
    uint32_t mSize = 0; // size in bytes
};

} // namespace filament

#endif // TNT_FILAMENT_DRIVER_UNIFORMINTERFACEBLOCK_H
