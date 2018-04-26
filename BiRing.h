#include <iostream>
using namespace std;
template <typename Key, typename Info>
class BiRing
{
struct Node;
public:

    class iterator;
    class const_iterator
    {
    protected:
        Node *ptr;
        friend class BiRing<Key,Info>;
        friend class iterator;
    public:
        const_iterator() : ptr(NULL) {};
        const_iterator(Node *ptr) {this->ptr=ptr;};
        ~const_iterator() {this->ptr = NULL;};
        const_iterator& operator= (const const_iterator & rhs) {ptr = rhs.ptr; return *this;};
        const_iterator& operator++ () {ptr = ptr->next; return *this;};
        const_iterator& operator-- () {ptr = ptr->prev; return *this;};
        const_iterator& operator++ (int) {ptr = ptr->next; return *this;};
        const_iterator& operator-- (int) {ptr = ptr->prev; return *this;};
        bool operator== (const const_iterator & rhs) const {return (ptr == rhs.ptr);};
        bool operator!= (const const_iterator & rhs) const {return (ptr != rhs.ptr);};
        const Key& key() const { return ptr->key; };
        const Info& info() const { return ptr->info; };
    };
    class iterator
    {
        Node *ptr;
        friend class BiRing<Key,Info>;
        friend class const_iterator;
    public:
        iterator() : ptr(NULL) {};
        iterator(Node * ptr){this->ptr=ptr;};
        ~iterator() {this->ptr = NULL;};

        iterator& operator= (const iterator & rhs) {ptr = rhs.ptr; return *this;};
        iterator& operator++ () {ptr = ptr->next; return *this;};
        iterator& operator++ (int) {ptr = ptr->next; return *this;};
        iterator& operator-- () {ptr = ptr->prev; return *this;};
        iterator& operator-- (int) {ptr = ptr->prev; return *this;};
        iterator& operator+=(unsigned int n) { while(n--) { ++(*this);} return *this; };
        iterator& operator-=(unsigned int n) { while(n--) { --(*this);} return *this; };
        bool operator== (const iterator & rhs) const {return ptr == rhs.ptr;};
        bool operator!= (const iterator & rhs) const {return ptr != rhs.ptr;};
        const Info & operator* () const {return (ptr->info);};
        Info & operator* () {return (ptr->info);};
        iterator add(Key key, Info info);
        iterator remove();
        const Key& getkey() const { return ptr->key; };
        Info& getinfo() const { return ptr->info; };
    };

public:
    BiRing();      // create new ring and set it's head to NULL
    BiRing(const BiRing<Key,Info> &rhs);
    ~BiRing() {clear();};
    const BiRing<Key,Info> & operator= (const BiRing<Key,Info> & rhs);

    void push_front(Key key, Info info);
    void push_back(Key key, Info info);
    void pop_front();
    void pop_back();

    void clear();

    iterator begin() const {return sentinel.ptr->next;};
    iterator end()const {return sentinel;};
    iterator add( Key key, Info info, const iterator& after);
    iterator remove(const iterator& it);
    iterator find_key(const Key key);
    iterator find_info(const Info info);
    iterator add(const iterator first , const iterator last);
    const_iterator cbegin() const { return sentinel.ptr->next;};
    const_iterator cend() const { return sentinel;};
    template<typename K, typename I>
    friend std::ostream& operator<<(std::ostream&, const BiRing<K, I>&);


private:
    iterator sentinel;
    struct Node
    {
        Key key;
        Info info;
        Node * next;
        Node * prev;
        Node() : next(this), prev(this) {};
        Node(Key key,Info info) {this->info=info;
            this->key=key;};
    };
};

template<typename Key, typename Info>
std::ostream& operator<<(std::ostream& os, const BiRing<Key, Info>& x){
    os << "[";
    for(typename BiRing<Key, Info>::iterator i = x.begin(); i != x.end(); ++i) {
        os << "(" << i.getkey() << "," << i.getinfo() << ")";
    }
    os << "]";
    return os;
}

template<typename Key, typename Info>
BiRing<Key, Info>::BiRing() {
    sentinel.ptr = new Node();
    sentinel.ptr->next = sentinel.ptr;
    sentinel.ptr->prev = sentinel.ptr;
}

template<typename Key, typename Info>
BiRing<Key, Info>::BiRing(const BiRing& toCopy) {
    sentinel = new Node();
    for(BiRing<Key, Info>::iterator it = toCopy.begin(); it != toCopy.end(); ++it) {
        push_back(it.getkey(), it.getinfo());
    }
}
template<typename Key, typename Info>
const BiRing<Key,Info> & BiRing<Key,Info>::operator= (const BiRing<Key,Info> & rhs)
{
    if(this == &rhs){
        return *this;
    }
    clear();
    for(BiRing<Key, Info>::iterator it = rhs.begin(); it != rhs.end(); ++it) {
        push_back(it.getkey(), it.getinfo());
    }
    return *this;
}
template<typename Key, typename Info>
typename BiRing<Key,Info>::iterator BiRing<Key,Info>::find_key(const Key  key) {
    BiRing<Key, Info>::iterator it = begin();
    if (begin()!=NULL) {
        do {
            if (it.getkey() == key)   // testing two diffrent solutions
                return it;
            it ++;
        } while (it != begin());
    }

    return NULL; // not found (could be empty)
}
template<typename Key, typename Info>
typename BiRing<Key,Info>::iterator BiRing<Key,Info>::find_info(const Info info){
    BiRing<Key, Info>::iterator it = begin();

    if (begin()!=NULL) {
        do {
            if (*it == info)   // testing two diffrent solutions
                return it;
            it ++;
        } while (it != begin());
    }

    return NULL; // not found (could be empty)
};
template<typename Key, typename Info>

void BiRing<Key,Info>::push_front(Key key, Info info) {
    add(key, info, begin());
}
template<typename Key, typename Info>
void BiRing<Key,Info>::push_back(Key key, Info info) {
    add(key,info,--end());
};

template<typename Key, typename Info>
void BiRing<Key,Info>::pop_front(){
    remove(begin());
};
template<typename Key, typename Info>
void BiRing<Key,Info>::pop_back() {
    remove(--end());
}
template<typename Key, typename Info>
typename BiRing<Key,Info>::iterator BiRing<Key, Info>::iterator::add(Key key, Info info) {
    Node* new_ptr = new Node(key,info);
    new_ptr->next = ptr->next;
    new_ptr->prev = ptr;
    ptr->next->prev = new_ptr;
    ptr->next = new_ptr;
    return iterator(new_ptr);
}



template<typename Key, typename Info>
void BiRing<Key, Info>::clear() {
    BiRing<Key, Info>::iterator it = begin();
    while(it != end()) {
        it = remove(it);
    }
}
template<typename Key, typename Info>
typename BiRing<Key,Info>::iterator BiRing<Key, Info>::iterator::remove() {

    iterator x;
    if(this->ptr->prev == this->ptr){
        return iterator(this->ptr);
    }
    this->ptr->next->prev = this->ptr->prev;
    this->ptr->prev->next = this->ptr->next;
    x.ptr = this->ptr->next;
    delete this->ptr;
    return x;
}

template<typename Key, typename Info>
typename BiRing<Key,Info>::iterator BiRing<Key, Info>::remove(const iterator& it){
    if (it == NULL)  // nothing to do, no element to remove pointed
    return it;

    Node * ptr = it.ptr;
    if (ptr == sentinel.ptr && ptr->next == sentinel.ptr->next) { // only one element, clear list
    clear();
}
    ptr->next->prev = ptr->prev;
    ptr->prev->next = ptr->next;
    iterator tmp;
    tmp.ptr = ptr->next;
    delete ptr;
    return tmp;
}

template<typename Key, typename Info>
typename BiRing<Key,Info>::iterator BiRing<Key, Info>::add(Key key, Info info, const BiRing<Key, Info>::iterator& after){

    Node * ptr = after.ptr;    // get pointer to node
    Node * new_ptr = new Node(key,info);
    new_ptr->next = ptr->next;
    new_ptr->prev = ptr;
    ptr->next->prev = new_ptr;
    ptr->next = new_ptr;

    if (new_ptr->next == sentinel.ptr->next) {
        sentinel.ptr->next = new_ptr;
    }

    return iterator(new_ptr);
}


template<typename Key, typename Info>
typename BiRing<Key,Info>::iterator BiRing<Key, Info>::add(const iterator first , const iterator last) {
    BiRing<Key, Info>::iterator it = begin();
    BiRing<Key, Info>::iterator it2 = begin();

    Node *last1= last.ptr;
    Node *first1 = first.ptr;

    if (first1 == NULL && last1  == NULL) {
        cout << "Nothing to add" << endl;
        return iterator(last);
    } else {
        for (it=begin();it!=end();it++){
            if(it.ptr==first1)
                break;
        }
        for (it2=begin();it2!=end();it2++){
            if(it2.ptr==last1)
                break;
        }
        //cout<<it2.getinfo()<<endl;
        //cout<<it.getinfo()<<endl;
        for (it;it!=it2;it++){
            push_front(it.getkey(),it.getinfo());
        }

        return iterator(it2);
    }
}






