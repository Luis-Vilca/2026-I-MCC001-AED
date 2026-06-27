#ifndef __BASE_TRAIT_H__
#define __BASE_TRAIT_H__
#include <functional> // less, greater

using namespace std;

template <typename _T, typename Comparator>
struct ContainerTrait{
    using value_type = _T;
    using Comp       = Comparator;
};

template <typename T>
using AscendingContainerTrait = ContainerTrait<T, less<T>>;

template <typename T>
using DescendingContainerTrait = ContainerTrait<T, greater<T>>;

template <typename T, typename NodeType, typename Comparator = less<T>>
struct NodeContainerTrait{
    using value_type = T;
    using Node       = NodeType;
    using Comp       = Comparator;
};

template <typename T, typename NodeType>
using AscendingNodeContainerTrait = NodeContainerTrait<T, NodeType, less<T>>;

template <typename T, typename NodeType>
using DescendingNodeContainerTrait = NodeContainerTrait<T, NodeType, greater<T>>;

#endif // __BASE_TRAIT_H__