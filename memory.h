/*
 *      memory.h
 *              Purpose: Interface for the memory module
 *      By: Ryan Schneiderman and Matthew Epstein <rschne01> <mepste03> 
 *      Date: 04/06/2016
 *      
 *
 */



#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "assert.h"
#include "seq.h"

typedef struct Mem_T *Mem_T;

/* Returns an initialized Mem_T struct.  Takes a file pointer to initialize 
 * segment 0 to the program contained in input.
 */ 
extern Mem_T Mem_new(FILE *input);

extern void MemT_free(Mem_T memory);

/* Retrieves a word from memory from the specified index and offset. 
 */
extern uint32_t load_word(Mem_T memory, unsigned segIndex, unsigned offset);

/* Adds a word to the segment dictated by segIndex with an offset. This 
 * function is used to perform a segmented store. CRE if segIndex does not map 
 * to a mapped segment
 */ 
extern void store_word(Mem_T memory, uint32_t word, unsigned segIndex, unsigned
                                                offset);

/* Adds a segment to memory with numWords words. Returns the segment index 
 * (i.e. where the function decided to store the segment in memory)
 */
extern int map_segment(Mem_T memory, unsigned numWords);

/* Removes a segment from memory at the specified index and frees the data. 
 */
extern void unmap_segment(Mem_T memory, unsigned segIndex);

/* Puts the segment located at segIndex into segment 0 and frees the data in 
 * segment 0.  
 */
extern void load_program(Mem_T memory, unsigned segIndex);

/* Returns the length of the segment at the given index */
extern int get_segment_length(Mem_T memory, unsigned segIndex);


#endif