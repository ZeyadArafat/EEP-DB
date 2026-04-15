#include <iostream>
#include <ostream>
#include <cstring>

struct BpNodeHeader {
    bool is_leaf;
    int num_keys;
    int parent_page_id;
    int next_page_id;
};

struct RID {
    int page_id;
    int slot_id;
};

class BpNode {
private:
    char* data;
    static const int MAX_KEYS = (4096 - sizeof(BpNodeHeader)) / (sizeof(int) + sizeof(RID));
public:
    BpNode(char* page_date) {
        data = page_date;
    }
    BpNodeHeader* get_header() {
        return reinterpret_cast<BpNodeHeader*>(data);
    }
    bool is_leaf() {
        return get_header()->is_leaf;
    }
    int get_size() {
        return get_header()->num_keys;
    }
    void set_size(int new_size) {
        get_header()->num_keys = new_size;
    }
    int* key_array() {
        return reinterpret_cast<int*>(data + sizeof(BpNodeHeader));
    }
    RID* value_array() {
        return reinterpret_cast<RID*>(data + sizeof(BpNodeHeader) + MAX_KEYS * sizeof(int));
    }
    int get_key(int index) {
        if (index < 0 || index >= get_size()) {
            std::cerr << "index out of bounds" << std::endl;
            return -1;
        }
        return key_array()[index];
    }
    void set_key(int index, int value) {
        if (index < 0 || index >= get_size()) {
            std::cerr << "index out of bounds" << std::endl;
            return;
        }
        key_array()[index] = value;
    }
    RID get_value(int index) {
        if (index < 0 || index >= get_size()) {
            std::cerr << "index out of bounds" << std::endl;
            return RID();
        }
        return value_array()[index];
    }
    void set_value(int index, RID value) {
        if (index < 0 || index >= get_size()) {
            std::cerr << "index out of bounds" << std::endl;
            return;
        }
        value_array()[index] = value;
    }
    int find_key_index(int key) {
        int left = 0;
        int right = get_size() - 1;
        while (left <= right) {
            int mid = (left + right) / 2;
            if (get_key(mid) == key) {
                return mid;
            }
            else if (get_key(mid) < key) {
                left = mid + 1;
            }
            else {
                right = mid - 1;
            }
        }
        return left;
    }
    void shift_right(int index) {
        for (int i = get_size(); i > index; i--) {
            set_key(i, get_key(i - 1));
            set_value(i, get_value(i - 1));
        }
    }
    void INSERT(int key, RID value) {
        if (get_size() >= MAX_KEYS) {
            std::cerr << "Too many keys inserted" << std::endl;
            return;
        }
        int index = find_key_index(key);
        shift_right(index);
        set_key(index, key);
        set_value(index, value);
        set_size(get_size() + 1);
    }
};

int main() {
    char page[4096];
    memset(page, 0, 4096);
    BpNode node(page);

    node.set_size(0);

    node.INSERT(10, {1,1});
    node.INSERT(5, {2,2});
    node.INSERT(20, {3,3});
    int* values = node.key_array();
        for (int i = 0; i < 3; ++i) {
        std::cout << values[i] << std::endl;
    }
}