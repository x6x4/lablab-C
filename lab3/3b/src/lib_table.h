#include <stdlib.h>

#define KSLIST_MAX_SZ 2

/*  To init last_node_offset in table creation.  */
#define ks_offset(i) BEGIN+TABLE+KS*(i-1)
/*  For new node creation.  */
#define ks_key_offset(i) ks_offset(i) + 2*sizeof (size_t)

#define BEGIN 0x10
#define TABLE 0x20
#define KS 0x40
#define NODE 0x20

typedef struct table_ram table_ram;
typedef struct keyspace ks_t;
typedef struct node_d node_d;

typedef size_t offset_t;

/*  Table descriptor in ram.  */
struct table_ram {
    size_t sz;
    size_t kslist_sz;
    size_t kslist_max_sz;
    offset_t last_node_offset;

    ks_t *kslist;
};

/*  Keyspace descriptor.  */
struct keyspace {
    size_t num;
    size_t ks_sz;
    char key [40];
    offset_t tail;
};

/*  Linked list node.  */
struct node_d
{
    size_t ver;
    offset_t next;
    offset_t key;
    size_t val;
};

