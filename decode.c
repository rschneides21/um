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

#include "decode.h"
#include "assert.h"

#define REGISTER_WIDTH 3
#define OPCODE_WIDTH 4
#define VALUE_WIDTH 25

/*
 * This function will take in a 32-bit word and an integer that is between 0 
 * and 29 (inclusive).  It will return an integer that represents the desired 
 * register.
 */
int decode_register(uint32_t word, int offset)
{
        assert(offset >= 0 && offset <= 29);
        return Bitpack_getu(word, REGISTER_WIDTH, offset);
}

/*
 * This function will take in a 32-bit word and return the first four bits 
 * (the opcode) represented as an integer.  The opcode is always represented in
 * the same place in the word, so no offset parameter is required.
 */
int decode_opcode(uint32_t word)
{
        int opcode = Bitpack_getu(word, OPCODE_WIDTH, 28);
        return opcode;
}

/*
 * This function will take in a 32-bit word and return the 25 bits that 
 * represent the word’s value.  Like the opcode, the value is always 
 * represented in the same place in the word, so no offset parameter is 
 * required.
 */
int decode_value(uint32_t word)
{
        return Bitpack_getu(word, VALUE_WIDTH, 0);
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



