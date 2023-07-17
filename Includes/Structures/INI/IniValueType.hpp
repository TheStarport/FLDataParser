#pragma once

namespace Ini
{
    enum class IniValueType : unsigned char
    {
        Boolean = 0x00,
        Int32 = 0x01,
        Single = 0x02,
        String = 0x03,
    };
} // namespace Ini
