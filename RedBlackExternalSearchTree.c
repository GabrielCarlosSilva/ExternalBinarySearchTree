// External Binary Search Tree code for BCC203
// Code by Gabriel Carlos Silva 23.1.4016
#include "RedBlackExternalSearchTree.h"

/// Sub-functions (general purposes)
// Erases The node data
Node NodeNullifier(){
    Node newNode;
    newNode.left = newNode.right = -2;
    newNode.father = -1;
    newNode.color = RED;
    newNode.line = -2;
    return newNode;
}

// Update one node in file based on main memory
void NodeCorretion(FILE* _treeFile, Node _targetNode){
    fseek(_treeFile, _targetNode.line * sizeof(Node), SEEK_SET);
    fwrite(&_targetNode, sizeof(Node), 1,_treeFile);
    rewind(_treeFile);
}

// Update one node in main memory based on file
void NodeRead(FILE* _treeFile, Node* _targetNode){
    fseek(_treeFile, (*_targetNode).line * sizeof(Node), SEEK_SET);
    fread(_targetNode, sizeof(Node), 1,_treeFile);
    rewind(_treeFile);   
}

// Create new external nodes
Node nodeStarter(registry_t information){
    Node newNode;
    newNode.data = information;
    newNode.left = newNode.right = -1;
    newNode.father = -1;
    newNode.color = RED;
    newNode.line = -1;
    return newNode;
}

// Checks if file Exists
bool fileExists(char* _filename){
    struct stat buffer;
    return (stat(_filename, &buffer));
}

// Print the major information of each node on the Red Black Tree, following this format:
// line: X| LeftPointer Key RightPointer Father:X
void printRedBlackTree(FILE* _treeFile, int _quantRegs){
    Node tempNode;
    rewind(_treeFile);
    for(int i = 0; i < _quantRegs; i++){
        fread(&tempNode, sizeof(Node), 1, _treeFile);
        if(tempNode.color == RED){
            printf(REDTEXT("line: %3.d|\t%2.d %d %2.d\tfather:%2.d\n"), tempNode.line, tempNode.left, tempNode.data.key, tempNode.right, tempNode.father);
        } else {
            printf("line: %3.d|\t%2.d %d %2.d\tfather:%2.d\n", tempNode.line, tempNode.left, tempNode.data.key, tempNode.right, tempNode.father);
        }
    }
    rewind(_treeFile);
}

/// External Search Tree generic functions
// Find registry at External File (não há necessidade)
bool ExternalFinder(search_result* _resultInfo, int _targetKey, char* _ExternalFileName){
    Node current;
    FILE * ExternalFile = fopen(_ExternalFileName, "rb");
    _resultInfo->target.key = -2;

    while (fread(&current, sizeof(Node), 1, ExternalFile) != EOF){
        if(current.data.key == _targetKey){
            _resultInfo->target = current.data;
            printf("The key was found! Returning Data...\n");
            break;
        } else if(current.data.key > _targetKey){
            fseek(ExternalFile, current.left * sizeof(Node), SEEK_SET);
        } else if(current.data.key < _targetKey){
            fseek(ExternalFile, current.right * sizeof(Node), SEEK_SET);
        }
    }
    fclose(ExternalFile);  

    if(_resultInfo->target.key == -2){
        printf("Register not found\n");
        return false;
    }
    return true;
}

// Insert the node in the External Memory
bool insertion(FILE* _treeFile, registry_t _entry, int _registersAtFile){
    Node NewNode = nodeStarter(_entry);

    // First entry on the binary search tree (root node)
    if(_registersAtFile == 0){
        NewNode.color = BLACK;
        NewNode.father = NULL;
        NewNode.line = 0;
        fwrite(&NewNode, sizeof(Node), 1 , _treeFile);
        printf(".");
        rewind(_treeFile);
        return true;
    }

    Node currentNode;
    bool processFail = false;
    bool processSucess = false;

    while(processSucess == false){

        if(!fread(&currentNode, sizeof(Node), 1, _treeFile)){
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
                NewNode.father = currentNode.line;
                currentNode.left = NewNode.line = _registersAtFile;
                fseek(_treeFile, -((long)sizeof(Node)), SEEK_CUR);
                fwrite(&currentNode, sizeof(Node), 1, _treeFile);

                //  Adds new node
                fseek(_treeFile, 0, SEEK_END);
                fwrite(&NewNode, sizeof(Node), 1, _treeFile);
                processSucess = true;

            } else {
                // Reset
                rewind(_treeFile);
                // Keep Looking
                fseek(_treeFile, (currentNode.left) * sizeof(Node), SEEK_CUR);
            }

        //  Check right
        } else if (NewNode.data.key > currentNode.data.key){

            //Check if the current node has a son
            if (currentNode.right == -1){
                // Insertion process
                // Update current node, now it points to the end of file
                NewNode.father = currentNode.line;
                currentNode.right = NewNode.line = _registersAtFile;
                fseek(_treeFile, -((long)sizeof(Node)), SEEK_CUR);
                fwrite(&currentNode, sizeof(Node), 1, _treeFile);

                //  Adds new node
                fseek(_treeFile, 0, SEEK_END);
                fwrite(&NewNode, sizeof(Node), 1, _treeFile);
                processSucess = true;


            } else {
                // Reset
                rewind(_treeFile);
                // Keep Looking
                fseek(_treeFile, (currentNode.right) * sizeof(Node), SEEK_CUR);
            }

        // Nodes are equal, that means a fail
        } else {
            printf("Error: Nodes are equal...\n");
            processFail = true;
            break;
        }
    }

    if(processFail == true){
        printf("Insertion Error: something went wrong...\n");
        return false;
    }

    rewind(_treeFile);
    printf(".");
    return true;
}

// Generates the file of the binary search tree + calls the balance function
bool fileGenerator(char* _dataFileName, int _registersQuant, int* _ExternalFileName){     
    FILE *dataFile, *treeFile;

    // Open/crete files and prevent errors
    if ((dataFile = fopen(_dataFileName, "rb")) == NULL){
        printf("Error: cannot open the data file\n");
        return false;
    }
    if ((treeFile = fopen(_ExternalFileName, "wb+")) == NULL){
        printf("Error: cannot create the External Search Tree file\n");
        return false;
    }

    // Start generating
    printf("Generating the External Search Tree File...\n");
    
    page_info pageInfo;                                                        
    int currentPage = 0;
    int registersAtFile = 0;
    int itensAtCurrentPage = ITENS_PER_PAGE;                                     
    int totalPages = _registersQuant / ITENS_PER_PAGE;                          

    while (currentPage <= totalPages){
        if(currentPage == totalPages)
            itensAtCurrentPage = _registersQuant - (currentPage * ITENS_PER_PAGE);
        
        fread(&pageInfo, sizeof(registry_t), itensAtCurrentPage, dataFile);

        for (int i = 0; i < itensAtCurrentPage; i++){                          
            if(insertion(treeFile, pageInfo[i], registersAtFile) == false){
                printf("Fatal Error! Insertion Failed");
                return false;                   
            }
            RedBlackTreeBalance(treeFile, registersAtFile);
            registersAtFile++;
            rewind(treeFile);                                           
        }
        printf("\n");
        currentPage++;
    }
    fclose(treeFile);
    fclose(dataFile);
    printf("The file was created successfully...\n");
    return true;
}

/// RedBlackTree exclusive Functions
// Left rotation
void rotateLeft(FILE* _treeFile, int _targetLine){
    Node X, Y, tempNode;
    int TemporaryPosition;
    // Set Nodes
    fseek(_treeFile, _targetLine * sizeof(Node), SEEK_SET);
    fread(&X, sizeof(Node), 1, _treeFile);
    fseek(_treeFile, X.right * sizeof(Node), SEEK_SET);
    fread(&Y, sizeof(Node), 1, _treeFile);
    // Y-left sub tree becomes X-right sub tree
    X.right = Y.left;
    // Updates Y's father (same as X)
    Y.father = X.father;
    // Changes X and Y position
    TemporaryPosition = Y.line;
    Y.line = X.line;
    X.line = TemporaryPosition;
    // Puts X above Y
    Y.left = X.line;
    // Fix the relation
    X.father = Y.line;
    // Updates X's right son
    if(X.right != -1){
        fseek(_treeFile, X.right * sizeof(Node), SEEK_SET);
        fread(&tempNode, sizeof(Node), 1, _treeFile);
        tempNode.father = X.line;
        fseek(_treeFile, -((long)sizeof(Node)), SEEK_CUR);
        fwrite(&tempNode   , sizeof(Node), 1, _treeFile);
    }
    // Updates X's left son 
    if(X.left != -1){
        fseek(_treeFile, X.left * sizeof(Node), SEEK_SET);
        fread(&tempNode, sizeof(Node), 1, _treeFile);
        tempNode.father = X.line;
        fseek(_treeFile, -((long)sizeof(Node)), SEEK_CUR);
        fwrite(&tempNode   , sizeof(Node), 1, _treeFile);
    }
    //Update file
    fseek(_treeFile, X.line * sizeof(Node), SEEK_SET);
    fwrite(&X, sizeof(Node), 1, _treeFile);
    fseek(_treeFile, Y.line * sizeof(Node), SEEK_SET);
    fwrite(&Y, sizeof(Node), 1, _treeFile);
}

// Right rotation
void rotateRight(FILE* _treeFile, int _targetLine){
    Node X, Y, tempNode;
    int TemporaryPosition;
    /// Set Nodes
    fseek(_treeFile, _targetLine * sizeof(Node), SEEK_SET);
    fread(&X, sizeof(Node), 1, _treeFile);
    fseek(_treeFile, X.left * sizeof(Node), SEEK_SET);
    fread(&Y, sizeof(Node), 1, _treeFile);
    /// Y-Right sub tree becomes X-Left sub tree
    X.left = Y.right;
    // Updates Y's father (same as X)
    Y.father = X.father; 
    // Changes X and Y position
    TemporaryPosition = Y.line;
    Y.line = X.line;
    X.line = TemporaryPosition;    
    // Puts X above Y
    Y.right = X.line;
    // Fix the relation
    X.father = Y.line;
    if(X.right != -1){
        fseek(_treeFile, X.right * sizeof(Node), SEEK_SET);
        fread(&tempNode, sizeof(Node), 1, _treeFile);
        tempNode.father = X.line;
        fseek(_treeFile, -((long)sizeof(Node)), SEEK_CUR);
        fwrite(&tempNode   , sizeof(Node), 1, _treeFile);
    }
    // Updates X's left son 
    if(X.left != -1){
        fseek(_treeFile, X.left * sizeof(Node), SEEK_SET);
        fread(&tempNode, sizeof(Node), 1, _treeFile);
        tempNode.father = X.line;
        fseek(_treeFile, -((long)sizeof(Node)), SEEK_CUR);
        fwrite(&tempNode   , sizeof(Node), 1, _treeFile);
    }
    //Update file
    fseek(_treeFile, X.line * sizeof(Node), SEEK_SET);
    fwrite(&X, sizeof(Node), 1, _treeFile);
    fseek(_treeFile, Y.line * sizeof(Node), SEEK_SET);
    fwrite(&Y, sizeof(Node), 1, _treeFile);
}

// Balancing the RedBlack Tree
void RedBlackTreeBalance(FILE* _treeFile, int _Xline){
    Node MainNode, MainFather, MainGrandpa, MainUncle;
    
    // Start Main Node 
    fseek(_treeFile, MainNode.line * sizeof(Node), SEEK_SET);
    fread(&MainNode, sizeof(Node), 1, _treeFile);
    if(MainNode.father != -1){
        fseek(_treeFile, MainNode.father * sizeof(Node), SEEK_SET);
        fread(&MainFather, sizeof(Node), 1, _treeFile);
    }
    // Set/update Nodes
    if(MainNode.father != -1){
        fseek(_treeFile, MainNode.father * sizeof(Node), SEEK_SET);
        fread(&MainFather, sizeof(Node), 1, _treeFile);
        if(MainFather.father != -1){
            fseek(_treeFile, MainFather.father * sizeof(Node), SEEK_SET);
            fread(&MainGrandpa, sizeof(Node), 1, _treeFile);   
        } else {
            MainGrandpa = NodeNullifier();
        }
    } else {
        MainFather = NodeNullifier();
        MainGrandpa = NodeNullifier();
    }

    // If: the MainNode is root or MainNode is Black or Father is Black, the balanncing ends
    while((MainNode.line != 0) && (MainNode.color != BLACK) && (MainFather.color == RED)){
        // MainFather is at left
        if(MainFather.line == MainGrandpa.left){
            //Set main uncle
            if(MainGrandpa.line != -2 && MainGrandpa.right != -1){
                fseek(_treeFile, MainGrandpa.right * sizeof(Node), SEEK_SET);
                fread(&MainUncle, sizeof(Node), 1, _treeFile);
            } else {
                MainUncle = NodeNullifier();
            }
            if(MainUncle.line != -2 && MainUncle.color == RED){
                // CASE 1.1: Uncle also is RED
                MainGrandpa.color   = RED;
                MainUncle.color     = BLACK;
                MainFather.color    = BLACK;
                NodeCorretion(_treeFile, MainGrandpa);
                NodeCorretion(_treeFile, MainUncle);
                NodeCorretion(_treeFile, MainFather);

                // Grandpa becomes the Main Node
                MainNode.line = MainGrandpa.line;
                NodeRead(_treeFile, &MainNode);
            } else {
                // CASE 1.2: Main Node is not at the same side as he's father
                if(MainNode.line == MainFather.right){
                    // Saves the position before the rotation, because the sequence: Grampa -> Father -> Son will change to Grandpa -> Son -> Father
                    int temp = MainFather.line;
                    MainFather.line = MainNode.line;
                    MainNode.line = temp;
                    rotateLeft(_treeFile, temp);
                    // Updates the position, bringing the sequence back to Grandpa -> Son -> Father                    
                    NodeRead(_treeFile, &MainFather);
                    NodeRead(_treeFile, &MainNode);
                    // Fix Relations
                    MainFather.father = MainGrandpa.line;
                    // Update file
                    NodeCorretion(_treeFile, MainFather);
                }
                //  CASE 1.3: MainNode is at the correct side
                MainNode.father = MainGrandpa.line;
                NodeCorretion(_treeFile, MainNode);
                
                rotateRight(_treeFile, MainGrandpa.line);
                NodeRead(_treeFile, &MainGrandpa);  // Grandpa Became Brother
                NodeRead(_treeFile, &MainFather);   // Stays father, but in grandpa's position
                
                bool temp = MainGrandpa.color;
                MainGrandpa.color = MainFather.color;
                MainFather.color = temp;
                NodeCorretion(_treeFile, MainFather);
                NodeCorretion(_treeFile, MainGrandpa);
                
                MainNode.line = MainGrandpa.line;
                NodeRead(_treeFile, &MainNode);
            }
        // MainFather is at right
        } else {
            // Set MainUncle
            if(MainGrandpa.line != -2 && MainGrandpa.left != -1){
                fseek(_treeFile, MainGrandpa.left * sizeof(Node), SEEK_SET);
                fread(&MainUncle, sizeof(Node), 1, _treeFile);
            } else {
                MainUncle = NodeNullifier();
            }

            if(MainUncle.line != -2 && MainUncle.color == RED){
                // CASE 2.1: Uncle also is RED
                MainGrandpa.color   = RED;
                MainUncle.color     = BLACK;
                MainFather.color    = BLACK;
                NodeCorretion(_treeFile, MainGrandpa);
                NodeCorretion(_treeFile, MainUncle);
                NodeCorretion(_treeFile, MainFather);

                // Grandpa becomes the Main Node
                MainNode.line = MainGrandpa.line;
                NodeRead(_treeFile, &MainNode);

            } else {
                // CASE 2.2: Main Node is not at the same side as he's father
                if(MainNode.line == MainFather.left){                    
                    // Saves the position before the rotation, because the sequence: Grampa -> Father -> Son will change to Grandpa -> Son -> Father
                    int temp = MainFather.line;
                    MainFather.line = MainNode.line;
                    MainNode.line = temp;
                    rotateRight(_treeFile, temp);
                    // Updates the position, bringing the sequence back to Grandpa -> Son -> Father                    
                    NodeRead(_treeFile, &MainFather);
                    NodeRead(_treeFile, &MainNode);
                    // Fix Relations
                    MainFather.father = MainGrandpa.line;
                    // Update file
                    NodeCorretion(_treeFile, MainFather);
                }
                // CASE 2.3: Main Node is at the correct side
                
                MainNode.father = MainGrandpa.line;
                NodeCorretion(_treeFile, MainNode);

                rotateLeft(_treeFile, MainGrandpa.line);
                NodeRead(_treeFile, &MainGrandpa);  // Grandpa Became Brother
                NodeRead(_treeFile, &MainFather);   // Stays father, but in grandpa's position

                bool temp = MainGrandpa.color;
                MainGrandpa.color = MainFather.color;
                MainFather.color = temp;
                NodeCorretion(_treeFile, MainFather);
                NodeCorretion(_treeFile, MainGrandpa);

                MainNode.line = MainGrandpa.line;
                NodeRead(_treeFile, &MainNode);
            }
        }
        // Set/update Nodes
        if(MainNode.father != -1){
            fseek(_treeFile, MainNode.father * sizeof(Node), SEEK_SET);
            fread(&MainFather, sizeof(Node), 1, _treeFile);
            if(MainFather.father != -1){
                fseek(_treeFile, MainFather.father * sizeof(Node), SEEK_SET);
                fread(&MainGrandpa, sizeof(Node), 1, _treeFile);   
            } else {
                MainGrandpa = NodeNullifier();
            }
        } else {
            MainFather = NodeNullifier();
            MainGrandpa = NodeNullifier();
        }
    }
    fseek(_treeFile, 0, SEEK_SET);
    fread(&MainNode, sizeof(Node), 1, _treeFile);
    MainNode.color = BLACK;
    NodeCorretion(_treeFile, MainNode);
}

/// Main function
bool ExternalSearchTree(search_result* _resultInfo ,char* _dataFileName, int _registersQuant, int _targetKey){ 
    char* ExternalFileName = "ExternalTree";
    // Delete file if it exists
    if(fileExists(ExternalFileName))
        remove(ExternalFileName);

    if(!fileGenerator(_dataFileName, _registersQuant, ExternalFileName)){
        printf("External Search Tree failed. Creation...\n Ending execution...\n");     
        return false;
    }
    if(ExternalFinder(_resultInfo ,_targetKey, ExternalFileName)){
        printf("External Search Tree failed. Search...\n Ending execution...\n");     
        return false;
    }
    return true;
}
