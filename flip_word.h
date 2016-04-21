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

#include "stdint.h"

/* flips the endian-ness of a 32 bit word */ 
uint32_t flip_word(uint32_t word);

#endif