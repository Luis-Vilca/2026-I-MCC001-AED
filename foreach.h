#ifndef __FOREACH_H__
#define __FOREACH_H__
#include <iostream>
#include <mutex>
#include <utility> // forward

using namespace std;

template <typename Iterator, typename Func, typename... Args>
void ForEach(Iterator begin, Iterator end, Func func, Args&&... args)
{
    Iterate(begin, end, forward<Func>(func), forward<Args>(args)...);
}

template <typename Container, typename Func, typename... Args>
void ForEach(const Container& container, Func func, Args&&... args)
{
    scoped_lock<mutex> lock(container.getMutex());
    ForEach(container.begin(), container.end(), forward<Func>(func), forward<Args>(args)...);
}

template <typename Iterator, typename Func, typename... Args>
Iterator FirstThat(Iterator begin, Iterator end, Func func, Args&&... args)
{
    return Iterate(begin, end, forward<Func>(func), forward<Args>(args)...);
}


template <typename Container, typename Func, typename... Args>
auto FirstThat(const Container& container, Func func, Args&&... args)
{
    scoped_lock<mutex> lock(container.getMutex());

    return FirstThat(container.begin(), container.end(), forward<Func>(func), forward<Args>(args)...);
}

template <typename Iterator, typename Func, typename... Args>
decltype(auto) Iterate(Iterator begin, Iterator end, Func func, Args&&... args)
{
    using result_type = invoke_result_t<Func, decltype(*begin), Args&...>;

    for (auto it = begin; it != end; ++it)
    {
        if constexpr (is_void_v<result_type>)
            invoke(func, *it, args...);
        else
        {
            if (invoke(func, *it, args...))
                return it;
        }
    }

    if constexpr (!is_void_v<result_type>)
        return end;
}
#endif // __FOREACH_H__