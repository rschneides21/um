/*
 *      execute.c
 *      Purpose:  Implementation for execute.h
 *                Implements the instruction cycle for the UM. Fetches
 *                instructions from segment 0 in memory and executes
 *                until there is a halt instruction or there are no more 
 *                            instructions to be executed
 *                
 *      Date:     4/14/2016 
 *      By:       Ryan Schneiderman and Matthew Epstein
 */

#include "execute.h"
#include "flip_word.h"
#include "assert.h"
#include "opmemory.h"

typedef enum opcodes {CONDITIONAL_MOVE = 0, SEGMENTED_LOAD, SEGMENTED_STORE, 
                        ADDITION, MULTIPLICATION, DIVISION, BITWISE_NAND, HALT,
                       MAP_SEGMENT, UNMAP_SEGMENT, OUTPUT, INPUT, LOAD_PROGRAM,
                       LOAD_VALUE} opcodes;

/*
 * This function takes in a 32-bit word and an integer that is between 0 and 
 * 29 (inclusive) that represents the location of registerA.  It will return an
 * integer that represents the A register.
 */
static inline unsigned decode_registerA(uint32_t word)
{
        uint32_t mask = word & 0x1c0;
        unsigned registerA = mask / 0x40;
        return registerA;
}

static inline unsigned decode_registerA_val(uint32_t word){
        uint32_t mask = word & 0x0e000000;
        unsigned registerA = mask / 0x02000000;
        return registerA;
}

/*
 * This function takes in a 32-bit word and returns the integer represented by
 * the B register
 */
static inline unsigned decode_registerB(uint32_t word)
{
        uint32_t mask = word & 0x38;
        unsigned registerB = mask / 0x8;
        return registerB;
}

/*
 * This function takes in a 32-bit word and returns the integer represented by
 * the C register
 */
static inline unsigned decode_registerC(uint32_t word)
{
        uint32_t mask = 0x7;
        unsigned registerC = word & mask;   
        return registerC;       
}


/*
 * This function will take in a 32-bit word and return the first four bits 
 * (the opcode) represented as an integer.  The opcode is always represented in
 * the same place in the word, so no offset parameter is required.
 */
static inline unsigned decode_opcode(uint32_t word)
{
        uint32_t mask = word & 0xf0000000;
        unsigned opcode = mask / 0x10000000;
        return opcode;
}

/*
 * This function will take in a 32-bit word and return the 25 bits that 
 * represent the word’s value.  Like the opcode, the value is always 
 * represented in the same place in the word, so no offset parameter is 
 * required.
 */
static inline int decode_value(uint32_t word)
{
        return(word & 0x1ffffff);
}


static inline void decode_registers(uint32_t word, unsigned *regA, unsigned *regB, unsigned
                                                                        *regC){
        *regA = decode_registerA(word);
        *regB = decode_registerB(word);
        *regC = decode_registerC(word);
}



/* See Purpose in header */
void execute(Mem_T memory){
        uint32_t programCounter = 0;
        uint32_t programLength = get_segment_length(memory, 0);
        uint32_t word = 0;
        int input;

        uint32_t regs[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        unsigned regA, regB, regC;
        while(programCounter < programLength) {
                word = load_word(memory, 0, programCounter);
                unsigned opcode = decode_opcode(word);
                if(opcode != LOAD_VALUE){
                        decode_registers(word, &regA, &regB, &regC);
                }
                switch(opcode){
                        case CONDITIONAL_MOVE:
                                if(regs[regC]!= 0) {
                                        regs[regA] = regs[regB];
                                }
                                break;

                        case SEGMENTED_LOAD:
                                regs[regA] = load_word(memory, regs[regB], 
                                        regs[regC]);
                                break;

                        case SEGMENTED_STORE:
                                store_word(memory, regs[regC], regs[regA], 
                                        regs[regB]);
                                break;

                        case ADDITION:
                                regs[regA] = (regs[regB] + regs[regC]);
                                break;

                        case MULTIPLICATION:
                                regs[regA] = (regs[regB] * regs[regC]);
                                break;

                        case DIVISION:
                                regs[regA] = (regs[regB] / regs[regC]); 
                                break;

                        case BITWISE_NAND:
                                regs[regA] = ~(regs[regC] & regs[regB]);
                                break;

                        case HALT:
                                MemT_free(memory);
                                exit(EXIT_SUCCESS);     
                                break;

                        case MAP_SEGMENT:
                                regs[regB] = map_segment(memory, regs[regC]);
                                break;

                        case UNMAP_SEGMENT:
                                unmap_segment(memory, regs[regC]);
                                break;

                        case OUTPUT:
                                fprintf(stdout, "%c", regs[regC]);
                                break;

                        case INPUT:
                                input = getchar();
                                if(input == EOF) {
                                        uint32_t endInput = ~0;
                                        regs[regC] = endInput;
                                } else {
                                    assert(input >=0 && input <= 255);
                                    regs[regC] = input;
                                }
                                break;
                                
                        case LOAD_PROGRAM:
                                if(regs[regB] != 0){
                                        fprintf(stderr, "loading into segment 0\n");
                                        load_program(memory, regs[regB]);
                                }
                                programCounter = regs[regC];
                                programLength = get_segment_length(memory, 0);
                                break;

                        case LOAD_VALUE:
                                regs[decode_registerA_val(word)] = 
                                                        decode_value(word);
                                break;
                }
                if(opcode != LOAD_PROGRAM) {
                        programCounter++;
                }
        }
        return;
}
    