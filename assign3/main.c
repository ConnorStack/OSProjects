/*
*File: 
*Connor Stack
*/

#include <stdlib.h>
#include <stdio.h>
#include "readyQ.h"
#include "IOQ.h"

int main(int argc, char *argv[]){
    char* schedule_type = argv[2];
    char* file_input = NULL;

    if(argc == 5){ 
        file_input = argv[4];


        printf("5");
    }else if(argc == 7){
        file_input = argv[6];

        
        printf("7");
    }else{
        printf("invalid number of arguments");
    }
    return 0;
}