#include "../../input/input.h"
#include "src/lib_table_hash.h"
#include <stdio.h>
#include <limits.h>

int insert (table_t *table, FILE *file);
int delete_by_key (table_t *table, FILE *file);
int find_by_key (table_t *table, FILE *file);
int print (table_t *table, FILE *file);

#define MAX_KS_SIZE 2

int main () {

    table_t *table = init_table (MAX_KS_SIZE);

    const char *msgs [] = {"\n0 - quit\n", "1 - insert\n", "2 - delete by key\n",
                           "3 - find by key\n", "4 - print\n"};

    const size_t msgc = sizeof msgs / sizeof msgs[0];

    int (*fptr[]) (table_t*, FILE *file) = 
        {NULL, insert, delete_by_key, 
         find_by_key, print};

    puts ("Table ui. ^D to quit.");

    FILE *file = user_file ();
    if (!file) {
        free_table (table);
        puts ("quit");
        return 0;
    }

    size_t fn_num = 0;

    while (fn_num = option_choice (msgs, msgc, file)) {
        //  each function returns 0 if EOF
        if (!fptr[fn_num](table, file))
            break;
    }

    free_table (table);
    puts ("quit");
    return 0;
}

int insert (table_t *table, FILE *file) {
    char key [50] = {};
    printf ("\nEnter key of item to insert: ");

    if (!fscanf (file, "%50s", key)) 
        return 0;

    size_t val = 0;
    printf ("Enter value of item to insert: ");
    char *errmsg = "";
    int status = -1;

    do {
        puts (errmsg);
        errmsg = "Bad input";
        status = get_int_file (&val, file, INT_MAX, 0);

        if (status == ERREOF)
            return 0;
    
    } while (status == ERRWRG);

    if (insert_table (table, key, val) == ERRSUC) 
        printf ("Item inserted successfully.\n");

    return 1; 
}

int delete_by_key (table_t *table, FILE *file) {
    char key [50] = {};
    printf ("\nEnter key of keyspace to delete: ");
    if (!fscanf (file, "%50s", key)) 
        return 0;

    if (erase_from_table_by_key (table, key) == ERRSUC) 
        printf ("Keyspace deleted successfully.");
    else 
        printf ("Keyspace not found.\n");

    return 1; 
}

int find_by_key (table_t *table, FILE *file) {
    char key [50] = {};
    printf ("\nEnter key of keyspace to find: ");
    if (!fscanf (file, "%50s", key)) 
        return 0;

    table_t *key_table = nodes_by_key (table, key);
    if (!key_table) 
        printf ("Key not found.\n");
    else {
        print_table (key_table);
        free_table (key_table);
    }

    return 1; 
}

int print (table_t *table, FILE *file) {
    printf ("Table:\n");
    print_table (table);
    return 1;
}