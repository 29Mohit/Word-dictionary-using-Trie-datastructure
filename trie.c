#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "trie.h"

#define ALPHABET_SIZE (26)
#define CHAR_TO_INDEX(c) ((int)c - (int)'a')


NODEPTR getNode(){
    NODEPTR newNode = (NODEPTR)malloc(sizeof(struct TrieNode));

    if (newNode!=NULL) {
        newNode->isLeaf = 0;
        int i;
        for (i = 0; i < ALPHABET_SIZE; i++)
            newNode->children[i] = NULL;
    }
    else
            printf("\n Could not create the Node\n");
    return newNode;
}

void insert(NODEPTR *root, const char *word, char *meaning){
           // Inserts a given word to dictionary.
    NODEPTR temp = *root;
    int i;

    for (i = 0; i < strlen(word); i++){
        int index = CHAR_TO_INDEX(word[i]);         //to get the relative position
        if (temp->children[index]==NULL)            // if the corresponding child doesnt exist
            temp->children[index] = getNode();    // then we create one
        temp = temp->children[index];
    }
    temp->isLeaf = 1;
    temp->meaning = meaning;
}

char* search( NODEPTR root, const char *word){
    NODEPTR temp = root;
    int i;
    for (i = 0; i < strlen(word); i++)
    {
        int index = CHAR_TO_INDEX(word[i]);

        if (temp->children[index] == NULL)      //if we have reached the end and nothing is matched
            return 0;
        temp = temp->children[index];
    }
    int ans = (temp != NULL && temp->isLeaf);
    if (ans){
        return temp->meaning;
    }
    else{
        return NULL;
    }    //if the string exits then we must reach leaf
}


NODEPTR traverseTill(NODEPTR root, char *word){
    NODEPTR temp = root;

    int i;
    for (i = 0; i < strlen(word); i++) {
        int index = CHAR_TO_INDEX(word[i]);
        if (temp->children[index] == NULL) {
            printf("\nNo Words present with the given Prefix: %s \n",word);
            return NULL;
        }
        temp = temp->children[index];
    }
    return temp;
}


char* ValidWord(char* input){

    int i;
    for(i = 0; i<strlen(input); i++){
        if(!isalpha(input[i])){
            printf("\n\n Not a valid Word !!!\n\n");
            return NULL;
        }
    }
    return strlwr(input);
}


void Display_contents(NODEPTR root, char *A, int Index) {
    int flag = 1;
    int i;
    for(i = 0; i < ALPHABET_SIZE; i++) {

        if(root->children[i] != NULL) {
                A[Index] = i + 'a';
                Display_contents(root->children[i], A, Index + 1);
        }
        if(root->isLeaf && flag && root->meaning){

                A[Index] = '\0';
                if(strlen(A) > 0) {

                    printf("---> %s \n", A);
                    flag=0;
                }
        }
    }
}

//void print_trie(NODEPTR* root) {
//    // Prints the nodes of the trie
//    if (!root)
//        return;
//    NODEPTR temp = root;
//    printf("%c -> ", temp->word,temp->meaning);
//    for (int i=0; i< ALPHABET_SIZE ; i++) {
//        print_trie(temp->children[i]);
//    }
//}



int is_leaf_node(NODEPTR root, char* word) {
    // Checks if the prefix match of word and root is a leaf node
    NODEPTR temp = root;
    for (int i = 0; word[i]; i++) {
        int position = (int) word[i] - 'a';
        if (temp->children[position]) {
            temp = temp->children[position];
        }
    }
    return temp->isLeaf;
}

char* find_longest_prefix(NODEPTR root, char* word) {
    if (!word || word[0] == '\0')                           //base case
        return NULL;

    int len = strlen(word);

    //search for the word itself as prefix
    //do backtracking from last position to a point of divergence

    char* longest_prefix = (char*) calloc (len + 1, sizeof(char));
    for (int i=0; word[i] != '\0'; i++)
        longest_prefix[i] = word[i];
    longest_prefix[len] = '\0';

 //No branching means, we are matching the same word

    int branch  = check_divergence(root, longest_prefix) - 1;
    if (branch >= 0) {
        // There is branching,so update the position
        // to the longest match and update the longest prefix
        // by the branch index length
        longest_prefix[branch] = '\0';
        longest_prefix = (char*) realloc (longest_prefix, (branch + 1) * sizeof(char));
    }

    return longest_prefix;
}

int check_divergence(NODEPTR root, char* word) {
     //if branching occurs at the last character return the largest position
    NODEPTR temp = root;
    int len = strlen(word);
    if (len == 0)
        return 0;
    int last_index = 0;
    for (int i = 0; i < len; i++) {
        int position = word[i] - 'a';
        if (temp->children[position]) {
             // If a child exists, then we check if any other child is present
            // so that branching occurs
            for (int j = 0; j < ALPHABET_SIZE ; j++) {
                if (j != position && temp->children[j]) {
                    // if another child found, this is a branch and we update the branch position
                    last_index = i + 1;
                    break;
                }
            }
            temp = temp->children[position];
        }
    }
    return last_index;
}
void free_trienode(NODEPTR node) {
    for(int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != NULL)
            free_trienode(node->children[i]);
        else
            continue;

    }
    free(node);
}

NODEPTR* delete_trie(NODEPTR* root, char* word) {
    if (!(*root))
        return NULL;

    if (!word || word[0] == '\0')
        return root;

    if (!is_leaf_node(*root, word)) {
        return root;
    }

    NODEPTR temp = *root;
    // longest prefix string that is not the current word
    char* longest_prefix = find_longest_prefix(*root, word);
    //printf("Longest Prefix = %s\n", longest_prefix);
    if (longest_prefix[0] == '\0') {
        free(longest_prefix);
        return root;
    }

    int i;
    for (i = 0; longest_prefix[i] != '\0'; i++) {
        int position = (int) longest_prefix[i] - 'a';
        if (temp->children[position] != NULL) {
            // Keep moving to the deepest node in the common prefix
            temp = temp->children[position];
        }
        else {
            free(longest_prefix);
            return root;
        }
    }
    //after reaching the deepest common node, we delete that sequence corresponding to the word
    int len = strlen(word);
    for (i = 0 ; i < len; i++) {
        int position = (int) word[i] - 'a';
        if (temp->children[position]) {
            // Delete the remaining sequence
            NODEPTR* remaining_node = temp->children[position];
            temp->children[position] = NULL;
            free_trienode(remaining_node);
        }
    }
    free(longest_prefix);
    return root;
}

char* searchinFile(FILE *fp, char* WordtoSearch){
    char *tobeReturned = (char*)malloc(sizeof(char)*100); 
    char word[20];
    char ch;
    int i = 0;
    rewind(fp);
    while (fscanf(fp, "%s", word) != EOF){

        //printf("%s ", word); //should print words
        if (strcmp(WordtoSearch, word) == 0){
            while((ch=fgetc(fp))!=EOF && ch != '\n'){
                tobeReturned[i++] = ch;
            }
            tobeReturned[i++] = '\n';
            tobeReturned[i] = '\0';
            return tobeReturned;
        }
        fscanf(fp, "%*[^\n]\n");
    }
    return "";
}

void sync (FILE *fp, NODEPTR *root){
    char word[20];
    int i = 0;
    while (fscanf(fp, "%s", word) == 1){
        // printf("%s %s", word, searchinFile(fp, word));
        insert(root, word, searchinFile(fp, word));
      
        i++;
         
    }
    rewind(fp);
}

void delete_from_file(char *wordtobeDeleted){
    //copy everything into replica.txt
    //open replica in "r" modde and check for word
    //if word = = word to be deleted then dont write into dictionary.txt
    //otherwise write

    FILE *fp1 = fopen("replica.txt", "w");
    FILE *fp2 = fopen("dictionary.txt", "r");
    

    char ch = fgetc(fp2);
    while (ch != EOF)
    {
        // Write to destination file 
        fputc(ch, fp1);

        // Read next character from source file 
        ch = fgetc(fp2);
    }
    rewind(fp2);
    int lines = 1;
    while ((ch = fgetc(fp2)) != EOF)
    {
      if (ch == '\n')
        lines++;
    }
  


    fclose(fp1);
    fclose(fp2);
    
    fp1 = fopen("replica.txt", "r");
    fp2 = fopen("dictionary.txt", "w");


    int i = 0;
    while (i < lines){
        char *line = (char*)malloc(200);
        fgets(line, 200, fp1);
        line[strlen(line)] = '\0';
        //find 1st word of line
        int j = 0;
        char *word = (char*)malloc(20);

        while (line[j] != ' ' && j < strlen(line)){
            word[j] = line[j];
            j++;
        }
        word[j] = '\0';
        //printf("\n");
        
        if (strcmp(wordtobeDeleted, word) != 0){
            
            fprintf(fp2, "%s", line);
        }
        free(word);
        free(line);

        i++;
    }

    fclose(fp1);
    fp1 = fopen("replica.txt", "w");
    fclose(fp1);
    fclose(fp2);
    
    
    return;    

}


void help(){
    printf(YELLOW "\n-a to add words\n");
    printf(YELLOW "\n-p to print all the words in alphabetical order\n");
    printf(YELLOW "\n-b to print words with given prefix\n");
    printf(YELLOW "\n-d to delete words\n");
    printf(YELLOW "\n-s to search words\n");
    printf("\n");
    printf(RESET);
    return ;
}