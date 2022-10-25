#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/stat.h>
#include<sys/types.h>

int isDir(char* file){ //To check if an input file name is a directory or not
    struct stat path;
    stat(file, &path);
    return S_ISREG(path.st_mode);
}

int main(int argc, char* argv[]){
    if(argc == 1){
        printf("rm: missing operand\n");
    }
    else if(argc == 2){
        if(strcmp(argv[1],"-v") == 0){
            printf("rm: missing operand\n");
        }
        else if(strcmp(argv[1],"-d") == 0){
            printf("rm: missing operand\n");
        }
        else{
            if(isDir(argv[1]) != 0){
                if(remove(argv[1]) == -1){
                    printf("rm: cannot remove '%s': No such file or directory", argv[1]);
                }
            }
            else{
                printf("rm: cannot remove '%s': is a directory\n", argv[1]);
            }
        }
    }
    else{
        if(strcmp(argv[1],"-v") == 0){
            for(int i = 2; i < argc; i++){
                if(isDir(argv[i]) != 0){
                    if(remove(argv[i]) == 0){
                        printf("File %s deleted successfully\n", argv[i]);
                    }
                    else{
                        printf("Unable to delete the file %s\n", argv[i]);
                    }
                }
                else{
                    printf("rm: cannot remove '%s'\n", argv[i]);
                }
            }  
        }
        else if(strcmp(argv[1], "-d") == 0){
            for(int i = 2; i < argc; i++){
                if(isDir(argv[i]) == 0){
                    if(remove(argv[i]) == 0){
                    }
                    else{
                        printf("Unable to delete the file %s\n", argv[i]);
                    }
                }
                else{
                    printf("rm: cannot remove '%s'\n", argv[i]);
                }
            }
        }
        else{
            for(int i = 2; i < argc; i++){
                if(isDir(argv[i]) != 0){
                    if(remove(argv[i]) == 0){
                    }
                    else{
                        printf("Unable to delete the file %s\n", argv[i]);
                    }
                }
                else{
                    printf("rm: cannot remove '%s'\n", argv[i]);
                }
            }
        }
    }       
}