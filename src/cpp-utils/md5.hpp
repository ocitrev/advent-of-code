#pragma once

#include "md5.h"

#include <array>
#include <string>
#include <string_view>
#include <vector>

class MD5
{
    MD5_CTX ctx{};

public:
    MD5()
    {
        MD5_Init(&ctx);
    }

    void Update(std::string_view text)
    {
        MD5_Update(&ctx, text.data(), static_cast<unsigned long>(text.size()));
    }

    void Update(std::vector<std::byte> const &bytes)
    {
        MD5_Update(&ctx, bytes.data(), static_cast<unsigned long>(bytes.size()));
    }

    [[nodiscard]] std::string Final()
    {
        std::array<unsigned char, 16> hashBytes{};
        MD5_Final(hashBytes.data(), &ctx);
        constexpr std::string_view kDigits{"0123456789abcdef"};

        std::string hashText(hashBytes.size() * 2, '\0');
        std::size_t offset = 0;

        for (unsigned char b : hashBytes)
        {
            hashText[offset++] = kDigits[static_cast<std::size_t>(b >> 4)];
            hashText[offset++] = kDigits[static_cast<std::size_t>(b & 0xf)];
        }

        return hashText;
    }
};
