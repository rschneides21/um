

#define SEG_H
#ifndef SEG_H

typedef struct Seg_T *Seg_T;

extern Seg_T Seg_new(unsigned numWords);

extern void Seg_free(Seg_T segment);

extern void Seg_store(Seg_T segment, unsigned offset, uint32_t word);

extern uint32_t Seg_load(Seg_T segment, unsigned offset);

extern Seg_T Seg_duplicate(Seg_T segment);

extern unsigned Seg_length(Seg_T segment);


#endif