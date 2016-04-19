
#include "stdlib.h"
#include "stdio.h"
#include "mem_arr.h"
#include "seg.h"


struct Mem_arr {
	unsigned length;
	unsigned capacity;
	Seg_T arr[];
};

Mem_arr Mem_arr_new(unsigned hint){
	Mem_arr memory = malloc(sizeof(*memory) + (hint * sizeof(Seg_T)));
	memory->length = 0;
	memory->capacity = hint;
	return memory;
}

void Mem_arr_free(Mem_arr memory){
	unsigned memory_length = memory->length;
	Seg_T to_free;
	for(unsigned i = 0; i < memory_length; i++){
		to_free = Mem_arr_get(memory, i);
		Seg_free(to_free);
	}
	free(memory);
}

Mem_arr Mem_arr_resize(Mem_arr memory){
	Mem_arr new_array = Mem_arr_new(2 * memory->capacity);
	unsigned memory_length = memory->length; 

	new_array->capacity = 2 * memory->capacity;
	new_array->length = memory_length;

	for(unsigned i = 0; i < memory_length; i++){
		//see if new_array->arr[i] = memory->arr[i] works
		Seg_T segment = Mem_arr_get(memory, i);
		Seg_T duplicate_segment = Seg_duplicate(segment);
		new_array->arr[i] = duplicate_segment;
	}

	Mem_arr_free(memory);
	return new_array;
}

unsigned Mem_arr_length(Mem_arr memory){
	return memory->length;
}

Seg_T Mem_arr_put(Mem_arr memory, Seg_T segment, unsigned segIndex){
	Seg_T previous_segment = memory->arr[segIndex];
	memory->arr[segIndex] = segment;  
	return previous_segment;
}

Seg_T Mem_arr_get(Mem_arr memory, unsigned segIndex){
	return memory->arr[segIndex];
}

Mem_arr Mem_arr_addhi(Mem_arr memory, Seg_T segment){
	unsigned length = memory->length;

	if(length == memory->capacity){
		Mem_arr new_memory = Mem_arr_resize(memory);
		new_memory->arr[length] = segment;
		new_memory->length++;
		return new_memory;
	}
	else{
		memory->arr[length] = segment;
		memory->length++;
		return memory;
	}
}