#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"

int main(int argc, char** argv){
    NODEPTR root = getNode();
    char string[20];
    char meaning[200] = "";
    FILE *fp;
    char *getmeaning;
    fp = fopen("dictionary.txt", "a+");
    if (!fp){
        printf("File could not be opened.");
        return ;
    }

    sync(fp, &root);

    if (strcmp(argv[1], "-help") == 0){
        help();
    }

    else if (strcmp(argv[1], "-a") == 0){
        //add
        //word = argv[2]
        //meaning = argc[3]....
        //how to get meaning
        int i = 3;
        while (i < argc){
            strcat(meaning, argv[i]);
            strcat(meaning, " ");
            i++;
        }
        int size = strlen(meaning);
        meaning[size] = '\0';
        fprintf(fp, "%s ", argv[2]);
        fprintf(fp, "%s\n", meaning);

        if(ValidWord(argv[2])!=NULL)
            insert(&root, strlwr(argv[2]),meaning);
             //printf(YELLOW "Word added");
             //printf(RESET);
    }
    else if (strcmp(argv[1], "-p") == 0){
        //print
        printf(BLUE "\nContents in the Dictionary are:\n");
        Display_contents(root,string,0);
        printf(RESET);
    }
    else if (strcmp(argv[1], "-s") == 0){
       //search

        if(ValidWord(argv[2])!=NULL){
            getmeaning = search(root, argv[2]);

            if(getmeaning != NULL){
                printf(BLUE "\n%s is present in the dictionary.\n",argv[2]);
                //printf("The meaning of %s is %s\n", wordToSearch, getmeaning);
                printf(RESET);
            }
            else {
                printf("\n%s is not present in the dictionary.\n",argv[2]);
               
            }
        
            getmeaning = searchinFile(fp, argv[2]);
            if (strlen(getmeaning) != 0){
                printf(GREEN "Meaning : %s\n",getmeaning);
                printf(RESET);
            }
            else{
                printf("Word not present.\n");
            }
        rewind (fp);
        }
    }
    else if (strcmp(argv[1], "-b") == 0){
        
        if(ValidWord(argv[2])!=NULL){
            strcpy(string, argv[2]);
            printf(GREEN "\nWords with given PREFIX : %s \n",argv[2]);
            
            NODEPTR prefixRoot= traverseTill(root, argv[2]);
            if(prefixRoot!=NULL){
                    Display_contents (prefixRoot,string,strlen(argv[2]));
                }
            printf(RESET);
        }
    }
   else if (strcmp(argv[1], "-d") == 0){
       delete_from_file(argv[2]);
        
        delete_trie(&root, argv[2]);
        // free_trienode(argv[2]);
        printf("deleted if existed\n");
   }

}
 