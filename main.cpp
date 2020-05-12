//
//  main.cpp
//  BloomFilters
//
//  Created by christophe on 04/04/2019.
//  Copyright © 2019 christophe. All rights reserved.
//

#include <iostream>
#include "bloomfilter.h"
#include "million.h"
#include <time.h>

void test(BloomFilter* filter, const char* url) {
    bool has_bben_seen = is_member_of(filter, url);
    if (has_bben_seen) {
        printf("The url %s is in the bloom filter\n", url);
    } else {
        printf("The url %s is not in the bloom filter\n", url);
    }
}


int main(int argc, const char * argv[]) {

    /*
    * Initializes a Bloom Filter :
    * - holding 1 000 000 elements
    * - with a false positive rate of 1%
    */
    printf("Creating the Bloom Filter...\n");
    BloomFilter* filter = init(10, 0.05);
    
    printf("Inserting 5 test URLs...\n");
    const char* url1 = "facebook.com";
    const char* url2 = "twitter.com";
    const char* url3 = "instagram.com";
    const char* url4 = "google.com";
    const char* url5 = "amazon.com";
    
    // insert URl 1 to 4 included
    insert(filter, url1, true);
    insert(filter, url2, true);
    insert(filter, url3, true);
    insert(filter, url4, true);
    
    // test if url3 is in the filter (should be)
    if (is_member_of(filter, url3)) {
        printf("\nCORRECT : Url %s is in the filter !\n", url3);
    } else {
        printf("\nERROR : Url %s is NOT in the filter !\n", url3);
    }
    
    // test if url5 is in the filter (should NOT be)
    if (is_member_of(filter, url5)) {
        printf("\nERROR : Url %s is in the filter !\n", url5);
    } else {
        printf("\nCORRECT : Url %s is NOT in the filter !\n", url5);
    }
    
    // we don't need the test bloom fitler any more
    free(filter);
    
    printf("\n\n-------------------------------\n");
    printf("The million URLs Bloom Filter starts.\n");
    
    printf("Creating the Bloom Filter for URLs...\n");
    BloomFilter* filterUrl = init(1000000, 0.01);
    
    clock_t begin = clock();
    printf("Clock begin : %d\n", begin);
    parse(filterUrl, "/home/user/bloom/top-1m.csv");
    clock_t end = clock();
    printf("Clock end : %d\n", end);
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time spent building the filter : %d k-ticks", time_spent/1000);
    
    // test facebook.com
    const char* top_url = "facebook.com";
    test(filterUrl, top_url);
    
    // test univ-st-etienne.fr
    const char* top_url2 = "univ-st-etienne.fr";
    test(filterUrl, top_url2);
    
    // test univ-st-etienne.fr
    const char* top_url3 = "telecom-st-etienne.fr";
    test(filterUrl, top_url3);
}
