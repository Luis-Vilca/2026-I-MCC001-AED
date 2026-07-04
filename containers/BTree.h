// btree.h

#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <mutex>
#include "BTreePage.h"
#include "iterators/TreeIterators.h"
#include "iterators/BTreeIterator.h"

#define DEFAULT_BTREE_ORDER 3

template <typename Traits>
class BTree 
// this is the full version of the BTree
{

public:
       //typedef Node iterator;
       using value_type  = typename Traits::value_type;
       using ref_type    = typename Traits::ref_type;
       using Comp        = typename Traits::Comp;
       using BTNode      = CBTreePage <Traits>;
       using Node        = typename BTNode::Node;

       using forward_iterator  = BTreeIterator<BTree<Traits>, BTreeIteratorDirection::Forward>;
       using backward_iterator = BTreeIterator<BTree<Traits>, BTreeIteratorDirection::Backward>;

       BTree(size_t order = DEFAULT_BTREE_ORDER, bool unique = true);
       ~BTree();
       //int           Open (char * name, int mode);
       //int           Create (char * name, int mode);
       //int           Close ();
       bool            Insert (const value_type& data, const ref_type& value);
       bool            Remove (const value_type& data, const ref_type& value);
       auto            Search (const value_type& data);
       size_t          size()     { return m_NumKeys; }
       size_t          height()   { return m_Height;  }
       size_t          GetOrder() { return m_Order;   }

       void            Print (ostream &os);

       template <typename Func, typename... Args>
       void  ForEach(Func lpfn, Args &&... args);

       template <typename Func, typename... Args>
       Node* FirstThat(Func lpfn, Args &&... args);

       TreeIterators<forward_iterator> forward(){
              return { forward_iterator(this, &m_Root),
                       forward_iterator(this, nullptr),
                       &m_mtx
              };
       }

       TreeIterators<backward_iterator> backward(){
              return { backward_iterator(this, &m_Root),
                       backward_iterator(this, nullptr),
                       &m_mtx
              };
       }

protected:
       mutex           m_mtx;
       BTNode          m_Root;
       Comp            m_comp;
       size_t          m_Height;  // height of tree
       size_t          m_Order;   // order of tree
       size_t          m_NumKeys; // number of keys
       bool            m_Unique;  // Accept the elements only once ?
};

const size_t MaxHeight = 5;
template <typename Traits>
BTree<Traits>::BTree(size_t order, bool unique)
                               : m_Root(2 * order  + 1, unique),
                                 m_Order(order),
                                 m_NumKeys(0),
                                 m_Unique(unique)
{
       m_Root.SetMaxKeysForChilds(order);
       m_Height = 1;
}

template <typename Traits>
BTree<Traits>::~BTree()
{
}

template <typename Traits>
bool BTree<Traits>::Insert(const value_type& data, const ref_type& value)
{
       bt_ErrorCode error = m_Root.Insert(data, value);
       if( error == bt_duplicate )
               return false;
       m_NumKeys++;
       if( error == bt_overflow )
       {
               m_Root.SplitRoot();
               m_Height++;
       }
       return true;
}

template <typename Traits>
bool BTree<Traits>::Remove (const value_type& data, const ref_type& value)
{
       bt_ErrorCode error = m_Root.Remove(data, value);
       if( error == bt_duplicate || error == bt_nofound )
               return false;
       m_NumKeys--;

       if( error == bt_rootmerged )
               m_Height--;
       return true;
}

template <typename Traits>
auto BTree<Traits>::Search (const value_type& data)
{
       typename Traits::ref_type value{};
       m_Root.Search(data, value);
       return value;
}


template <typename Traits>
template <typename Func, typename... Args>
void BTree<Traits>::ForEach(Func lpfn, Args&&... args)
{
       m_Root.ForEach(lpfn, 0, forward<Args>(args)...);
}

template <typename Traits>
template <typename Func, typename... Args>
typename BTree<Traits>::Node *
BTree<Traits>::FirstThat(Func lpfn, Args&&... args)
{
       return m_Root.FirstThat(lpfn, 0, forward<Args>(args)...);
}

template <typename Traits>
void BTree<Traits>::Print(ostream &os){
       m_Root.Print(os);
}






#endif