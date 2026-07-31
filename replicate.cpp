template <typename T>
class BST;

template <typename T>
class BSTNode
{
private:
    T data_;
    BSTNode<T>* left_;
    BSTNode<T>* right_;
    BSTNode<T>* parent_;
    int sizeHelper() const;
public:
    explicit BSTNode(T data_, BSTNode<T>* left_, BSTNode<T>* right_);
    BSTNode() : data_(T()), left_(nullptr), right_(nullptr) {}
    friend class BST<T>;
};

template <typename T>
int BSTNode<T>::sizeHelper() const
{
    int count = 1;
    count += left_->sizeHelper();
    count += right_->sizeHelper();
    return count;
}

template <typename T>
BSTNode<T>::BSTNode(T data, BSTNode<T>* right, BSTNode<T>* left) : data_(data), left_(left), right_(right)
{   
    if(left_ != nullptr) left_->parent_ = this;
    if(right_ != nullptr) right_
    ->parent_ = this;
}

template <typename T>
class BST
{
private:
    BSTNode<T>* root_;

public:
    int size() const;
    void insert(T data);
};

template <typename T>
int BST<T>::size() const
{
    if(root_ == nullptr) return 0;
    return root_->sizeHelper();
}

template <typename T>
void BST<T>::insert(T data)
{
    if(root_ == nullptr)
    {
        root_ = new BSTNode(data);
        return;
    }
    BSTNode<T>* cur = root_;
    while(cur->next != nullptr)
    {
        if(data > cur->data_)
            
    }

}
265