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
    if (ranges.empty())
        return ranges;

    std::sort(ranges.begin(), ranges.end(),
        [](Range const &a, Range const &b)
        {
            return a.low < b.low;
        });

    std::vector<Range> result;
    result.push_back(ranges.front());
    auto first = begin(ranges);

    for (auto iter = std::next(first); iter != end(ranges); ++iter)
    {
        Range &last = result.back();

        if (iter->low <= last.high + 1)
        {
            last.high = std::max(last.high, iter->high);
        }
        else
        {
            result.push_back(*iter);
        }
    }

    return result;
}
