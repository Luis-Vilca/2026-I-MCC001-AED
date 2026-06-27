// btree.h

#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include "BTreePage.h"

#define DEFAULT_BTREE_ORDER 3

template <typename keyType, typename ObjIDType = TL>
class BTree 
// this is the full version of the BTree
{
       typedef CBTreePage <keyType, ObjIDType> BTNode;// useful shorthand
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
       TB              Insert (const keyType key, const TI ObjID);
       TB              Remove (const keyType key, const TI ObjID);
       ObjIDType       Search (const keyType key);
       size_t          size()  { return m_NumKeys; }
       size_t          height() { return m_Height;      }
       size_t          GetOrder() { return m_Order;     }

       void            Print (ostream &os);
       void            ForEach( lpfnForEach2 lpfn, void *pExtra1 );
       void            ForEach( lpfnForEach3 lpfn, void *pExtra1, void *pExtra2);
       Node*     FirstThat( lpfnFirstThat2 lpfn, void *pExtra1 );
       Node*     FirstThat( lpfnFirstThat3 lpfn, void *pExtra1, void *pExtra2);
       //typedef               Node iterator;

protected:
       BTNode          m_Root;
       size_t          m_Height;  // height of tree
       size_t          m_Order;   // order of tree
       size_t          m_NumKeys; // number of keys
       TB              m_Unique;  // Accept the elements only once ?
};

const size_t MaxHeight = 5;
template <typename keyType, typename ObjIDType>
BTree<keyType, ObjIDType>::BTree(size_t order, TB unique)
                               : m_Root(2 * order  + 1, unique),
                                 m_Order(order),
                                 m_NumKeys(0),
                                 m_Unique(unique)
{
       m_Root.SetMaxKeysForChilds(order);
       m_Height = 1;
}

template <typename keyType, typename ObjIDType>
BTree<keyType, ObjIDType>::~BTree()
{
}

template <typename keyType, typename ObjIDType>
TB BTree<keyType, ObjIDType>::Insert(const keyType key, const TI ObjID)
{
       bt_ErrorCode error = m_Root.Insert(key, ObjID);
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

template <typename keyType, typename ObjIDType>
TB BTree<keyType, ObjIDType>::Remove (const keyType key, const TI ObjID)
{
       bt_ErrorCode error = m_Root.Remove(key, ObjID);
       if( error == bt_duplicate || error == bt_nofound )
               return false;
       m_NumKeys--;

       if( error == bt_rootmerged )
               m_Height--;
       return true;
}

template <typename keyType, typename ObjIDType>
ObjIDType BTree<keyType, ObjIDType>::Search (const keyType key)
{
       ObjIDType ObjID = -1;
       m_Root.Search(key, ObjID);
       return ObjID;
}


template <typename keyType, typename ObjIDType>
void BTree<keyType, ObjIDType>::ForEach(lpfnForEach2 lpfn, void *pExtra1)
{
       m_Root.ForEach(lpfn, 0, pExtra1);
}

template <typename keyType, typename ObjIDType>
void BTree<keyType, ObjIDType>::ForEach(lpfnForEach3 lpfn, void *pExtra1, void *pExtra2)
{
       m_Root.ForEach(lpfn, 0, pExtra1, pExtra2);
}

template <typename keyType, typename ObjIDType>
typename BTree<keyType, ObjIDType>::Node *
BTree<keyType, ObjIDType>::FirstThat(lpfnFirstThat2 lpfn, void *pExtra1)
{
       return m_Root.FirstThat(lpfn, 0, pExtra1);
}

template <typename keyType, typename ObjIDType>
typename BTree<keyType, ObjIDType>::Node *
BTree<keyType, ObjIDType>::FirstThat(lpfnFirstThat3 lpfn, void *pExtra1, void *pExtra2)
{
       return m_Root.FirstThat(lpfn, 0, pExtra1, pExtra2);
}

template <typename keyType, typename ObjIDType>
void BTree<keyType, ObjIDType>::Print(ostream &os){
       m_Root.Print(os);
}






#endif