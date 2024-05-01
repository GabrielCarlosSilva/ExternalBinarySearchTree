#ifndef EXTERNALSEARCHTREE.H
#define EXTERNALSEARCHTREE.H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

// include "commons.h"
// Tipos temporários, serão substituidos pelos tipos do arquivo Commons
// ################################

#define NAME 50
#define ITENS_PAG 50

// Registry data_2 array size.
#define RD2_SIZE	1000

// Registry data_3 array size.
#define RD3_SIZE	5000


// A representation of a single registry in the database.
typedef struct {
	int key;		// The information for localization.

	int64_t data_1;
	char data_2[RD2_SIZE];
	char data_3[RD3_SIZE];
} registry_t;		// Occupies 6016 [bytes]. Allign by 8 [bytes].

typedef struct {
    registry_t target;
} Result;

typedef registry_t pagina[ITENS_PAG];

// ################################




// Tipos finais desse modulo

typedef struct {
    registry_t data;
    int left, right;
} Node;

Result ExternalSearchTree(char*, int);


#endif 