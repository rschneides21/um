

#include <stdint.h>
#include "assert.h"
#include "seg.h"
#include "stdio.h"

struct Seg_T {
	unsigned length;
	uint32_t arr[];
};


Seg_T Seg_new(unsigned numWords){
	Seg_T segment = malloc(sizeof(*segment) + (numWords * sizeof(uint32_t)));
	segment->length = numWords;
	for(unsigned i = 0; i < numWords; i++){
		segment->arr[i] = 0;
	}
	return segment;
}

void Seg_free(Seg_T segment){
	free(segment);
}

void Seg_store(Seg_T segment, unsigned offset, uint32_t word){
	segment->arr[offset] = word;
}

uint32_t Seg_load(Seg_T segment, unsigned offset){
	return segment->arr[offset];
}

Seg_T Seg_duplicate(Seg_T segment){
	Seg_T dup_segment = Seg_new(segment->length);
	for(unsigned i = 0; i < segment->length; i++){
		dup_segment->arr[i] = segment->arr[i];
	}
	return dup_segment;
}

unsigned Seg_length(Seg_T segment){
	return segment->length;
}

void print_segment(Seg_T segment){
	for(unsigned i = Seg_length(segment) - 10; i < Seg_length(segment); i++){
		fprintf(stderr, "word is: %x\n", segment->arr[i]);
	}
}
