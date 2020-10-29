/** @file problem3.c
 *  @brief
 * 
 *  Write code to parse a CSV file named 'data.csv' and store the values into a
    key-value data structure made using structures and implement a search function
    for the data structure which will search through the data structure when given
    timestamp value.
 *  
 *  @author KEERTHAN PORANDLA
 *  @bug I didn't find a bug from my side
 */

/*
 *#####################################################################
 *  Initialization block
 *  ---------------------
 *  This block contains initialization code for this particular file.
 *  It typically contains Includes, constants or global variables used
 *  throughout the file.
 *#####################################################################
 */

/* --- Standard Includes --- */
#include <stdio.h> //standard input output library
#include<stdlib.h> // header defines four variable types, several macros, and various functions for performing general functions.
#include<string.h> // header contains library function of string manipulation functions
#include <errno.h> //include both the errno variable and specific error codes like EFAULT
/* --- Project Includes --- */


/*
 *#####################################################################
 *  Process block
 *  -------------
 *  Solve all your problems here 
 *#####################################################################
 */
 
 /************************** 
 * Error-handling functions
 **************************/
/* $begin errorfuns */
/* $begin unixerror */
void unix_error(char *msg) /* unix-style error */
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}
/* $end unixerror */

void posix_error(int code, char *msg) /* posix-style error */
{
    fprintf(stderr, "%s: %s\n", msg, strerror(code));
    exit(0);
}

void app_error(char *msg) /* application error */
{
    fprintf(stderr, "%s\n", msg);
    exit(0);
}
/* $end errorfuns */

/* $begin standard file i/p o/p errors */
void Fclose(FILE *fp) 
{
    if (fclose(fp) != 0)
    unix_error((char*)"Fclose error");
}

FILE *Fopen(const char *filename, const char *mode) 
{
    FILE *fp;

    if ((fp = fopen(filename, mode)) == NULL)
    unix_error((char*)"Fopen error");

    return fp;
}

char *Fgets(char *ptr, int n, FILE *stream) 
{
    char *rptr;

    if (((rptr = fgets(ptr, n, stream)) == NULL) && ferror(stream))
    app_error((char*)"Fgets error");

    return rptr;
}

/* $end standard file i/p o/p errors */

/* $begin Dynamic Storage errors */
void* Calloc(size_t nmemb, size_t size) 
{
    void* p;

    if ((p = calloc(nmemb, size)) == NULL)
    unix_error((char*)"Calloc error");
    return p;
}

void Free(void *ptr) 
{
    free(ptr);
}
/* $end Dynamic Storage errors */

/** 
 *  @brief 
 * "Search" function will search(by using the linked list formed and searching by strstr library function and searches by traversing through the linked lsit received as argument) the timestamp given as an input from the file data.csv and displays the coresponding values needed like sample value, MLII value and V5 value. and its returns no value.
 * "parse" function will store the data.csv file content into a data Structure called in a single link list(by FILE handling concepts like fopen and storing into the link list by traversing through the list) and returns the inital (starting, i.e head) node address to the head pointer which is declared in main function.
 *  "Search" function have no return value so, void and it accepts an argument which is of structure pointer type.
 *  "parse" function has a return value which of structure pointer type and in which structure is globally declared and it accepts an argument structure pointer type.
 *
 * structure has two members one is sample which is char type and it is an array. another member is pointer to the structure itself which self referential structure.
 */


struct data             // declaring structure with tagname as data and members with sample and structure pointer of its type with variable name as next (self referential pointer)
{
    char sample[80];
    struct data *next;  
};

struct data *parse(struct data *ptr)                                    // collecting the address of initial link list into a pointer which is structure type.
{                                                                       // declarations and assigning the values
    struct data *temp=NULL,*nu=NULL; 
    char temp1[80]={0};
    
    FILE *fp;                                                           // declaring a FILE pointer 
    
    fp=Fopen("data.csv","r");                                           // Calling Fopen function in read mode  
    
    while(Fgets(temp1,80,fp)!=NULL)                                     // Calling Fgets function 
    {
        nu=(struct data*)Calloc(1,sizeof(struct data));                               // allocating memory for new node with sizeof structure using Calloc function 
        strcpy(nu->sample,temp1);                                       //by using strcpy string copy, copying the content of temp1 to new node. 
        
        if(ptr==NULL) // checking condition if ptr is NULL the list is empty so, we allocate a new node to ptr initial one.
        {
            ptr=nu;
        }
        else // if above case fails... traverse along the list and if NULL found we allocate the new node to the temp->next value.  (note: list is link list here and in other comments also... and link list formed is single link list)
        {
            temp=ptr;
            while(temp->next)                                               // traversing upto last node but not upto NULL
            {
                temp=temp->next;
            }
            temp->next=nu;                                               // allocating new node to temp->next value
        }
    }
    
    Fclose(fp);
    Free(temp);
    Free(nu);
    return ptr;                                                                 // returning the initial value of the list.
}

void Search(struct data *ptr)                                                   // collecting the address of initial link list into a pointer which is structure type.
{                                                                                // declarations and assigning the values
    struct data *temp=ptr;                                                      // declaring other variable of temp which of structure pointer type not to make disturb the ptr initial value.
    char str[40]={0},*token=NULL,temp1[80]={0};
    static unsigned int cnt=0;                                                  // static count which remains until the program ends
    printf("What timestamp you want to search for: ");                          // prints the message
    Fgets(str,40,stdin);                                                        // taking the input from user
    while(temp&&strstr(temp->sample,str)==NULL)                                // if above case is false then here, traverse untill the temp is a valid node (upto last node) and checking until getting an string we are searching for, if found loop terminates otherwise continues if anyone case is false(ie is string found or temp reached to NULL value(last node next)).
    {
        temp=temp->next;
    }
    if(temp==NULL)                                                              // if both above cases fail then this is true otherwise it is false. if temp reaches last node next(NULL) and still not found the string this case valid otherwise false
    {
        return;
    }
    else                                                                                // if above cases are false then this excutes;
    {
        strncpy(temp1,temp->sample,(strlen(temp->sample)-(strlen(str))+1));                 // copying the string upto our need 
        token=strtok(temp1,",");                                                            // using string tokeniser dividing the string and printing the values untill token reaches NULL
        while(token!=NULL)                                                                   // when token reaches NULL loop terminates
        {
            if(cnt==0)
                printf("sample:");                                                          // printing the message according to count    
            if(cnt==1)
                printf("\t| MLII:");                                                        // printing the message according to count
            if(cnt==2)
                printf(" | V5:");                                                           // printing the message according to count
            printf("%s",token);                                                             // printing the token value as for every string
            token=strtok(NULL,",");                                                         // making it to NULL after printing receiving a string after a token found otherwise the same recieved at first will be printed in loop
            cnt++;                                                                          // incrementing count
        }
        printf(" |");
        printf("\n");
    }
    Free(temp);
}

/** 
 *  @brief 
 * main function takes no arguments and return int value to its calling function (startup function). mostly the return value will be like 0, which is like termination or end of the main function.
 * it has an declaration for pointer called head which of structure type and initialised with NULL(making sure to point to NULL address rather than poiniting to undefined location)
 * and then calling parse function with input argument as head which is NULL intially when return it has some defined value of address which is the starting address of linked lsit formed and collecting its return value in head pointer 
 * and then calling Search function with input argument as head and based on this Search function will search the inout string in the linked list already form in parse function and prints the value correspondingly.
 */

int main(void)
{
    struct data *head=NULL;                                              // declaring a head pointer sr
    head=parse(head);                                                   // calling parse function with an input argument as head pointer which is declared with initial value NULL
    Search(head);                                                      //calling the Search function with an input argument as head pointer which is declared with a value returned by parse funtion and which is collected in head
    Free(head);
    return 0;                                                           // return 0 for main calling function which is startup function
}
