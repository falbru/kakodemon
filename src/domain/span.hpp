#ifndef SPAN_HPP_INCLUDED
#define SPAN_HPP_INCLUDED

#include <cstddef>
#include <vector>

namespace domain
{

template <typename T> struct Span
{
    Span(T value, size_t start_index) : value(value), start_index(start_index)
    {
    }

    T value;
    size_t start_index;

    bool operator<(const Span<T> &other) const
    {
        return start_index < other.start_index;
    }
};

template <typename T>
typename std::vector<Span<T>>::const_iterator spanIteratorFromIndex(const std::vector<Span<T>> &spans, int index)
{
    if (spans.empty())
    {
        return spans.end();
    }

    if (index < static_cast<int>(spans.front().start_index))
    {
        return spans.end();
    }

    if (index >= static_cast<int>(spans.back().start_index))
    {
        return spans.end() - 1;
    }

    size_t left = 0;
    size_t right = spans.size() - 1;

    while (left < right)
    {
        size_t mid = (left + right) / 2;

        if (spans[mid].start_index <= static_cast<size_t>(index))
        {
            if (mid + 1 < spans.size() && spans[mid + 1].start_index <= static_cast<size_t>(index))
            {
                left = mid + 1;
            }
            else
            {
                return spans.begin() + mid;
            }
        }
        else
        {
            right = mid - 1;
        }
    }

    if (spans[left].start_index <= static_cast<size_t>(index))
    {
        return spans.begin() + left;
    }

    return spans.end();
}
} // namespace domain

#endif
