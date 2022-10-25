#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/stat.h>
#include<sys/types.h>

void mkdirRecursive(const char *path, mode_t mode) {
    char path_desired[1024];
    size_t len;

    strncpy(path_desired, path, sizeof(path_desired));
    path_desired[sizeof(path_desired) - 1] = '\0';
    len = strlen(path_desired);
    if (len == 0){
        return;
    }
    else if (path_desired[len - 1] == '/'){
        path_desired[len - 1] = '\0';
    }

    for(char* ptr = path_desired; *ptr; ptr++)
        if (*ptr == '/') {
            *ptr = '\0';
            mkdir(path_desired, mode);
            *ptr = '/';
        }
    if (access(path_desired, F_OK))       
        mkdir(path_desired, mode);
}

int main(int argc, char* argv[]){
    if(argc == 1){
        printf("mkdir: missing operand\n");
    }
    else if(argc == 2){
        if(strcmp(argv[1], "-v") == 0){
            printf("mkdir: missing operand\n");
            exit(1);
        }
        else if(strcmp(argv[1], "-pV") == 0){
            printf("mkdir: missing operand\n");
            exit(1);
        }
        else{
            int ret = mkdir(argv[1], 0777);
            if(ret == -1){
                printf("mkdir: can't make directory %s\n", argv[1]);
                exit(1);
            }
        }
    }
    else{
        if(strcmp(argv[1], "-v") == 0){
            if(argv[2] == NULL){
                printf("mkdir: illegal usage error");
            }
            else{
                for(int i = 2; i < argc; i++){
                    if(mkdir(argv[i], 0777) == -1){
                        printf("mkdir: can't make directory %s\n", argv[i]);
                        exit(1);
                    }
                    else{
                        printf("mkdir: created directory '%s'\n", argv[i]);
                    }
                }
            }
            
        }
        else if(strcmp(argv[1], "-pv") == 0){
            if(argc == 3){
                mkdirRecursive(argv[2], S_IRWXU);
            }
            else{
                printf("mkdir: too many args\n");
            }
        }
        else if(strcmp(argv[1], "NULL") == 0){
            for(int i = 2; i < argc; i++){
                if(mkdir(argv[i], 0777) == -1){
                    printf("mkdir: can't make directory %s\n", argv[i]);
                    exit(1);
                }
            }
        }
        else{
            for(int i = 1; i < argc; i++){
                if(mkdir(argv[i], 0777) == -1){
                    printf("mkdir: can't make directory %s\n", argv[i]);
                    exit(1);
                }
            }
        }
    }
}