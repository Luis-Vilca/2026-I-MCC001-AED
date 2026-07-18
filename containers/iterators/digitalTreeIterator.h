#ifndef __DIGITALTREE_ITERATOR_H__
#define __DIGITALTREE_ITERATOR_H__

#include <vector>
#include "general_iterator.h"

enum class DigitalTreeIteratorDirection{Forward, Backward};

template<typename Container, DigitalTreeIteratorDirection Direction>
class DigitalTreeIterator :
    public general_iterator<Container,DigitalTreeIterator<Container,Direction>>
{
    using MySelf = DigitalTreeIterator<Container,Direction>;
    using Parent = general_iterator<Container,MySelf>;

protected:

    using Node = typename Container::Node;

    std::vector<Node*> m_stack;

public:

    DigitalTreeIterator(Container* pContainer = nullptr,Node* root = nullptr)
        : Parent(pContainer,nullptr)
    {
        if(root)
            m_stack.push_back(root);
        advance();
    }

    MySelf& operator++()
    {
        advance();
        return *this;
    }

protected:

    void advance()
    {
        this->m_pNode = nullptr;

        while(!m_stack.empty())
        {
            Node* node = m_stack.back();
            m_stack.pop_back();

            auto& children = node->Children();

            if constexpr(Direction == DigitalTreeIteratorDirection::Forward)
            {
                // Se insertan al revés para visitar el primero primero.
                for(auto it = children.rbegin();
                    it != children.rend();
                    ++it)
                {
                    m_stack.push_back(*it);
                }
            }
            else
            {
                for(auto child : children)
                    m_stack.push_back(child);
            }

            if(node->IsTerminal())
            {
                this->m_pNode = node;
                return;
            }
        }
    }
};

#endif