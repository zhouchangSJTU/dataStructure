
#ifndef ZHASHMAP_H_INCLUDED
#define ZHASHMAP_H_INCLUDED

template <class Key, class Value, class HashFunc, class EqualKey>
class HashMap
{
private:
    template <class _Key, class _Value>
    class KeyNode
    {
        public:
        _Value  value;      //Store the value
        _Key    key;        //Store the keyword
        int    used;
        //if the type of Value/Key is your own class, make sure they can handle copy constructor and operator =
        KeyNode():used(0){}
        KeyNode(const KeyNode & kn)
        {
            value = kn.value;
            key = kn.key;
            used = kn.used;
        }
        KeyNode & operator=(const KeyNode & kn)
        {
            if(this == &kn) return *this;
            value = kn.value;
            key = kn.key;
            used = kn.used;
            return *this;
        }
    };

public:
    HashMap(int size_);
    ~HashMap();
    bool insert(const Key& hashKey, const Value& value);
    bool remove(const Key& hashKey);
    void rehash(int size_);  //use it when rehashing
    Value& find(const Key& hashKey);
    const Value& operator [](const Key& hashKey) const;
    Value& operator [](const Key& hashKey);

private:
    HashFunc hash;
    EqualKey equal;
    KeyNode<Key ,Value> *table;
    int size;    //current number of itmes
    int capacity;   //capacity of the array
    static const double loadingFactor;
    int findKey(const Key& hashKey);  //find the index of a key
};

template<class Key , class Value , class HashFunc , class EqualKey>
const double HashMap<Key, Value, HashFunc, EqualKey>::loadingFactor = 0.9;

template<class Key , class Value , class HashFunc , class EqualKey>
HashMap<Key, Value, HashFunc, EqualKey>::HashMap(int size_)
{
    hash = HashFunc();
    equal = EqualKey();
    capacity = size_; //initialize the capacity with first primer 57
    //resize the table with capacity because an extra one is used
    //to return the NULL type of Value in the function find
    table = new KeyNode<Key,Value>[capacity+1];
    for(int i = 0 ; i < capacity ; i++)    //initialize the table
        table[i].used = 0;
    size = 0;
}

template<class Key, class Value, class HashFunc, class EqualKey>
HashMap<Key, Value, HashFunc, EqualKey>::~HashMap()
{
    delete []table;
}

template<class Key, class Value, class HashFunc, class EqualKey>
bool HashMap<Key, Value, HashFunc, EqualKey>::insert(const Key& hashKey, const Value& value)
{
    int index = hash(hashKey)%capacity;
    //cout<<"Index is "<<index<<endl;
    if(table[index].used == 1)  //the key-value's hash is unique
    {
        //cout<<"The key-value must be unique!"<<endl;
        return false;
    }
    table[index].used = 1;         //modify the KeyNode
    table[index].key = hashKey;
    table[index].value = value;

    size++;
    //if the table's size is too large ,then rehash it
    if (size >= capacity * loadingFactor)
        rehash(capacity);
    return true;
}

template<class Key, class Value, class HashFunc, class EqualKey>
void HashMap<Key, Value, HashFunc, EqualKey>::rehash(int size_)
{
    int pastsize = capacity;
    //create a new array to copy the information in the old table
    capacity = size_;
    KeyNode<Key,Value>* tmp = new KeyNode<Key,Value>[capacity];
    for(int i = 0 ; i < pastsize ; i++)
    {
        if(table[i].used == 1)       //copy the KeyNode into the tmp array
        {
            tmp[i] = table[i];
        }
    }
    delete []table; //release the memory of the old table

    table = new KeyNode<Key,Value>[capacity+1];   //resize the table
    for(int i = 0 ; i < capacity ; i++) //initialize the table
    {
        table[i].used = 0;
    }
    for(int i = 0 ; i < pastsize ; i++) //insert the item into the table one by one
    {
        if(tmp[i].used == 1)
            insert(tmp[i].key, tmp[i].value);
    }
    delete []tmp;               //delete the tmp array
}

template<class Key, class Value, class HashFunc, class EqualKey>
bool HashMap<Key, Value, HashFunc, EqualKey>::remove(const Key& hashKey)
{
    int index = findKey(hashKey); //find the index of the key
    if(index < 0) //if find modify the flag with 0,else print out "no such key!"
    {
        return false;
    }
    else
    {
        table[index].used = 0;
        size--;
        return true;
    }
}

template<class Key, class Value, class HashFunc, class EqualKey>
Value& HashMap<Key, Value, HashFunc, EqualKey>::find(const Key& hashKey)
{
    int index = findKey(hashKey);
    if(index < 0) //if index <0 ,not found,else return the index
    {
        return table[capacity].value; //return NULL
    }
    else
    {
        return table[index].value;
    }
}

template<class Key, class Value, class HashFunc, class EqualKey>
const Value& HashMap<Key, Value, HashFunc, EqualKey>::operator[](const Key& hashKey) const
{
    return find(hashKey); //overload the operation to return the value of the element
}

template<class Key, class Value, class HashFunc, class EqualKey>
Value& HashMap<Key, Value, HashFunc, EqualKey>::operator[](const Key& hashKey)
{
    return find(hashKey); //overload the operation to return the value of the element
}

template<class Key, class Value, class HashFunc, class EqualKey>
int HashMap<Key, Value, HashFunc, EqualKey>::findKey(const Key& hashKey)
{
    int index = hash(hashKey)%capacity;
    if ((table[index].used != 1) || !equal(table[index].key,hashKey))
        return -1;
    else
        return index;
}

#endif
