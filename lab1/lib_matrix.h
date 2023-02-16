

typedef struct matrix matrix_t;
typedef struct line line_t;

struct matrix {
    /*  Vector of pointers to lines of matrix.  */
    line_t **lns;
    /*  Number of rows in matrix.  */
    int lns_cnt;
};

struct line {
    /*  Vector of elements of line.  */
    int *nums;
    /*  Number of elements in line.  */
    int num_cnt;
};


/*  General functions.  */

/*  Creates matrix with cnt lines.  */
matrix_t *new_matrix (int cnt);
/*  Creates line with cnt elements.  */
line_t *new_line (int cnt);

/*  Fills matrix with input string.  */
matrix_t *fill_matrix (matrix_t *m, char **input);
/*  Fills line with input string.  */
line_t *fill_line (line_t *l, char **input);

/*  Prints matrix pointed by m.  */
void print_matrix (matrix_t *m);
/*  Prints line pointed by l.  */
void print_line (line_t *l);

/*  Frees matrix pointed by m.  */
void free_matrix (matrix_t *m);
/*  Frees line pointed by m.  */
void free_line (line_t *l);