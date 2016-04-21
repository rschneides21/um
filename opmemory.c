

#include "opmemory.h"
#include "decode.h"
#include "seg.h"
#include "mem_arr.h"
#include "stack.h"
#include "stdbool.h" 

struct Mem_T {
        Mem_arr segment_seq;
        Stack_T reusable_indices;
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

        memory->segment_seq = Mem_arr_new(1);
        memory->reusable_indices = Stack_init(1);

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
        for(unsigned i = 0; i < buffer_size; i++){
                uint32_t word = buffer[i];
                word = flip_word(word);
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
        Mem_arr_free(memory->segment_seq);
        Stack_free(memory->reusable_indices);        
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
        Seg_T segment = Mem_arr_get(memory->segment_seq, segIndex);

        Seg_store(segment, offset, word);
        Mem_arr_put(memory->segment_seq, segment, segIndex);
}

/* 
 * Input:  Mem_T structure, unsigned segment index, unsigned offset
 * Output: Returns a uint32_t
 * Purpose: Retrieves a word from memory from at specified index and offset.
 */  
uint32_t load_word(Mem_T memory, unsigned segIndex, unsigned offset)
{
        Seg_T segment = (Mem_arr_get(memory->segment_seq, segIndex));

        return Seg_load(segment, offset);
}

/* 
 * Input:   Mem_T structure, unsigned integer
 * Output:  returns the index at which the segment was stored.
 * Purpose: Adds a segment to memory with numWords words. 
 */
int map_segment(Mem_T memory, unsigned numWords)
{
        unsigned segIndex;
        Seg_T segment = Seg_new(numWords);

        if(!(Stack_is_empty(memory->reusable_indices))){
                segIndex = Stack_pop(memory->reusable_indices);
                Mem_arr_put(memory->segment_seq, segment, segIndex);
        }

        else{ 
                segIndex = Mem_arr_length(memory->segment_seq);
                memory->segment_seq = Mem_arr_addhi(memory->segment_seq, segment);
        }        

        return segIndex;

        // if(!(Stack_is_empty(memory->reusable_indices))){
        //         segIndex = Stack_pop(memory->reusable_indices);
        // }
        // else{
        //         segIndex = Mem_arr_length(memory->segment_seq);
        // }
        // memory->segment_seq = Mem_arr_put(memory->segment_seq, segment, segIndex);
        // return segIndex;

}

/* 
 * Input:   Mem_T structure, unsigned integer
 * Output:  void
 * Purpose: Unmaps a segment from memory at the given segment index and frees
 *          all memory associated with it.
 */
void unmap_segment(Mem_T memory, unsigned segIndex)
{
        Seg_T unmapped_segment = Mem_arr_put(memory->segment_seq, NULL, segIndex);
        Seg_free(unmapped_segment);
        //fprintf(stderr, "about to push onto stack\n");
       
        memory->reusable_indices =  Stack_push(memory->reusable_indices, segIndex);
        //fprintf(stderr, "pushing onto stack\n");
}

static inline void print_segment0(Mem_T memory){
        print_segment(Mem_arr_get(memory->segment_seq, 0));
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
        Seg_T program_to_load = Mem_arr_get(memory->segment_seq, segIndex);

        Seg_T duplicate_segment = Seg_duplicate(program_to_load);
        /*Copies program_to_load_temp to program_to_load*/

        Seg_T old_program = Mem_arr_put(memory->segment_seq, duplicate_segment, 0);

        Seg_free(old_program);
}

/* 
 * Input:  Mem_T structure, unsigned integer
 * Output: Integer
 * Purpose: Returns the length of a segment in memory at segIndex.
 *          CRE if segIndex is outside the bounds of memory
 */
int get_segment_length(Mem_T memory, unsigned segIndex)
{
        assert(segIndex <= Mem_arr_length(memory->segment_seq));
        int returnVal = Seg_length(Mem_arr_get(memory->segment_seq, segIndex));
        return returnVal;
}

