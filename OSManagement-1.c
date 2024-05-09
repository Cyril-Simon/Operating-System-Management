// Cyril Simon
// CGS 3763
// 3/28/2022
// Spring 2022

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>


// Global constants
#define EXIT 0
#define INVALID -1
#define MEMORY 1
#define FILES 2
#define THREAD 3

#define FIRST 0
#define BEST 1
#define WORST 2
#define NEXT 3

// Function Prototypes
int displayMenu();
void clearScreen();
void memoryManagement();
void displayProcess(int allocation[], int processes, int processSize[]);
void firstFit(int blockSize[], int blocks, int processSize[], int processes);
void worstFit(int blockSize[], int blocks, int processSize[], int processes);
void bestFit(int blockSize[], int blocks, int processSize[], int processes);
void nextFit(int blockSize[], int blocks, int processSize[], int processes);
void printAttributes(char name[], struct stat statBuff);
void fileAttributes();
void multiThreads(); 
void* threadFunction(void* vargp);



int main() {
  int choice = INVALID;

  while (choice != EXIT) 
  {

    choice = displayMenu();
    switch (choice) {
    case MEMORY:
      memoryManagement();
      break;

    case FILES:
      fileAttributes();
      break;

    case THREAD:
      multiThreads();
      break;

    case EXIT:
      exit(EXIT);

    } // End of switch statement

  } // End of while loop

  return EXIT;

} // End of main

int displayMenu() {

  int choice = INVALID;

  while (choice == INVALID) {
    printf("\t\t\t\t\t********** Operating System Management Menu ********** "
           "\n\n");
    printf("Enter a number which corresponds to the OS program you would like "
           "to run.\n");
    printf("Press 1 for Memory management.\n");
    printf("Press 2 for File management.\n");
    printf("Press 3 for Multithreading.\n");
    printf("Press 0 to Exit.\n");

    // Enter input for the menu

    scanf("%d", &choice);

    if (choice <= 0 && choice >= 3) 
    {
      choice = INVALID;
    } // End of if cstatement

  } // End of while loop

  return choice;
 
} // End of displayMenu function

// Clears scrren after output of menu
void clearScreen() {

  char enter;

  printf("\n\t\t\t(Hit Enter to Continue)\n\n\n");

  scanf("%c", &enter);

  // Windows command
  system("cls");

  // Mac command
  // system("clear");

} // End of clearScreen function

// Displays the memory management option
void memoryManagement() 
{
  printf("\t\t\t\t\t********** Memory Management ********** \n\n");

  clearScreen();

  int algorithm;

  for (algorithm = 0; algorithm <= NEXT; algorithm++) {
    int blockSize[] = {50, 20, 30, 45, 60};
    int processSize[] = {55, 40, 20, 25, 30};
    int blocks = sizeof(blockSize) / sizeof(blockSize[0]);
    int processes = sizeof(processSize) / sizeof(processSize[0]);

    switch (algorithm) {
    case FIRST:
      firstFit(blockSize, blocks, processSize, processes);
      break;

    case BEST:
      bestFit(blockSize, blocks, processSize, processes);
      break;

    case WORST:
      worstFit(blockSize, blocks, processSize, processes);
      break;

    case NEXT:
      nextFit(blockSize, blocks, processSize, processes);
      break;

    } // End of switch

  } // End of for loop
} // End of memory management function

void nextFit(int blockSize[], int blocks, int processSize[], int processes) 
{
  printf("\n ********** Next Fit **********\n\n");

  int i;

  int store = 0;

  int allocation[processes];

  memset(allocation, INVALID, sizeof(allocation));

  for (i = 0; i < processes; i++) 
  {
    while (store < blocks) 
    {
      if (blockSize[store] >= processSize[i]) 
      {
        allocation[i] = store;
        blockSize[store] -= processSize[i];
        break;
      } // End of if statement
      
      store = (store + 1) % blocks;
    } // End of while loop

  } // end of i loop

  displayProcess(allocation, processes, processSize);

} // End of function

void firstFit(int blockSize[], int blocks, int processSize[], int processes) {

  printf("\n ********** First Fit **********\n\n");

  int i;

  int store;

  int allocation[processes];

  memset(allocation, INVALID, sizeof(allocation));

  for (i = 0; i < processes; i++) {
    for (store = 0; store < blocks; store++) 
    {
      if (blockSize[store] >= processSize[i]) 
      {
        allocation[i] = store;
        blockSize[store] -= processSize[i];
        break;
      } // End of if statement

    } // End of store loop

  } // end of i loop

  displayProcess(allocation, processes, processSize);

} // End of first fit function

void worstFit(int blockSize[], int blocks, int processSize[], int processes) {
  printf("\n ********** Worst Fit **********\n\n");

  int k;

  int store;

  int allocation[processes];

  memset(allocation, INVALID, sizeof(allocation));

  for (k = 0; k < processes; k++) 
  {
    int wstIdx = INVALID;

    for (store = 0; store < blocks; store++) 
    {
      if (blockSize[store] >= processSize[k]) 
      {
        if (wstIdx == INVALID) 
        {
          wstIdx = store;
        } // End of if
        else if (blockSize[wstIdx] < blockSize[store]) 
        {
          wstIdx = store;
        } // End of else if
      }   // End of if statement

      if (wstIdx != INVALID) 
      {
        allocation[k] = wstIdx;
        blockSize[wstIdx] -= processSize[k];
      }

    } // End of store loop

    displayProcess(allocation, processes, processSize);
  } // End of worst fit function
}

void bestFit(int blockSize[], int blocks, int processSize[], int processes) {
  int i;

  int j;

  int allocation[processes];

  memset(allocation, INVALID, sizeof(allocation));

  printf("\n ********** Best Fit **********\n\n");

  for (i = 0; i < processes; i++) 
  {
    int bestIdx = INVALID;

    for (j = 0; j < blocks; j++) 
    {
      if (blockSize[j] >= processSize[i]) 
      {
        if (bestIdx == INVALID) 
        {
          bestIdx = j;
        } // End of if
        else if (blockSize[bestIdx] > blockSize[j]) 
        {
          bestIdx = j;
        } // End of else if
      }   // End of if statement

      if (bestIdx != INVALID) 
      {
        allocation[i] = bestIdx;
        blockSize[bestIdx] -= processSize[i];
      }

    } // End of store loop

    displayProcess(allocation, processes, processSize);
  } // End of best fit function
}

// Used to display the process
void displayProcess(int allocation[], int processes, int processSize[]) {

  printf("\nProcess No.  \tProcess Size   \tBlock no.\n");

  int i;

  for (i = 0; i < processes; i++) {

    printf(" %d\t\t        %d\t\t ", i + 1, processSize[i]);
    if (allocation[i] == INVALID) {
      printf("         Not allocated\n");

    } // End of if

    else {
      printf("         %d\n", (allocation[i] + 1));
    }

  } // End of for loop

  printf("\n");
}

void fileAttributes()
{
  struct stat statBuff;

  int error;

  struct dirent *de;

  DIR *dr = opendir(".");

  if (dr == NULL)
  {
    printf("Can not open the directory");
  }//End of if statement

  while((de = readdir(dr)) != NULL)
  {
    error = stat(de -> d_name, &statBuff);
    
    if(error == INVALID)
    {
      printf("\nError in stat\n");
      printf("\nPress enter to continue...");
      getchar();
    }//End of if statement
    
      printAttributes(de -> d_name, statBuff);
    
  }//End of while loop
  
}//End of function

//Prints out attributes of the file
void printAttributes(char name[], struct stat statBuff)
{
  time_t t;

  char timeStr[100];

  int i;

  printf("\n----------File = %s -------------\n\n", name);

  printf("Device id: %lu\n", statBuff.st_dev);

  printf("Serial Number = %lu\n", statBuff.st_ino);

  printf("User id = %d\n", statBuff.st_uid);

  printf("Group id = %d\n", statBuff.st_gid);

  printf("Owner permission");

  if(statBuff.st_mode & S_IRUSR)
    {
      printf(" read");
    }
  if(statBuff.st_mode & S_IWUSR)
    {
      printf(" write");
    }
  if(statBuff.st_mode & S_IXUSR)
    {
      printf(" execute");
    }

  printf("Group permission");

  if(statBuff.st_mode & S_IRGRP){printf(" read");}
  if(statBuff.st_mode & S_IWGRP){printf(" write");}
  if(statBuff.st_mode & S_IXGRP){printf(" execute");}

  printf("Other permissions");

  if(statBuff.st_mode & S_IROTH){printf(" read");}
  if(statBuff.st_mode & S_IWOTH){printf(" write");}
  if(statBuff.st_mode & S_IXOTH){printf(" execute");}

  for (i = 0; i < 3; i++)
  {
    if(i == 0)
    {
      struct tm *time;
      t = statBuff.st_ctime;
      time = localtime(&t);
      printf("Date created = %s", asctime(time));
    }//End of if

    else if(i == 1)
    {
      struct tm *time;
      t = statBuff.st_mtime;
      time = localtime(&t);
      printf("Date Modified = %s", asctime(time));
    }//End of else if

    else
    {
      struct tm *time;
      t = statBuff.st_atime;
      time = localtime(&t);
      printf("Last accessed = %s", asctime(time));
    }
  
  }//End of for loop 

  printf("File size = %ld bytes\n", statBuff.st_size);
  
}//End of function

  //Prints out thread ID
void* threadFunction(void* vargp)
{
  const int LOOP = 5;

  int i;

  int *myid;

  myid = (int *)vargp;

  printf("\n---------------------------\n");

  printf("Thread ID %d has started\n", *myid);

  printf("\n---------------------------\n\n"); 

  for(i = 0; i < LOOP; i++)
  {
    printf("thread ID %d is printing iteration %d\n", *myid, i);
  }//End of for loop

    printf("\n---------------------------\n");

  printf("Thread ID %d has finished\n", *myid);

  printf("\n---------------------------\n\n"); 

  return NULL;
  
}//End of function

void multiThreads()
{
  const int SIZE = 5;

  int i = 0;

  int error;

  pthread_t tid[SIZE];

  for(i = 0; i < SIZE; i++)
  {
    error = pthread_create (&(tid[i]), NULL, &threadFunction, (void *) &tid[i]);

    if(error != 0)
    {
      printf("\n Thread cannot be created [%s]", strerror(error));
    }//End of if statement
  }//End of for loop 1 

  for (i = 0; i < SIZE; i++)
  {
    pthread_join(tid[i], NULL);
  }

  
}//End of function