#ifndef __BTREE_ITERATOR_H__
#define __BTREE_ITERATOR_H__

#include <vector>
#include "general_iterator.h"

enum class BTreeIteratorDirection { Forward, Backward };

template <typename Container,BTreeIteratorDirection Direction>
class BTreeIterator
    : public general_iterator<Container, BTreeIterator<Container, Direction>>
{
    using MySelf = BTreeIterator<Container, Direction>;
    using Parent = general_iterator<Container, MySelf>;

protected:
    using Node   = typename Container::Node;
    using BTPage = typename Container::BTNode;

    struct State
    {
        BTPage* page;
        size_t index;
    };

    vector<State> m_path;

protected:
    void descend(BTPage* page);
    void setCurrent();

public:
    BTreeIterator(Container* container, BTPage* root)
        : Parent(container, nullptr)
    {
        descend(root);
        setCurrent();
    }

    MySelf& operator++()
    {
        next();
        return *this;
    }

private:
    void next();
};

template <typename Container,BTreeIteratorDirection Direction>
void BTreeIterator<Container, Direction>::descend(BTPage* page)
{
    while (page)
    {
        if constexpr (
            Direction == BTreeIteratorDirection::Forward)
        {
            m_path.push_back({page, 0});
            page = page->getSubPage(0);
        }
        else
        {
            size_t index = page->getKeyCount();
            m_path.push_back({page, index});
            page = page->getSubPage(index);
        }
    }
}

template <typename Container,BTreeIteratorDirection Direction>
void BTreeIterator<Container, Direction>::setCurrent()
{
    if (m_path.empty())
    {
        this->m_pNode = nullptr;
        return;
    }

    auto& state = m_path.back();

    if constexpr (Direction == BTreeIteratorDirection::Forward)
        this->m_pNode = state.page->getNode(state.index);
    else
        this->m_pNode = state.page->getNode(state.index - 1);
}

template <typename Container,BTreeIteratorDirection Direction>
void BTreeIterator<Container, Direction>::next()
{
    if (m_path.empty())
        return;

    auto& state = m_path.back();
    size_t childIndex;

    if constexpr (Direction == BTreeIteratorDirection::Forward)
        childIndex = state.index + 1;
    else
        childIndex = state.index - 1;

    BTPage* nextPage = state.page->getSubPage(childIndex);

    if constexpr (Direction == BTreeIteratorDirection::Forward)
        ++state.index;
    else
        --state.index;

    if (nextPage)
        descend(nextPage);
    else
    {
        while (!m_path.empty())
        {
            auto& current = m_path.back();
            bool finished;

            if constexpr (Direction == BTreeIteratorDirection::Forward)
                finished = current.index >=current.page->getKeyCount();
            else
                finished = current.index == 0;
            if (!finished)
                break;
            m_path.pop_back();
        }
    }

    setCurrent();
}
#endif //__BTREE_ITERATOR_H__
