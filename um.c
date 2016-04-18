
/*
 *      um.c
 *      Purpose:  Main file for the UM. Client to the execute module
 *                
 *      Date:     4/14/2016 
 *      By:       Ryan Schneiderman and Matthew Epstein
 */

#include <stdlib.h>
#include <stdio.h>
#include "execute.h"
#include "opmemory.h" 


int main(int argc, char *argv[]){
	if(argc != 2){
		fprintf(stderr, "Incorrect number of arguments");
		exit(EXIT_FAILURE);
	}
	FILE *input = fopen(argv[1], "rb");
	Mem_T memory = Mem_new(input);
	fclose(input);
	execute(memory);
	MemT_free(memory);
	exit(EXIT_SUCCESS);
}
