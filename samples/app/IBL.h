/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef TNT_FILAMENT_SAMPLE_IBL_H
#define TNT_FILAMENT_SAMPLE_IBL_H

#include <math/vec3.h>

namespace filament {
class Engine;
class IndexBuffer;
class IndirectLight;
class Material;
class MaterialInstance;
class Renderable;
class Texture;
class Skybox;
}

namespace utils {
    class Path;
}

class IBL {
public:
    IBL(filament::Engine& engine);
    ~IBL();

    bool loadFromDirectory(const utils::Path& path);

    filament::IndirectLight* getIndirectLight() const noexcept {
        return mIndirectLight;
    }

    filament::Skybox* getSkybox() const noexcept {
        return mSkybox;
    }

private:
    bool loadCubemapLevel(filament::Texture** texture, const utils::Path& path,
            size_t level = 0, std::string const& levelPrefix = "") const;

    filament::Engine& mEngine;

    math::float3 mBands[9] = {};

    filament::Texture* mTexture = nullptr;
    filament::IndirectLight* mIndirectLight = nullptr;
    filament::Texture* mSkyboxTexture = nullptr;
    filament::Skybox* mSkybox = nullptr;
};

#endif // TNT_FILAMENT_SAMPLE_IBL_H
