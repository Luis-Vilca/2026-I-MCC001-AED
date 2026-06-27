// btree.h

#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include "BTreePage.h"

#define DEFAULT_BTREE_ORDER 3

template <typename Traits>
class BTree 
// this is the full version of the BTree
{
       using value_type  = typename Traits::value_type;
       using ref_type    = typename Traits::ref_type;
       using Comp        = typename Traits::Comp;
       using BTNode      = CBTreePage <Traits>;
       /*struct Node
       {
               keyType first;
               long    second;
               Node *&operator->() { return this; }
       };*/

public:
       //typedef Node iterator;
       typedef typename BTNode::lpfnForEach2    lpfnForEach2;
       typedef typename BTNode::lpfnForEach3    lpfnForEach3;
       typedef typename BTNode::lpfnFirstThat2  lpfnFirstThat2;
       typedef typename BTNode::lpfnFirstThat3  lpfnFirstThat3;
       typedef typename BTNode::Node      Node;

public:
       BTree(size_t order = DEFAULT_BTREE_ORDER, TB unique = true);
       ~BTree();
       //int           Open (char * name, int mode);
       //int           Create (char * name, int mode);
       //int           Close ();
       TB              Insert (const value_type& data, const ref_type& value);
       TB              Remove (const value_type& data, const ref_type& value);
       auto            Search (const value_type& data);
       size_t          size()     { return m_NumKeys; }
       size_t          height()   { return m_Height;  }
       size_t          GetOrder() { return m_Order;   }

       void            Print (ostream &os);
       void            ForEach( lpfnForEach2 lpfn, void *pExtra1 );
       void            ForEach( lpfnForEach3 lpfn, void *pExtra1, void *pExtra2);
       Node*           FirstThat( lpfnFirstThat2 lpfn, void *pExtra1 );
       Node*           FirstThat( lpfnFirstThat3 lpfn, void *pExtra1, void *pExtra2);
       //typedef               Node iterator;

protected:
       BTNode          m_Root;
       Comp            m_comp;
       size_t          m_Height;  // height of tree
       size_t          m_Order;   // order of tree
       size_t          m_NumKeys; // number of keys
       TB              m_Unique;  // Accept the elements only once ?
};

const size_t MaxHeight = 5;
template <typename Traits>
BTree<Traits>::BTree(size_t order, TB unique)
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
TB BTree<Traits>::Insert(const value_type& data, const ref_type& value)
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
TB BTree<Traits>::Remove (const value_type& data, const ref_type& value)
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
void BTree<Traits>::ForEach(lpfnForEach2 lpfn, void *pExtra1)
{
       m_Root.ForEach(lpfn, 0, pExtra1);
}

template <typename Traits>
void BTree<Traits>::ForEach(lpfnForEach3 lpfn, void *pExtra1, void *pExtra2)
{
       m_Root.ForEach(lpfn, 0, pExtra1, pExtra2);
}

template <typename Traits>
typename BTree<Traits>::Node *
BTree<Traits>::FirstThat(lpfnFirstThat2 lpfn, void *pExtra1)
{
       return m_Root.FirstThat(lpfn, 0, pExtra1);
}

template <typename Traits>
typename BTree<Traits>::Node *
BTree<Traits>::FirstThat(lpfnFirstThat3 lpfn, void *pExtra1, void *pExtra2)
{
       return m_Root.FirstThat(lpfn, 0, pExtra1, pExtra2);
}

template <typename Traits>
void BTree<Traits>::Print(ostream &os){
       m_Root.Print(os);
}






#endif