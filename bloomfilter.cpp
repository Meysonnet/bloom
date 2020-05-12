//
//  BloomFilter.cpp
//  BloomFilters
//
//  Created by christophe on 04/04/2019.
//  Copyright © 2019 christophe. All rights reserved.
//

#include "bloomfilter.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "murmur3.h"
#include <inttypes.h>

/**
 *
 * Initialize a Bloom Filter for a given number of items and a given false positive rate.
 *
 * @param n number of items expected in the bloom filter
 * @param p false positive rate (e.g. 0.01 for 1%)
 *
 */
BloomFilter* init(const int n, const double p) {
    
    /**
     *
     * Dimensioning the Bloom Filter param.
     
     Here is an example from the lab
     * n = 1 000 000
     * k = 0.01
     *
     * Therefore :
     * m = -nln(p) / ln(2)^2
     * m = 1,000,000*ln(0.01) / ln(2)*ln(2)
     * m = 9.585058 . 10^6 (~ 1,142 Mo)
     *
     * k = m/n ln(2)
     * k = (9.585058 . 10^6 / 1,000,000) * ln(2)
     * k ~ 6,643855928203548 (=> k = 7)
     */
    
    BloomFilter* filter = new BloomFilter;
    
    // compute the number of bits we need
    const int m = -1 * ((n*log(p))/(log(2)*log(2)));
    
    // compute the number of hash functions we need
    const int k = round((m/n)*log(2));
    
    // print some infos
    printf("Bloom filter parameters :\n");
    printf("n = %d items\n", n);
    printf("p = %f false positive rate\n", p);
    printf("m = %d bits\n", m);
    printf("k = %d hash functions used\n\n", k);
    
    // initialize the bloom filter fields
    filter->bitset_size = m; // keep the info of nuimber of bits in the bitset
    filter->bitarray = new char[BITNSLOTS(m)]; // create the bitset itself
    filter->nb_hash_functions = k; // keep track of the number of hash functions used
    filter->seeds = new uint32_t[k]; // seeds for the k hash functions
    
    
    // create the k DIFFERENT seeds numbers once for all
    // Code adapted from : https://www.quora.com/How-can-I-generate-10-different-random-numbers-in-the-C-program-in-one-run
    time_t t;
    int random_numbers[k];
    
    // initialize the values to 0
    for (int i = 0; i < k; i++) {
        random_numbers[k] = 0;
    }
    int count = 0;
    
    // given current time as the seed for the pseudo random generator used in the program.
    srand((unsigned) time(&t));
    
    while(count < k) {
        int randNum = rand(); //Generate a random number
        
        // we exclude the value 0 (it was the initialization value)
        if (randNum == 0) {
            break;
        }
        
        //assume the random number hasnt been generated
        bool found = false;
        
        //make sure the random number hasnt already been generated
        for (int i = 0; i < k; i++) {
            if(random_numbers[i] == randNum) {
                found = true;
                //once we have located the number we dont need to continue checking
                break;
            }
        }
        
        //we have a new random number
        if(!found) {
            filter->seeds[count] = randNum;
            count++;
        }
    }
    
    // let's print some infos
    printf("Seeds used in the filter : \n");
    for (int idx = 0; idx < k; idx++) {
        printf("Seed %d : %d\n", idx, filter->seeds[idx]);
    }
    
    return filter;
    
}


/**
 *
 * Insert an element in the bloom filter
 *
 * @param filter the bloom filter to insert into
 * @param url the url to insert in the bloom filter
 *
 */
void insert(BloomFilter* filter, const char* url, bool verbose) {
   
    if (verbose)
        printf("\nInserting %s in the the filter\n", url);
    
    for (int i = 0; i < filter->nb_hash_functions; i++) {
        
        // fetch the i-th seed
        uint32_t seed = filter->seeds[i];
        
        // create a variable to will hold the hash compute using murmur
        // since we will use the function that yields 32 bits so a single uint32_t is enough.
        uint32_t* hash = new uint32_t(sizeof(uint32_t));
        
        // hash the url (it will fill the hash variable)
        MurmurHash3_x86_32(url, (int) strlen(url), seed, hash);
        
        // given the hash, apply modulo to know which bit to flip
        uint32_t idx = hash[0] % filter->bitset_size;
        
        // Flip the associated bit
        if (verbose)
            printf("Hash %d : set bit %" PRIu32 " to 1 ("
                   "url :  %s, "
                   "hash value:  %" PRIu32 ", "
                   "bitset size: %" PRIu32 ") ...\n",
                   i, idx, url, hash[0], filter->bitset_size);
        
        BITSET(filter->bitarray, idx);
        
    }
}

/**
 * Membership query on the bloom filter
 */
bool is_member_of(BloomFilter* filter, const char* url) {
    
    printf("\n\nTesting if %s is member of the filter\n", url);
    
    for (int i = 0; i < filter->nb_hash_functions; i++) {
        
        // fetch the i-th seed
        uint32_t seed = filter->seeds[i];
        
        // create a variable to will hold the hash compute using murmur
        // since we will use the function that yields 32 bits so a single uint32_t is enough.
        uint32_t* hash = new uint32_t(sizeof(uint32_t));
        
        // hash the url (it will fill the hash variable)
        MurmurHash3_x86_32(url, (int) strlen(url), seed, hash);

        // given the hash, apply modulo to know is that bit was set
        uint32_t idx = hash[0] % filter->bitset_size;
        
        bool is_set = BITTEST(filter->bitarray, idx);
        // Test the associated bit
        printf("Hash %d : bit %" PRIu32 " is set to %d ("
               "url: %s, "
               "hash value: %" PRIu32 ", "
               "bitsetsize: %" PRIu32 ") ...\n",
               i, idx, is_set, url, hash[0], filter->bitset_size);
        
        if (!is_set) {
            return false;
        }
    }
    
    // all bits that should been set were set.
    return true;
    
}
