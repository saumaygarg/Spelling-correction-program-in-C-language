#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define SEED 123     // seed for murmurhash2
#define SIZ_STR 19   // max length of word in library/txt file
#define SIZ_TABLE 370103 // size of hash table = number of words in library

//trie stucture defination
typedef struct binary_node
{
  struct binary_node* left;
  struct binary_node* right;
  char b_word[SIZ_STR];
}b_node;

// function for new trie node
b_node* getnode()
{
  b_node* return_node = NULL;
  return_node = (b_node*)malloc(sizeof(b_node));


  if(return_node)
    {
      return_node->left = NULL;
      return_node->right = NULL;
    }

  return return_node;
}

// function to insert a string(str) in tree
b_node* b_insert(b_node* root, char* str)
{
  int cond;

  if(root == NULL)
    {
      root = getnode();
      strcpy(root->b_word, str);
     }

  else if((cond = strcmp(str, root->b_word)) == 0)
    return root;

  else if(cond < 0)
    root->left = b_insert(root->left, str);

  else
    root->right = b_insert(root->right, str);

  return root;

}

// function to search a key in trie tree
int search(b_node* root, char* key)
{
  int cond;

  if (root == NULL)
    return 0;

  if((cond = strcmp(key, root->b_word)) == 0)
    return 1;

  else if(cond < 0)
    search(root->left, key);

  else
    search(root->right, key);
}

// murmurhash2 function simple c implementation
unsigned int MurmurHash2 ( const void * key, unsigned int seed )
{
  int len = strlen(key);
  unsigned int max_hash_size=SIZ_TABLE;
  const unsigned int m = 0x5bd1e995;
  const int r = 24;

  unsigned int h = seed ^ len;

  const unsigned char * data = (const unsigned char *)key;

  while(len >= 4)
    {
      unsigned int k = *(unsigned int *)data;

      k *= m;
      k ^= k >> r;
      k *= m;

      h *= m;
      h ^= k;

      data += 4;
      len -= 4;
    }

  switch(len)
    {
    case 3: h ^= data[2] << 16;
    case 2: h ^= data[1] << 8;
    case 1: h ^= data[0];
      h *= m;
    };

  h ^= h >> 13;
  h *= m;
  h ^= h >> 15;

  return h % max_hash_size;
}

//function to alter a string by removing a character at index
void function_remove (char* ret_str, int index)
{
  int len = strlen(ret_str);
  int i;
  int count=0;
  for (i=0;i<len;i++){
    if (count>=index){
      ret_str[i]=ret_str[i+1];
    }
    count++;
  }
  return;
}

//function to alter a string by replacing a character by (char value) at index
void function_replace (char* ret_str, int index, char value)
{
  ret_str[index]=value;
  return;
}

//function to alter a string by inserting a character (char value) at index and shifting subsiquent characters forward
void function_putchar (char* ret_str, int index, char value )
{
  int len = strlen(ret_str);

  if (index == len)
    {
      ret_str[index] = value;
      return;
    }

  else
    {
      int i;
      for(i=len-1;i>=index;i--)
        {
	  ret_str[i+1] = ret_str[i];
        }
      ret_str[i+1] = value;
      ret_str[len+1]='\0';
    }
  return;
}

void function_interchange(char* ret_str, int index)
{
  char temporary;
  
  temporary = ret_str[index];
  ret_str[index] = ret_str[index+1];
  ret_str[index+1] = temporary;
  return ;
}
	  
// linked list structure
struct list
{
  struct list* next;
  char string[SIZ_STR];
}*head = NULL, *temp = NULL;

// returns new linked list node
struct list* list_newnode()
{
  struct list* ret_ptr = (struct list*)malloc(sizeof(struct list));
  ret_ptr->next = NULL;

  return ret_ptr;
}

// adds the string check_str to the list if it is found in hash_table
void list_append (char* check_str)
{
  if((strlen(check_str)!= 1) && (strlen(check_str)!= 0))
    {
      if (head == NULL)
	{
	  head = list_newnode();
	  temp = head;
	  strcpy(temp->string, check_str);
	}
      else
	{
	  struct list* temp_2 = NULL;
	  temp = head;
	  // check for repetition of words in list
	  while(temp != NULL)
	    {
	      if(strcmp(temp->string, check_str) == 0)
		return ;
	      else
		{
		  temp_2 = temp;
		  temp = temp->next;
		}
	    }
	  temp = temp_2;
	  temp->next = list_newnode();
	  temp = temp->next;
	  strcpy(temp->string, check_str);
	}
      return ;
    }

}

int main()
{

  FILE* file_ptr;
  char buf[SIZ_STR];
  file_ptr = fopen("rand_file_big.txt", "r");

  b_node* hash_table[SIZ_TABLE];
  memset(hash_table, 0, SIZ_TABLE*sizeof(b_node*));
  unsigned int i;

  char str[SIZ_STR];
  char check_str[SIZ_STR];
  unsigned int h;

  int vary_index;
  int vary_alpha;

  char response_exit='y';

  //reading from text file with list of words and storing in hash table and trie
  while( fgets(buf, SIZ_STR+1, file_ptr) != NULL )
  {
    buf[(strlen(buf))-1] = '\0';
    i = MurmurHash2(buf, SEED);
    if (hash_table[i] == NULL)
      {
        hash_table[i] = getnode();
        b_insert( hash_table[i], buf);
      }

    else
      b_insert( hash_table[i], buf);
  }

  printf(ANSI_COLOR_YELLOW "_________________________________________________________\n\n");
  printf("\t\t   THE WORD CORRECTOR\n");
  printf("_________________________________________________________\n\n");

 printf("This is word correction program developed in C language.\nIt would help you identify the correct spelling for \n \t\t   a misspelled word.\n\n" ANSI_COLOR_RESET);

  while(response_exit=='y' || response_exit=='Y')
  {
   printf(ANSI_COLOR_CYAN "Enter the word you wish to check:  "ANSI_COLOR_RESET);
    scanf("%s", str);
    int limit = strlen(str);

    int k;
    for( k=0; str[k]; k++)
      str[k] = tolower(str[k]);

    // altering the user input string - using putchar()

    for ( vary_index=0; vary_index <= limit; vary_index++)
      { for( vary_alpha=97; vary_alpha<=122; vary_alpha++)
	  {
	    strcpy(check_str, str);
	    char c = vary_alpha;
	    function_putchar(check_str, vary_index, c);
	    h = MurmurHash2(check_str, SEED);
	    if( search(hash_table[h], check_str) == 1)
	      list_append(check_str);
	  }
      }

    // altering the user input string - using function_replace()

    for ( vary_index=0; vary_index <= limit-1; vary_index++)
      { for( vary_alpha=97; vary_alpha<=122; vary_alpha++)
	  {
	    strcpy(check_str, str);
	    char c = vary_alpha;
	    function_replace(check_str, vary_index, c);
	    h = MurmurHash2(check_str, SEED);
	    if( search(hash_table[h], check_str) == 1)
	      list_append(check_str);
	  }
      }

    // altering the user input string - using function_remove()

    for ( vary_index=0; vary_index <= limit-1; vary_index++)
      {
	strcpy(check_str, str);
	function_remove(check_str, vary_index);
	h = MurmurHash2(check_str, SEED);
	if( search(hash_table[h], check_str) == 1)
	  list_append(check_str);
      }

     for ( vary_index=0; vary_index <= limit-1; vary_index++)
      {
	strcpy(check_str, str);
	function_interchange(check_str, vary_index);
	h = MurmurHash2(check_str, SEED);
	if( search(hash_table[h], check_str) == 1)
	  list_append(check_str);
      }

    //printing the liSt of words found in hash table - trie data structue

    // if list is empty
    if(head == NULL)
      {
	printf(ANSI_COLOR_RED "\nOops! No similar words were found\n" ANSI_COLOR_RESET);
	goto end;
      }

    // if word entered by user is not misspelt
    else 
      {
	int counter = 0;
	temp = head;
	while(temp != NULL)
	  {
	    if(strcmp(temp->string, str) == 0)
	      {
		++counter;
		break;
	      }
	    temp = temp->next;
	  }
	if(counter != 0)
	  {
	    printf(ANSI_COLOR_CYAN "\nThe word you entered is correct, do you still want to see \nother similar words? (y/n):" ANSI_COLOR_RESET);
	    char garbage_1 = getchar();
	    char resp = getchar();

	    if(resp=='y' || resp=='Y')
	      goto print_list;

	    else
	      goto end;
	  }
	else
	  goto wrong_word_case;
      }

    //printing the complete list
  wrong_word_case:
    printf(ANSI_COLOR_CYAN "That's not a word, Did you mean?\n" ANSI_COLOR_RESET);
  print_list:
    temp = head;
    while (temp != NULL)
      {
	printf(ANSI_COLOR_RESET "->" ANSI_COLOR_CYAN" %s \n", temp->string);
	temp = temp->next;
      }
      

  end:
    printf(ANSI_COLOR_CYAN "\nDo you want to check another word? (y/n):" ANSI_COLOR_RESET);
    char garbage_2 = getchar();
    response_exit = getchar();
    temp = NULL;head = NULL;
  }

  printf(ANSI_COLOR_YELLOW "\n\n\t\t\tTHANK YOU!\n\n" ANSI_COLOR_RESET);
  return 0;

}
