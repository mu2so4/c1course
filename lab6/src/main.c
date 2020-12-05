#include <stdio.h>
#include <stdlib.h>
typedef struct SearchTree SearchTree;

struct SearchTree {
    int value;
    char depth;
    SearchTree * left, * right;
};

SearchTree * createNode(int value) {
    SearchTree * node = (SearchTree *) malloc(sizeof(SearchTree));
    if(node == NULL) {
        printf("Out of memory\n");
        abort();
    }
    node->left = node->right = NULL;
    node->depth = 1;
    node->value = value;
    return node;
}

char putDepth(SearchTree * tree) {
    if(tree == NULL) {
        return 0;
    }
    char left = (tree->left == 0 ? 0 : tree->left->depth);
    char right = (tree->right == 0 ? 0 : tree->right->depth);
    tree->depth = (left > right ? left : right) + 1;
    return right - left;
}

char getDif(const SearchTree * tree) {
    if(tree == NULL) {
        return 0;
    }
    char left = (tree->left == NULL ? 0 : tree->left->depth);
    char right = (tree->right == NULL ? 0 : tree->right->depth);
    return right - left;
}

void destroyTree(SearchTree * root) {
    if(root == NULL) {
        return;
    }
    destroyTree(root->left);
    destroyTree(root->right);
    free(root);
}

void simpleLeftRotation(SearchTree ** root) {
    SearchTree * p = (*root)->right->left;
    (*root)->right->left = *root;
    *root = (*root)->right;
    (*root)->left->right = p;
    putDepth((*root)->left);
    putDepth(*root);
}

void simpleRightRotation(SearchTree ** root) {
    SearchTree * p = (*root)->left->right;
    (*root)->left->right = *root;
    *root = (*root)->left;
    (*root)->right->left = p;
    putDepth((*root)->right);
    putDepth(*root);
}

void rightLeftRotation(SearchTree ** root) {
    simpleRightRotation(&((*root)->right));
    simpleLeftRotation(root);
}

void leftRightRotation(SearchTree ** root) {
    simpleLeftRotation(&((*root)->left));
    simpleRightRotation(root);
}

void addNode(SearchTree ** root, int value) {
    if(*root == NULL) {
        *root = createNode(value);
        return;
    }
    if(value < (*root)->value) {
        addNode(&((*root)->left), value);
        if(putDepth(*root) == -2) {
            if(getDif((*root)->left) <= 0) {
                simpleRightRotation(root);
            }
            else {
                leftRightRotation(root);
            }
        }
    }
    else {
        addNode(&((*root)->right), value);
        if(putDepth(*root) == 2) {
            if(getDif((*root)->right) >= 0) {
                simpleLeftRotation(root);
            }
            else {
                rightLeftRotation(root);
            }
        }
    }
}

int main() {
    SearchTree * tree = NULL;
    int count;
    if(scanf("%d\n", &count) < 1) {
        return 0;
    }
    for(int index = 0; index < count; index++) {
        int number;
        if(scanf("%d", &number) < 1) {
            return 0;
        }
        addNode(&tree, number);
    }
    if(!count) {
        printf("0\n");
    }
    else {
        printf("%d\n", tree->depth);
    }
    destroyTree(tree);
    return 0;
}
