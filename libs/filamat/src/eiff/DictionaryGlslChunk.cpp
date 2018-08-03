/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include "DictionaryGlslChunk.h"

namespace filamat {

DictionaryGlslChunk::DictionaryGlslChunk(LineDictionary& dictionary) :
        Chunk(ChunkType::DictionaryGlsl), mDictionary(dictionary){
}

void DictionaryGlslChunk::flatten(Flattener& f) {
    // NumStrings
    f.writeUint32(mDictionary.getLineCount());

    // Strings
    for (size_t i = 0 ; i < mDictionary.getLineCount() ; i++) {
        f.writeString(mDictionary.getString(i).c_str());
    }
}

} // namespace filaflat
