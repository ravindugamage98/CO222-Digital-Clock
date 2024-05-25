/*
         CO222 PROJECT 1 - DIGITAL CLOCK

         Arthur           :- G.D Ravindu Mihiranga
                             E/18/224
         Last modified on :- 29/08/2021

*/

// Calling all the essential header files
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <termios.h>

// Function to off echo
void errhandler(){
    struct termios term;
    tcgetattr(fileno(stdin), &term);
    term.c_lflag |= ECHO;
    tcsetattr(fileno(stdin), 0, &term);
}
// Function to catch Ctr C using signal.h
void inthandler(int dummy){
    // echo off
    struct termios term;
    tcgetattr(fileno(stdin), &term);
    term.c_lflag |= ECHO;
    tcsetattr(fileno(stdin), 0, &term);
    // Clearing the screen line by line
    int cursor=0;
    printf("\033[u");                         // Restoring the cursor at the line which prints the date
    printf("\033[K");                         // Erase the line
    for(cursor=1;cursor<=7;cursor++){         // Loop to erase the content line by line
	  printf("\033[1A");                      // Cursor up
	  printf("\033[K");                       // Erase the line
    }
    printf("\033[?1049l");                    // Disable alternative screen buffer, from xterm
    printf("\033[?25h");                      // Make the cursor visible
    printf("\033[0;m");                       // Reset the color
    exit(1);                                  // Terminating the program
}

//Function to print numbers(1:9) & two dots line by line
void printNumber(int x, int col, int row){    // Parameters:- number[0;9]/two dots[10], color, row number
    int num = 3*(row-1);                      // Starting index of the given line
    int bgc = 40 + col;                       // Background color
    // Declaring & initializing an integer 2D array with 10 rows & 15 columns to print the lines
    // There 11 sub arrays to print the 10 numbers & the two dots
    // Each sub array consists 1,0 (1 = where to print colored block,0 = where to leave a space) code to print the numbers
    // Here I am printing two space colored block at a time
    int numbers[11][15] = {{1,1,1,1,0,1,1,0,1,1,0,1,1,1,1},{0,0,1,0,0,1,0,0,1,0,0,1,0,0,1},{1,1,1,0,0,1,1,1,1,1,0,0,1,1,1},
                           {1,1,1,0,0,1,1,1,1,0,0,1,1,1,1},{1,0,1,1,0,1,1,1,1,0,0,1,0,0,1},{1,1,1,1,0,0,1,1,1,0,0,1,1,1,1},
                           {1,1,1,1,0,0,1,1,1,1,0,1,1,1,1},{1,1,1,0,0,1,0,0,1,0,0,1,0,0,1},{1,1,1,1,0,1,1,1,1,1,0,1,1,1,1},
                           {1,1,1,1,0,1,1,1,1,0,0,1,1,1,1},{0,0,0,0,1,0,0,0,0,0,1,0,0,0,0}};
    int i=0;
    for (i=num;i<num+3;++i){                  // Loop to print the block according to the given number
        if (numbers[x][i]==1){
            printf("\033[%dm  ",bgc);         // Printing two space block at once
            printf("\033[0;m");               // Reseting the color
        }
        else{
            if(x==10 && i!=num+1){            // Exception when x = 10(two dots) and when the cursor approaches to the middle column
                printf(" ");                  // print the block with one space
            } else {
                printf("  ");                 // Otherwise it will print two space block at once
            }
        }
    }
    printf(" ");                              // Leaving a space in order to print the next number
}

// Function to get the current time
void getTime(int col){                        // Parameter:- color
    int fgc = 30 + col;                       // Foreground color
    time_t currentTime;
    struct tm* localTime;
    printf("\033[?1049h");                    // Enable alternative screen buffer, from xterm
    printf("\033[2J");                        // Clears the entire screen
    printf("\033[H");                         // Moves cursor to home position (0,0)
    printf("\033[?25l");                      // Make the cursor invisible
    printf("\n");                             // Leaving a new line to start the simulation

    // Infinite loop to call the time continuously
	while(1){
        currentTime = time(NULL);             // Time in seconds
        localTime = localtime(&currentTime);  // Get the current time
        printf("\r");
        int i=1;
        // Loop to print the numbers of the current time line by line(going through five lines)
        // Arithmetic operations are used to separate the number which I want to print
        // Finally I have called the printNumber function
        for(i=1;i<=5;++i){
            printf("\033[C");                 // Move cursor right 1 column
            printNumber(localTime->tm_hour/10,col,i);
            printNumber(localTime->tm_hour%10,col,i);
            printNumber(10,col,i);
            printNumber(localTime->tm_min/10,col,i);
            printNumber(localTime->tm_min%10,col,i);
            printNumber(10,col,i);
            printNumber(localTime->tm_sec/10,col,i);
            printNumber(localTime->tm_sec%10,col,i);
            printf("\n");
        }
        printf("\n");                           // Leaving a line in order to print the date
        printf("\033[22C");                     // Moves cursor right 22 columns
        // Printing the date in the given format
        printf("\033[%dm%04d-%02d-%02d",fgc,localTime->tm_year+1900,localTime->tm_mon+1,localTime->tm_mday);
        printf("\033[0;m");                     // Reseting the color
        printf("\033[32D");                     // Moves cursor left 32 columns
        printf("\033[s");                       // Save cursor position
        printf("\033[6A");                      // Moves cursor up 6 lines
    }
}

// Function to convert uppercase characters to lowercase
char toLower(char letter){
    if (letter >= 'A' && letter<= 'Z'){
        return letter + ('a'- 'A');
    } else {
        return letter;
    }
}
// Function to compare two strings
int compare(char inpStr[],char word[]){
    int wordlen =0;
    int strlen =0;
    int isEqual = 1;
    while(word[wordlen]!='\0'){                 // Calculating the length of the word array
        ++wordlen;
    }
    while (inpStr[strlen]!= '\0'){              // Calculating the length of the input string array
        ++strlen;
    }
    if (wordlen!= strlen) return 0;             // Checking whether the lengths are same
    int i;
    for (i=0;i<wordlen;++i){
        if (toLower(inpStr[i])!=toLower(word[i])) { // Checking whether the lowercased characters are equal in the two strings
            isEqual = 0;
            break;
        }
    }
    return isEqual;
}
//*********************************************************************************************************************************
int main(int argc, char ** argv){               // Calling the main function with the command line arguments
    // Echo on to disable the keyboard while running the program except for ctr C
    // This will prevent the output misbehaving when some keys are entered while the time is displaying on screen
    struct termios term;
    tcgetattr(fileno(stdin), &term);
    term.c_lflag &= ~ECHO;
    tcsetattr(fileno(stdin),0,&term);

    // Calling the inthandler function along with SIGINT to catch ctr C
    signal(SIGINT, inthandler);

    // Declaring & initializing a 2D string array to store the colors
    char colors[8][10] = {"BLACK","RED","GREEN","YELLOW","BLUE","MAGENTA","CYAN","WHITE"};
    // Declaring a string array
    char remain[128];

    // Declaring variables
    int checkCH,error1=-1,error2=-1,hIndex=-1,randIndex=-1,j,k,done=-1,isH=0,isC=0,isRandom=0,col,spaces,cstop=0;

    /*             Variable Explanation
       checkCH,j,k,spaces     =      counter variables
       isC                    =      is '-c' entered?
       isH                    =      is '-h' entered?
       isRandom               =      is '-x' or '--y' entered?
       hIndex                 =      index of the argument which includes '-h'
       randIndex              =      index of the argument which includes '-x' or '--y'
       done                   =      to indicate whether the argument has matched with the color or not
       col                    =      the index of the colors array which has matched with the argument
       cstop                  =      indicates whether argv array ends with a '-c' or not
       error1                 =      when the 'xxx' part of '-cxxx' has n't matched with any color
       error2                 =      when the 'xxx' part of '-c xxx' has n't matched with any color
    */

    // Looping through the arguments to detect '-h' and '-c'
    for (checkCH=1;checkCH<argc;++checkCH){
        // Checking for '-h'
        if (argv[checkCH][0]=='-' && argv[checkCH][1]=='h'){
            isH = 1;                              // Indicates that I have found '-h'
            hIndex = checkCH;                     // Storing the index of the argument which includes '-h'
        }
        // If user inputs '--' (double dash) as an argument it will print the output according to those inputs which user has input before '--'
        // This will break the current loop
        if (argv[checkCH][0]=='-' && argv[checkCH][1]=='-' && argv[checkCH][2]=='\0') break;
        // Checking for '-x'(x can be any random character instead of 'c' and 'h')
        // Neglecting the '-' (single dash) scenario
        // And checking for '--y' (y can be any random character including '-')
        if ((argv[checkCH][0]=='-' && argv[checkCH][1]!='h' && argv[checkCH][1]!='c' && argv[checkCH][1]!='\0') || (argv[checkCH][0]=='-' && argv[checkCH][1]=='-' && argv[checkCH][2]!='\0')){
            isRandom = 1;                         // Indicates that I have found '-x' or '--y'
            randIndex = checkCH;                  // Storing the index of the argument which includes '-x' or '--y'
        }
        // Checking for '-c'
        if (argv[checkCH][0]=='-' && argv[checkCH][1]=='c'){
            // There are two scenarios where '-c' can be seen
            // Scenario 1. Checking for '-cxxx' scenario
            if (argv[checkCH][2]!='\0'){
                isC = 1;                          // Changing the isC variable to 1 if there is a '-c'
                j =0;
                // Loop to append the characters of the argument to the predefined string array excluding the first two characters.
                for (k=2;argv[checkCH][k]!='\0';++k){
                    remain[j] = argv[checkCH][k]; // Storing the characters of the argument in the remain array
                    ++j;
                }
                remain[j] = '\0';
                done=0;                           // Changing the done variable -1 to 0
                // Loop to compare the remain string array with the sub arrays of the colors 2D string array
                for (k=0;k<8;++k){
                    // Checking if both strings are equal
                    if (compare(remain,colors[k])==1){
                        col = k;                  // Storing the index of the color 2D array in another variable
                        done=1;                   // Changing the done variable to 1
                        break;
                    }
                }
                if (done==0) error1 = checkCH;    // If done equals 0 storing the index of the argument in a variable which included '-cxxx' scenario
            }
            // Scenario 2. Checking for '-c' 'xxx'
            if(argv[checkCH][2]=='\0' && checkCH!=argc-1){
                isC=1;                            // Changing the isC variable to 1 if there is a '-c'
                done=0;                           // Changing the done variable to 0
                // Loop to compare the argument with the sub arrays of the colors 2D string array
                for (k=0;k<8;++k){
                    // Checking if both strings are equal
                    if (compare(argv[checkCH+1],colors[k])==1){
                        col = k;                  // Storing the index of the color 2D array in another variable
                        done=1;                   // Changing the done variable to 1
                        break;
                    }
                }
                if (done==0) error2 = checkCH+1;  // If done equals 0 storing the index of the argument in a variable which included '-c' 'xxx' scenario
                ++checkCH;                        // Incrementing the counter variable to skip checking the 'xxx' part in order to search '-h' or '-c'
            // Special case where the argv array ends with '-c'.
            // If this happens changing the cstop variable to 1
            } else if (argv[checkCH][2]=='\0' && checkCH==argc-1) cstop=1;
        }
    // If user inputs any random characters without a dash(ex:- qwerth) or a single dash(-) then the program will neglect those inputs
    }

    // If the below conditions satisfy then the program will terminate after printing the user manual
    // Here I am using those variables which were declared above
    if ((isH==1 && isRandom!=1) || (isH==1 && randIndex > hIndex)){
        // Printing the user manual of the clock
        printf("usage : clock -h");
        for (spaces=1;spaces<=16;++spaces){
            printf(" ");
        }
        printf("quick help on cmd\n");
        printf("usage : clock -c <color>");
        for (spaces=1;spaces<=8;++spaces){
            printf(" ");
        }
        printf("print clock with a color\n");
        printf("<color-black|red|green|yellow|blue|magenta|cyan|white>  supported colors\n");
        errhandler();                             // Calling the function to off echo
        return 1;                                 // Terminating the program
    }
    // Checking for the below conditions if those conditions satisfy then it will print the error message along with the user manual
    // Here also I am using those variables which were declared above
    if(cstop==1 || isRandom==1){
        printf("Invalid use of arguments.\n");
        printf("usage : clock -h");
        for (spaces=1;spaces<=16;++spaces){
            printf(" ");
        }
        printf("quick help on cmd\n");
        printf("usage : clock -c <color>");
        for (spaces=1;spaces<=8;++spaces){
            printf(" ");
        }
        printf("print clock with a color\n");
        errhandler();                             // Calling the function to off echo
        return 2;                                 // Terminating the program
    }
    // Checking for the below conditions if those conditions satisfy then it will start printing the time until user enters ctr c
    // Here also I am using those variables which were declared above
    if (done==1 && isH!=1 && cstop!=1 && isRandom!=1){
        getTime(col);                             // Calling the function to get the current time & to start printing
    }
    // If there are no '-c', '-h' & '-x' have entered or there are no arguments
    // then the output will start printing the time in white color(default) until user enters ctr c
    // Here also I am using those variables which were declared above
    if (isH==0 && isC==0 && isRandom==0){
        getTime(7);                               // Calling the function to get the current time & to start printing
    }
    // Checking for the below conditions if those conditions satisfy then it will print the corresponding error messages
    // Here also I am using those variables which were declared above
    if(error1>error2 && cstop!=1 && isRandom!=1){
        printf("%s :This is not a valid color, Please enter one of these colours: black, red, green, yellow, blue, magenta, cyan, white\n",remain);
        errhandler();                             // Calling the function to off echo
    }
    if(error1<error2 && cstop!=1 && isRandom!=1){
        printf("%s :This is not a valid color, Please enter one of these colours: black, red, green, yellow, blue, magenta, cyan, white\n",argv[error2]);
        errhandler();                             // Calling the function to off echo
    }

    return 0;                                     // Then end of the program
}
