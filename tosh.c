#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>



int ls(void){
    //struct stat *buf;
    //char *path;
  DIR           *d;
  struct dirent *dir;
  d = opendir(".");
  if (d)
  {
    while ((dir = readdir(d)) != NULL)
    {
        //stat(dir->d_name,buf);
        //printf("%s\n", buf->st_mode);
        printf("%s\n", dir->d_name);
    }

    closedir(d);
  }

  return(0);
}
//void cd();
//void cat();
//void pwd();
//void find();
//void wc();
//void ps();
//void runProgram();
//void diff();
//char *currFolder();

int main(int argc, char *argv[]){
    //getting the current working directory
    char *cwd = NULL;
    //int pid; //<---unused
    int status;

    char *ptr;
    char *ptr_too = malloc(sizeof(*ptr_too));
    char *foldertmp;
    char *folder;

    //get cwd and current folder name
    ptr = getcwd(cwd,(size_t)pathconf(".", _PC_PATH_MAX));//get the new cwd
    strcpy(ptr_too,ptr);
    foldertmp = strtok(ptr_too, "/");
    while(foldertmp != NULL){
        folder = foldertmp;
        foldertmp = strtok(NULL, "/");
    }

    /* // Display each command-line argument.
    printf( "\nCommand-line arguments:\n" );
    for( count = 0; count < argc; count++ ){
        printf( "  argv[%d]   %s\n", count, argv[count] );
    }//*/

    char *line = NULL;
    char *yarn = NULL;
    size_t len = 0;
    ssize_t read;

    while(1) {
        printf("%s[tosh %s]$%s", "\x1B[36m", folder, "\x1B[0m");
        if((read = getline(&line, &len, stdin)) != -1){
            line = strtok(line, "\n"); //remove the newline character
            yarn = strtok(line, " "); //tokenize by spaces

            while(yarn!=NULL){
                printf(">%s<\n", yarn); //printing current token

                //exit the terminal
                if(strcmp(yarn,"exit")==0){
                    printf("Let me out!!!!\n");
                    return 0;
                }

                //change the working directory
                else if(strcmp(yarn,"cd")==0){
                    yarn = strtok(NULL, " ");//move to next token
                    if (chdir(yarn) != 0) //if chdir failed,
                        perror(""); //print the error
                    else //else chdir worked,
                        ptr = getcwd(cwd,(size_t)pathconf(".", _PC_PATH_MAX));//get the new cwd
                        strcpy(ptr_too,ptr);
                    foldertmp = strtok(ptr_too, "/");
                    while(foldertmp != NULL){
                        folder = foldertmp;
                        foldertmp = strtok(NULL, "/");
                    }

                    break;
                }

                //print the working directory
                else if(strcmp(yarn,"pwd")==0){
                    printf("pwd?\n");
                    int cpid = fork();
                    if(cpid == -1) //fork failed
                        printf("fork failed. ...now what...?");
                    else if (cpid == 0) {//this is the child process
                        printf("%s\n", ptr);
                        //printf("returning %d\n", cpid);
                        return cpid;
                    }
                    else { //this is the parent process
                        /*pid = */waitpid(cpid, &status, 0);

                        //printf("cpid=%d\n",cpid);
                        //printf("just recieved %d\n", pid);
                    }
                    break;
                }

                //list files in current directory
                else if(strcmp(yarn,"ls")==0){
                    ls();
                    //printf("files in this directory: i dont know... ask again later...\n");
                    break;
                }

                //else command not found
                else {
                    printf("%s: Command not found.\n", yarn);
                    break;
                }
                printf("%s\n", yarn);
                yarn = strtok(NULL, " ");
            }

        }

    }

    if (line)
        free(line);
    return 0;
}
