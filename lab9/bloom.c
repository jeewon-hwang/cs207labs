#include "bloom.h"

index_t hash1(bloom_filter_t *B, key_t k) {
    uint64_t x = k;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x);
    return x;
}

index_t hash2(bloom_filter_t *B, key_t k) {
	uint64_t a = k;
	a = (a ^ 61) ^ (a >> 16);
    a = a + (a << 3);
    a = a ^ (a >> 4);
    a = a * 0x27d4eb2d;
    a = a ^ (a >> 15);
    return a;
}

void set_bit(bloom_filter_t *B, index_t i) {
	index_t table_index = i>>(uint64_t)6;
	index_t bit_index = i%(uint64_t)64;
	B->table[table_index] = B->table[table_index] || (uint64_t)1<<bit_index;
}

index_t get_bit(bloom_filter_t *B, index_t i) {
	index_t table_index = i>>(uint64_t)6;
	index_t bit_index = i%(uint64_t)64;
	index_t mask = ((uint64_t)1)<<bit_index;
	return (B->table[table_index] && mask)>>bit_index;
}

void bloom_init(bloom_filter_t *B, index_t size_in_bits) {
	B->count = 0;
	B->size = size_in_bits;
	
	index_t size_in_uint64;

	if (size_in_bits%(sizeof(uint64_t)*8) != 0)
		size_in_uint64 = size_in_bits/(8*sizeof(uint64_t))+1;
	else
		size_in_uint64 = size_in_bits/(8*sizeof(uint64_t));
	
	B->table = calloc(size_in_uint64, sizeof(uint64_t));
}

void bloom_destroy(bloom_filter_t *B) {
	free(B);
}

int bloom_check(bloom_filter_t *B, key_t k) {
	index_t counter = 0;
	int i;
	
	for (i=0; i < N_HASHES; i++) {
		key_t hash_key = (hash1(B, k) + i*hash2(B, k))%(B->size);
		counter += get_bit(B, hash_key);
	}

	if (counter == N_HASHES)
		return 1;
	else 
		return 0;
}

void bloom_add(bloom_filter_t *B, key_t k) {
	int i;
	for(i=0; i < N_HASHES; i++) {
		key_t hash_key = (hash1(B, k) + i*hash2(B, k))%(B->size);
		set_bit(B, hash_key);
		B->count+=1;
	}
}

void smoke_test()
{
	bloom_filter_t *B = (bloom_filter_t *) malloc(sizeof(bloom_filter_t));
	bloom_init(B, 1000);
	
	int i;
	for(i = 1; i <= 70; i++){
		bloom_add(B, i);
	}
	
	int counter;
	for(i = 0; i < 1000; i++) {
		if (get_bit(B, i))
			counter+=1;
	}
	printf("Number of bits set from smoke test = %d\n", counter);
}



void final_test(index_t *array1, index_t *array2) {
	//Finally, create a loop that checks whether 
    //the numbers in the second array are in the table. 
    //Recall that we never added them, 
    //so if our hash table was perfect, there would be a very small number of collisions 
    //(equal to the number of expected collisions 
    //between two sets of 100 random numbers from 0-1000000). 
    //In reality, there will be many more, because of the way Bloom filters work. 
    //Count up and print out how many of the numbers in the second array returned true.
	
	bloom_filter_t *B = (bloom_filter_t *) malloc(sizeof(bloom_filter_t));
	bloom_init(B, 1000);
	
	int i;
	for (i = 0; i < 100; i++) {
		bloom_add(B, array1[i]);
	}
	
	// for computing occupancy 
	int counter = 0;
	for (i = 0; i < 1000; i++) {
		if (get_bit(B, i))
			counter+=1;
	}
	printf("Occupancy : %d\n", counter);
	
	// for computing FP rate
	counter = 0;
	for (i = 0; i < 100; i++) {
		if(bloom_check(B, array2[i]))
			counter+=1;
	}
	printf("False Positive Rate : %d\n", counter);
}


int main() {
    bloom_filter_t *B;
    int problem_1_array[] = {0, 1, 2, 3, 13, 97};
    int i;

    printf("Hash 1 for {0, 1, 2, 3, 13, 97}\n");
    for(i=0; i < sizeof(problem_1_array)/sizeof(int); i++) {
    	printf("%llu\n", hash1(B, problem_1_array[i]));	
    }
    printf("Hash 2 for {0, 1, 2, 3, 13, 97}\n");
    for(i=0; i < sizeof(problem_1_array)/sizeof(int); i++) {
    	printf("%llu\n", hash2(B, problem_1_array[i]));	
    }

    // 3. For Smoke test
    smoke_test();

    // 4. For final test
    index_t array1[100];
    index_t array2[100];

    for(i=0; i<100; i++) {
    	array1[i] = rand()%1000000;
    	array2[i] = rand()%1000000;
    }

    final_test(array1, array2);

    
    return 0;
}