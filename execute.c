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
#include "decode.h"
#include "assert.h"
#include "opmemory.h"

typedef enum opcodes {CONDITIONAL_MOVE = 0, SEGMENTED_LOAD, SEGMENTED_STORE, 
                        ADDITION, MULTIPLICATION, DIVISION, BITWISE_NAND, HALT,
                       MAP_SEGMENT, UNMAP_SEGMENT, OUTPUT, INPUT, LOAD_PROGRAM,
                       LOAD_VALUE} opcodes;

static inline void decode_registers(uint32_t word, int *regA, int *regB, int
                                                                        *regC){
        *regA = decode_register(word, 6);
        *regB = decode_register(word, 3);
        *regC = decode_register(word, 0);
}

/* See Purpose in header */
void execute(Mem_T memory){
        uint32_t programCounter = 0;
        uint32_t programLength = get_segment_length(memory, 0);
        uint32_t word = 0;
        int input;

        uint32_t regs[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        int regA, regB, regC;

        while(programCounter < programLength) {
                word = load_word(memory, 0, programCounter);
                int opcode = decode_opcode(word);
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
                                //fprintf(stderr, "test");
                                programCounter = regs[regC];
                                programLength = get_segment_length(memory, 0);
                                //fprintf(stderr, "program length: %u\n", programLength);
                                break;

                        case LOAD_VALUE:
                                regs[decode_register(word, 25)] = 
                                                        decode_value(word);
                                break;
                }
                if(opcode != LOAD_PROGRAM) {
                        programCounter++;
                }
        }
        return;
}








