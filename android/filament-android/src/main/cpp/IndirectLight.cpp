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

#include <jni.h>

#include <filament/IndirectLight.h>

using namespace filament;

extern "C" JNIEXPORT jlong JNICALL
Java_com_google_android_filament_IndirectLight_nCreateBuilder(JNIEnv*, jclass) {
    return (jlong) new IndirectLight::Builder();
}

extern "C" JNIEXPORT void JNICALL
Java_com_google_android_filament_IndirectLight_nDestroyBuilder(JNIEnv*, jclass,
        jlong nativeBuilder) {
    IndirectLight::Builder* builder = (IndirectLight::Builder*) nativeBuilder;
    delete builder;
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_google_android_filament_IndirectLight_nBuilderBuild(JNIEnv*, jclass,
        jlong nativeBuilder, jlong nativeEngine) {
    IndirectLight::Builder* builder = (IndirectLight::Builder*) nativeBuilder;
    Engine *engine = (Engine *) nativeEngine;
    return (jlong) builder->build(*engine);
}

extern "C" JNIEXPORT void JNICALL
Java_com_google_android_filament_IndirectLight_nBuilderReflections(JNIEnv*, jclass,
        jlong nativeBuilder, jlong nativeTexture) {
    IndirectLight::Builder* builder = (IndirectLight::Builder*) nativeBuilder;
    const Texture *texture = (const Texture *) nativeTexture;
    builder->reflections(texture);
}

extern "C" JNIEXPORT void JNICALL
Java_com_google_android_filament_IndirectLight_nIrradiance(JNIEnv* env, jclass,
        jlong nativeBuilder, jint bands, jfloatArray sh_) {
    IndirectLight::Builder* builder = (IndirectLight::Builder*) nativeBuilder;
    jfloat* sh = env->GetFloatArrayElements(sh_, NULL);
    builder->irradiance((uint8_t) bands, (const math::float3*) sh);
    env->ReleaseFloatArrayElements(sh_, sh, JNI_ABORT);
}

extern "C" JNIEXPORT void JNICALL
Java_com_google_android_filament_IndirectLight_nIrradianceAsTexture(JNIEnv*, jclass,
        jlong nativeBuilder, jlong nativeTexture) {
    IndirectLight::Builder* builder = (IndirectLight::Builder*) nativeBuilder;
    const Texture* texture = (const Texture*) nativeTexture;
    builder->irradiance(texture);
}

extern "C" JNIEXPORT void JNICALL
Java_com_google_android_filament_IndirectLight_nIntensity(JNIEnv*, jclass,
        jlong nativeBuilder, jfloat envIntensity) {
    IndirectLight::Builder* builder = (IndirectLight::Builder*) nativeBuilder;
    builder->intensity(envIntensity);
}

extern "C" JNIEXPORT void JNICALL
Java_com_google_android_filament_IndirectLight_nRotation(JNIEnv *, jclass, jlong nativeBuilder,
        jfloat v0, jfloat v1, jfloat v2, jfloat v3, jfloat v4, jfloat v5, jfloat v6, jfloat v7,
        jfloat v8) {
    IndirectLight::Builder *builder = (IndirectLight::Builder *) nativeBuilder;
    builder->rotation(math::mat3f{v0, v1, v2, v3, v4, v5, v6, v7, v8});
}



extern "C" JNIEXPORT void JNICALL
Java_com_google_android_filament_IndirectLight_nSetIntensity(JNIEnv*, jclass,
        jlong nativeIndirectLight, jfloat intensity) {
    IndirectLight* indirectLight = (IndirectLight*) nativeIndirectLight;
    indirectLight->setIntensity(intensity);
}

extern "C" JNIEXPORT jfloat JNICALL
Java_com_google_android_filament_IndirectLight_nGetIntensity(JNIEnv*, jclass,
        jlong nativeIndirectLight) {
    IndirectLight* indirectLight = (IndirectLight*) nativeIndirectLight;
    return indirectLight->getIntensity();
}

extern "C" JNIEXPORT void JNICALL
Java_com_google_android_filament_IndirectLight_nSetRotation(JNIEnv *env, jclass type,
        jlong nativeIndirectLight, jfloat v0, jfloat v1, jfloat v2, jfloat v3, jfloat v4, jfloat v5,
        jfloat v6, jfloat v7, jfloat v8) {
    IndirectLight *indirectLight = (IndirectLight *) nativeIndirectLight;
    indirectLight->setRotation(math::mat3f{v0, v1, v2, v3, v4, v5, v6, v7, v8});
}
