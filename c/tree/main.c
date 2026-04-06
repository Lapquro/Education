#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int          data;
    struct Node *left;
    struct Node *right;
} Node;

static Node *node_create(int value)
{
    Node *n = (Node *)malloc(sizeof(Node));
    if (!n) { fprintf(stderr, "Memory error\n"); exit(EXIT_FAILURE); }
    n->data  = value;
    n->left  = NULL;
    n->right = NULL;
    return n;
}

static Node *bst_insert(Node *root, int value)
{
    if (!root) return node_create(value);
    if      (value < root->data) root->left  = bst_insert(root->left,  value);
    else if (value > root->data) root->right = bst_insert(root->right, value);
    return root;
}

static void tree_free(Node *root)
{
    if (!root) return;
    tree_free(root->left);
    tree_free(root->right);
    free(root);
}

static void tree_print(const Node *root, int depth)
{
    if (!root) return;
    tree_print(root->right, depth + 1);
    for (int i = 0; i < depth * 4; i++) putchar(' ');
    printf("%d\n", root->data);
    tree_print(root->left, depth + 1);
}

static Node *find_min(Node *root)
{
    while (root->left)
        root = root->left;
    return root;
}

static Node *bst_delete(Node *root, int value)
{
    if (!root)
        return NULL;

    if (value < root->data)
        root->left = bst_delete(root->left, value);
    else if (value > root->data)
        root->right = bst_delete(root->right, value);
    else
    {
        if (!root->left && !root->right)
        {
            free(root);
            return NULL;
        }
        else if (!root->left)
        {
            Node *temp = root->right;
            free(root);
            return temp;
        }
        else if (!root->right)
        {
            Node *temp = root->left;
            free(root);
            return temp;
        }

        {
            Node *min_node = find_min(root->right);
            root->data = min_node->data;
            root->right = bst_delete(root->right, min_node->data);
        }
    }
    return root;
}

static int value_exists(const Node *root, int value)
{
    if (!root) return 0;
    if (value == root->data) return 1;
    if (value < root->data) return value_exists(root->left, value);
    return value_exists(root->right, value);
}

static void find_deepest_nonterminal(const Node *root,
                                     int         depth,
                                     int        *best_depth,
                                     int        *best_value,
                                     int        *found)
{
    if (!root) return;

    if (root->left != NULL || root->right != NULL) {
        if (!(*found) || depth > *best_depth) {
            *best_depth = depth;
            *best_value = root->data;
            *found      = 1;
        }
    }

    find_deepest_nonterminal(root->left,  depth + 1, best_depth, best_value, found);
    find_deepest_nonterminal(root->right, depth + 1, best_depth, best_value, found);
}

static int deepest_nonterminal_value(const Node *root, int *out_value)
{
    int best_depth = 0, found = 0;
    find_deepest_nonterminal(root, 0, &best_depth, out_value, &found);
    return found;
}

int main(void)
{
    Node *root  = NULL;
    int   choice;

    printf("=== Lab 23, Task 6 ===\n");
    printf("Find the deepest non-terminal node in a BST\n\n");

    int defaults[] = {50, 30, 70, 20, 40, 60, 80, 10, 35, 65};
    for (int i = 0; i < 10; i++)
        root = bst_insert(root, defaults[i]);
    printf("Test tree loaded.\n\n");

    do {
        printf("---------------------------------\n");
        printf("1. Add node\n");
        printf("2. Print tree\n");
        printf("3. Run task #6\n");
        printf("4. Delete node\n");
        printf("0. Exit\n");
        printf("---------------------------------\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            choice = -1;
        }

        switch (choice) {
            case 1: {
                int value;
                printf("  Enter value: ");
                if (scanf("%d", &value) == 1) {
                    root = bst_insert(root, value);
                    printf("  Node %d added.\n", value);
                }
                break;
            }
            case 2:
                if (!root) {
                    printf("  Tree is empty.\n");
                } else {
                    printf("\n  Tree (right -> root -> left):\n\n");
                    tree_print(root, 0);
                    putchar('\n');
                }
                break;

            case 3: {
                int value = 0;
                if (!root)
                    printf("  Tree is empty.\n");
                else if (deepest_nonterminal_value(root, &value))
                    printf("\n  Deepest non-terminal node value: %d\n\n", value);
                else
                    printf("\n  No non-terminal nodes (single-node tree).\n\n");
                break;
            }

            case 4: {
                int value;
                if (!root) {
                    printf("  Tree is empty.\n");
                } else {
                    printf("  Enter value to delete: ");
                    if (scanf("%d", &value) == 1) {
                        int found = value_exists(root, value);
                        root = bst_delete(root, value);
                        if (found)
                            printf("  Node %d deleted.\n", value);
                        else
                            printf("  Value %d not found.\n", value);
                    }
                }
                break;
            }

            case 0:
                printf("Bye.\n");
                break;
            default:
                printf("  Unknown option.\n");
        }
    } while (choice != 0);

    tree_free(root);
    return 0;
}