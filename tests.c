/*
 *          memory.h
 *      By: Ryan Schneiderman and Matthew Epstein <rschne01> <mepste03> 
 *      Date: 04/06/2016
 *      Purpose: File to test functions in memory.h
 *
 */



#include "memory.h"

int main(int argc, char *argv[])
{
        (void)argc;
        FILE *fp = fopen(argv[1], "rb");

        Mem_T memory = Mem_new(fp);
        fprintf(stderr, "creates memory\n");
        int seg_index = map_segment(memory, 5);
        fprintf(stderr, "maps segment\n");
        uint32_t word1 = 7;
        uint32_t word2 = 10;

        /*will result in a checked runtime error if map_segment did not work 
         *properly
         */
        put_word(memory, word1, seg_index, 0);
        put_word(memory, word2, seg_index, 1);
        fprintf(stderr, "puts words\n");


        /*returned_word1 should equal word1 */
        uint32_t returned_word1 = get_word(memory, seg_index, 0);

        /*returned_word2 should equal word2 */
        uint32_t returned_word2 = get_word(memory, seg_index, 1);
        fprintf(stderr, "gets words\n");

        if(returned_word2 == word2 && returned_word1 == word1){
                printf("Yay");
        }

        unmap_segment(memory, seg_index);

        /*should result in a checked run_time error*/
        put_word(memory, word1, seg_index, 0);
}

