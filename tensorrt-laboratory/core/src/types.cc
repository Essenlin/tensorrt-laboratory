/* Copyright (c) 2018-2019, NVIDIA CORPORATION. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of NVIDIA CORPORATION nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "tensorrt/laboratory/core/types.h"

#include <glog/logging.h>

namespace trtlab {
namespace types {

dtype::dtype(const DLDataType& dlpack) : m_DLPackType(dlpack)
{
    m_Bytes = ((uint64_t)m_DLPackType.bits * (uint64_t)m_DLPackType.lanes) / 8;
}

dtype::dtype(uint8_t code, uint8_t bits, uint16_t lanes) : dtype(DLDataType{code, bits, lanes}) {}

dtype::dtype(): dtype(0, 0, 0) {}

dtype::dtype(dtype&& other) noexcept { *this = std::move(other); }

dtype& dtype::operator=(dtype&& other) noexcept
{
    m_DLPackType = std::exchange(other.m_DLPackType, dtype().to_dlpack());
    m_Bytes = std::exchange(other.m_Bytes, 0);
}

dtype::dtype(const dtype& other) { *this = other; }

dtype& dtype::operator=(const dtype& other)
{
    if(&other == this)
    {
        return *this;
    }
    m_DLPackType = other.m_DLPackType;
    m_Bytes = other.m_Bytes;
}

bool dtype::operator==(const dtype& other) const
{
    if(m_DLPackType.code == other.m_DLPackType.code &&
       m_DLPackType.bits == other.m_DLPackType.bits &&
       m_DLPackType.lanes == other.m_DLPackType.lanes)
    {
        return true;
    }
    return false;
}

int64_t dtype::bytes() const { return m_Bytes; }

const DLDataType& dtype::to_dlpack() const { return m_DLPackType; }

std::ostream& operator<<(std::ostream& os, const dtype& dt)
{
    // clang-format off
    std::string t = "unknown";
    if(dt.m_DLPackType.code == kDLInt) { t = "int"; }
    else if(dt.m_DLPackType.code == kDLUInt) { t = "uint"; }
    else if(dt.m_DLPackType.code == kDLFloat) { t = "fp"; }
    os << "[dtype: " << t << (uint32_t)dt.m_DLPackType.bits
       << " x" << (uint32_t)dt.m_DLPackType.lanes << "]";
    // clang-format on
    return os;
}

} // namespace types
} // namespace trtlab