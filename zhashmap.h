
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
        _Value  value;
        _Key    key;
        int    used;
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
    void rehash(int size_);
    Value& find(const Key& hashKey);
    const Value& operator [](const Key& hashKey) const;
    Value& operator [](const Key& hashKey);

private:
    HashFunc hash;
    EqualKey equal;
    KeyNode<Key ,Value> *table;
    int size;
    int capacity;
    static const double loadingFactor;
    int findKey(const Key& hashKey);
};

template<class Key , class Value , class HashFunc , class EqualKey>
const double HashMap<Key, Value, HashFunc, EqualKey>::loadingFactor = 0.9;

template<class Key , class Value , class HashFunc , class EqualKey>
HashMap<Key, Value, HashFunc, EqualKey>::HashMap(int size_)
{
    hash = HashFunc();
    equal = EqualKey();
    capacity = size_;
    table = new KeyNode<Key,Value>[capacity+1];
    for(int i = 0 ; i < capacity ; i++)
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
    if(table[index].used == 1)
    {
        return false;
    }
    table[index].used = 1;
    table[index].key = hashKey;
    table[index].value = value;
    size++;
    if (size >= capacity * loadingFactor)
        rehash(capacity);
    return true;
}

template<class Key, class Value, class HashFunc, class EqualKey>
void HashMap<Key, Value, HashFunc, EqualKey>::rehash(int size_)
{
    int pastsize = capacity;
    capacity = size_;
    KeyNode<Key,Value>* tmp = new KeyNode<Key,Value>[capacity];
    for(int i = 0 ; i < pastsize ; i++)
    {
        if(table[i].used == 1)
        {
            tmp[i] = table[i];
        }
    }
    delete []table;
    table = new KeyNode<Key,Value>[capacity+1];
    for(int i = 0 ; i < capacity ; i++)
    {
        table[i].used = 0;
    }
    for(int i = 0 ; i < pastsize ; i++)
    {
        if(tmp[i].used == 1)
            insert(tmp[i].key, tmp[i].value);
    }
    delete []tmp;
}

template<class Key, class Value, class HashFunc, class EqualKey>
bool HashMap<Key, Value, HashFunc, EqualKey>::remove(const Key& hashKey)
{
    int index = findKey(hashKey);
    if(index < 0)
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
    if(index < 0)
    {
        return table[capacity].value;
    }
    else
    {
        return table[index].value;
    }
}

template<class Key, class Value, class HashFunc, class EqualKey>
const Value& HashMap<Key, Value, HashFunc, EqualKey>::operator[](const Key& hashKey) const
{
    return find(hashKey);
}

template<class Key, class Value, class HashFunc, class EqualKey>
Value& HashMap<Key, Value, HashFunc, EqualKey>::operator[](const Key& hashKey)
{
    return find(hashKey);
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
