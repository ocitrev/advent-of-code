#pragma once
#include <vector>

struct Range
{
    int low;
    int high;

    bool FullOverlap(Range other) const
    {
        if (low >= other.low && high <= other.high)
            return true;

        if (other.low >= low && other.high <= high)
            return true;

        return false;
    }

    bool NoOverlap(Range other) const
    {
        if (high < other.low || low > other.high)
            return true;

        if (other.high < low || other.low > high)
            return true;

        return false;
    }

    bool Union(Range other)
    {
        if (NoOverlap(other))
            return false;

        low = std::min(low, other.low);
        high = std::max(high, other.high);
        return true;
    }

    bool Intersection(Range other)
    {
        if (NoOverlap(other))
            return false;

        low = std::max(low, other.low);
        high = std::min(high, other.high);
        return true;
    }

    auto Length() const
    {
        return high - low + 1;
    }
};

inline std::vector<Range> MergeRanges(std::vector<Range> ranges)
{
    while (true)
    {
        auto first = begin(ranges);
        std::vector<Range> result{*first};
        int nbMerge = 0;

        for (auto iter = first + 1; iter != end(ranges); ++iter)
        {
            std::vector<Range> alone;
            bool merged = false;

            for (auto &candidate : result)
            {
                if (candidate.Union(*iter))
                {
                    ++nbMerge;
                    merged = true;
                }
            }

            if (not merged)
            {
                result.push_back(*iter);
            }
        }

        if (nbMerge == 0)
            return result;
        else
            ranges = std::move(result);
    }
}
