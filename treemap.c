#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap*)malloc(sizeof(TreeMap));
    if (new == NULL) return NULL;

    new-> root = NULL;
    new-> current = NULL;
    new->lower_than = lower_than;

    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree == NULL) return;

    if (searchTreeMap(tree, key) != NULL) return;

    TreeNode * nuevoNodo = createTreeNode(key, value);
    if (nuevoNodo == NULL) return;

    if (tree->root == NULL) {
        tree->root = nuevoNodo;
        tree->current = nuevoNodo;
        return;
    }

    TreeNode * papito = NULL;
    TreeNode * actual = tree->root;
    
    while (actual != NULL) {
        papito = actual;
        if (tree->lower_than(key, actual-> pair -> key)) {
            actual = actual -> left;
        } else {
            actual = actual -> right;
        }
    }

    nuevoNodo-> parent = papito;
    if (tree->lower_than(key, papito -> pair -> key)) {
        papito -> left = nuevoNodo;
    } else {
        papito -> right = nuevoNodo;
    }

    tree-> current = nuevoNodo;
}

TreeNode * minimum(TreeNode * x){
    if (x == NULL) return NULL;

    while (x -> left != NULL) {
        x = x -> left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (node == NULL) return;

    if (node -> left == NULL && node -> right == NULL) {
        if (node -> parent == NULL) {
            tree -> root = NULL;
        } else {
            if (node -> parent -> left == node) {
                node -> parent -> left = NULL;
            } else {
                node -> parent -> right = NULL;
            }
        }
        free (node -> pair);
        free (node);
        return;
    }

    if (node -> left == NULL || node -> right == NULL) {
        TreeNode * hijou = (node -> left != NULL) ? node -> left : node -> right;

        if (node -> parent == NULL) {
            tree -> root = hijou;
        } else {
            if (node -> parent -> left == node) {
                node -> parent -> left = hijou;
            } else {
                node -> parent -> right = hijou;
            }
        }
        hijou -> parent = node -> parent;
        free (node -> pair);
        free (node);
        return;
    }

    TreeNode * sucesor = minimum(node -> right);
    node -> pair -> key = sucesor -> pair -> key;
    node -> pair -> value = sucesor -> pair -> value;
    removeNode(tree, sucesor);
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL || tree-> root == NULL) return NULL;
    TreeNode * actual = tree-> root;

    while (actual != NULL) {
        if (is_equal(tree, key, actual -> pair -> key)) {
            tree -> current = actual;
            return actual -> pair;
        } else if (tree -> lower_than(key, actual -> pair -> key)) {
            actual = actual -> left;
        } else {
            actual = actual -> right;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree -> root == NULL) return NULL;

    TreeNode * minimo = minimum(tree -> root);

    tree -> current = minimo;
    return minimo -> pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree -> current == NULL) return NULL;

    TreeNode * current = tree -> current;

    if (current -> right != NULL) {
        TreeNode * sucesor = minumum(current -> right);
        tree -> current = sucesor;
        return sucesor -> pair;
    }

    TreeNode * parent = current -> parent;
    while (parent != NULL && current == parent -> right) {
        current = parent;
        parent = parent -> parent;
    }

    tree -> current = parent;
    return (parent != NULL) ? parent -> pair : NULL;
}
