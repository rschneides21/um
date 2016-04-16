/*
 *      Decode.h
 *      Purpose:  Interface for the decode module
 *                                Deals with decoding values from 32 bit words.
 *                Used to unpack registers, opcodes, and values from
 *                program instructions.
 *                
 *      Date:     4/14/2016 
 *      By:           Ryan Schneiderman and Matthew Epstein
 */


#ifndef DECODE_H
#define DECODE_H

#include "32bitpack.h"

/*
 * This function takes in a 32-bit word and an integer that is between 0 and 
 * 29 (inclusive).  It will return an integer that represents the desired 
 * register.
 */
int decode_register(uint32_t word, int offset);

/*
 * This function takes in a 32-bit word and return the first four bits 
 * (the opcode) represented as an integer.  The opcode is always represented in
 * the same place in the word, so no offset parameter is required.
 */
int decode_opcode(uint32_t word);

/*
 * This function takes in a 32-bit word and return the 25 bits that 
 * represent the word’s value.  Like the opcode, the value is always 
 * represented in the same place in the word, so no offset parameter is 
 * required.
 */
int decode_value(uint32_t word);

/* flips the endian-ness of a 32 bit word */ 
uint32_t flip_word(uint32_t word);

#endif