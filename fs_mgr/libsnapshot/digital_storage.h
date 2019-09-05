// Copyright (C) 2019 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <stdint.h>
#include <stdlib.h>

namespace android {
namespace digital_storage {

template <size_t Power>
struct Size {
    static constexpr size_t power = Power;
    constexpr Size(uint64_t count) : value_(count) {}

    constexpr uint64_t bytes() const { return value_ << (Power * 10); }
    constexpr uint64_t count() const { return value_; }
    operator uint64_t() const { return bytes(); }

  private:
    uint64_t value_;
};

using B = Size<0>;
using KiB = Size<1>;
using MiB = Size<2>;
using GiB = Size<3>;

constexpr B operator""_B(unsigned long long v) {  // NOLINT
    return B{v};
}

constexpr KiB operator""_KiB(unsigned long long v) {  // NOLINT
    return KiB{v};
}

constexpr MiB operator""_MiB(unsigned long long v) {  // NOLINT
    return MiB{v};
}

constexpr GiB operator""_GiB(unsigned long long v) {  // NOLINT
    return GiB{v};
}

template <typename Dest, typename Src>
constexpr Dest size_cast(Src src) {
    if (Src::power < Dest::power) {
        return Dest(src.count() >> ((Dest::power - Src::power) * 10));
    }
    if (Src::power > Dest::power) {
        return Dest(src.count() << ((Src::power - Dest::power) * 10));
    }
    return Dest(src.count());
}

static_assert((1_B).bytes() == 1);
static_assert((1_KiB).bytes() == 1 << 10);
static_assert((1_MiB).bytes() == 1 << 20);
static_assert((1_GiB).bytes() == 1 << 30);
static_assert(size_cast<KiB>(1_B).count() == 0);
static_assert(size_cast<KiB>(1024_B).count() == 1);
static_assert(size_cast<KiB>(1_MiB).count() == 1024);

}  // namespace digital_storage
}  // namespace android
