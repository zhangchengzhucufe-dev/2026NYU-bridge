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
    friend class BST<T>;
    BSTNode() : data_(T()), left_(nullptr), right_(nullptr) {}
    BSTNode(T data, BSTNode<T>* left, BSTNode<T>* right);
    BSTNode(T data) : data_(data), left_(nullptr), parent_(nullptr), right_(nullptr) {}
};

template <typename T>
class BST
{
private:
    BSTNode<T>* root_;
public:
    int size() const;
    BST() : root_(nullptr) {}
    explicit BST(BSTNode<T>* root);
    void insert(T data);
    void remove(BSTNode<T>* temp);
};

template <typename T>
int BSTNode<T>::sizeHelper() const
{
    int count = 1;
    if(left_ != nullptr)
        count += left_->sizeHelper();
    if(right_ != nullptr)   
        count += right_->sizeHelper();
    return count;
}

template <typename T>
BSTNode<T>::BSTNode(T data, BSTNode<T>* left, BSTNode<T>* right) : data_(data), left_(left), right_(right), parent_(nullptr)
{
    if(left_ != nullptr) left_->parent_ = this;
    if(right_ != nullptr) right_->parent_ = this;
}

template <typename T>
int BST<T>::size() const
{
    if(root_ == nullptr) return 0;
    return root_->sizeHelper();
}

template <typename T>
BST<T>::BST(BSTNode<T>* root)
{
    root_ = root;
}

template <typename T>
void BST<T>::insert(T data)
{

    if(root_ == nullptr)
    {
        root_ = new BSTNode<T>(data);
        return;
    }
    BSTNode<T>* cur = root_;
    BSTNode<T>* par = cur;
    while(cur != nullptr)
    {
        par = cur;
        if(cur->data_ > data)
            cur = cur->left_;
        else
            cur = cur->right_;
    }
    BSTNode<T>* newNode = new BSTNode<T>(data);
    newNode->parent_ = par;
    if(par->data_ > data)
        par->left_ = newNode;
    else
        par->right_ = newNode;
}

template <typename T>
void BST<T>::remove(BSTNode<T>* temp)
{
    if(temp == nullptr) return;
    BSTNode<T>* left = temp->left_;
    BSTNode<T>* right = temp->right_;
    BSTNode<T>* parent = temp->parent_;
    if(parent == nullptr)
    {
        BSTNode<T>* minRight = temp->right_;
        BSTNode<T>* par = minRight;
        while(minRight != nullptr)
        {
            par = minRight;
            root_ = minRight->left_;
        }
        root_ = par;
        remove(par);
        delete temp;
        return;
    }
    if(left == nullptr && right == nullptr)
    {
        if(parent->left_ == temp)
            parent->left_ = nullptr;
        else
            parent->right_ = nullptr;
    }
    else if(right == nullptr)
    {
        left->parent_ = parent;
        if(parent->left_ == temp)
            parent->left_ = left;
        else
            parent->right_ = left;
    }
    else if(left == nullptr)
    {
        right->parent_ = parent;
        if(parent->left_ == temp)
            parent->left_ = right;
        else
            parent->right_ = right;
    }
    else
    {
        BSTNode<T>* minRight = right;
        while(minRight->left_ != nullptr)
        {
            minRight =minRight->left_;
        }
        temp->data_ = minRight->data_;
        remove(minRight);
        return;
    }
    delete temp;
}
