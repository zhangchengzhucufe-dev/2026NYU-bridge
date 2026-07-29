template <typename T>
class LList;

template <typename T>
class ListNode
{
private:
    T data_;
    ListNode<T>* next_;
public:
    ListNode() : data_(T()), next_(nullptr) {}
    ListNode(T data, ListNode<T>* next) : data_(data), next_(next) {}
    friend class LList<T>;
};

template <typename T>
class LList
{
private:
    ListNode<T>* head_;
    ListNode<T>* recursiveCopy(const ListNode<T>* rhs);
public:
    LList() : head_(nullptr) {}
    explicit LList(const ListNode<T>* rhs);
    ~LList();
    LList(const LList<T>& rhs);
    LList<T>& operator=(const LList<T>& rhs);
};

template <typename T>
ListNode<T>* LList<T>::recursiveCopy(const ListNode<T>* rhs)
{
    if(rhs == nullptr) return nullptr;
    return new ListNode<T>(rhs->data_, recursiveCopy(rhs->next_));
}

template <typename T>
LList<T>::LList(const ListNode<T>* rhs)
{
    head_ = recursiveCopy(rhs);
}

template <typename T>
LList<T>& LList<T>::operator=(const LList<T>& rhs)
{
    if(this == &rhs) return *this;
    ListNode<T>* cur = head_;
    while(cur != nullptr)
    {
        ListNode<T>* temp = cur;
        cur = cur->next_;
        delete temp;
    }
    head_ = nullptr;
    head_ = recursiveCopy(rhs.head_);
    return *this;
}

template <typename T>
LList<T>::~LList()
{
    ListNode<T>* cur = head_;
    while(cur != )
}