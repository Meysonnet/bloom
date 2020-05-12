//
//  BloomFilter.h
//  BloomFilters
//
//  Created by christophe on 04/04/2019.
//  Copyright © 2019 christophe. All rights reserved.
//

#ifndef BloomFilter_h
#define BloomFilter_h

#include <limits.h>        /* for CHAR_BIT */
#include <stdint.h>

#define BITMASK(b) (1 << ((b) % CHAR_BIT))
#define BITSLOT(b) ((b) / CHAR_BIT)
#define BITSET(a, b) ((a)[BITSLOT(b)] |= BITMASK(b))
#define BITCLEAR(a, b) ((a)[BITSLOT(b)] &= ~BITMASK(b))
#define BITTEST(a, b) ((a)[BITSLOT(b)] & BITMASK(b))
#define BITNSLOTS(nb) ((nb + CHAR_BIT - 1) / CHAR_BIT)

typedef struct BloomFilter {
    
    // BitSet of the bloom filter
    char* bitarray;
    
    //  number of bits in the Bloom Filter
    uint32_t bitset_size;
    
    // number of hash functions to use
    int nb_hash_functions;
    
    // array of seeds to use in order to have different hash functions
    uint32_t* seeds;
    
} BloomFilter;

BloomFilter* init(const int n, const double p);
void insert(BloomFilter* filter, const char* url, bool verbose);
bool is_member_of(BloomFilter* f, const char* url);

#endif /* BloomFilter_h */
