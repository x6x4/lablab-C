#include "src/lib_bintree.h"
#include "src/generic.h"
#include <stddef.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

int insert_tree (BNodePtr *root, FILE *file);
int delete_tree (BNodePtr *root, FILE *file);
int find_tree (BNodePtr *root, FILE *file);
int findmax_tree (BNodePtr *root, FILE *file);
int traverse_tree (BNodePtr *root, FILE *file);
int import_tree (BNodePtr *root, FILE *file);
int print_tree (BNodePtr *root, FILE *file);

/*  GLOBAL VARIABLES  */

const char *msgs [] = {"\n0 - quit", "1 - insert", "2 - delete by key+version",
                       "3 - find by key", "4 - find max", "5 - traverse", 
                       "6 - import", "7 - print"};
const size_t msgc = sizeof msgs / sizeof msgs[0];

int (*fptr[]) (BNodePtr*, FILE*)  = {NULL, insert_tree, delete_tree, 
                                      find_tree, findmax_tree, traverse_tree,
                                      import_tree, print_tree};

/*  MAIN  */

int main () {

    FILE *file = stdin;
    BNodePtr root = NULL;
    int fn_num = 0;
    printf ("B23 tree UI.\n");

    while ((fn_num = option_choice (msgs, msgc, file))) {
        if (fptr[fn_num](&root, file) == ERREOF)
            break;
    }

    fclose (file);
    free_bt (&root);
    puts ("quit");
    return 0;
}

int insert_tree (BNodePtr *root, FILE *file) {
    
    char *key = NULL, *val = NULL;
    const char *s = "Enter key of item to insert: \n";

    printf ("%s", s);

    key = get_str (file);
    if (!key)
        return ERREOF;

    s = "Enter value of item to insert: \n";

    printf ("%s", s);

    val = get_str (file);
    if (!val)
        return ERREOF;   

    switch (insert_bt (root, key, val)) {
        case ERRSUC:
            printf ("\nItem inserted successfully.\n");
            break;
        case ERRDUP:
            printf ("Duplicate key/value.\n");
            break;
        default:
            printf (RED("Unknown error."));
    }

    free_nullify (key);
    free_nullify (val);

    return ERRSUC; 
};

int delete_tree (BNodePtr *root, FILE *file) {

    char *key = NULL, *val = NULL;
    size_t ver = 0;
    const char *s = "Enter key of item to delete: \n";

    printf ("%s", s);
     
    key = get_str (file);
    if (!key)
        return ERREOF;

    /*s = "Enter version of item to delete: \n";

    printf ("%s", s);
    
    status = get_int_file (file, &ver, INT_MAX, 0);
    if (status == ERREOF) 
        return 0;  */

    switch (delete_bt (root, key, ver)) {
        case ERRSUC:
            printf ("\nItem deleted successfully.\n");
            break;
        case ERRWRG:
            printf ("No such key or version.\n");
            break;
        default:
            printf (RED("Unknown error."));
    }

    print_tree (root, file);

    free (key);

    return ERRSUC; 
};

int find_tree (BNodePtr *root, FILE *file) {
    BNodePtr node = NULL;
    size_t pos = 0;

    char *key = NULL;
    const char *s = "Enter key of item to find: \n";

    printf ("%s", s);
    
    key = get_str (file);
    if (!key)
        return ERREOF;

    node = find_bt (*root, key, &pos);

    if (node) 
        colored_print_bt (*root, key);
    else 
        printf ("No such key\n");
    
    free (key);

    return ERRSUC;
};

int findmax_tree (BNodePtr *root, FILE *file) {
    BNodePtr node = find_max_node (*root);

    if (node) {
        size_t pos = node->csize - 1;
        printf (" %s (%lu) \n", node->info[pos]->key, node->info[pos]->csize);
    }
    else
        puts ("Void tree.");

    return ERRSUC;
};

int traverse_tree (BNodePtr *root, FILE *file) {
/*    size_t key = 0;
    int status = -1;
    const char *s = "Enter key to traverse from or \"a\" to traverse all tree: \n";

    printf ("%s", s);
    char ch = 0;
    scanf ("%c", &ch);

    if (ch != 'a') {
        ungetc (ch, stdin);
        status = get_int_file (file, &key, INT_MAX, 0);
        if (status == ERREOF) 
            return 0;
    } else {
        key = NO_KEY;
    }

    printf ("\nTraverse:\n");
    traverse_bst (*root, key);
    printf ("\n");*/
    return ERRSUC;
}

int import_tree (BNodePtr *root, FILE *file) {
    FILE *import = user_file ();
    if (!import) 
        return ERREOF;

    while (insert_tree (root, import) != ERREOF);

    fclose (import);

    return ERRSUC;
}

int print_tree (BNodePtr *root, FILE *file) {
    printf ("\nTree:\n");
    if (!(*root) || (*root)->csize == 0) {
        printf ("(void)\n");
        return 1;
    }

    (*root)->height = 0;
    colored_print_bt (*root, NULL);
    printf ("\n");

    /*int status = -1;
    const char *s = "\nUnwrap some branch? (y/n): \n";
    while ((status = user_choice (s)) == ERRSUC) 
        find_tree (root, file);

    if (status == ERREOF)
        return ERREOF;
    else */
        return ERRSUC;
};




