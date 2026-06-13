#ifndef __TREE_VIEW_H__
#define __TREE_VIEW_H__

template <typename IteratorType>
class TreeView {
    IteratorType m_begin;
    IteratorType m_end;
public:
    TreeView(IteratorType b, IteratorType e): m_begin(b), m_end(e) {}

    IteratorType begin() const { return m_begin; }
    IteratorType end()   const { return m_end;   }
};

#endif // __TREE_VIEW_H__