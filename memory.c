/*
 *      memory.c
 *      Purpose:  Implementation for memory.h
 *                Implements the "secret" of how memory is structured in the UM
 *                This includes creating and freeing memory, mapping
 *                and unmapping segments, loading and storing words in
 *                segments, loading a program into segment 0 and getting
 *                the length of a segment
 *
 *      Date:     4/14/2016 
 *      By:           Ryan Schneiderman and Matthew Epstein
 */

#include "memory.h"
#include "decode.h"


struct Mem_T {
        Seq_T segment_seq;
        Seq_T reusable_indices;
};

/* Input:   a file pointer
 * Output:  a Mem_T structure
 * Purpose: Initializes the Mem_T memory structure. Stores 32 bit words from 
 *          input into segment 0 in memory. This is location which holds the 
 *          instructions that the um will execute
 */
Mem_T Mem_new(FILE *input)
{
        long unsigned lSize;
        uint32_t *buffer;
        size_t buffer_size;

        assert(input != NULL);

        Mem_T memory = malloc(sizeof(*memory));
        assert(memory!=NULL);

        memory->segment_seq = Seq_new(0);
        memory->reusable_indices = Seq_new(0);

        /* determines length of input file */
        fseek(input, 0, SEEK_END);
        lSize = ftell(input);
        rewind(input);

        /* creates buffer that will grab words from input*/
        buffer = malloc(sizeof(uint32_t) * lSize/4);
        assert(buffer != NULL);

        buffer_size = fread(buffer, 4, lSize / 4, input);

        if(buffer_size != lSize/4){
                fprintf(stderr, "Reading error\n");
        }

        /* Program index will be 0*/
        int programIndex = map_segment(memory, buffer_size);

        /*transfers instructions from buffer to segment 0 in memory*/
        for(unsigned i = 0; i < 3; i++){
                uint32_t word = buffer[i];
                word = flip_word(word);
                fprintf(stderr, "word: %x\n", word);
                store_word(memory, word, programIndex, i);
        }
        free(buffer);
        return memory;
}


/* 
 * Input: Mem_T structure
 * Output: void
 * Purpose: Frees all memory. Frees each segment, the sequence of segments,   
 *          the sequence of reusable indices and the pointer to the memory 
 *          structure
 */
void MemT_free(Mem_T memory){
        int length = Seq_length(memory->segment_seq);
        for(int i = 0; i < length; i++){
                Seq_T segment = Seq_get(memory->segment_seq, i);
                if(segment != NULL)
                        Seq_free(&segment);
        }
        Seq_free(&(memory->segment_seq));
        Seq_free(&(memory->reusable_indices));        
        free(memory);
}

/* 
 * Input:  Mem_T structure, 32 bit word, unsigned segment index and an unsigned
 *         offset
 * Output: void
 * Purpose: Puts a word into a segment in memory at index segIndex with an 
 *          offset. Used to execute the segmented store instruction and to put
 *          put words into segment 0 during initialization
 */ 
void store_word(Mem_T memory, uint32_t word, unsigned segIndex, unsigned 
                                                                        offset)
{       
        Seq_T segment = (Seq_T)Seq_get(memory->segment_seq, segIndex);

        assert(segment != NULL);

        Seq_put(segment, offset, (void *)(uintptr_t)word);
        Seq_put(memory->segment_seq, segIndex, segment);
}

/* 
 * Input:  Mem_T structure, unsigned segment index, unsigned offset
 * Output: Returns a uint32_t
 * Purpose: Retrieves a word from memory from at specified index and offset.
 */  
uint32_t load_word(Mem_T memory, unsigned segIndex, unsigned offset)
{
        Seq_T segment = (Seq_T)Seq_get(memory->segment_seq, segIndex);

        assert(segment != NULL);

        uint32_t word = (uint32_t)(uintptr_t)Seq_get(segment, offset);
        return word;
}

/* 
 * Input:   Mem_T structure, unsigned integer
 * Output:  returns the index at which the segment was stored.
 * Purpose: Adds a segment to memory with numWords words. 
 */
int map_segment(Mem_T memory, unsigned numWords)
{

        int segIndex;
        int reusedTest = 0;

        if(Seq_length(memory->reusable_indices) > 0){
                segIndex = (int)(intptr_t)Seq_remlo(memory->reusable_indices);
                reusedTest = 1;
        }
        else segIndex = Seq_length(memory->segment_seq);

        Seq_T segment = Seq_new(numWords);
        uint32_t initializer = 0;

        for(unsigned i = 0; i < numWords; i++){
                Seq_addhi(segment, (void *)(uintptr_t)initializer);
        }

        if(reusedTest) {
                Seq_put(memory->segment_seq, segIndex, segment);
        } else {
                Seq_addhi(memory->segment_seq, segment);
        }

        return segIndex;
}

/* 
 * Input:   Mem_T structure, unsigned integer
 * Output:  void
 * Purpose: Unmaps a segment from memory at the given segment index and frees
 *          all memory associated with it.
 */
void unmap_segment(Mem_T memory, unsigned segIndex)
{
        Seq_T unmapped_segment = Seq_put(memory->segment_seq, segIndex, NULL);
        Seq_free(&unmapped_segment);
        Seq_addhi(memory->reusable_indices, (void *)(intptr_t)segIndex);
}

/* 
 * Input:   Mem_T structure, unsigned integer
 * Ouput:   void
 * Purpose: Copies the segment located at segIndex and stores it in segment 0 
 *          The segment previously at index 0 is freed.
 */
void load_program(Mem_T memory, unsigned segIndex)
{
        /*Segment that will become new program*/
        Seq_T program_to_load_temp = Seq_get(memory->segment_seq, segIndex);

        int length = Seq_length(program_to_load_temp);

        /*New sequence that will be put in segment 0*/
        Seq_T program_to_load = Seq_new(length);
        /*Copies program_to_load_temp to program_to_load*/
        for(int i = 0; i < length; i++) {
                Seq_addhi(program_to_load, Seq_get(program_to_load_temp, i));
        }
        Seq_T old_program = Seq_put(memory->segment_seq, 0, program_to_load);
        Seq_free(&old_program);
}

/* 
 * Input:  Mem_T structure, unsigned integer
 * Output: Integer
 * Purpose: Returns the length of a segment in memory at segIndex.
 *          CRE if segIndex is outside the bounds of memory
 */
int get_segment_length(Mem_T memory, unsigned segIndex)
{
        assert(segIndex < (unsigned)Seq_length(memory->segment_seq));
        int returnVal = Seq_length(Seq_get(memory->segment_seq, segIndex));
        return returnVal;
}

