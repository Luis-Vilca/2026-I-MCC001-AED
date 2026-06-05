#ifndef __AVL_TREE_H__
#define __AVL_TREE_H__
 
#include "binarytree.h"
 
template <typename Traits>
class AVLTree : public BinaryTree<Traits> {
public:
    // --------------------------------------------------------
    //  Tipos heredados
    // --------------------------------------------------------
    using Base       = BinaryTree<Traits>;
    using value_type = typename Base::value_type;
    using MySelf     = AVLTree<Traits>;
 
    class AVLNode : public Base::BinaryTreeNode {
    public:
        using ParentNode = typename Base::BinaryTreeNode;
        using NodePtr    = AVLNode*;
 
        size_t m_height = 1;

        AVLNode(const value_type& data, const Ref& ref,
                NodePtr left  = nullptr, NodePtr right = nullptr)
                : ParentNode(data, ref,
                         static_cast<typename Base::NodePtr>(left),
                         static_cast<typename Base::NodePtr>(right))
        {}
    };

    using NodePtr    = AVLNode*;
 
    // --------------------------------------------------------
    //  Constructor / destructor — delega en Base
    // --------------------------------------------------------
    AVLTree()  = default;
    ~AVLTree() = default;
 
    // --------------------------------------------------------
    //  insert  (sobreescribe el de BinaryTree)
    // --------------------------------------------------------
    void insert(const value_type& value, Ref ref) {
        scoped_lock<mutex> lock(this->m_mtx);
        this->m_pRoot = avl_insert(
            static_cast<NodePtr>(this->m_pRoot), value, ref, nullptr);
    }
 
private:
    // ----------------------------------------------------------
    //  Utilidades de altura y factor de balance
    // ----------------------------------------------------------
    static size_t height(AVLNode* n) {
        return n ? n->m_height : 0;
    }
 
    static void update_height(AVLNode* n) {
        if (n)
            n->m_height = 1 + max(height(static_cast<AVLNode*>(n->getChild(0))),
                                  height(static_cast<AVLNode*>(n->getChild(1))));
    }
 
    // balance > 0  → pesado a la izquierda
    // balance < 0  → pesado a la derecha
    static TI balance_factor(AVLNode* n) {
        if (!n) return 0;
        return height(static_cast<AVLNode*>(n->getChild(0)))
             - height(static_cast<AVLNode*>(n->getChild(1)));
    }
 
    // ----------------------------------------------------------
    //  Rotaciones
    //
    //      rotate_right(y)          rotate_left(x)
    //
    //        y                x              x          y
    //       / \              / \            / \        / \
    //      x   T3    =>    T1   y    =>   T1  T2  =>  x  T3
    //     / \                  / \
    //   T1  T2               T2  T3
    // ----------------------------------------------------------
    AVLNode* rotate_right(AVLNode* y) {
        AVLNode* x  = static_cast<AVLNode*>(y->getChild(0));
        AVLNode* T2 = static_cast<AVLNode*>(x->getChild(1));
 
        // Giro
        x->setChild(1, y);
        y->setChild(0, T2);
 
        // El padre de x pasa a ser el antiguo padre de y
        x->setParent(y->getParent());
        y->setParent(x);
        if (T2) T2->setParent(y);
 
        // Actualizar alturas (primero y, luego x)
        update_height(y);
        update_height(x);
        return x;   // nueva raíz del subárbol
    }
 
    AVLNode* rotate_left(AVLNode* x) {
        AVLNode* y  = static_cast<AVLNode*>(x->getChild(1));
        AVLNode* T2 = static_cast<AVLNode*>(y->getChild(0));
 
        // Giro
        y->setChild(0, x);
        x->setChild(1, T2);
 
        // Actualizar padres
        y->setParent(x->getParent());
        x->setParent(y);
        if (T2) T2->setParent(x);
 
        // Actualizar alturas (primero x, luego y)
        update_height(x);
        update_height(y);
        return y;   // nueva raíz del subárbol
    }
 
    // ----------------------------------------------------------
    //  Rebalanceo tras inserción
    //  Aplica las 4 rotaciones AVL clásicas según el caso.
    // ----------------------------------------------------------
    AVLNode* rebalance(AVLNode* node, const value_type& value) {
        update_height(node);
        int bf = balance_factor(node);
 
        // ── Caso LL (rotación simple derecha) ─────────────────
        if (bf > 1 &&
            this->m_comp(value,
                static_cast<AVLNode*>(node->getChild(0))->getDataRef()))
            return rotate_right(node);
 
        // ── Caso RR (rotación simple izquierda) ───────────────
        if (bf < -1 &&
            !this->m_comp(value,
                static_cast<AVLNode*>(node->getChild(1))->getDataRef()))
            return rotate_left(node);
 
        // ── Caso LR (rotación doble: izquierda-derecha) ───────
        if (bf > 1 &&
            !this->m_comp(value,
                static_cast<AVLNode*>(node->getChild(0))->getDataRef())) {
            node->setChild(0, rotate_left(
                static_cast<AVLNode*>(node->getChild(0))));
            return rotate_right(node);
        }
 
        // ── Caso RL (rotación doble: derecha-izquierda) ───────
        if (bf < -1 &&
            this->m_comp(value,
                static_cast<AVLNode*>(node->getChild(1))->getDataRef())) {
            node->setChild(1, rotate_right(
                static_cast<AVLNode*>(node->getChild(1))));
            return rotate_left(node);
        }
 
        return node;    // ya está balanceado
    }
 
    // ----------------------------------------------------------
    //  avl_insert  — inserción recursiva con rebalanceo
    //  Devuelve la (posiblemente nueva) raíz del subárbol.
    // ----------------------------------------------------------
    AVLNode* avl_insert(AVLNode* node, const value_type& value,
                        Ref ref, AVLNode* parent)
    {
        // 1. Inserción BST normal
        if (!node) {
            AVLNode* n = new AVLNode(value, ref);
            n->setParent(parent);
            return n;
        }
 
        size_t pos = !this->m_comp(value, node->getDataRef()); // 0=izq, 1=der
        AVLNode* child = avl_insert(
            static_cast<AVLNode*>(node->getChild(pos)), value, ref, node);
 
        // Reconectar hijo (la recursión puede haber cambiado la raíz del subárbol)
        node->setChild(pos, child);
        child->setParent(node);
 
        // 2. Rebalancear en el camino de vuelta
        return rebalance(node, value);
    }
};
 
// ============================================================
//  Traits de conveniencia (espejo de los de BinaryTree)
// ============================================================
template <typename T>
struct AscendingAVLTreeTrait : public AscendingBinaryTreeListTrait<T> {};
 
template <typename T>
struct DescendingAVLTreeTrait : public DescendingBinaryTreeListTrait<T> {};
 
#endif // __AVL_TREE_H__