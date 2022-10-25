#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<pthread.h>
#include<stdbool.h> 
//Internal Commands Handled directly by Shell

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

void pthread_implement(char *command)//Function to use system() call to call external programs
{
    if(command == NULL){
        printf("ERROR::Input pointer argument is NULL\n");
        return;
    }
    system(command);
}

bool file_exists (char *file) { //Program to check if a file exists in current directory
  struct stat   buffer;   
  return (stat(file, &buffer) == 0);
}//returns true if exists

int isDir(char* file){ //To check if an input file name is a directory or not
    struct stat path;
    stat(file, &path);
    return S_ISREG(path.st_mode);
}//returns 0 if directory, 1 if file

void exe_cd(int args, char* inputs[]){
    char cwd[1024];
    if(args > 2){
        printf("cd: too many arguments\n");
        return;
    }
    else if(args == 2){
        if(strcmp(inputs[1], "..") == 0){
            getcwd(cwd, 1024);
            printf("Current Directory: %s\n", cwd);
            chdir("..");
            getcwd(cwd, 1024);
            printf("Changed Directory: %s\n", cwd);
        }
        else{
            if(chdir(inputs[1]) == -1){
                printf("cd: Can't change path to %s\n", inputs[1]);
            }
            else{
                getcwd(cwd, 1024);
                printf("Changed to: %s\n", getcwd(cwd, 1024));
            }
        }
    }
    else{
        getcwd(cwd, 1024);
        printf("Current Directory: %s\n", cwd);
        chdir(getenv("HOME"));
        getcwd(cwd, 1024);
        printf("Changed Directory to: %s\n", cwd);
    }
}

char* exe_pwd(int args, char* inputs[]){ //Execution of pwd command
    char* cwd = (char*)malloc(4096);
    if(args == 1){
        getcwd(cwd, 1024);
        return cwd;
    }
    else if(args == 2){
        if(strcmp(inputs[1], "-P") == 0){
            getcwd(cwd, 1024);
            return cwd;
        }
        else if(strcmp(inputs[1], "-L") == 0){
            getcwd(cwd, 1024);
            return cwd;
        }
        else{
	    getcwd(cwd, 1024);
            return "pwd: invalid option";
        }
    }
    else{
	getcwd(cwd, 1024);
        return "pwd: too many arguments";
    }
    
}

void exe_echo(int args, char* inputs[]){ //Execution of echo command
    if(args == 1){
        printf("\n");
    }
    else if(args == 2){
        if(strcmp(inputs[1], "-n") == 0){
            return;
        }
        else if(strcmp(inputs[1], "-help") == 0){
            printf("NAME\n");
            printf("\t echo - write arguments to the standard output\n");
            printf("SYNOPSIS\n");
            printf("\t echo [-n]... [STRING]...\n");
            printf("DESCRIPTION\n");
            printf("\t -n\t\tDo not output the trailing newline\n");
            printf("\t -help\t\tDisplay this help and exit\n");
        }
        else{
            printf("%s\n", inputs[1]);
        }
    }
    else{
        if(strcmp(inputs[1], "-n") == 0){
            for(int i = 2; i < args; i++){
                printf("%s ", inputs[i]);
            }
        }
        else{
            for(int i = 1; i < args; i++){
                printf("%s ", inputs[i]);
            }
            printf("\n");
        }
    }
}

void fork_rm(char* path, char* argname, char* mode){
    pid_t pid;
    pid = fork();
    if(pid == -1){
        perror("Fork Failed");
        exit(1);
    }
    else if(pid == 0){
        execl(path, "rm", mode, argname, NULL);
    }
    else{
        wait(NULL);
    }
    return;
}

int fork_mkdir(char* path, char* mode, char* dir){
    pid_t pid;
    pid = fork();
    if(pid == -1){
        perror("Fork Failed");
        exit(1);
    }
    else if(pid == 0){
        execl(path, "mkdir", mode, dir, NULL);
    }
    else{
        wait(NULL);
    }
    return 0;
}

void fork_date(char* path, char* argname, char* mode){
    pid_t pid;
    pid = fork();
    if(pid == -1){
        perror("Fork Failed");
        exit(1);
    }
    else if(pid == 0){
        execl(path, "date", mode, argname, NULL);
    }
    else{
        wait(NULL);
    }
    return;
}

void fork_cat(char* path, char* arg1, char* arg2){
    pid_t pid;
    pid = fork();
    if(pid == -1){
        perror("Fork Failed");
        exit(1);
    }
    else if(pid == 0){
        execl(path, "cat", arg1, arg2, NULL);
    }
    else{
        wait(NULL);
    }
    return;
}

void fork_ls(char* path, char* arg1){
    pid_t pid;
    pid = fork();
    if(pid == -1){
        perror("Fork Failed");
        exit(1);
    }
    else if(pid == 0){
        execl(path, "ls", arg1, NULL);
    }
    else{
        wait(NULL);
    }
    return;
}

int main(int argc, char* argv[]){
    char perm_directory[4096];
    char temp_perm_directory[4096];
    getcwd(perm_directory, 4096);
    getcwd(temp_perm_directory, 4096);
    printf("Shell Started\n");
    printf("Shell Name: pabloo\n");
    printf("Shell Version: 1.0\n");
    printf("Shell Author: arnav21519\n");
    int exit_status = 1;
    while (exit_status)
    {
        char cwd_curr[4096];
        getcwd(cwd_curr, 4096);
        strcat(perm_directory, "/");
        printf("pabloo$ > ");
        
        size_t size = 1024;
        char arr_fgets[4096];
        fgets(arr_fgets, 4096, stdin);
    
        char *delim = " \n";
        char *token;
        char **inputs = (char**)malloc(4096 * sizeof(char*));
        token = strtok(arr_fgets, delim);
        int i = 0;
        inputs[i] = token;
        while(token != NULL){
            token = strtok(NULL, delim);
            i++;
            inputs[i] = token;
        }
        int arg_count = 0;
        while(inputs[arg_count] != NULL){
            arg_count++;
        }
        if(arg_count == 0){
            continue;
        }
        else{
            //Internal Commands
            if(strcmp(inputs[0], "cd") == 0){
                exe_cd(arg_count, inputs);
            }
            else if(strcmp(inputs[0], "pwd") == 0){
                char* cwd = exe_pwd(arg_count, inputs);
                printf("%s\n", cwd);
            }
            else if(strcmp(inputs[0], "echo") == 0){
                exe_echo(arg_count, inputs);
            }
            else if(strcmp(inputs[0], "exit") == 0){
                exit_status = 0;
            }
            else if(strcmp(inputs[arg_count-1], "&t") == 0){
                if(arg_count == 1){
                    printf("No command entered\n");
                }
                else{
                    inputs[arg_count-1] = NULL;
                    int i = 0;
                    while(inputs[i] != NULL){
                        strcat(perm_directory, inputs[i]);
                        strcat(perm_directory, " ");
                        i++;
                    }
                    int cmd_flag = 0;
                    //execute external commands using pthread_create()/system()
                    if(strcmp(inputs[0], "date") == 0){
                        cmd_flag = 1;
                    }
                    else if(strcmp(inputs[0], "ls") == 0){
                        cmd_flag = 1;
                    }
                    else if(strcmp(inputs[0], "cat") == 0){
                        cmd_flag = 1;
                    }
                    else if(strcmp(inputs[0], "mkdir") == 0){
                        cmd_flag = 1;
                    }
                    else if(strcmp(inputs[0], "rm") == 0){
                        cmd_flag = 1;
                    }
                    else{
                        cmd_flag = 0;
                        printf("Command: %s Not Found\n", inputs[0]);
                    }
                    if(cmd_flag == 1){
                        pthread_t thread_id;
                        int thread = pthread_create(&thread_id, NULL, (void *(*)(void *))pthread_implement, (void*)perm_directory);
                        if(thread == 0){
                            printf("Thread Created\n");
                        }
                        pthread_join(thread_id, NULL);
                        
                    }
                    strcpy(perm_directory, temp_perm_directory);
                }
                
            }
            else{
                //external commands using fork()/execl()
                if(strcmp(inputs[0], "mkdir") == 0){
                    strcat(perm_directory, "mkdir");
                    if(arg_count == 1){
                        printf("mkdir: missing operand\n");
                    }
                    else if(arg_count == 2){
                        if(strcmp(inputs[1], "-v") == 0){
                            printf("mkdir: missing operand\n");
                        }
                        else if(strcmp(inputs[1],"-pv") == 0){
                            printf("mkdir: missing operand\n");
                        }
                        else{
                            fork_mkdir(perm_directory, "NULL", inputs[1]);
                        }                           
                    }
                    else{
                        if(strcmp(inputs[1], "-v") == 0){
                            for(int i = 2; i < arg_count; i++){
                                fork_mkdir(perm_directory, "-v", inputs[i]);
                            }
                        }
                        else if(strcmp(inputs[1],"-pv") == 0){
                            if(arg_count == 3){
                                fork_mkdir(perm_directory, "-pv", inputs[2]);
                            }
                            else{
                            }
                            
                        }
                        else{
                            for(int i = 1; i < arg_count; i++){
                                fork_mkdir(perm_directory, "NULL", inputs[i]);
                            }
                        }
                    }
                    strcpy(perm_directory, temp_perm_directory);
                }
                else if(strcmp(inputs[0], "ls") == 0){
                    strcat(perm_directory, "ls");
                    if(arg_count > 2){
                        printf("ls: too many arguments\n");
                    }
                    else if(arg_count == 2){
                        if(strcmp(inputs[1],"-a") == 0){
                            fork_ls(perm_directory, inputs[1]);
                        }
                        else if(strcmp(inputs[1],"-i") == 0){
                            fork_ls(perm_directory, inputs[1]);
                        }
                        else{
                            printf("ls: option not supported\n");
                        }
                    }
                    else{
                        fork_ls(perm_directory, NULL);
                    }
                    strcpy(perm_directory, temp_perm_directory);
                }
                else if(strcmp(inputs[0], "cat") == 0){
                    strcat(perm_directory, "cat");
                    if(arg_count > 3){
                        printf("cat: too many arguments\n");
                    }
                    else if(arg_count == 3){
                        if(strcmp(inputs[1], "-n") == 0){
                            fork_cat(perm_directory, inputs[1], inputs[2]);
                        }
                        else if(strcmp(inputs[1], "-b") == 0){
                            fork_cat(perm_directory, inputs[1], inputs[2]);
                        }
                        else{
                            printf("cat: invalid option\n");
                        }
                    }
                    else if(arg_count == 2){
                        fork_cat(perm_directory, inputs[1], NULL);
                    }
                    else{
                        fork_cat(perm_directory, NULL, NULL);
                    }
                    strcpy(perm_directory, temp_perm_directory);
                }
                else if(strcmp(inputs[0], "rm") == 0){
                    strcat(perm_directory, "rm");
                    if(arg_count == 1){
                        printf("rm: invalid input\n");
                    }
                    else if(arg_count == 2){
                        if(strcmp(inputs[1],"-v") == 0){
                            printf("rm: usage: rm -v file_name\n");
                        }
                        else if(strcmp(inputs[1], "-d") == 0){
                            printf("rm: usage: rm -d directory_name\n");
                        }
                        else{
                            fork_rm(perm_directory, inputs[1], "");
                        }
                    }
                    else{
                        if(strcmp(inputs[1],"-v") == 0){
                            for(int i = 2; i < arg_count; i++){
                                fork_rm(perm_directory, inputs[i], "-v");
                            }
                        }
                        else if(strcmp(inputs[1], "-d") == 0){
                            for(int i = 2; i < arg_count; i++){
                                fork_rm(perm_directory, inputs[i], "-d");
                            }
                        }
                        else{
                            for(int i = 1; i < arg_count; i++){
                                fork_rm(perm_directory, inputs[i], "");
                            }
                        }
                    }
                    strcpy(perm_directory, temp_perm_directory);
                }
                else if(strcmp(inputs[0], "date") == 0){
                    strcat(perm_directory, "date");
                    if(arg_count > 3){
                        printf("date: too many arguments\n");
                    }
                    else if(arg_count == 3){
                        if(strcmp(inputs[1],"-r") == 0){
                            if(file_exists(inputs[2])){
                                fork_date(perm_directory, inputs[2], "-r");
                            }
                            else{
                                printf("date: file %s does not exist\n", inputs[2]);
                            }
                        }
                        else{
                            printf("date: invalid input\n");
                        }
                    }
                    else if(arg_count == 2){
                        if(strcmp(inputs[1],"-u") == 0){
                            fork_date(perm_directory, NULL, "-u");
                        }
                        else if(strcmp(inputs[1],"-r") == 0){
                            printf("date: usage: date -r file_name\n");
                        }
                        else{
                            printf("date: invalid input\n");
                        }
                    }
                    else{
                        fork_date(perm_directory, NULL, NULL);
                    }
                    strcpy(perm_directory, temp_perm_directory);
                }
                
                else{
                    printf("Command: %s Not Found\n", inputs[0]);
                }

            }
            
        }
    }
}
