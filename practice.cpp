#include <iostream>

template <typename T>
struct BSTNode
{
    T data_;
    BSTNode<T>* left_;
    BSTNode<T>* right_;
    BSTNode<T>* parent_;
    int size() const;
    void printInOrder(const BSTNode<T>* node);
    void printPostOrder(const BSTNode<T>* node);
    void printPreOrder(const BSTNode<T>* node);  
    explicit BSTNode(T data = T(), BSTNode<T>* left = nullptr, BSTNode<T>* right = nullptr);
};

template <typename T>
int BSTNode<T>::size() const
{   
    int count = 1;
    count += left_->size();
    count += right_->size();
    return count;
}

template <typename T>
void BSTNode<T>::printPostOrder(const BSTNode<T>* node)
{
    printPostOrder(node->left_);
    printPostOrder(node->right_);
    std::cout << node->data_;
}

template <typename T>
struct BST
{
    BSTNode<T>* root_;
    BST() : root_(nullptr) {}
    void insert(T data);
    void remove(BSTNode<T>* temp);
};

template <typename T>
void BST<T>::insert(T data)
{
    if(root_ == nullptr)
    {
        root_ = new BSTNode<T>(data);
        return;
    }
    BSTNode<T>* cur = root_;
    BSTNode<T>* prev = cur;
    while(cur != nullptr)
    {
        prev = cur;
        if(data < cur->data_) 
            cur = cur->left_;
        else
            cur = cur->right_;
    }
    if(data < prev->data_)
        prev->left_ = new BSTNode<T>(data);
    else
        prev->right_ = new BSTNode<T>(data);
}

template <typename T>
void BST<T>::remove(BSTNode<T>* temp)
{
    BSTNode<T>* parent = temp->parent_;
    if(temp->left_ == nullptr && temp->right_ == nullptr)
    {
        if(parent == nullptr)
            root_ = nullptr;
        else if(parent->left_ == temp)
            parent->left_ = nullptr;
        else
            parent->right_ == nullptr;
        delete temp;
    }
    else if(temp->left_ == nullptr)
    {
        BSTNode<T>* child = temp->right_;
        BSTNode<T>* parent = temp->parent_;
        child->parent_ = parent;
        if(parent == nullptr)
            root_ = child;
        else if(parent->left_ == temp)
            parent->left_ = child;
        else
            parent->right_ = child;
        delete temp;
    }
    else if(temp->right_ == nullptr)
    {
        BSTNode<T>* parent = temp->parent_;
        BSTNode<T>* child = temp->left_;
        child->parent_ = parent;
        if(parent == nullptr)
            root_ = child;
        else if(parent->left_ == temp)
            parent->left_ = child;
        else
            parent->right_ = child;
        delete temp;
    }
    else
    {
        BSTNode<T>* minRight = temp->right_;
        while(minRight->left_ != nullptr)
        {
            minRight = minRight->left_;
        }
        temp->data_ = minRight->data_;
        remove(minRight); 
    }

}

template <typename T>
BSTNode<T>::BSTNode(T data, BSTNode<T>* left, BSTNode<T>* right) : data_(data), left_(left), right_(right), parent_(nullptr)
{
    if(right_ != nullptr) right_->parent_ = this;
    if(left_ != nullptr) left_->parent_ = this;
}