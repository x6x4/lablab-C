#include "lib_table_hash.h"

#include <stdio.h>
#include <string.h>

enum {
	ERREOF = -1,
	ERRSUC,
	ERRWRG
};

size_t hash_func (char *key, table_t *table) {
    if (key) {
        return (key[0] % (table->hsvec_max_sz) + 1);
    }
    else 
        return 0;
}

int insert_checks (table_t *table, char *key, int val) {

    int hash = hash_func (key, table);

    if (!(table->hsvec[hash - 1])) 
        return ERRSUC;

    /*  Check hashspace for duplication.  */
    else {
        ks_t *cur_node = table->hsvec[hash - 1]->next;
        ks_t *head = cur_node;

        do {
            if (!strcmp (cur_node->key, key)) {
                printf ("Error: repeated key\n");
                return ERRWRG;
            }
            if (cur_node->info->val == val) {
                printf ("Error: repeated value\n");
                return ERRWRG;
            }
            cur_node = cur_node->next;
        } while (cur_node != head);
    }

    return ERRSUC;
}

int insert_table (table_t *table, char *key, int val) {

    if (insert_checks (table, key, val) == ERRWRG)
        return ERRWRG;

    new_ks (table, key, val);

    return ERRSUC;
}

void print_table (table_t *table) {

    if (!table) {
        return;
    }

    for (size_t i = 0; i < table->hsvec_max_sz; i++) {

        if (!(table->hsvec[i])) {
            printf ("Hash %lu: (void)\n", i+1);
            continue;
        }
        
        printf ("Hash %lu:\n", i+1);

            ks_t *cur_node = table->hsvec[i]->next;
            ks_t *head = cur_node;

            do {

                printf ("(\"%s\", %lu)", cur_node->info->key, cur_node->info->val);

                if (cur_node->next != head) {
                    printf ("-> ");
                }

                cur_node = cur_node->next;
            } while (cur_node != head);

            printf ("\n");
    }
}

table_t *nodes_by_key (table_t *table, char *key) {

    int hash = hash_func (key, table);

    if (!(table->hsvec[hash - 1])) 
        return NULL;

    table_t *key_table = init_table (1);

    ks_t *cur_node = table->hsvec[hash - 1]->next;
    ks_t *head = cur_node; 

    do { 
        if (!strcmp (cur_node->info->key, key)) {
            insert_table (key_table, key, cur_node->info->val);
            return key_table;
        }
        cur_node = cur_node->next;
    } while (cur_node != head);

    return NULL;
}

table_t *init_table (size_t klist_max_sz) {

    table_t *table = calloc (1, sizeof *table);

    table->hsvec_max_sz = klist_max_sz;
    table->hsvec = calloc (klist_max_sz, sizeof *(table->hsvec));

    return table;
}

void new_ks (table_t *table, char *key, int val) {
    
    ks_t *ks = calloc (1, sizeof *ks);
    ks->key = calloc (1, strlen (key) + 1);
    strncpy (ks->key, key, strlen (key) + 1);

    ks->info = new_info (key, val);

    int hash = hash_func (key, table);

    if (table->hsvec[hash - 1]) {
        ks_t *tail = table->hsvec[hash - 1];

        ks->next = tail->next;
        tail->next = ks;
        table->hsvec[hash - 1] = ks;

    } else {
        table->hsvec[hash - 1] = ks;
        ks->next = ks;
    }
    
    return;
}

info_t *new_info (char *key, int val) {
    info_t *info = calloc (1, sizeof *info); 
    info->val = val;
    info->key = calloc (1, strlen (key) + 1);
    strncpy (info->key, key, strlen (key) + 1);
    return info;
}


void free_table (table_t *table) {

    for (int i = table->hsvec_max_sz - 1; i >= 0; i--) {
        if (!(table->hsvec[i])) 
            continue;
        ks_t *cur_node = table->hsvec[i]->next;
        ks_t *head = cur_node; 
        do { 
            ks_t *next_node = cur_node->next;
            free (cur_node->info->key);
            free (cur_node->info);
            free (cur_node->key);
            free (cur_node);
            cur_node = next_node;
        } while (cur_node != head);
    }

    free (table->hsvec);
    free (table);
}

int erase_from_table_by_key (table_t *table, char *key) {

    int hash = hash_func (key, table);

    if (!(table->hsvec[hash - 1])) 
        return ERRWRG;

    ks_t *cur_node = table->hsvec[hash - 1]->next;
    ks_t *head = cur_node; 

    do { 
        if (!strcmp (cur_node->next->info->key, key)) {

            ks_t *node_to_free = cur_node->next;

            /*  Hs_sz = 1  */
            if (cur_node == cur_node->next) 
                table->hsvec[hash - 1] = NULL;
            else {    
                cur_node->next = node_to_free->next;

                if (node_to_free->next == head) {
                    table->hsvec[hash - 1] = cur_node;
                }
            }

            free (node_to_free->info->key);
            free (node_to_free->info);
            free (node_to_free->key);
            free (node_to_free);

            return ERRSUC;
        }
        cur_node = cur_node->next;
    } while (cur_node != head);

    return ERRWRG;
}


