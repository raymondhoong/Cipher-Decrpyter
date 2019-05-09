// MyHash.h

// Skeleton for the MyHash class template.  You must implement the first seven
// member functions; we have implemented the eighth.

template<typename KeyType, typename ValueType>
class MyHash
{
public:
    MyHash(double maxLoadFactor = 0.5);
    ~MyHash();
    void reset();
    void associate(const KeyType& key, const ValueType& value);
    int getNumItems() const;
    double getLoadFactor() const;

      // for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

      // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
    }

      // C++11 syntax for preventing copying and assignment
    MyHash(const MyHash&) = delete;
    MyHash& operator=(const MyHash&) = delete;
private:
	struct HashNode
	{
		HashNode(KeyType key, ValueType value)
		{
			this->m_key = key;
			this->m_value = value;
			this->m_next = nullptr;
			m_vptr = &m_value;
		}
		KeyType m_key;
		ValueType m_value;
		HashNode* m_next;
		const ValueType* m_vptr;
	};

	const int DEFAULT_SIZE = 100;
	int m_bucketNum;
	int m_itemNum;
	double m_maxLoad;
	HashNode** htable;

	unsigned int hashFunction(const KeyType& key) const
	{
		unsigned int hasher(const KeyType& k);
		unsigned int hvalue = hasher(key) % m_bucketNum;
		return hvalue;
	}
};

template<typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::MyHash(double maxLoadFactor)
	: m_bucketNum(DEFAULT_SIZE), m_itemNum(0)
{
	if (maxLoadFactor <= 0)
		m_maxLoad = 0.5;
	else if (maxLoadFactor > 2)
		m_maxLoad = 2.0;
	else
		m_maxLoad = maxLoadFactor;

	htable = new HashNode*[DEFAULT_SIZE];
	for (int k = 0; k < DEFAULT_SIZE; k++)
		htable[k] = nullptr;
}

template<typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::~MyHash()
{
	for (int k = 0; k < m_bucketNum; k++)
	{
		HashNode* curr = htable[k];
		while (curr != nullptr)
		{
			HashNode* prev = curr;
			curr = curr->m_next;
			delete prev;
		}
	}
	delete[] htable;
}

template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::reset()
{
		// Delete old table
	for (int k = 0; k < m_bucketNum; k++)
	{
		HashNode* curr = htable[k];
		while (curr != nullptr)
		{
			HashNode* prev = curr;
			curr = curr->m_next;
			delete prev;
		}
	}
	delete[] htable;

		// Create new, empty table
	htable = new HashNode*[DEFAULT_SIZE];
	for (int k = 0; k < DEFAULT_SIZE; k++)
		htable[k] = nullptr;

	m_bucketNum = DEFAULT_SIZE;
	m_itemNum = 0;
}

template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
			// If load factor exceeds the maximum load factor, resize the hash table
	if (getLoadFactor() > m_maxLoad)
	{
			// Temporary variables to hold old table
		HashNode** htableOld = htable;
		int bucketNumOld = m_bucketNum;

			// Create a new hash table with double the old size
		m_bucketNum *= 2;
		m_itemNum = 0;
		int oldItemNum = m_itemNum;
		htable = new HashNode*[m_bucketNum];
		for (int k = 0; k < m_bucketNum; k++)
			htable[k] = nullptr;

			// Copy items from old to new table; Delete old items
		for (int k = 0; k < bucketNumOld; k++)
		{
			HashNode* curr = htableOld[k];
			while (curr != nullptr)
			{
				associate(curr->m_key, curr->m_value);
				HashNode* prev = curr;
				curr = curr->m_next;
				delete prev;
			}
		}
		delete[] htableOld;
	}
		// Find correct bucket to add item
	unsigned int hashID = hashFunction(key);
	HashNode* prev = nullptr;
	HashNode* curr = htable[hashID];

		// If collisions, find the end of the list
	while (curr != nullptr)
	{
			// If there is a duplicate key found, break and continue
		if (curr->m_key == key)
			break;
		prev = curr;
		curr = curr->m_next;
	}

		// If there are no duplicate keys, create a new HashNode and add to list; Otherwise change duplicate's value
	if (curr == nullptr)
	{
		m_itemNum++;
		curr = new HashNode(key, value);

			// If bucket is empty, add item at the front; Otherwise add item to the end of linked list
		if (prev == nullptr)
			htable[hashID] = curr;
		else
			prev->m_next = curr;
	}
	else
		curr->m_value = value;	
}

template<typename KeyType, typename ValueType>
int MyHash<KeyType, ValueType>::getNumItems() const
{
	return m_itemNum;
}

template<typename KeyType, typename ValueType>
double MyHash<KeyType, ValueType>::getLoadFactor() const
{
	return (m_itemNum / (double)m_bucketNum);
}

template<typename KeyType, typename ValueType>
const ValueType* MyHash<KeyType, ValueType>::find(const KeyType& key) const
{
	unsigned int hashID = hashFunction(key);
	HashNode* curr = htable[hashID];
	while (curr != nullptr)
	{
			// If found, return pointer to item
		if (curr->m_key == key)
			return curr->m_vptr;
		curr = curr->m_next;
	}
	return nullptr;
}
