#include <stdio.h>
#include <stdlib.h>
typedef struct SearchTree SearchTree;

struct SearchTree {
    int value;
    char depth;
    SearchTree * left, * right;
};

SearchTree createNode(int value) {
    SearchTree node = {value, 1, NULL, NULL};
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

char getDisbalance(const SearchTree * tree) {
    if(tree == NULL) {
        return 0;
    }
    char left = (tree->left == NULL ? 0 : tree->left->depth);
    char right = (tree->right == NULL ? 0 : tree->right->depth);
    return right - left;
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

void addNode(SearchTree ** root, SearchTree * node) {
    if(!*root) {
        *root = node;
        return;
    }
    if(node->value < (*root)->value) {
        addNode(&((*root)->left), node);
        if(putDepth(*root) == -2) {
            if(getDisbalance((*root)->left) <= 0) {
                simpleRightRotation(root);
            }
            else {
                leftRightRotation(root);
            }
        }
    }
    else {
        addNode(&((*root)->right), node);
        if(putDepth(*root) == 2) {
            if(getDisbalance((*root)->right) >= 0) {
                simpleLeftRotation(root);
            }
            else {
                rightLeftRotation(root);
            }
        }
    }
}

int main() {
    int count;
    if(scanf("%d\n", &count) < 1) {
        return 0;
    }
    if(!count) {
        printf("0\n");
        return 0;
    }

    SearchTree * array = (SearchTree *) malloc(sizeof(SearchTree) * count);
    if(!array) {
        printf("Out of memory\n");
        abort();
    }
    SearchTree * tree = NULL;
    for(int index = 0; index < count; index++) {
        int number;
        if(scanf("%d", &number) < 1) {
            free(array);
            return 0;
        }
        array[index] = createNode(number);
        addNode(&tree, &array[index]);
    }
    printf("%d\n", tree->depth);

    free(array);
    return 0;
}
