#include "day04.hpp"
#include "../common/assert.hpp"
#include "../common/string.hpp"
#include <fmt/format.h>
#include <regex>

using namespace std::string_view_literals;

bool IsPassportValid1(std::string_view passport)
{
    static constexpr std::array required{
        "byr"sv, // (Birth Year)
        "iyr"sv, // (Issue Year)
        "eyr"sv, // (Expiration Year)
        "hgt"sv, // (Height)
        "hcl"sv, // (Hair Color)
        "ecl"sv, // (Eye Color)
        "pid"sv, // (Passport ID)
    };

    return std::all_of(begin(required), end(required), [&passport](std::string_view field) {
        return passport.find(field) != std::string_view::npos;
    });
}

bool IsPassportValid2(std::string_view passport)
{
    if (not IsPassportValid1(passport))
        return false;

    for (auto &&v : SplitFirstOf(passport, " \t\n"sv))
    {
        auto parts = Split(v, ':');

        if (parts[0] == "byr"sv) // Birth Year
        {
            int const value = svtoi(parts[1]);
            if (value < 1920 || value > 2002)
                return false;
        }
        else if (parts[0] == "iyr"sv) // Issue Year
        {
            int const value = svtoi(parts[1]);
            if (value < 2010 || value > 2020)
                return false;
        }
        else if (parts[0] == "eyr"sv) // Expiration Year
        {
            int const value = svtoi(parts[1]);
            if (value < 2020 || value > 2030)
                return false;
        }
        else if (parts[0] == "hgt"sv) // Height
        {
            int const value = svtoi(parts[1]);

            if (ends_with(parts[1], "cm"sv))
            {
                if (value < 150 || value > 193)
                    return false;
            }
            else if (ends_with(parts[1], "in"sv))
            {
                if (value < 59 || value > 76)
                    return false;
            }
            else
            {
                return false;
            }
        }
        else if (parts[0] == "hcl"sv) // Hair Color
        {
            std::regex re("#[a-f0-9]{6}");
            if (not std::regex_match(begin(parts[1]), end(parts[1]), re))
                return false;
        }
        else if (parts[0] == "ecl"sv) // Eye Color
        {
            static constexpr std::array eyeColors{"amb"sv, "blu"sv, "brn"sv, "gry"sv, "grn"sv, "hzl"sv, "oth"sv};

            if (std::find(begin(eyeColors), end(eyeColors), parts[1]) == end(eyeColors))
                return false;
        }
        else if (parts[0] == "pid"sv) // Passport ID
        {
            std::regex re("\\d{9}");
            if (not std::regex_match(begin(parts[1]), end(parts[1]), re))
                return false;
        }
    }

    return true;
}

int Part1()
{
    auto passports = Split(input::batchFile, "\n\n");
    return static_cast<int>(std::count_if(begin(passports), end(passports), &IsPassportValid1));
}

int Part2()
{
    auto passports = Split(input::batchFile, "\n\n");
    return static_cast<int>(std::count_if(begin(passports), end(passports), &IsPassportValid2));
}

int main()
{
    // https://adventofcode.com/2020/day/4
    fmt::print("Day 4: Passport Processing\n"sv);

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(204 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
}
