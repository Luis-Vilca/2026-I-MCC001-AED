#ifndef __DIGITALTREE_H__
#define __DIGITALTREE_H__

#include <string>
#include <vector>
#include <mutex>

#include "iterators/general_iterator.h"
#include "iterators/digitalTreeIterator.h"
#include "iterators/TreeIterators.h"

using namespace std;

template<typename T>
class DigitalTreeNode
{
public:
    using value_type = T;
    using key_type = char;
    using Node = DigitalTreeNode<T>;

private:
    
    key_type    m_symbol;
    bool        m_terminal;
    value_type  m_value;

    Node*       m_parent;
    vector<Node*> m_children;

public:

    DigitalTreeNode(char symbol = '\0'):
    m_symbol(symbol), m_terminal(false), m_parent(nullptr){};

    ~DigitalTreeNode(){
    for (auto child : m_children)
        delete child;
    };

    key_type Symbol()                const{return m_symbol;}
    bool IsTerminal()                const{return m_terminal;}
    value_type& Value()                   {return m_value;}
    const value_type& Value()        const{return m_value;}
    Node* Parent()                   const{return m_parent;}
    vector<Node*>& Children()             {return m_children;}
    const vector<Node*>& Children()  const{return m_children;}
    string Key() const;
    void SetTerminal(bool terminal)       {m_terminal = terminal;}
    void SetValue(const value_type&value) {m_value = value;}
    Node* FindChild(key_type symbol);
    Node* AddChild(key_type symbol);
    void RemoveChild(Node* child);
};

template<typename T>
class DigitalTree
{
public:

    using value_type = T;
    using Node = DigitalTreeNode<T>;
    using forward_iterator = DigitalTreeIterator<DigitalTree<T>,DigitalTreeIteratorDirection::Forward>;
    using backward_iterator =DigitalTreeIterator<DigitalTree<T>,DigitalTreeIteratorDirection::Backward>;

private:

    Node* m_root;
    size_t m_size;

    mutable mutex m_mtx;

public:
    DigitalTree(): m_root(new Node()), m_size(0){};
    ~DigitalTree(){delete m_root;};
    bool Empty() const  {return m_size == 0;};
    size_t Size() const {return m_size;};
    void Clear(){
        delete m_root;
        m_root = new Node();
        m_size = 0;
    };
    bool Insert(const string& key, const value_type& value);
    Node* Search(const string& key) const;
    bool StartsWith(const string& prefix) const;
    bool Remove(const string& key);
    TreeIterators<forward_iterator> forward();
    TreeIterators<backward_iterator> backward();
};

template<typename T>
string DigitalTreeNode<T>::Key() const
{
    string key;
    const Node* current = this;
    while(current->Parent() != nullptr)
    {
        key.push_back(current->Symbol());
        current = current->Parent();
    }
    reverse(key.begin(), key.end());
    return key;
}

template<typename T>
typename DigitalTreeNode<T>::Node*
DigitalTreeNode<T>::FindChild(char symbol)
{
    for (auto child : m_children)
        if (child->m_symbol == symbol)
            return child;
    return nullptr;
}

template<typename T>
typename DigitalTreeNode<T>::Node*
DigitalTreeNode<T>::AddChild(char symbol)
{
    Node* child = FindChild(symbol);
    if (child)
        return child;

    child = new Node(symbol);
    child->m_parent = this;
    m_children.push_back(child);
    return child;
}

template<typename T>
void DigitalTreeNode<T>::RemoveChild(Node* child)
{
    auto it = find(m_children.begin(),m_children.end(),child);
    if (it != m_children.end())
    {
        delete *it;
        m_children.erase(it);
    }
}

template<typename T>
typename DigitalTree<T>::Node*
DigitalTree<T>::Search(const string& key) const
{
    Node* current = m_root;
    for(char ch : key)
    {
        current = current->FindChild(ch);
        if(current == nullptr)
            return nullptr;
    }
    return current->IsTerminal()? current: nullptr;
}

template<typename T>
bool DigitalTree<T>::Insert(const string& key, const value_type& value)
{
    Node* current = m_root;
    for(char ch : key)
        current = current->AddChild(ch);
    if(current->IsTerminal())
        return false;
    current->SetTerminal(true);
    current->SetValue(value);
    ++m_size;
    return true;
}

template<typename T>
bool DigitalTree<T>::Remove(const string& key)
{
    Node* node = Search(key);
    if(node == nullptr)
        return false;
    node->SetTerminal(false);
    --m_size;
    return true;
}

template<typename T>
TreeIterators<typename DigitalTree<T>::forward_iterator>
DigitalTree<T>::forward()
{
    return {forward_iterator(this,m_root),forward_iterator(this,nullptr),&m_mtx};
}

template<typename T>
TreeIterators<typename DigitalTree<T>::backward_iterator>
DigitalTree<T>::backward()
{
    return {backward_iterator(this,m_root), backward_iterator(this,nullptr), &m_mtx};
}

template<typename T>
bool DigitalTree<T>::StartsWith(const string& prefix) const
{
    Node* current = m_root;
    for(char ch : prefix)
    {
        current = current->FindChild(ch);
        if(current == nullptr)
            return false;
    }
    return true;
}
#endif //__DIGITALTREE_H__