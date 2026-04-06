#include <stdio.h>
#include <stdlib.h>

typedef enum {
    STILL_ALIVE,
    FART_SILENT,
    FART_AUDIBLE,
    FART_STINKY,
    FART_WET,
    FART_BOSS,
    FART_ELEVATOR,
    FART_DATE,
    FART_FUNERAL,
    FART_DIMENSIONAL
} FartLevel;

const char *fart_name(FartLevel f)
{
    switch (f) {
        case STILL_ALIVE:       return "STILL_ALIVE";
        case FART_SILENT:       return "FART_SILENT";
        case FART_AUDIBLE:      return "FART_AUDIBLE";
        case FART_STINKY:       return "FART_STINKY";
        case FART_WET:          return "FART_WET";
        case FART_BOSS:         return "FART_BOSS";
        case FART_ELEVATOR:     return "FART_ELEVATOR";
        case FART_DATE:         return "FART_DATE";
        case FART_FUNERAL:      return "FART_FUNERAL";
        case FART_DIMENSIONAL:  return "FART_DIMENSIONAL";
        default:                return "UNKNOWN";
    }
}

typedef struct Node {
    FartLevel    data;
    struct Node *left;
    struct Node *right;
} Node;

static Node *node_create(FartLevel value)
{
    Node *n = (Node *)malloc(sizeof(Node));
    if (!n) { fprintf(stderr, "Memory error\n"); exit(EXIT_FAILURE); }
    n->data  = value;
    n->left  = NULL;
    n->right = NULL;
    return n;
}

static Node *bst_insert(Node *root, FartLevel value)
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
    printf("%s\n", fart_name(root->data));
    tree_print(root->left, depth + 1);
}

static Node *find_min(Node *root)
{
    while (root->left)
        root = root->left;
    return root;
}

static Node *bst_delete(Node *root, FartLevel value)
{
    if (!root) return NULL;

    if (value < root->data)
        root->left = bst_delete(root->left, value);
    else if (value > root->data)
        root->right = bst_delete(root->right, value);
    else {
        if (!root->left && !root->right) {
            free(root);
            return NULL;
        } else if (!root->left) {
            Node *temp = root->right;
            free(root);
            return temp;
        } else if (!root->right) {
            Node *temp = root->left;
            free(root);
            return temp;
        } else {
            Node *min_node = find_min(root->right);
            root->data = min_node->data;
            root->right = bst_delete(root->right, min_node->data);
        }
    }
    return root;
}

static int value_exists(const Node *root, FartLevel value)
{
    if (!root) return 0;
    if (value == root->data) return 1;
    if (value < root->data) return value_exists(root->left, value);
    return value_exists(root->right, value);
}

static void find_deepest_nonterminal(const Node *root,
                                     int         depth,
                                     int        *best_depth,
                                     FartLevel  *best_value,
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

static int deepest_nonterminal_value(const Node *root, FartLevel *out_value)
{
    int best_depth = 0, found = 0;
    find_deepest_nonterminal(root, 0, &best_depth, out_value, &found);
    return found;
}

static void print_menu(void)
{
    printf("---------------------------------\n");
    printf("1. Add node\n");
    printf("2. Print tree\n");
    printf("3. Run task #6\n");
    printf("4. Delete node\n");
    printf("5. List all levels\n");
    printf("0. Exit\n");
    printf("---------------------------------\n");
    printf("Choice: ");
}

static void print_all_levels(void)
{
    printf("\n  Available FartLevel values:\n");
    for (int i = STILL_ALIVE; i <= FART_DIMENSIONAL; i++)
        printf("  %2d = %s\n", i, fart_name((FartLevel)i));
    putchar('\n');
}

int main(void)
{
    Node *root = NULL;
    int   choice;

    printf("=== Lab 23, Task 6 ===\n");
    printf("Find the deepest non-terminal node in a BST\n\n");

    FartLevel defaults[] = {
        FART_BOSS, FART_AUDIBLE, FART_DIMENSIONAL,
        FART_SILENT, FART_STINKY, FART_ELEVATOR,
        STILL_ALIVE, FART_WET, FART_DATE, FART_FUNERAL
    };
    for (int i = 0; i < 10; i++)
        root = bst_insert(root, defaults[i]);
    printf("Test tree loaded.\n\n");

    do {
        print_menu();

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            choice = -1;
        }

        switch (choice) {
            case 1: {
                int raw;
                printf("  Enter level (0-%d): ", FART_DIMENSIONAL);
                if (scanf("%d", &raw) == 1) {
                    if (raw < STILL_ALIVE || raw > FART_DIMENSIONAL) {
                        printf("  Invalid level.\n");
                    } else {
                        root = bst_insert(root, (FartLevel)raw);
                        printf("  Node %s added.\n", fart_name((FartLevel)raw));
                    }
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
                FartLevel value = STILL_ALIVE;
                if (!root)
                    printf("  Tree is empty.\n");
                else if (deepest_nonterminal_value(root, &value))
                    printf("\n  Deepest non-terminal node: %s\n\n", fart_name(value));
                else
                    printf("\n  No non-terminal nodes (single-node tree).\n\n");
                break;
            }

            case 4: {
                int raw;
                if (!root) {
                    printf("  Tree is empty.\n");
                } else {
                    printf("  Enter level to delete (0-%d): ", FART_DIMENSIONAL);
                    if (scanf("%d", &raw) == 1) {
                        if (raw < STILL_ALIVE || raw > FART_DIMENSIONAL) {
                            printf("  Invalid level.\n");
                        } else {
                            FartLevel val = (FartLevel)raw;
                            int found = value_exists(root, val);
                            root = bst_delete(root, val);
                            if (found)
                                printf("  Node %s deleted.\n", fart_name(val));
                            else
                                printf("  Value %s not found.\n", fart_name(val));
                        }
                    }
                }
                break;
            }

            case 5:
                print_all_levels();
                break;

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