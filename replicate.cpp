template <typename T>
struct BSTNode
{
    T data_;
    BSTNode<T>* left_;
    BSTNode<T>* right_;
    BSTNode<T>* parent_;
    BSTNode() : left_(nullptr), right_(nullptr), parent_(nullptr) {}
    BSTNode(T data, BSTNode<T>* left, BSTNode<T>* right);
    int countHelper() const;
};

template <typename T>
int BSTNode<T>::countHelper() const
{
    int count = 1;
    if(right_) count += countHelper(right_);
    if(left_) count += countHelper(left_);
    return count;
}

template <typename T>
BSTNode<T>::BSTNode(T data, BSTNode<T>* left, BSTNode<T>* right) : left_(left_), right_(right), parent_(nullptr), data_(data)
{
    if(left_ != nullptr) left_->parent_ = this;
    if(right_ != nullptr) right_->parent_ = this;
}

template <typename T>
struct BST
{
    BSTNode<T>* root_;
    int size();
};

template <typename T>
int BST<T>::size()
{
    if(root_ == nullptr) return 0;
    return countHelper(root_);
}