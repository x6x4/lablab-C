#include "lib_graph.h"
#include "generic.h"
#include "lib_ll.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>



void print_graph (Graph graph) {
    if (graph->csize == 0)
        printf ("(void)\n");
    
    for (size_t i = 0; i < graph->csize; i++) {
        print_ll (graph->adj_list[i]);
        printf ("\n");
    }

}

void free_graph (Graph graph) {

    for (size_t i = 0; i < graph->csize; i++)
        free_ll (&(graph->adj_list[i]));
}



int add_vertex (Graph graph, char *name, size_t port) {
    
    if (graph->csize == MAX_VERTEX_NUM)
        return ERROVF;

    size_t num = 0;
    if (find_vertex_in_graph (graph, name, &num))
        return ERRDUP;

    Vertex v = new_vertex (name, port);
    insert_to_ll (&(graph->adj_list[graph->csize]), v, NULL);

    graph->csize++;

    return ERRSUC;
}

Vertex new_vertex (char *name, size_t port) {

    Vertex v = calloc (1, sizeof *v);

    v->info = calloc (1, sizeof *(v->info));
    v->info->name = name;
    v->info->port = port;

    v->next = NULL;
    
    return v;
}


int remove_vertex (Graph graph, char *name) {
    size_t num = 0;
    Vertex v = find_vertex_in_graph (graph, name, &num);
    if (!v)
        return ERRWRG;

    Vertex *dest = graph->adj_list + num;
    Vertex *src = graph->adj_list + num + 1;

    free_ll (&(graph->adj_list[num]));

    memcpy (dest, src, (graph->csize - num) * sizeof (Vertex));

    remove_vertex_from_adj_lists (graph, name);
 
    graph->csize--;

    return ERRSUC;
}

void remove_vertex_from_adj_lists (Graph graph, char *name) {
    for (size_t i = 0; i < graph->csize; i++)
        delete_from_ll (&(graph->adj_list[i]), name);
}


void change_vertex_name (V_info v, char *new_name) {
    free_nullify (v->name);
    v->name = new_name;
}

void change_vertex_port (V_info v, size_t new_port) {
    v->port = new_port;
}


Vertex find_vertex_in_graph (Graph graph, char *name, size_t *num) {

    if (graph->csize == 0)
        return NULL;

    for (size_t i = 0; i < graph->csize; i++) {

        if (EQ(graph->adj_list[i]->info->name, name)) {

            if (num) *num = i;

            return graph->adj_list[i];
        }
    }

    return NULL;
}



int add_edge (Graph graph, char *name1, char *name2, size_t *avl_ports, size_t ports_num) {
    
    size_t num1 = 0;
    Vertex v1 = find_vertex_in_graph (graph, name1, &num1);
    if (!v1) 
        return ERRWRG;

    size_t num2 = 0;
    Vertex v2 = find_vertex_in_graph (graph, name2, &num2);
    if (!v2) 
        return ERRWRG;

    v1 = new_vertex (v1->info->name, v1->info->port);
    v2 = new_vertex (v2->info->name, v2->info->port);

    Edge e = new_edge (avl_ports, ports_num);
    v1->weight = e;
    v2->weight = e;
    
    insert_to_ll (&(graph->adj_list[num1]), v2, NULL);
    if (!EQ(name1, name2))
        insert_to_ll (&(graph->adj_list[num2]), v1, NULL);

    return ERRSUC;
}

Edge new_edge (size_t *avl_ports, size_t ports_num) {
    Edge e = calloc (1, sizeof *e);
    e->avl_ports = avl_ports;
    e->ports_num = ports_num;

    return e;
}


int remove_edge (Vertex v1, Vertex v2) {
    return ERRSUC;
}

void change_edge_ports (Vertex v1, Vertex v2, size_t *new_avl_ports) {

}