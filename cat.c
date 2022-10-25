#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char* argv[]){
    char input[2048];
    int inpsize = 2048;
    int line_count = 1;
    if(argc == 1){
        while(fgets(input, inpsize, stdin)){
            int length = strlen(input);
            input[length-1] = '\0';
            fprintf(stdout, "%s\n", input);
            if(strcmp(input, "exit") == 0){
                break;
            }
        }
    }
    else if(argc == 2){
        if(strcmp(argv[1], "-n") == 0){ 

            while(fgets(input, inpsize, stdin)){
                int length = strlen(input);
                input[length-1] = '\0';
                fprintf(stdout, "%s\n", input);
                if(strcmp(input, "exit") == 0){
                    break;
                }
            }
        }
        else if(strcmp(argv[1], "-b") == 0){ 
            while(fgets(input, inpsize, stdin)){
                int length = strlen(input);
                input[length-1] = '\0';
                fprintf(stdout, "%s\n", input);
                if(strcmp(input, "exit") == 0){
                    break;
                }
            }
        }
        else{ // file is sent
            FILE *fp = fopen(argv[1], "rb");
            if(fp == NULL){
                printf("cat: %s: No such file or directory\n", argv[1]);
                return 1;
            }

            while(fgets(input, inpsize, fp)){
                int length = strlen(input);
                input[length-1] = '\0';
                fprintf(stdout, "%s\n", input);
            }  
        }
    }
    else if(argc == 3){
        if(argv[1] == NULL){
            while(fgets(input, inpsize, stdin)){
                int length = strlen(input);
                input[length-1] = '\0';
                fprintf(stdout, "%s\n", input);
                if(strcmp(input, "exit") == 0){
                    break;
                }
            }
        }
        else if(strcmp(argv[1],"-n") == 0){ //implement cat -n filename
            FILE *fp = fopen(argv[2], "rb");
            if(fp == NULL){
                printf("cat: %s: No such file or directory\n", argv[2]);
                return 1;
            }
            
            while(fgets(input, inpsize, fp)){
                int length = strlen(input);
                input[length-1] = '\0';
                char* tmp = strdup(input);
                input[0] = '\0';
                sprintf(input, "%*d\t", 6, line_count++);
                strcat(input, tmp);
                fprintf(stdout, "%s\n", input);
            }  
        }
        else if(strcmp(argv[1], "-b") == 0){ //implement cat -b filename
            FILE *fp = fopen(argv[2], "rb");
            if(fp == NULL){
                printf("cat: %s: No such file or directory\n", argv[2]);
                return 1;
            }
            
            while(fgets(input, inpsize, fp)){
                int length = strlen(input);
                input[length-1] = '\0';
                length = strlen(input);
                if(length >= 1){
                    char* tmp = strdup(input);
                    input[0] = '\0';
                    sprintf(input, "%*d\t", 6, line_count++);
                    strcat(input, tmp);
                }
                fprintf(stdout, "%s\n", input);
            } 
        }
        else{//file name is sent
            if(argv[2] == NULL){
                FILE *fp = fopen(argv[1], "rb");
                if(fp == NULL){
                    printf("cat: %s: No such file or directory\n", argv[1]);
                    return 1;
                }
                while(fgets(input, inpsize, fp)){
                    int length = strlen(input);
                    input[length-1] = '\0';
                    fprintf(stdout, "%s\n", input);
                }
            }
            else{
                printf("cat: too many arguments\n");
            }
        }
    }
    else{
        printf("cat: too many arguments\n");
    }
}