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

#include "driver/noop/NoopDriver.h"
#include "driver/CommandStream.h"

namespace filament {


std::unique_ptr<Driver> NoopDriver::create() {
    return std::unique_ptr<Driver>(new NoopDriver());
}

NoopDriver::NoopDriver() noexcept
        : DriverBase(new ConcreteDispatcher<NoopDriver>(this)) {
}

NoopDriver::~NoopDriver() noexcept = default;

// explicit instantiation of the Dispatcher
template class ConcreteDispatcher<NoopDriver>;

} // namespace filament
