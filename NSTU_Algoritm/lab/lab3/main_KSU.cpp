#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

class HashTable {
private:
    struct Entry {
        string key;
        bool occupied;
        bool deleted;
    };

    Entry** table;
    size_t size;
    size_t capacity;

    int hash(const string& key) const {
        unsigned long long num = 0;
        for (char ch : key) {
            num = (num << 5) + ch;
        }
        return num % capacity;
    }

    void rehash() {
        size_t newCapacity = nextPrime(capacity * 2);
        Entry** newTable = new Entry*[newCapacity];
        for (size_t i = 0; i < newCapacity; ++i) {
            newTable[i] = nullptr;
        }

        for (size_t i = 0; i < capacity; ++i) {
            if (table[i] && table[i]->occupied && !table[i]->deleted) {
                int newHashedKey = hash(table[i]->key) % newCapacity;
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

    bool isPrime(int n) {
        if (n < 2) return false;
        for (int i = 2; i * i <= n; ++i) {
            if (n % i == 0) return false;
        }
        return true;
    }

    int nextPrime(int n) {
        while (!isPrime(n)) ++n;
        return n;
    }

public:
    HashTable(size_t cap = 11) : capacity(nextPrime(cap)), size(0) {
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

    bool insert(const string& key) {
        if (size >= capacity * 0.7) {
            rehash();
        }
        int hashedKey = hash(key);
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

    bool search(const string& key) {
        int hashedKey = hash(key);
        for (int i = 0; i < capacity; ++i) {
            int idx = (hashedKey + i * i) % capacity;
            if (!table[idx]) return false;
            if (table[idx]->occupied && !table[idx]->deleted && table[idx]->key == key) return true;
        }
        return false;
    }

    bool remove(const string& key) {
        int hashedKey = hash(key);
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

vector<string> findPotentialPlagiarism(const vector<string>& original, const vector<string>& submitted) {
    HashTable hashTable(original.size() * 2);
    for (const string& text : original) {
        hashTable.insert(text);
    }

    vector<string> result;
    for (const string& text : submitted) {
        if (hashTable.search(text)) {
            result.push_back(text);
        }
    }
    return result;
}

int main() {
    HashTable ht;
    ht.insert("HELLO");
    ht.insert("WORLD");
    ht.insert("TEST");
    ht.display();

    cout << "\nSearching for 'WORLD': " << (ht.search("WORLD") ? "Found" : "Not Found") << endl;
    ht.remove("WORLD");
    cout << "After deletion: " << (ht.search("WORLD") ? "Found" : "Not Found") << endl;

    vector<string> original = {"HELLO", "WORLD", "TEST", "HASH"};
    vector<string> submitted = {"WORLD", "HASH", "FAKE"};

    vector<string> result = findPotentialPlagiarism(original, submitted);
    cout << "\nPotential Plagiarism Found: ";
    for (const string& text : result) {
        cout << text << " ";
    }
    cout << endl;
    return 0;
}
