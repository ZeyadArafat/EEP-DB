#include <iostream>

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
    static const int MAX_KEYS;

public:
    BpNode(char* page_data);

    BpNodeHeader* get_header();

    bool is_leaf();

    int get_size();
    void set_size(int new_size);

    int* key_array();
    RID* value_array();

    int get_key(int index);
    void set_key(int index, int value);

    RID get_value(int index);
    void set_value(int index, RID value);

    int find_key_index(int key);

    void shift_right(int index);

    void INSERT(int key, RID value);
};
