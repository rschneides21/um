/*
 *      Decode.c
 *      Purpose:  Implementation for decode.h
 *                Deals with decoding values from 32 bit words.
 *                Used to unpack registers, opcodes, and values from
 *                program instructions.
 *
 *      Date:     4/14/2016 
 *      By:           Ryan Schneiderman and Matthew Epstein
 */

#include "flip_word.h"
#include "assert.h"
#include "stdint.h"
#include "stdbool.h" 

#define REGISTER_WIDTH 3
#define OPCODE_WIDTH 4
#define VALUE_WIDTH 25
#define MAX_WIDTH 32

 Except_T Bitpack_Overflow = { "Overflow packing bits" };



/* Helper function for shift uint32_t integer to left. */
static inline uint32_t shift_leftu(uint32_t n, unsigned width){
    /* If we shift an integer to left 32 bits, we should get 0. */
    if(width == MAX_WIDTH) return 0;

    uint32_t result = n << width;
    return result;
} 

/* Helper function for shift uint32_t integer to right. */
static inline uint32_t shift_rightu(uint32_t n, unsigned width){
    /* If we shift an integer to right 32 bits, we should get 0. */
    if(width == MAX_WIDTH) return 0;

    uint32_t result = n >> width;
    return result;
} 

static inline uint32_t Bitpack_getu(uint32_t word, unsigned width, unsigned lsb) {
    
    /* It should be a checked run-time error if width does not satisfy 
     * 0 ≤ w ≤ 32.
     * It should be a checked run-time error to width and lsb do not 
     * satisfy w + lsb ≤ 32.*/
    assert(width <= MAX_WIDTH);
    assert((width + lsb) <= MAX_WIDTH);

    /* Fields of width zero are defined to contain the value zero. */
    if (width == 0) return 0;

    /* Create mask for extracting field. */
    uint32_t mask = shift_leftu(1, width) - 1;
    mask = shift_leftu(mask, lsb);

    uint32_t result = (word & mask);
    result = shift_rightu(result, lsb);

    return result;
}

static inline bool Bitpack_fitsu(uint32_t n, unsigned width) {
    /* If the number of bits is greater than or equal to 32, it should 
     * always 
     * return true.*/
    if (width >= MAX_WIDTH) return true;
    
    /* unsigned integer range of width bits should be from 0 to (2 ** width 
     * - 1)*/
    uint32_t max_val = shift_leftu(1, width);
    return (n < max_val);
}

static inline uint32_t Bitpack_newu(uint32_t word, unsigned width, unsigned lsb, 
                            uint32_t value) {

    /* It should be a checked run-time error if width does not satisfy 
     * 0 ≤ w ≤ 32.
     * It should be a checked run-time error to width and lsb do not 
     * satisfy w + lsb ≤ 32.*/
    assert(width <= MAX_WIDTH);
    assert((width + lsb) <= MAX_WIDTH);

    /* If given unsigned value does not fit in width signed bits, it must 
     * raise
     * the exception Bitpack Overflow. */
    if (Bitpack_fitsu(value, width) != true) {
        RAISE(Bitpack_Overflow);
        assert(0);
    }

    /* Create the mask. */
    uint32_t left_field = shift_leftu(1, MAX_WIDTH - width - lsb) - 1;;
    left_field = shift_leftu(left_field, width + lsb);
    uint32_t right_field = shift_leftu(1, lsb) - 1;
    uint32_t mask = left_field + right_field;
    
    /* Use mask to get the new word. */
    uint32_t result = (word & mask) | shift_leftu(value, lsb);

    return result;              
}



/* 
 * Flips the endian-ness of a 32 bit word. 
 */
uint32_t flip_word(uint32_t word){
        uint32_t return_word = 0;
        for(int i = 0; i < 4; i++){
                uint32_t byte = Bitpack_getu(word, 8, i*8);
                return_word = Bitpack_newu(return_word, 8, 24-(8*i), byte);
        }
        return return_word;
}