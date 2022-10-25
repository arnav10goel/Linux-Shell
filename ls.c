#include<stdio.h>
#include<dirent.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<stdbool.h>

bool file_exists (char *filename) {
  struct stat   buffer;   
  return (stat (filename, &buffer) == 0);
}

int main(int argc, char* argv[]){
    DIR *dir;
    struct dirent *ent;
    char cwd[4096];
    getcwd(cwd, sizeof(cwd));
    dir = opendir(cwd);
    if(argc == 1){
        printf("\n");
        if(dir != NULL){
            while((ent = readdir(dir)) != NULL){
                if(ent->d_name[0] != '.'){
                    printf("%s\n", ent->d_name);
                }
            }
        }
        
        printf("\n");
    }
    else if(argc == 2){
        printf("\n");
        if(argv[1] == NULL){
            if(dir != NULL){
                while((ent = readdir(dir)) != NULL){
                    if(ent->d_name[0] != '.'){
                        printf("%s\n", ent->d_name);
                    }
                }
            }
        }
        else if(strcmp(argv[1], "-a") == 0){
            if(dir != NULL){
                while((ent = readdir(dir)) != NULL){
                    printf("%s\n", ent->d_name);
                }
            }
        }
        else if(strcmp(argv[1], "-i") == 0){
            if(dir != NULL){
                while((ent = readdir(dir)) != NULL){
                    if(ent->d_name[0] != '.'){
                        printf("%llu %s\n", ent->d_ino, ent->d_name);
                    }
                }
            }
        }
        else{
            printf("ls: invalid input\n");
        }
        printf("\n");
    }
    else{
        printf("ls: too many arguments\n");
    }
    closedir(dir);
    return 0;
}