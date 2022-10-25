#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char* argv[]){
    struct stat fileStat;
    if(argc == 1){
        time_t IST_time = time(NULL);
        printf("IST %s", ctime(&IST_time));
    }
    else if(argc == 2){
        if(strcmp(argv[1], "-r") == 0){
            printf("date: missing file operand\n");
        }
        else if(strcmp(argv[1],"-u") == 0){
            time_t utc_now = time(&utc_now);
            struct tm *ptm = gmtime(&utc_now);
            printf("UTC %s", asctime(ptm));
            return 0;
        }
        else{
            printf("date: invalid option -- %s", argv[1]);
        } 
    }
    else if(argc == 3){
        if(strcmp(argv[1], "-r") == 0){
            if(stat(argv[2],&fileStat) < 0){
                printf("File does not exist\n");
                return 1;
            }
            else{
                printf("IST %s", ctime(&fileStat.st_mtime));
                return 2;
            }  
            
        }
        else if(strcmp(argv[1], "-u") == 0){
            if(argv[2] != NULL){
                printf("date: too many arguments\n");
            }
            else{
                time_t utc_now = time(&utc_now);
                struct tm *ptm = gmtime(&utc_now);
                printf("UTC %s", asctime(ptm));
                return 2;
            }
        }
        else if(argv[1] == NULL){
            time_t IST_time = time(NULL);
            printf("IST %s", ctime(&IST_time));
        }
    }
    else{
        printf("date: too many arguments\n");
        return 1;
    }       
}
