// <commons.h>

#ifndef _COMMON_HEADER_
#define _COMMON_HEADER_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>


// Modifier for forcing function inlining.
// * Definition compatible with gcc.
#define finline			__attribute__((always_inline)) inline


// Registry and Data
// -----------------

// Registry data_2 array size.
#define RD2_SIZE	1000

// Registry data_3 array size.
#define RD3_SIZE	5000


typedef int32_t key_t;


// A representation of a single registry in the database.
typedef struct {
	key_t key;		// The information for localization.

	int64_t data_1;
	char data_2[RD2_SIZE];
	char data_3[RD3_SIZE];
} registry_t;		// Occupies 6016 [bytes]. Allign by 8 [bytes].


typedef struct {
    registry_t target;  //6016 al. 8

    struct {
        double construction_time;
        double time_span;
    } measures; // 16 al. 8
} search_result; // 6032 al. 8



#endif // _COMMON_HEADER_
