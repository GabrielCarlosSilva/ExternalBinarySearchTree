// <paging.h>


#ifndef _PAGING_HEADER_
#define _PAGING_HEADER_

#include "commons.h"

//
#define ITENS_PER_PAGE 5

//
#define PAGES_PER_FRAME 5


//
typedef struct{
    registry_t reg[ITENS_PER_PAGE];
    uint32_t num_page;   
} page_t;


#define SIZEOFPAGE              30080       // * for instance: I = 5
#define locate_page(_Index)     _Index * SIZEOFPAGE

//
typedef struct{
    page_t pages[PAGES_PER_FRAME];
    int first;
    int last;
    int sized;
} frame_t;


#define NULL_INDEX      (-1)


//
#define isFrameFull(_Frame)  (_Frame -> sized == PAGES_PER_FRAME)

//
#define isFrameEmpty(_Frame) (_Frame -> sized == 0)


void makeFrame(frame_t *_Frame);

bool removePage(frame_t *_Frame);


bool addPage(uint32_t num_page, frame_t *_Frame, FILE *_Stream);

void showFrame(frame_t * _Frame);



/*  From a data file-stream, reads a single registry page, specified by 
    its index. Returns the number of registries read that were read onto the page. */
size_t readPage(FILE * _RegistryStream, uint32_t _Index, page_t * _ReturnPage);

/*  asd */
size_t writePage(FILE * _RegistryStream, uint32_t _Index, const page_t * _Registry);



#endif  // _PAGING_HEADER_