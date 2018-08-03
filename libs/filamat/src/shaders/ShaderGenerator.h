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

#ifndef TNT_FILAMENT_DETAILS_SHADERGENERATOR_H
#define TNT_FILAMENT_DETAILS_SHADERGENERATOR_H

#include <algorithm>

#include <filament/EngineEnums.h>
#include <filament/MaterialEnums.h>

#include <filamat/MaterialBuilder.h>

#include "MaterialInfo.h"

#include <utils/CString.h>

namespace filamat {

class CodeGenerator;

class ShaderGenerator {
public:
    ShaderGenerator(
            MaterialBuilder::PropertyList const& properties,
            MaterialBuilder::VariableList const& variables,
            utils::CString const& materialCode,
            size_t lineOffset,
            utils::CString const& materialVertexCode,
            size_t vertexLineOffset) noexcept;

    const std::string createVertexProgram(filament::driver::ShaderModel sm,
            MaterialBuilder::TargetApi ta, MaterialInfo const& material, uint8_t variantKey,
            filament::Interpolation interpolation,
            filament::VertexDomain vertexDomain) const noexcept;
    const std::string createFragmentProgram(filament::driver::ShaderModel sm,
            MaterialBuilder::TargetApi ta, MaterialInfo const& material, uint8_t variantKey,
            filament::Interpolation interpolation) const noexcept;
    bool hasCustomDepthShader() const noexcept;

private:
    MaterialBuilder::PropertyList mProperties;
    MaterialBuilder::VariableList mVariables;
    utils::CString mMaterialCode;
    utils::CString mMaterialVertexCode;
    size_t mMaterialLineOffset;
    size_t mMaterialVertexLineOffset;
};

struct ShaderPostProcessGenerator {
    static const std::string createPostProcessVertexProgram(filament::driver::ShaderModel sm,
            MaterialBuilder::TargetApi ta, filament::PostProcessStage variant,
            uint8_t firstSampler) noexcept;
    static const std::string createPostProcessFragmentProgram(filament::driver::ShaderModel sm,
            MaterialBuilder::TargetApi ta, filament::PostProcessStage variant,
            uint8_t firstSampler) noexcept;
    static void generatePostProcessStageDefines(std::stringstream& vs, CodeGenerator const& cg,
            filament::PostProcessStage variant) noexcept;
};

} // namespace filament

#endif // TNT_FILAMENT_DETAILS_SHADERGENERATOR_H
