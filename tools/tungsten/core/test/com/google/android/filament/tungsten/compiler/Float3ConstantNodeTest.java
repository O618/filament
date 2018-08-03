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

package com.google.android.filament.tungsten.compiler;

import static org.mockito.Mockito.verify;

import org.junit.Before;
import org.junit.Test;

public class Float3ConstantNodeTest {

    private Float3ConstantNode mConstantNode;
    private GraphCompiler mGraphCompiler;

    @Before
    public void setUp() {
        mConstantNode = new Float3ConstantNode();
        mGraphCompiler = TestUtils.createMockGraphCompiler("unused", "constant1");
    }

    @Test
    public void testConstantFloat3() {
        mConstantNode.setFloat3(1.0f, 2.0f, 3.0f);
        mConstantNode.compile(mGraphCompiler);
        String expected = "float3 constant1 = float3(1.0, 2.0, 3.0);\n";
        verify(mGraphCompiler).addCodeToMaterialFunctionBody(expected);
    }
}
