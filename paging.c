// <paging.c>
#include "paging.h"


// Inicializa os valores iniciais do frame.
inline void 
makeFrame(frame_t * _Frame) { 
    _Frame -> first = -1;
    _Frame -> last = -1;
    _Frame -> sized = 0;
}

bool removePage(frame_t * _Frame) { // Remove uma paǵina usando o sistema de Fila Circular (nesse caso, é uma adaptação da função "desenfileirar")
    if (isFrameEmpty(_Frame))
        return false;

    //
    if (_Frame -> last == _Frame -> first){
        _Frame -> last = -1;
        _Frame -> first = -1;
    }
    //
    else {
        _Frame->first = (_Frame -> first + 1) % PAGES_PER_FRAME;
    }
    _Frame->sized--;

    return true;
}



// será que depois você adapta pro ing. pá nóis? Claro, só vou deixar assim por enquanto, comentário dá pra mudar de boas ok irmandade
bool addPage(uint32_t num_page, frame_t *_Frame, FILE *_Stream){ // Adiciona uma página nova para o _Frame. Recebe o número da página que será colocada
    if (isFrameFull(_Frame)) {
        if (! removePage(_Frame))
            return false;
    }
    if(isFrameEmpty(_Frame)){
        _Frame->first = 0;
        _Frame->last = 0;
    }
    else{
        _Frame->last = (_Frame->last + 1) % PAGES_PER_FRAME;
    }
    _Frame->pages[_Frame -> last].num_page = num_page; //Adiciona o número da página para a estrutura
    
    fseek(_Stream, locate_page(num_page), 0);
    fread(_Frame->pages[_Frame -> last].reg, sizeof(registry_t), ITENS_PER_PAGE, _Stream); //lê os registros do _Streamuivo e o escreve na página do _Frame
    
    _Frame -> sized ++;
    return true;
}


void showFrame(frame_t *_Frame){ // Mostra as páginas do _Frame, com apenas o seus registros e o número da respectiva página
    if(isFrameEmpty(_Frame)){
        return;
    }
    for (int i = 0; i < PAGES_PER_FRAME; i++) {
        printf("Page %d | (%d)\t", i+1, _Frame -> pages[i].num_page);
        if (i == _Frame -> first) {
            printf("<- first");
        } else if (_Frame -> last == i) {
            printf("<- last");
        }

        putchar('\n');

        for(int j=0; j<ITENS_PER_PAGE; j++){
            printf("Reg %d\n", _Frame->pages[i].reg[j].key);
        }
        printf("\n");
    }
    printf("\n\n\n");
}


/*  From a data file-stream, reads a single registry page, specified by 
    its index. Returns the number of registries read that were read onto the page. */
inline size_t readPage(FILE * _Stream, uint32_t _Index, page_t * _ReturnPage)
{
    // Setting the file pointer to the beggining of the indexed page on the file.
    fseek(_Stream, (uint64_t) _Index * SIZEOFPAGE, SEEK_SET);
    return fread(_ReturnPage -> reg, sizeof(registry_t), ITENS_PER_PAGE, _Stream);
}

/*  */
inline size_t writePage(FILE * _Stream, uint32_t _Index, const page_t * _Page)
{
    //
    fseek(_Stream, (uint64_t) _Index * SIZEOFPAGE, SEEK_SET);
    return fwrite(_Page -> reg, sizeof(registry_t), ITENS_PER_PAGE, _Stream);
}

