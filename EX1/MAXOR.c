#include <stdio.h>
#include <stdlib.h>
typedef struct TrieNode {
    struct TrieNode* children[2];
} TrieNode;
TrieNode* createNode() {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    node->children[0] = NULL;
    node->children[1] = NULL;
    return node;
}
void insert(TrieNode* root, int num) {
    TrieNode* curr = root;
    for (int i = 30; i >= 0; i--) {
        int bit = (num >> i) & 1;
        if (curr->children[bit] == NULL) {
            curr->children[bit] = createNode();
        }
        curr = curr->children[bit];
    }
}

int getMaxXor(TrieNode* root, int num) {
    TrieNode* curr = root;
    int max_xor_for_num = 0;
    
    for (int i = 30; i >= 0; i--) {
        int bit = (num >> i) & 1;
        int toggled_bit = 1 - bit; 
        
        if (curr->children[toggled_bit] != NULL) {
            max_xor_for_num |= (1 << i);
            curr = curr->children[toggled_bit];
        } else {
            curr = curr->children[bit];
        }
    }
    return max_xor_for_num;
}

void freeTrie(TrieNode* root) {
    if (root == NULL) return;
    freeTrie(root->children[0]);
    freeTrie(root->children[1]);
    free(root);
}

int findMaximumXOR(int* nums, int numsSize) {
    if (numsSize < 2) return 0;
    
    TrieNode* root = createNode();
        for (int i = 0; i < numsSize; i++) {
        insert(root, nums[i]);
    }
    
    int max_overall_xor = 0;
    for (int i = 0; i < numsSize; i++) {
        int current_max = getMaxXor(root, nums[i]);
        if (current_max > max_overall_xor) {
            max_overall_xor = current_max;
        }
    }
    
    freeTrie(root);
    
    return max_overall_xor;
}
