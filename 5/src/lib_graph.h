#pragma once

#include "generic.h"
#include <stddef.h>
#include <stdlib.h>

#define num(v) ( (size_t) ((v) - g->adj_list) ) 

/*  Contains array of ports and its size.  */
typedef struct edge *Edge;

struct edge {

    size_t ports_num;
    size_t *avail_ports;
};

/*  Contains computer name, port and color for dfs.  */
typedef struct v_info *V_info;

struct v_info {

    char *name;
    size_t port;

    int color_dfs;
};

/*  Contains info, next and edge between it and head.  */
typedef struct vertex *V_node;

struct vertex {

    V_info info;
    V_node next;
    Edge edge_info;
    
};

/*  Contains info and head of its adj list.  */
typedef struct vertex_header *V_header;

struct vertex_header {

    V_info info;
    V_node head;
    size_t comp_num;
};



enum {
    WHITE, 
    GREY,
    BLACK
};

typedef struct graph *Graph;

#define MAX_VERTEX_NUM 20

struct graph {
    struct vertex_header adj_list [MAX_VERTEX_NUM];
    size_t sz;
};



/*||||||||||||||||||||||||| < VERTEX FUNCTIONS > |||||||||||||||||||||||||*/

/**
* @brief Add vertex to graph. 
* 
* @param graph [IN] - graph for addition. 
* @param name  [IN] - UNIQUE computer name. 
* @param port  [IN] - port number on which network service is running.
* @return int - Error code. Possible ERRDUP - computer name already exists.
  ERROVF - vertex limit is reached. 
*/
int add_vertex (Graph graph, char *name, size_t port);


/**
* @brief Check if both vertices belong to graph. 
* 
* @param graph        [IN] - graph for check. 
* @param name1, name2 [IN] - UNIQUE vertex names. 
* @return V_head - ptr to first vertex node found. 
  NULL if ine or two names not found.
*/
V_header check_vertices (Graph graph, char *name1, char *name2);

/**
* @brief Check if edge belong to graph. 
* 
* @param graph        [IN] - graph for check. 
* @param name1, name2 [IN] - UNIQUE names of edge ends. 
* @return int - Error code. Possible ERRWRG - edge not found.
*/
int check_edge (Graph graph, char *name1, char *name2);


/**
* @brief Create new isolated vertex info. 
* 
* @param name [IN] - UNIQUE computer name. 
* @param port [IN] - port number on which network service is running.
* @return V_info - ptr to info created.
*/
V_info new_info (char *name, size_t port);

/**
* @brief Create new isolated vertex node. 
* 
* @param info [IN] - node info. 
* @return V_info - ptr to info created.
*/
V_node new_vertex_node (V_info info);

/**
* @brief Remove vertex from graph. 
* 
* @param graph [IN] - graph for removal. 
* @param name  [IN] - UNIQUE computer name. 
* @return int - Error code. Possible ERRWRG - name not found.
*/
int remove_vertex (Graph graph, char *name);

/**
* @brief Clean vertex header, its name and info. 
* 
* @param v [IN] - vertex head to clean.
*/
void free_vertex_header (V_header v);

/**
* @brief Remove vertex from adjacency lists by name. 
* 
* @param graph [IN] - graph for removal. 
* @param name  [IN] - UNIQUE computer name. 
*/
void remove_vertex_from_adj_lists (Graph graph, char *name);

/**
* @brief Change name field of vertex. 
* 
* @param v        [IN] - vertex which name is changed.
* @param new_name [IN] - new UNIQUE computer name. 
*/
void change_vertex_name (V_info v, char *new_name);

/**
* @brief Change port field of vertex. 
* 
* @param v        [IN] - vertex which name is changed.
* @param new_port [IN] - new port number on which network service is running.
*/
void change_vertex_port (V_info v, size_t new_port);

/**
* @brief Find vertex head by name. 
* 
* @param graph [IN] - graph for search
* @param name  [IN] - UNIQUE computer name
* @return V_header - header of matching adjacency list. NULL if not found
*/
V_header find_vertex_in_graph (Graph graph, char *name);


/*||||||||||||||||||||||||| < EDGE FUNCTIONS > |||||||||||||||||||||||||*/

/*  Note: loops and multiple edges are allowed. */

/**
* @brief Add edge to graph. 
* 
* @param graph        [IN] - graph for addition. 
* @param name1, name2 [IN] - names of ends of newly created edge. 
* @param avail_ports  [IN] - vector of ports, available for traffic transmission.
* @param ports_num    [IN] - number of ports.
* @return int - Error code. Possible ERRWRG - vertex not found.
*/
int add_edge (Graph graph, char *name1, char *name2, size_t *avail_ports, size_t ports_num);

/**
* @brief Create new isolated edge. 
* 
* @param avail_ports [IN] - vector of ports, available for traffic transmission.
* @param ports_num   [IN] - number of ports.
* @return Edge - ptr to edge created.
*/
Edge new_edge (size_t *avail_ports, size_t ports_num);

/**
* @brief Remove edge from graph. 
* 
* @param graph          [IN] - graph for removal. 
* @param name1, name2   [IN] - names of ends of removed edge. 
* @return int - Error code. Possible ERRWRG - edge not found.
*/
int remove_edge (Graph graph, char *name1, char *name2);

/**
* @brief Clean edge and its info. 
* 
* @param v [IN] - edge to clean.
*/
void free_edge (Edge *e);

/**
* @brief Change avail_ports field of edge. 
* 
* @param graph           [IN] - graph for change. 
* @param name1, name2    [IN] - names of ends of changed edge. 
* @param new_avail_ports [IN] - new vector of ports, available for traffic transmission.
  @param new_ports_num   [IN] - number of ports.
* @return int - Error code. Possible ERRWRG - edge not found.
*/
int change_edge_ports (Graph graph, char *name1, char *name2, size_t *new_avail_ports, size_t new_ports_num);



/*||||||||||||||||||||||||| < GRAPH FUNCTIONS > |||||||||||||||||||||||||*/

/**
* @brief Print graph as colored adjacency lists.
* 
* @param graph [IN] - graph to print.
*/
void print_graph (const Graph graph);

/**
* @brief Print header of adjacency list in color.
* 
* @param graph [IN] - head to print.
*/
void print_vertex_header (const V_header v);

/**
* @brief Clear graph.
* 
* @param graph [IN] - graph to clear.
*/
void free_graph (Graph graph);


/**
* @brief Create graph components (each has its color). 
* 
* @param g [IN] - graph.
*/
void create_components (Graph g);

/**
* @brief Init graph colors before split on components.
  Set all vertices to white.
* 
* @param graph [IN] - graph for DFS.
*/
void init_colors (Graph graph);

/**
* @brief Handle graph in split on components
  and set component number for each vertex.
* 
* @param graph [IN] - graph for DFS.
* @return size_t - number of components in graph.
*/
size_t handle_components (Graph graph);

/**
* @brief Visit vertex in split on components.
  Set every new vertice to grey, 
  traverse vertices from its adj list and their lists until possible.
* 
* @param graph    [IN] - graph for DFS.
* @param v        [IN] - vertex to visit.
* @param comp_num [IN] - number of vertex component.
*/
void visit_vertex_comps (Graph g, V_header v, size_t comp_num);

/**
* @brief Split graph on components. 
* 
* @param g        [IN] - graph for split.
* @param comp_num [IN] - number of components.
* @return Graph - array of graph components.
*/
Graph split_graph (Graph g, size_t comp_num);

/**
* @brief Print components. 
* 
* @param comps    [IN] - components array to print.
* @param comp_num [IN] - number of components.
*/
void print_components (Graph comps, size_t comp_num);

/**
* @brief Finds adj list header by name in node. 
* 
* @param graph [IN] - graph for search.
* @param v     [IN] - node to take name from.
* @return V_header - found header, NULL if not found
*/
V_header take_header_from_node (Graph g, V_node v);

/**
* @brief Print graph as adjacency lists in no color.
* 
* @param graph [IN] - graph to print.
*/
void print_graph_comps (const Graph graph);

/**
* @brief Print header of adjacency list in no color.
* 
* @param graph [IN] - head to print.
*/
void print_vertex_header_no_color (const V_header v);

/**
* @brief Print header of adjacency list in no color with backward arrows.
* 
* @param graph [IN] - head to print.
*/
void print_vertex_header_no_color_rev (const V_header v);

/**
* @brief Traverse graph from vertex by port in depth.
* 
* @param graph [IN] - graph to traverse.
* @param v  [IN] - vertex to start from. 
* @param port  [IN] - port to traverse by.
*/
void dfs (Graph g, V_header v, size_t port);

/**
* @brief Recursively print dfs paths.
* 
* @param start [IN] - global start point. 
* @param graph [IN] - graph to traverse.
* @param v     [IN] - vertex to start from. 
* @param port  [IN] - port to traverse by.
*/
void print_dfs_paths (V_header start, Graph g, V_header v, size_t port);

/**
* @brief Check if ports array has port.
* 
* @param ports        [IN] - vector of ports, available for traffic transmission.
* @param ports_num    [IN] - number of ports.
* @param ports        [IN] - port to check
* @return Bool - Error code. Possible ERRWRG - port not found.
*/
Bool is_port_avail (size_t *ports, size_t ports_num, size_t port);

/**
* @brief Check vertices existence and print shortest path in success.
* 
* @param g     [IN] - graph to traverse.
* @param name1 [IN] - unique name of first vertex.
* @param name2 [IN] - unique name of second vertex.
* @param port  [IN] - port to traverse by.
* @return int - Error code. Possible ERRWRG - vertex not found.
*/
int djkstra (Graph g, char *name1, char *name2, size_t port);


/**
* @brief Compute shortest path between two vertices.
* 
* @param g     [IN]  - graph to traverse.
* @param start [IN]  - first vertex.
* @param end   [IN]  - second vertex.
* @param path  [IN/OUT] OPTIONAL - shortest path.
* @return size_t shortest distance between start and end
  or INF if end is unreachable from start
*/
size_t short_path (Graph g, V_header start, V_header end, 
                    size_t port, size_t **path);