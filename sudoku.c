
#include <pthread.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <string.h>
#include <stdbool.h>

void *testRows(void *arg);
void *testColumns(void *arg);
void *testCell(void *arg);
bool results[11];


//Incorrect value at sudoku[3][4]
int sudoku[9][9] = { 
{1,2,5,3,7,8,9,4,6},
{3,7,8,9,6,4,2,1,5}, 
{4,9,6,1,2,5,8,3,7}, 
{2,6,9,4,6,3,1,7,8},
{8,4,1,7,9,2,6,5,3}, 
{5,3,7,8,1,6,4,9,2}, 
{9,1,2,5,8,7,3,6,4}, 
{6,5,3,2,4,9,7,8,1}, 
{7,8,4,6,3,1,5,2,9}
};

struct parameter
{
	int taskid;
	int startx; 
	int starty;
};

int main (int argc, char * const argv[])
{
    pthread_t tid[11];
    struct parameter param[11];

    int i;

    //Initialize Parameters
    for(i = 0; i < 11; i++)
    {
        param[i].taskid = i;
        param[i].startx = (i % 3) * 3;
        param[i].starty = (i / 3) * 3;
    }

    //Start Threads
    for(i = 0; i < 9; i++)
    {
        pthread_create(&(tid[i]), NULL, testCell, &(param[i]));
    }
    pthread_create(&(tid[i]), NULL, testRows, &(param[i]));
    i++;
    pthread_create(&(tid[i]), NULL, testColumns, &(param[i]));


    //Join Threads
    for(int j = 0; j < 11; j++)
        pthread_join(tid[j], NULL);

    //Validate Results
    bool isValid = true;

    for(int i = 0; i < 11 && isValid; i++)
    {
        isValid = results[i];
    }

    printf("\nFinal Result: %s\n", isValid ? "Pass" : "Fail");

    return 0;
}

void *testRows(void *arg)
{
	struct parameter *param;    	
	param = (struct parameter *) arg;

    bool isValid = true;
    bool isFound = false;

    //Loop Through Rows
    for (int r = 0; r < 9 && isValid; r++)
    {
        //Loop Through Numbers to Count
        for(int i = 0; i < 9 && isValid; i++)
        {
            isFound = false;
            //Loop Through Columns of Row
            for(int c = 0; c < 9 && !isFound; c++)
            {
                if(sudoku[r][c] == i + 1)
                {
                    isFound = true;
                }
            }
            if(!isFound)
            {
                isValid = false;
            }
        }
    }

    results[param->taskid] = isValid;
    printf("Test Rows: %s\n", isValid ? "Pass" : "Fail");

    return NULL;
}

void *testColumns(void *arg)
{
	struct parameter *param;    	
	param = (struct parameter *) arg;

    bool isValid = true;
    bool isFound = false;

    //Loop Through Columns
    for (int c = 0; c < 9 && isValid; c++)
    {
        //Loop Through Numbers to Count
        for(int i = 0; i < 9 && isValid; i++)
        {
            isFound = false;
            //Loop Through Rows of Column
            for(int r = 0; r < 9 && !isFound; r++)
            {
                if(sudoku[r][c] == i + 1)
                {
                    isFound = true;
                }
            }
            if(!isFound)
            {
                isValid = false;
            }
        }
    }

    results[param->taskid] = isValid;
    printf("Test Columns: %s\n", isValid ? "Pass" : "Fail");

    return NULL;
}

void *testCell(void *arg)
{
	struct parameter *param;    	
	param = (struct parameter *) arg;

    bool isValid = true;
    bool isFound = false;

    //Loop Through Numbers to Count
    for (int i = 0; i < 9 && isValid; i++)
    {
        //Loop Through Cell
        isFound = false;
        for (int r = 0; r < 3 && !isFound; r++)
        {
            for (int c = 0; c < 3 && !isFound; c++)
            {
                if(sudoku[r + param->starty][c + param->startx] == i + 1)
                {
                    isFound = true;
                }
            }
            
        }
        if(!isFound)
        {
            isValid = false;
        }
    }

    results[param->taskid] = isValid;
    printf("Test Cell %d: %s\n", param->taskid + 1, isValid ? "Pass" : "Fail");

    return NULL;
}
