#include "../../include/index/BpTree.h"
#include "../../include/index/BpNode.h"

// TODO: fix : Remove PageManager and use buffer pool manager instead (storage API)
BpTree::BpTree(PageManager* manager) {
    pm = manager;
    root_page_id = pm->allocate_page();
    char* root_page = pm->get_page(root_page_id);
    memset(root_page, 0, 4096);
    BpLeafNode root(root_page, root_page_id);
    root.get_header()->is_leaf = true;
    root.set_size(0);
    root.get_header()->parent_page_id = -1;
}

int BpTree::get_root_id() {
    return root_page_id;
}

int BpTree::find_leaf(int key) {
    int current_page_id = root_page_id;
    while (true) {
        char* current_page = pm->get_page(current_page_id);
        BpNode current_node(current_page, current_page_id);
        if (current_node.is_leaf()) {
            return current_page_id;
        }
        BpInternalNode internal(current_page, current_page_id);
        int index = internal.find_key_index(key);
        int* children = internal.children_array();
        if (index >= internal.get_size()) {
            index = internal.get_size() -1;
        }
        current_page_id = children[index];
    }
}

RID BpTree::search(int key) {
    int leaf_page_id = find_leaf(key);
    char* leaf_page = pm->get_page(leaf_page_id);
    BpLeafNode node(leaf_page, leaf_page_id);
    int index = node.find_key_index(key);
    if (index < node.get_size() && key == node.get_key(index)) {
        return node.get_value(index);
    }
    return RID{-1, -1};
}
