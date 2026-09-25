/*
INSTRUCTIONS
In this assignment, it is required that you fill out areas under comments labeled as "TODO" appropriately based on the accompanying directions. 
You are also required to follow any directions accompanying comments such as "NOTE".
You can add/modify code anywhere, with the exception of the provided "main" (which we will use for testing).
You can use the constants RED and BLACK, instead of the ints 0 and 1, when appropriate.
*/

#include <iostream>
#include <math.h> // for asserting height
#include <queue>

using namespace std;

#define RED 0
#define BLACK 1

template <class T>
class RBT;

//Do not delete or modify the following 2 lines of code
template<class T>
class Tester;

// swapColor swaps the color from red to black and vice versa
int swapColor(int c) {
    return (c==0)?1:0;
}

template <class T>
class RBTNode {
    RBTNode<T> *parent, *left, *right;
    T data;
    int color;

public:
    RBTNode(T data)
        : data(data),
          color(RED),
          parent(nullptr),
          left(nullptr),
          right(nullptr) {}
    friend class RBT<T>;
    //Do not delete or modify the following line of code.
    friend class Tester<T>;

    void prettyPrint(int indent) const;

    template <class T1>
    friend void swapColor(RBTNode<T1> *);
    template <class T1>
    friend int getColor(RBTNode<T1> *);

    int height() const;

    //Do not delete or modify the following line of code.
    friend class Tester<T>;
};

template <class T>
int RBTNode<T>::height() const {
    int left_h = 0;
    if (left != nullptr) {
        left_h = left->height();
    }
    int right_h = 0;
    if (right != nullptr) {
        right_h = right->height();
    }
    return 1 + max(left_h, right_h);
}

template <class T>
void RBTNode<T>::prettyPrint(int indent) const {
    if (right != nullptr) {
        right->prettyPrint(indent + 1);
    }
    int margin = indent * 2;
    for (int i = 0; i < margin; ++i) {
        cout << '\t';
    }
    cout << "DATA: " << data << endl;
    for (int i = 0; i < margin; ++i) {
        cout << '\t';
    }
    cout << "COLOR: " << (color == RED ? "RED" : "BLACK") << endl;
    if (left != nullptr) {
        left->prettyPrint(indent + 1);
    }
}

template <class T>
void swapColor(RBTNode<T> *node) {
    if (node != nullptr) {
        if (node->color == RED)
            node->color = BLACK;
        else
            node->color = RED;
    }
}

// getColor handles null nodes
template <class T>
int getColor(RBTNode<T> *node) {
    if (node != nullptr) {
        return node->color;
    }
    return BLACK;
}

template <class T>
class RBT {
    RBTNode<T> *root;
    void singleCCR(RBTNode<T> *&point);
    void doubleCR(RBTNode<T> *&point);
    void singleCR(RBTNode<T> *&point);
    void doubleCCR(RBTNode<T> *&point);

public:
    RBT() : root(nullptr) {}

    void insert(const T &);
    void insert(const T &, RBTNode<T> *&point, RBTNode<T> *parent);
    void prettyPrint() const { root->prettyPrint(0); }

    int height() const { return root->height(); }
};

template <class T>
void RBT<T>::doubleCCR(RBTNode<T> *&point) {
    singleCR(point->right);
    singleCCR(point);
}

template <class T>
void RBT<T>::doubleCR(RBTNode<T> *&point) {
    singleCCR(point->left);
    singleCR(point);
}

template <class T>
void RBT<T>::singleCR(RBTNode<T> *&point) {
    RBTNode<T> *grandparent = point;
    RBTNode<T> *parent = point->left;
    // TODO: ADD ROTATION CODE HERE
    grandparent->left=parent->right;
    if(parent->right!=nullptr){
        parent->right->parent=grandparent;
    }
    parent->parent=grandparent->parent;
    //上层祖父的父节点 X 存的孩子指针仍然指向旧 GP，必须手动判断是左 / 右分支，改成指向 parent
    if(parent->parent != nullptr) {
        if(grandparent == parent->parent->left) {
            parent->parent->left = parent;
        } else {
            parent->parent->right = parent;
        }
    }
    grandparent->parent=parent;
    parent->right=grandparent;

    point->parent
}

template <class T>
void RBT<T>::singleCCR(RBTNode<T> *&point) {
    RBTNode<T> *grandparent = point;
    RBTNode<T> *parent = point->right;
    // TODO: ADD ROTATION CODE HERE
    grandparent->right=parent->left;
    if(grandparent->right!=nullptr){
        parent->left->parent=grandparent;
    }
    parent->parent=grandparent->parent;
    if(grandparent->parent!=nullptr){
        if(grandparent->parent->right==grandparent){
            grandparent->parent->right=parent;
        }else grandparent->parent->left=parent;
    }
    parent->left=grandparent;
    grandparent->parent=parent;
    point=parent;
}

template <class T>
void RBT<T>::insert(const T &toInsert, RBTNode<T> *&point, RBTNode<T> *parent) {
    if (point == nullptr) {               // leaf location is found so insert node
        point = new RBTNode<T>(toInsert); // modifies the pointer itself since *point
                                          // is passed by reference
        point->parent = parent;

        RBTNode<T> *curr_node = point; // curr_node will be set appropriately when walking up the tree
        // TODO: ADD RBT RULES HERE
        // 只有当当前节点不是根节点，且它和它的父节点都是红色时，才发生冲突
        RBTNode<T> *curr = point;
        if (curr->parent != nullptr && curr->parent->color == RED && curr->color == RED) {
            RBTNode<T> *p = curr->parent;          // 父节点
            RBTNode<T> *g = p->parent;              // 祖父节点

            if (g != nullptr) {
                bool isParentOnLeft = (p == g->left);
                RBTNode<T> *uncle = isParentOnLeft ? g->right : g->left;

            // 情况 1：叔叔节点存在且为红色 -> 仅需变色 (Recoloring)
                if (uncle != nullptr && uncle->color == RED) {
                    p->color = BLACK;
                    uncle->color = BLACK;
                    g->color = RED;
                } 
                else {
                // 情况 2 & 3：叔叔节点为黑色或为空 -> 需要旋转 (Rotations)
                    if (isParentOnLeft) {
                    // LL 型 或 LR 型
                        if (curr == p->right) {
                        // LR 型：先对父节点左单旋
                            singleCCR(p); // 假设你的旋转函数会调整指针和颜色
                            p = curr;     // 调整指针指向
                        }
                    // LL 型：祖父节点右单旋
                        g->color = RED;
                        p->color = BLACK;
                        singleCR(g);
                    } 
                    else {
                    // RR 型 或 RL 型
                        if (curr == p->left) {
                        // RL 型：先对父节点右单旋
                            singleCR(p);
                            p = curr;
                        }
                    // RR 型：祖父节点左单旋
                        g->color = RED;
                        p->color = BLACK;
                        singleCCR(g);
                    }
                }
            }
        }

    // 确保根节点永远是黑色（红黑树性质规定）
        if (point->parent == nullptr) {
            point->color = BLACK;
        }

    } else if (toInsert < point->data) { // recurse down the tree to left to find correct leaf location
        insert(toInsert, point->left, point);
    } else { // recurse down the tree to right to find correct leaf location
        insert(toInsert, point->right, point);
    }
}

template <class T>
void RBT<T>::insert(const T &toInsert) {
    insert(toInsert, root, nullptr);
}

// NOTE: DO NOT MODIFY THE MAIN FUNCTION BELOW
int main() {
    RBT<int> b;
    int count = 10;
    for (int i = 0; i < count; i++) {
        b.insert(i);
    }
    b.prettyPrint();
    /* EXPECTED OUTPUT:
                                                                    Data: 9
                                                                    COLOR: RED
                                                    Data: 8
                                                    COLOR: BLACK
                                    Data: 7
                                    COLOR: RED
                                                    Data: 6
                                                    COLOR: BLACK
                    Data: 5
                    COLOR: BLACK
                                    Data: 4
                                    COLOR: BLACK
    Data: 3
    COLOR: BLACK
                                    Data: 2
                                    COLOR: BLACK
                    Data: 1
                    COLOR: BLACK
                                    Data: 0
                                    COLOR: BLACK
    */
    // TEST
    // the below tests the validity of the height of the RBT
    // if the assertion fails, then your tree does not properly self-balance
    int height = b.height();
    
    cout<<endl;
    cout<<"---------------------"<<endl;
    cout<<endl;
    RBT<int> c;
    count = 5;
    for (int i = count; i > 0; i--) {
        c.insert(i);
    }
    c.prettyPrint();
    /* EXPECTED OUTPUT:
                    Data: 5
                    COLOR: BLACK
    Data: 4
    COLOR: BLACK
                                    Data: 3
                                    COLOR: RED
                    Data: 2
                    COLOR: BLACK
                                    Data: 1
                                    COLOR: RED
    */
    height = c.height();
    
}
