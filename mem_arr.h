
#ifndef MEM_ARRAY_H
#define MEM_ARRAY_H

#include "seg.h"

typedef struct Mem_arr *Mem_arr;

extern Mem_arr Mem_arr_new(unsigned hint);
extern void Mem_arr_free(Mem_arr memory);
extern Mem_arr Mem_arr_resize(Mem_arr memory);
extern unsigned Mem_arr_length(Mem_arr memory);
extern Seg_T Mem_arr_put(Mem_arr memory, Seg_T segment, unsigned segIndex);
extern Seg_T Mem_arr_get(Mem_arr memory, unsigned segIndex);
extern Mem_arr Mem_arr_addhi(Mem_arr memory, Seg_T segment);


#endif