// External Binary Search Tree code for BCC203
// Code by Gabriel Carlos Silva 23.1.4016

#include "ExternalSearchTree.h"

// Sub-functions (general purposes)
/// Create new external nodes
Node nodeStarter(registry_t information){
    Node newNode;
    newNode.data = information;
    newNode.left = newNode.right = -1;
    return newNode;
}

// Main functions
/// Generates the file of the binary search tree
bool fileGenerator(char _dataFileName[NAME], int _registersQuant){                               /// USED PLACEHOLDER FOR COMMONS (NAME const)
    FILE *dataFile, *treeFile;

    // Open/crete files and prevent errors
    if ((dataFile = fopen(_dataFileName, "rb")) == NULL){
        printf("Error: cannot open the data file\n");
        return false;
    }
    if ((treeFile = fopen("ExternalTree", "wb")) == NULL){
        printf("Error: cannot create the External Search Tree file\n");
        return false;
    }
    fclose(treeFile);

    // Start generating
    printf("Generating the External Search Tree File...\n");
    
    pagina pageInfo;                                                        // USED PLACEHODER FOR COMMONS (pagina type)
    int currentPage = 0;
    int registersAtFile = 0;
    int itensAtCurrentPage = ITENS_PAG;                                     /// USED PLACEHOLDER FOR COMMONS (ITENS_PAGE)
    int totalPages = _registersQuant / ITENS_PAG;                           /// USED PLACEHOLDER FOR COMMONS (ITENS_PAGE)

    while (currentPage <= totalPages){
        if(currentPage == totalPages)
            itensAtCurrentPage = _registersQuant - (currentPage * ITENS_PAG);       /// USED PLACEHOLDER FOR COMMONS (ITENS_PAGE)
        
        fread(&pageInfo, sizeof(registry_t), itensAtCurrentPage, dataFile);

        for (int i = 0; i < itensAtCurrentPage; i++){                           // Futuramente vou tentar passar a pagina inteira como parametro, reduzindo as vezes que o arquivo é aberto, mas isso só será feito após eu ter a certeza que tudo está em ordem com a inserção
            if(insertion(pageInfo[i], registersAtFile) == false){
                printf("Fatal Error! Insertion Failed");
                return false;                   
            }
            registersAtFile++;                                             
        }
        currentPage++;
    }

    fclose(dataFile);
    printf("The file was created successfully...\n");
    return true;
}


/// Insert the node in the External Memory
bool insertion(registry_t _entry, int _registersAtFile){
    FILE *treeFile;
    if ((treeFile = fopen("ExternalTree", "rb+")) == NULL){
        printf("Error: cannot open the Exeternal Search Tree file\n");
        return false;
    }

    Node NewNode = nodeStarter(_entry);

    // First entry on the binary search tree (root node)
    if(_registersAtFile == 0){
        fwrite(&NewNode, sizeof(Node), 1 ,treeFile);
        fclose(treeFile);
        return true;
    }

    Node currentNode;
    bool processFail = false;
    bool processSucess = false;

    while(processSucess == false){

        if(!fread(&currentNode, sizeof(Node), 1, treeFile)){
            printf("Reading Error...\n");
            processFail = true;
            break;
        }

        //  Check left first
        if(NewNode.data.key < currentNode.data.key){

            //Check if the current node has a son
            if (currentNode.left == -1){
                // Insertion process
                // Update current node, now it points to the end of file

                currentNode.left = _registersAtFile;
                fseek(treeFile, -1 * sizeof(Node), SEEK_CUR);
                fwrite(&currentNode, sizeof(Node), 1, treeFile);

                //  Adds new node

                fseek(treeFile, 0, SEEK_END);
                fwrite(&NewNode, sizeof(Node), 1, treeFile);
                processSucess = true;

            } else {
                // Keep Looking
                fseek(treeFile, (currentNode.left) * sizeof(Node), SEEK_END);
            }

        //  Check right
        } else if (NewNode.data.key > currentNode.data.key){

            //Check if the current node has a son
            if (currentNode.right == -1){
                // Insertion process
                // Update current node, now it points to the end of file

                currentNode.right = _registersAtFile;
                fseek(treeFile, -1 * sizeof(Node), SEEK_CUR);
                fwrite(&currentNode, sizeof(Node), 1, treeFile);

                //  Adds new node

                fseek(treeFile, 0, SEEK_END);
                fwrite(&NewNode, sizeof(Node), 1, treeFile);
                processSucess = true;


            } else {
                // Keep Looking
                fseek(treeFile, (currentNode.right) * sizeof(Node), SEEK_END);
            }

        // Nodes are equal, that means a fail
        } else {
            printf("Error: Nodes are equal...\n");
            processFail = true;
            break;
        }
    }

    fclose(treeFile);

    if(processFail == true){
        printf("Insertion Error: something went wrong...\n");
        return false;
    }

    printf("Temp: sucess %d !\n", _registersAtFile);                                            // Only for tests, remove after
    return true;
}


Result ExternalSearchTree(char _dataFileName[NAME], int _registersQuant){                        /// USED PLACEHOLDER FOR COMMONS (NAME const)
    Result tempResult;
    // inicializa result (função esperada do arquivo commons)               /// USED PLACEHOLDER FOR COMMONS (inicialization function)

    if(fileGenerator(_dataFileName, _registersQuant) == false){
        printf("External Search Tree failed. Ending execution...\n");     
        return tempResult;
    }

}

