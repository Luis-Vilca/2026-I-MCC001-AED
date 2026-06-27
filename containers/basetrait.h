#ifndef __BASE_TRAIT_H__
#define __BASE_TRAIT_H__
#include <functional> // less, greater

using namespace std;

template <typename T, typename Ref = void, typename Comparator = less<T>>
struct ContainerTrait{
    using value_type = T;
    using ref_type   = Ref;
    using Comp       = Comparator;
};

template <typename T>
using AscendingContainerTrait = ContainerTrait<T>;

template <typename T>
using DescendingContainerTrait = ContainerTrait<T, void , greater<T>>;

template <typename T, typename Ref>
using AscendingMapContainerTrait = ContainerTrait<T, Ref>;

template <typename T, typename Ref>
using DescendingMapContainerTrait = ContainerTrait<T, Ref , greater<T>>;


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