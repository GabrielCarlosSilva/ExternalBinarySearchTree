#ifndef REDBLACKEXTERNALSEARCHTREE.H
#define REDBLACKEXTERNALSEARCHTREE.H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#include "paging.h"
#include "commons.h"

// Types used in the algorithm

typedef registry_t page_info[ITENS_PER_PAGE];

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
//	search_result tempResult;
//	if(!ExternalSearchTree(&tempResult ,FileName, RegisterQuant, TargetKey))	
//		printf("Execution Failed");
//
bool RedBlackExternalSearchTree(search_result*, char*, int, int);


#endif 