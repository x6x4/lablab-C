#include "generic.h"
#include <stdlib.h>
#include <string.h>

FILE *user_file () {
    char *filename = NULL;
    char *errmsg = "";
    FILE *file = NULL;
	size_t len = 0;

	puts ("Enter filename of file to read from. Enter \"/proc/self/fd/0\" to read from stdin.");

    while (!file) {
		
        if (getline (&filename, &len, stdin) == EOF) {
            free (filename);
			return NULL;
		}

		/*  getline appends extra \n to to the end of the line  */
		filename[strcspn(filename, "\n")] = '\0';

        file = fopen (filename, "r");
		
		if (!file)
			perror (errmsg);
    }
	
	/*  clean buffer allocated by getline  */
   	free (filename);	
    
	return file;
}

int option_choice (const char *msgs[], size_t msgc, FILE *file) {
    int option_num = -1;
    char *errmsg = "";

    for (int i = 0; i < msgc; i++) 
        puts (msgs[i]);
    
    int status = get_int_file (file, &option_num, msgc - 1, 0);

    if (status == ERREOF)
        return 0;
    
    return option_num;
}


int get_int_file (FILE *file, int *numptr, int high, int low) {
    const char *errmsg = "";
    int status;

    do {
        printf ("%s", errmsg);
        errmsg = "Bad integer\n";
        status = fscanf (file, "%d", numptr);
        if (status == EOF) {
            return ERREOF;
        }
        while ( fgetc (file) != '\n');
    } while (!status  || (*numptr > high || *numptr < low));

    return ERRSUC;
}
