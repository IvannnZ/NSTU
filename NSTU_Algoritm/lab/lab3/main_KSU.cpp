#include <iostream>
#include <vector>

using namespace std;

class HashTable {
private:
    struct Entry {
        int key;
        bool occupied;
        bool deleted;
    };

    Entry** table;
    size_t size;
    size_t capacity;

    int hash(int key) const
    {
        return key % capacity;
    }

    static int foldKey(int key)
    {
        return (key / 10000) + (key % 10000);
    }

    void rehash() {
        const size_t newCapacity = capacity * 2;
        Entry** newTable = new Entry*[newCapacity];
        for (size_t i = 0; i < newCapacity; ++i) {
            newTable[i] = nullptr;
        }

        for (size_t i = 0; i < capacity; ++i) {
            if (table[i] && table[i]->occupied && !table[i]->deleted) {
                const int newHashedKey = table[i]->key % newCapacity;
                for (int j = 0; j < newCapacity; ++j) {
                    int idx = (newHashedKey + j * j) % newCapacity;
                    if (!newTable[idx]) {
                        newTable[idx] = new Entry{table[i]->key, true, false};
                        break;
                    }
                }
            }
        }

        for (size_t i = 0; i < capacity; ++i) {
            delete table[i];
        }
        delete[] table;

        table = newTable;
        capacity = newCapacity;
    }

public:
    HashTable(size_t cap) : capacity(cap), size(0) {
        table = new Entry*[capacity];
        for (size_t i = 0; i < capacity; ++i) {
            table[i] = nullptr;
        }
    }


    HashTable() : capacity(10), size(0) {
        table = new Entry*[capacity];
        for (size_t i = 0; i < capacity; ++i) {
            table[i] = nullptr;
        }
    }

    ~HashTable() {
        for (size_t i = 0; i < capacity; ++i) {
            delete table[i];
        }
        delete[] table;
    }

    bool insert(int key) {
        if (size >= capacity * 0.7) {
            rehash();
        }
        int hashedKey = hash(foldKey(key));
        for (int i = 0; i < capacity; ++i) {
            int idx = (hashedKey + i * i) % capacity;
            if (!table[idx] || table[idx]->deleted) {
                if (!table[idx]) {
                    table[idx] = new Entry{key, true, false};
                } else {
                    table[idx]->key = key;
                    table[idx]->occupied = true;
                    table[idx]->deleted = false;
                }
                ++size;
                return true;
            }
        }
        return false;
    }

    bool search(int key) {
        int hashedKey = hash(foldKey(key));
        for (int i = 0; i < capacity; ++i) {
            int idx = (hashedKey + i * i) % capacity;
            if (!table[idx]) return false;
            if (table[idx]->occupied && !table[idx]->deleted && table[idx]->key == key) return true;
        }
        return false;
    }

    bool remove(int key) {
        int hashedKey = hash(foldKey(key));
        for (int i = 0; i < capacity; ++i) {
            int idx = (hashedKey + i * i) % capacity;
            if (!table[idx]) return false;
            if (table[idx]->occupied && !table[idx]->deleted && table[idx]->key == key) {
                table[idx]->deleted = true;
                --size;
                return true;
            }
        }
        return false;
    }

    size_t getSize() const {
        return size;
    }

    size_t getCapacity() const {
        return capacity;
    }

    bool isEmpty() const {
        return size == 0;
    }

    void clear() {
        for (size_t i = 0; i < capacity; ++i) {
            delete table[i];
            table[i] = nullptr;
        }
        size = 0;
    }

    void display() const {
        for (size_t i = 0; i < capacity; ++i) {
            if (table[i] && !table[i]->deleted) {
                cout << i << ": " << table[i]->key << endl;
            }
        }
    }
};

vector<int> findUniqueInFirstNotInSecond(const vector<int>& arr1, const vector<int>& arr2) {
    HashTable hashTable(arr2.size() * 2);
    for (int num : arr2) {
        hashTable.insert(num);
    }

    vector<int> result;
    for (int num : arr1) {
        if (!hashTable.search(num)) {
            result.push_back(num);
        }
    }
    return result;
}

int main() {
    HashTable ht(10);
    ht.insert(100000001);
    ht.insert(200000002);
    ht.insert(150000003);
    ht.display();

    cout << "\nSearching for 200000002: " << (ht.search(200000002) ? "Found" : "Not Found") << endl;
    ht.remove(200000002);
    cout << "After deletion: " << (ht.search(200000002) ? "Found" : "Not Found") << endl;

    vector<int> arr1 = {100000001, 200000002, 300000003, 400000004};
    vector<int> arr2 = {200000002, 400000004};

    vector<int> result = findUniqueInFirstNotInSecond(arr1, arr2);
    cout << "\nElements in arr1 but not in arr2: ";
    for (int num : result) {
        cout << num << " ";
    }
    cout << endl;
    return 0;
}


// спросить про capacity, должно ли оно быть простым