
#include "seg.h"

struct Seg_T{
	unsigned length
	uint32_t arr[];
};


extern Seg_T Seg_new(unsigned numWords){
	//test make sure this works
	Seg_T segment = malloc(sizeof(*segment) + numWords * sizeof(uint32_t));
	segment->length = numWords;
	return segment;
}

extern void Seg_free(Seg_T segment){
	assert(segment && *segment);
	//free(segment->arr);
	free(segment);
}

extern void Seg_store(Seg_T segment, unsigned offset, uint32_t word){
	segment->arr[offset] = word;
}

extern uint32_t Seg_load(Seg_T segment, unsigned offset){
	return segment->arr[offset];
}

extern Seg_T Seg_duplicate(Seg_T segment){
	Seg_T dup_segment = Seq_new(segment->length);
	return dup_segment;
}

extern unsigned Seg_length(Seg_T segment){
	return segment->length;
}
