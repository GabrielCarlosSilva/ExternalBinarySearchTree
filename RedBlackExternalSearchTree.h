#ifndef REDBLACKEXTERNALSEARCHTREE.H
#define REDBLACKEXTERNALSEARCHTREE.H

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




// Types used in the algorithm

#define RED 1
#define BLACK 0

#define ANSI_RESET            "\x1b[0m"  // desativa os efeitos anteriores
#define ANSI_COLOR_RED        "\x1b[31m"
#define REDTEXT(string)        ANSI_COLOR_RED        string ANSI_RESET

typedef struct {
    registry_t data;
    int left, right, father, line;
	bool color;
} Node;

/// How use this function:
// Parameters: 1- name of the binary file that contains the registers	(char[])
// 2- number of registers in the binary file							(int)
// 3- The key of the register that needs to be found					(int)
// Return: the register and the data related to the performance
// If something go wrong during the execution the boolean variable <> will be false
// Exemple of implemetantion:
//
//	Result tempResult = ExternalSearchTree(FileName, RegisterQuant, TargetKey);
//	if(tempResult.<> == False)	
//		printf("Execution Failed");
//
Result RedBlackExternalSearchTree(char*, int, int);


#endif 