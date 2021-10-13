#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define ALPHABET_SIZE (26)
#define GREEN "\e[0;32m"
#define BLUE "\e[0;34m"
#define RESET "\x1b[0m"
#define YELLOW "\e[0;33m"

struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int isLeaf;
    char *meaning;
    char word;
};

typedef struct TrieNode* NODEPTR;

void insert(NODEPTR *root, const char *word, char *meaning);
char* search( NODEPTR root, const char *word);
char* ValidWord(char* input);
void Display_contents(NODEPTR root, char *A, int Index);
int is_leaf_node(NODEPTR root, char* word);
char* find_longest_prefix(NODEPTR root, char* word);
int check_divergence(NODEPTR root, char* word);
void free_trienode(NODEPTR node);
void delete_from_file(char *wordtobeDeleted);
