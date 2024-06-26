#include "dialog.h"
#include "generic.h"
#include "lib_graph.h"
#include "lib_ll.h"
#include <complex.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>



/*  DIALOG  */

void dialog (Dialog_info info, int (*fptr[]) (Graph, FILE*), Graph graph, FILE* file) {

    int fn_num = 0;
    
    puts (info->greeting);

    while ((fn_num = option_choice (info->msgs, info->msgc, file))) {
        if (fptr[fn_num](graph, file) == ERREOF)
            break;
    }

    puts (info->exit_msg);
}

/*  Format of data file: * <vertices> * <edges>  */
int Import (Graph graph, FILE *file) {

    FILE *data = user_file();

    if (fgetc (data) == '*') {
        /*  eat trailing newline  */
        fgetc (data);
        while (InsertVertex (graph, data) != ERREOF);
    }

    while (InsertEdge (graph, data) != ERREOF);

    fclose (data);

    return ERRSUC;
}



/*||||||||||||||||||||||||| < GENERAL FUNCTIONS > |||||||||||||||||||||||||*/


/*  INSERTION  */

int (*fptr_I[]) (Graph, FILE*)  = {NULL, InsertVertex, InsertEdge};

static size_t dump_num = 0;
int Dump (Graph graph, FILE *file) {

    char error_string[255] = {};
    char dump_name[127] = {};
    system ("mkdir -p dumps");

    sprintf (dump_name, "dumps/tree_dump_%lu.dot",dump_num);

    FILE *dump_file = fopen (dump_name, "w");
    if (!dump_file) perror (error_string);

    fprintf (dump_file, "graph {\n");
    dump_graph (graph, dump_file);
    fprintf (dump_file, "}\n");

    fclose (dump_file);

    char cmd[255] = {};
    sprintf (cmd, "dot %s -T png -o dumps/dump_%lu.png", dump_name, dump_num++);
    system(cmd);

    return ERRSUC;
}

void dump_graph (Graph G, FILE *file) {

    if (!G || !G->sz) {
        fprintf (file, "NULL\n");
        return;
    }

    for (size_t i = 0; i < G->sz; i++) 
        G->adj_list[i].info->color_dfs = WHITE;

    for (size_t i = 0; i < G->sz; i++) {

        fprintf (file, "node_%p [label=\"", &(G->adj_list[i]) );
        fprintf (file, "%s (%zu)", G->adj_list[i].info->name, G->adj_list[i].info->port);
        fprintf (file, "\"];\n");

        dump_adj_list (G, &(G->adj_list[i]), file);
        G->adj_list[i].info->color_dfs = BLACK;
    }
}

void dump_adj_list (Graph G, V_header v, FILE *file) {

    V_node node = v->head;

    while (node) {

        V_header v_cur = find_vertex_in_graph (G, node->info->name);

        if (v_cur->info->color_dfs == WHITE) {

            fprintf (file, "node_%p -- node_%p [label=\"(", v, v_cur);
            
            for (size_t i = 0; i < node->edge_info->ports_num; i++) 
                fprintf (file, "%zu%c", node->edge_info->avail_ports[i], 
                    i == node->edge_info->ports_num - 1 ? ')' : ' ');
            
            fprintf (file, "  \"];\n");
        }

        node = node->next;
    }
}

int Insert (Graph graph, FILE *file) {

    struct dialog_strings info = {NULL, NULL, 0, NULL};

    info.greeting = "\nInsert graph UI.";
    info.msgs = "\n0 - quit\n""1 - InsertVertex\n""2 - InsertEdge\n";
    info.msgc = 3;
    info.exit_msg = "quit Insert submenu";

    dialog (&info, fptr_I, graph, file);
        
    return ERRSUC;
}

int InsertVertex (Graph graph, FILE *file) {

    puts ("Enter computer name:");
    char *name = get_str (file);
    if (!name)
        return ERREOF;

    if (EQ(name, "*")) {
        free_nullify (name);
        return ERREOF;
    }


    puts ("Enter service port:");
    size_t port = 0;
    if (get_sizet_file (file, &port, 255, 0) == ERREOF) {
        free_nullify (name);
        return ERREOF;
    }

    switch (add_vertex (graph, name, port))  {
        case ERRDUP:
            puts ("This name already exists");
            free_nullify (name);
            return ERRDUP;
        case ERROVF:
            puts ("Graph overflow");
            free_nullify (name);
            return ERROVF;
        default:
            puts ("Vertex inserted successfully.");
    }

    return ERRSUC;
}

int InsertEdge (Graph graph, FILE *file) {

    puts ("Enter first computer name");
    char *name1 = get_str (file);
    if (!name1)
        return ERREOF;
    
    puts ("Enter second computer name");
    char *name2 = get_str (file);
    if (!name2) {
        free_nullify (name1);
        return ERREOF;
    }

    puts ("Enter number of edge ports");
    size_t ports_num = 0;
    if (get_sizet_file (file, &ports_num, 255, 0) == ERREOF) {
        free_nullify (name1);
        free_nullify (name2);
        return ERREOF;
    }

    puts ("Enter edge ports");
    size_t *ports = calloc (ports_num, sizeof (size_t));
    for (size_t i = 0; i < ports_num; i++) {
        if (get_sizet_file (file, ports+i, 255, 0) == ERREOF) {
            free_nullify (name1);
            free_nullify (name2);
            free_nullify (ports);
            return ERREOF;
        }
    }

    if (add_edge (graph, name1, name2, ports, ports_num) == ERRWRG) {
        puts ("Vertex not found");
        free_nullify (name1);
        free_nullify (name2);
        free_nullify (ports);
        return ERRWRG;
    } else 
        puts ("Edge inserted successfully.");

    free_nullify (name1);
    free_nullify (name2);

    return ERRSUC;
}


/*  DELETION  */

int (*fptr_D[]) (Graph, FILE*)  = {NULL, DeleteVertex, DeleteEdge};


int Delete (Graph graph, FILE *file) {

    struct dialog_strings info = {NULL, NULL, 0, NULL};

    info.greeting = "\nDelete graph UI.";
    info.msgs = "\n0 - quit\n""1 - DeleteVertex\n""2 - DeleteEdge\n";
    info.msgc = 3;
    info.exit_msg = "quit Delete submenu";

    dialog (&info, fptr_D, graph, file);
        
    return ERRSUC;
}

int DeleteVertex (Graph graph, FILE *file) {

    puts ("Enter computer name:");
    char *name = get_str (file);
    if (!name)
        return ERREOF;

    if (remove_vertex (graph, name) == ERRWRG)
        printf ("Vertex %s not found\n", name);
    else 
        puts ("Vertex deleted successfully.");

    free_nullify (name);

    return ERRSUC;
}

int DeleteEdge (Graph graph, FILE *file) {

    puts ("Enter first computer name");
    char *name1 = get_str (file);
    if (!name1)
        return ERREOF;

    puts ("Enter second computer name");
    char *name2 = get_str (file);
    if (!name2) {
        free_nullify (name1);

        return ERREOF;
    }

    if (remove_edge (graph, name1, name2) == ERRWRG) {
        puts ("Edge not found");
        free_nullify (name1);
        free_nullify (name2);

        return ERRWRG;
    }
    else 
        puts ("Edge deleted successfully.");

    free_nullify (name1);
    free_nullify (name2);

    return ERRSUC;
}


/*  UPDATE  */

int (*fptr_U[]) (Graph, FILE*)  = {NULL, UpdateVertex, UpdateEdge};


int Update (Graph graph, FILE *file) {

    struct dialog_strings info = {NULL, NULL, 0, NULL};

    info.greeting = "\nUpdate graph UI.";
    info.msgs = "\n0 - quit\n""1 - UpdateVertex\n""2 - UpdateEdge\n";
    info.msgc = 3;
    info.exit_msg = "quit Update submenu";

    dialog (&info, fptr_U, graph, file);
        
    return ERRSUC;
}


int (*fptr_UV[]) (Graph, FILE*)  = {NULL, UpdateVertexName, UpdateVertexPort};

int UpdateVertex (Graph graph, FILE *file) {

    struct dialog_strings info = {NULL, NULL, 0, NULL};

    info.greeting = "\nUpdateVertex graph UI.";
    info.msgs = "\n0 - quit\n""1 - UpdateVertexName\n""2 - UpdateVertexPort\n";
    info.msgc = 3;
    info.exit_msg = "quit UpdateVertex submenu";

    dialog (&info, fptr_UV, graph, file);
        
    return ERRSUC;
}

int UpdateVertexName (Graph graph, FILE *file) {

    puts ("Enter old computer name:");
    char *old_name = get_str (file);
    if (!old_name)
        return ERREOF;

    V_header v = find_vertex_in_graph (graph, old_name);

    if (!v) {
        printf ("Vertex %s not found\n", old_name);
        free_nullify (old_name);
        return ERRWRG;
    } 

    free_nullify (old_name);

    puts ("Enter new computer name:");
    char *new_name = get_str (file);
    if (!new_name)
        return ERREOF;

    if (find_vertex_in_graph (graph, new_name)) {
        puts ("This name already exists");
        free_nullify (new_name);
        return ERRDUP;
    }

    change_vertex_name (v->info, new_name);
    puts ("Vertex name updated successfully.");

    return ERRSUC;
}

int UpdateVertexPort (Graph graph, FILE *file) {

    puts ("Enter computer name:");
    char *name = get_str (file);
    if (!name) 
        return ERREOF;

    V_header v = find_vertex_in_graph (graph, name);

    if (!v) {
        printf ("Vertex %s not found\n", name);
        free_nullify (name);
        return ERRWRG;
    } 

    free_nullify (name);

    puts ("Enter new service port:");
    size_t new_port = 0;
    if (get_sizet_file (file, &new_port, 255, 0) == ERREOF)
        return ERREOF;

    change_vertex_port (v->info, new_port);
    puts ("Vertex port updated successfully.");

    return ERRSUC;
}


int (*fptr_UE[]) (Graph, FILE*)  = {NULL, UpdateEdgePorts};

int UpdateEdge (Graph graph, FILE *file) {

    struct dialog_strings info = {NULL, NULL, 0, NULL};

    info.greeting = "\nUpdateEdge graph UI.";
    info.msgs = "\n0 - quit\n""1 - UpdateEdgePorts\n";
    info.msgc = 2;
    info.exit_msg = "quit UpdateEdge submenu";

    dialog (&info, fptr_UE, graph, file);
        
    return ERRSUC;
}

int UpdateEdgePorts (Graph graph, FILE *file) {

    puts ("Enter first computer name");
    char *name1 = get_str (file);
    if (!name1)
        return ERREOF;

    puts ("Enter second computer name");
    char *name2 = get_str (file);
    if (!name2) {
        free_nullify (name1);
        return ERREOF;
    }

    puts ("Enter new number of edge ports");
    size_t ports_num = 0;
    if (get_sizet_file (file, &ports_num, 255, 0) == ERREOF)
        return ERREOF;

    puts ("Enter new edge ports");
    size_t *ports = calloc (ports_num, sizeof (size_t));
    for (size_t i = 0; i < ports_num; i++) {
        if (get_sizet_file (file, ports+i, 255, 0) == ERREOF)
            return ERREOF;
    }

    change_edge_ports (graph, name1, name2, ports, ports_num);
    puts ("Edge ports updated successfully.");

    free_nullify (name1);
    free_nullify (name2);

    return ERRSUC;
}


/*  PRINT  */

int Print (Graph graph, FILE *file) {

    print_graph (graph);

    return ERRSUC;
}


/*  COMPLEX FUNCTIONS  */

int CreateComponents (Graph g, FILE *file) {

    create_components (g);

    return ERRSUC;
}

int DFS (Graph g, FILE *file) {

    puts ("Enter computer name to search from");
    char *name = get_str (file);
    if (!name)
        return ERREOF;

    V_header v = find_vertex_in_graph (g, name);
    if (!v) {

        printf ("No such computer\n");
        free_nullify (name);
        
        return ERRWRG;
    }

    puts ("Enter number of service port:");
    size_t port = 0;
    if (get_sizet_file (file, &port, 255, 0) == ERREOF) {

        free_nullify (name);

        return ERREOF;
    }

    dfs (g, v, port);

    free_nullify (name);

    return ERRSUC;
}

int Djkstra (Graph graph, FILE *file) {

    puts ("Enter first computer name");
    char *name1 = get_str (file);
    if (!name1)
        return ERREOF;

    puts ("Enter second computer name");
    char *name2 = get_str (file);
    if (!name2) {
        free_nullify (name1);

        return ERREOF;
    }

    puts ("Enter number of service port:");
    size_t port = 0;
    if (get_sizet_file (file, &port, 255, 0) == ERREOF) {

        free_nullify (name1);
        free_nullify (name2);

        return ERREOF;
    }

    if (djkstra (graph, name1, name2, port) == ERRWRG) {
        
        puts ("One or two vertices not found");
        free_nullify (name1);
        free_nullify (name2);

        return ERRWRG;
    }

    free_nullify (name1);
    free_nullify (name2);

    return ERRSUC;

}

