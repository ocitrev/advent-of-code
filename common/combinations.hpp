#pragma once

template <typename ContainerT, typename CallableT>
static void ForEachCombinations(ContainerT &&cont, CallableT &&callable)
{
    auto first = begin(cont);
    auto last = end(cont);

    for (auto iterOuter = first; iterOuter != last; ++iterOuter)
    {
        for (auto iterInner = std::next(iterOuter); iterInner != last; ++iterInner)
        {
            callable(*iterOuter, *iterInner);
        }
    }
}
