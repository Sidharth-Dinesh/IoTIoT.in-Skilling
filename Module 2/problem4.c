/*
 * @file        problem4.c
 * @brief       This is a program to fix a previously existing memory leak
 * @author      Sidharth Dinesh
 * @bugs        No bugs found.
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
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/* --- Project Includes --- */


/*
 *#####################################################################
 *  Process block
 *  -------------
 *  Solve all your problems here 
 *#####################################################################
 */

/**
 * @brief      Search the given string in the file
 *
 * @param      fname  The file name
 * @param      str    The string to search
 *
 * @return     Returns -1 on Error and 0 on not found and 1 on SUCCESS
 */
int searchInFile(char *fname,char *str) 
{
    FILE *fp;
    int line_num = 1;
    int find_result = 0;
    char temp[512];
    
    fp = fopen(fname, "r");         /*fp object initialised in the start*/
    
    if(fp == NULL) {                /*Ensuring file is opened at the start*/
        printf("Error: Opening the file.\n");
        
        fclose(fp);             /*Inside if statement, before it jumps out of the function, file should be ensured to be closed.*/
        free(fname);            //Freeing the pointer
        free(str);              //Freeing the pointer
        fname=NULL;
        str=NULL;
        
        return(-1);
    }

    while(fgets(temp, 512, fp) != NULL) 
    {
        if((strstr(temp, str)) != NULL) 
        {
            printf("A match found on line: %d\n", line_num);
            printf("\n%s\n", temp);
            
            find_result++;
            
            fclose(fp);             /*Inside if statement, before it jumps out of while loop and function, file should be ensured to be closed.*/
            free(fname);            //Freeing the pointer
            free(str);              //Freeing the pointer
            fname=NULL;
            str=NULL;
            
            return (1);
        }
        line_num++;
    }

    if(find_result == 0) {
        printf("\nSorry, couldn't find a match.\n");
    }
    
    //Close the file if still open.
    if(fp) {
        fclose(fp);
    }
    
    free(fname);        //Freeing the pointer
    free(str);          //Freeing the pointer
    
    fname=NULL;
    str=NULL;
    
    return(0);
}

int main (void)
{       
        /*Make sure you comment every line */
        searchInFile("db.txt", "iotiot");
        searchInFile("db.txt", "iotiot");
        return 0;
}
