/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include <stdio.h>

#include <jawt.h>
#include <win32/jawt_md.h>
#include <windows.h>

#include <utils/unwindows.h>

#include "JAWTUtils.h"
#include <filament/Engine.h>

extern "C" {

void chooseAndSetPixelFormat(HDC dc) {
    PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL  | PFD_DOUBLEBUFFER,
            PFD_TYPE_RGBA,
            32,                   // Colordepth of the framebuffer.
            0, 0, 0, 0, 0, 0,
            0,
            0,
            0,
            0, 0, 0, 0,
            24,                   // Number of bits for the depthbuffer
            0,                    // Number of bits for the stencilbuffer
            0,                    // Number of aux buffers in the framebuffer.
            PFD_MAIN_PLANE,
            0,
            0, 0, 0
    };
    int pixelFormat = ChoosePixelFormat(dc, &pfd);
    SetPixelFormat(dc, pixelFormat, &pfd);
}

void *getNativeWindow(JNIEnv *env, jclass, jobject surface) {
    void *win = nullptr;
    JAWT_DrawingSurface *ds = nullptr;
    JAWT_DrawingSurfaceInfo *dsi = nullptr;
    if (!acquireDrawingSurface(env, surface, &ds, &dsi)) {
        return win;
    }
    JAWT_Win32DrawingSurfaceInfo *dsi_win32 = (JAWT_Win32DrawingSurfaceInfo *) dsi->platformInfo;
    HDC dc = dsi_win32->hdc;
    chooseAndSetPixelFormat(dsi_win32->hdc);
    win = (void *) dsi_win32->hdc;
    releaseDrawingSurface(ds, dsi);
    return win;
}


jlong createNativeSurface(jint width, jint height) {
    // We need to adjust the window size so the "client area" matches width and height requested.
    // Otherwise, the window itself will be of dimension width-height but the "client area" and the
    // resulting surface will be smaller than requested.
    RECT dimension = {0, 0, width, height};
    AdjustWindowRect(&dimension, WS_OVERLAPPEDWINDOW, FALSE);
    width = dimension.right - dimension.left;
    height = dimension.bottom - dimension.top;

    HWND window = CreateWindowA("STATIC", "dummy", 0, 0, 0, width, height, NULL, NULL, NULL, NULL);
    SetWindowLong(window, GWL_STYLE, 0); //remove all window styles
    HDC dc = GetDC(window);
    chooseAndSetPixelFormat(dc);
    return (jlong)dc;
}

void destroyNativeSurface(jlong surface) {
    HDC dc = (HDC)surface;
    HWND window = WindowFromDC(dc);
    ReleaseDC(window, dc);
    DestroyWindow(window);
}

}
