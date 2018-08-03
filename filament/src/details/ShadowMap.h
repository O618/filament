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

#ifndef TNT_FILAMENT_DETAILS_SHADOWMAP_H
#define TNT_FILAMENT_DETAILS_SHADOWMAP_H

#include "components/LightManager.h"

#include "details/Camera.h"
#include "details/Scene.h"

#include "driver/DriverApiForward.h"
#include "driver/SamplerBuffer.h"

#include <filament/Viewport.h>

#include <utils/Slice.h>

#include <math/mat4.h>
#include <math/vec4.h>

namespace filament {
namespace details {

class FCamera;

class ShadowMap {
public:
    explicit ShadowMap(FEngine& engine) noexcept;
    ~ShadowMap();

    void prepare(driver::DriverApi& driver, SamplerBuffer& buffer) noexcept;

    void terminate(driver::DriverApi& driverApi) noexcept;

    void setVisibleLayers(uint8_t layers) noexcept { mVisibleLayers = layers; }

    // Whether we need to re-render the shadow map.
    bool needsRefresh() const noexcept { return true; }

    // Set-up the render target, call before rendering the shadow map.
    void beginRenderPass(driver::DriverApi& driverApi) const noexcept;

    // Call once per frame if the light, scene (or visible layers) or camera changes.
    // This computes the light camera.
    void update(const FScene::LightSoa& lightData,
            size_t index, FScene const* scene, details::CameraInfo const& camera) noexcept;

    // Do we have visible shadows. Valid after calling update().
    bool hasVisibleShadows() const noexcept { return mHasVisibleShadows; }

    // Returns the light's projection. Valid after calling update().
    FCamera const& getCamera() const noexcept { return *mShadows.camera; }

    // Returns the shadow map's viewport. Valid after init().
    Viewport const& getViewport() const noexcept { return mShadows.viewport; }

    // Computes the transform to use in the shader to access the shadow map.
    // Valid after calling update().
    math::mat4f const& getLightSpaceMatrix() const noexcept {
        return mShadows.lightSpace;
    }

    // return the size of a texel in world space (pre-warping)
    float getTexelSizeWorldSpace() const noexcept { return mShadows.texelSizeWs; }

    // Returns the shadow map's depth range. Valid after init().
    float getSceneRange() const noexcept { return mShadows.sceneRange; }

    FCamera const& getDebugCamera() const noexcept { return *mShadows.debugCamera; }

private:
    struct CameraInfo {
        math::mat4f projection;
        math::mat4f model;
        math::mat4f view;
        math::mat4f worldOrigin;
        float zn = 0;
        float zf = 0;
        float dzn = 0;
        float dzf = 0;
        Frustum frustum;
        float getNear() const noexcept { return zn; }
        float getFar() const noexcept { return zf; }
        math::float3 const& getPosition() const noexcept { return model[3].xyz; }
        math::float3 getForwardVector() const noexcept {
            return -normalize(model[2].xyz);   // the camera looks towards -z
        }
    };

    struct Segment {
        uint8_t v0, v1;
    };

    struct Quad {
        uint8_t v0, v1, v2, v3;
    };

    // 8 corners, 12 segments w/ 2 intersection max -- all of this twice (8 + 12 * 2) * 2 (768 bytes)
    using FrustumBoxIntersection = std::array<math::float3, 64>;

    void computeShadowCameraDirectional(math::float3 const& direction,
            FScene const* scene, CameraInfo const& camera) noexcept;

    static math::mat4f applyLISPSM(
            CameraInfo const& camera, float dzn, float dzf, const math::mat4f& LMpMv,
            Aabb const& wsShadowReceiversVolume, const math::float3 wsViewFrustumCorners[8],
            const math::float3& dir);

    static inline void snapLightFrustum(math::float2& s, math::float2& o,
            uint32_t shadowMapDimension) noexcept;

    static inline void computeFrustumCorners(math::float3* out,
            const math::mat4f& projectionViewInverse) noexcept;

    static inline math::float2 computeNearFar(math::mat4f const& lightView,
            Aabb const& wsShadowCastersVolume) noexcept;

    static inline void intersectWithShadowCasters(Aabb& lightFrustum, const math::mat4f& lightView,
            Aabb const& wsShadowCastersVolume) noexcept;

    static inline math::float2 computeWpNearFarOfWarpSpace(math::mat4f const& lightView,
            math::float3 const wsViewFrustumCorners[8]) noexcept;

    static inline bool intersectSegmentWithPlane(math::float3& p,
            math::float3 s0, math::float3 s1,
            math::float3 pn, math::float3 p0) noexcept;

    static inline bool intersectSegmentWithPlanarQuad(math::float3& p,
            math::float3 s0, math::float3 s1,
            math::float3 t0, math::float3 t1,
            math::float3 t2, math::float3 t3) noexcept;

    static size_t intersectFrustums(math::float3* out, size_t vertexCount,
            math::float3 const* segmentsVertices, math::float3 const* quadsVertices) noexcept;

    static size_t intersectFrustumWithBox(
            FrustumBoxIntersection& outVertices,
            const Frustum& frustum,
            const math::float3* wsFrustumCorners,
            Aabb const& wsBox);

    static math::mat4f warpFrustum(float n, float f) noexcept;

    math::mat4f getTextureCoordsMapping() const noexcept;

    float texelSizeWorldSpace(const math::mat4f& lightSpaceMatrix) const noexcept;
    float texelSizeWorldSpace(const math::mat4f& lightSpaceMatrix, math::float3 const& str) const noexcept;

    static constexpr const Segment sBoxSegments[12] = {
            { 0, 1 }, { 1, 3 }, { 3, 2 }, { 2, 0 },
            { 4, 5 }, { 5, 7 }, { 7, 6 }, { 6, 4 },
            { 0, 4 }, { 1, 5 }, { 3, 7 }, { 2, 6 },
    };
    static constexpr const Quad sBoxQuads[6] = {
            { 2, 0, 1, 3 },  // far
            { 6, 4, 5, 7 },  // near
            { 2, 0, 4, 6 },  // left
            { 3, 1, 5, 7 },  // right
            { 0, 4, 5, 1 },  // bottom
            { 2, 6, 7, 3 },  // top
    };

    struct Shadowing {
        FCamera* camera = nullptr;
        FCamera* debugCamera = nullptr;
        math::mat4f lightSpace;
        float sceneRange = 0.0f;
        float texelSizeWs = 0.0f;

        // set-up in prepare()
        Viewport viewport;
        Handle<HwTexture> shadowMapHandle;
        Handle<HwRenderTarget> shadowMapRenderTarget;
    };

    Shadowing mShadows;

    // set-up in update()
    uint32_t mShadowMapDimension = 0;
    bool mHasVisibleShadows = false;
    uint8_t mVisibleLayers = 0x1; // used in update()

    // use a member here (instead of stack) because we don't want to pay the
    // initialization of the float3 each time
    FrustumBoxIntersection mWsClippedShadowReceiverVolume;

    FEngine& mEngine;
    const bool mClipSpaceFlipped;
};

} // namespace details
} // namespace filament

#endif // TNT_FILAMENT_DETAILS_SHADOWMAP_H
