//
//  million.cpp
//  BloomFilters
//
//  Created by christophe on 04/04/2019.
//  Copyright © 2019 christophe. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "million.h"
#include "bloomfilter.h"

/**
 * Parse the Alexa top wibsite file with path <code>filename</code> and adds each URL in the file to the given Bloom Filter.
 *
 * @param filter The filter in which we will add the top URLs.
 * @param filename the Alexa top 1 millions csv file. It contains (index, URL) pairs in each line.
 * Example of lines in this file are as follows:
 * (...)
 * 998978,sameep.gov.ar
 * 998979,sanadoctrinas.com
 * 998980,sanagua.com.ar
 * 998981,sanantoniotandil.word
 * (...)
 */
void parse(BloomFilter* filter, char* filename) {
    
    // re-read the file to parse each line and add them to the table
    FILE* csv = fopen(filename, "r");
    
    char line[1024];
    while (fgets(line, 1024, csv))
    {
        // let's point to the begining of the line
        char* current = line;
        
        // we look for the index of the first occurence of column
        size_t field_len_idx = strcspn(current, ",");
        
        // We strip the first info - the index in the file that we don't care -
        current = &line[field_len_idx + 1];
        
        // we look for the index of the first occurence of column
        size_t field_len_url = strcspn(current, ",");
        
        // we allocate an empty string
        char* url = (char*)malloc(100);
        // we copy from "current" the first "field_len_name" characters into the "name" string
        strncpy(url, current, field_len_url - 1); // minus 1 because we remove the trailing end line ('\n')
        
        // do not forgot to end the string with \0 !
        url[field_len_url] = '\0';
        
        // we add the url to the filter
        insert(filter, url, false);
	//printf( "url insert %s\n", url);        
    }
    
    // close the opened file
    fclose(csv);
}
