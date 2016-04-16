
/*
 *     Filename: 32bitpack.h
 *     Purpose: Interface for 32bitpack module
 *     By: Ryan Schneiderman and Matthew Epstein
 *     Date: April 14, 2016
 */

#ifndef _32BITPACK_H
#define _32BITPACK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <except.h>
#include <stdint.h>

uint32_t Bitpack_getu(uint32_t word, unsigned width, unsigned lsb);

bool Bitpack_fitsu(uint32_t n, unsigned width);

uint32_t Bitpack_newu(uint32_t word, unsigned width, unsigned lsb, 
                                                        uint32_t value);

#endif 