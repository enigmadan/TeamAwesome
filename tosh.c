#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>




//void cd();
//void pwd();
//void find();
//void wc();
//void ps();
//void runProgram();
//void diff();

int main(int argc, char *argv[]){
    //getting the current working directory
    char *cwd = NULL; //malloc(sizeof(*cwd));
    //int pid; //<---unused
    int status;

    char *ptr;// malloc(sizeof(*ptr));
    char *ptr_too = malloc(40*sizeof(char));
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
printf( " argv[%d] %s\n", count, argv[count] );
}//*/

    char *line = NULL;
    char *yarn = NULL;
    size_t len = 0;
    ssize_t read;
    char *ball[40];
    ball[0] = NULL;

    while(1) {
        printf("%s[tosh %s]$%s ", "\x1B[36m", folder, "\x1B[0m");
        if((read = getline(&line, &len, stdin)) != -1){
            int pipe = 0;
            int iored = 0;
            line = strtok(line, "\n"); //remove the newline character
            yarn = strtok(line, " "); //tokenize by spaces
            int h = 0;
            while(yarn!=NULL && h<19){
                if(strcmp(yarn,"|")==0){
                    pipe = 1;
                }
                if(strcmp(yarn,"<")==0||strcmp(yarn,">")==0){
                    iored = 1;
                }
                //printf("%d",h);
                ball[h] = yarn;
                //printf(">%s<\n", yarn); //printing current token
                //printf("%s\n", ball[h]);
                yarn = strtok(NULL, " ");
                h++;
            }
            ball[h] = NULL;
            if(h<19){
                if(pipe == 0 && iored == 0){
                    if(ball[0]==NULL){
                        //printf("enter something please\n");
                    }else if(strcmp(ball[0],"exit")==0){
                        return 0;
                    }else if(strcmp(ball[0],"cd")==0){
                        if (chdir(ball[1]) != 0) //if chdir failed,
                            perror(""); //print the error
                        else //else chdir worked,
                            ptr = getcwd(cwd,(size_t)pathconf(".", _PC_PATH_MAX));//get the new cwd
                            strcpy(ptr_too,ptr);
                        foldertmp = strtok(ptr_too, "/");
                        while(foldertmp != NULL){
                            folder = foldertmp;
                            foldertmp = strtok(NULL, "/");
                        }
                    }else{
                        int cpid = fork();
                        if(cpid == -1) //fork failed
                            printf("fork failed. ...now what...?");
                        else if (cpid == 0) {//this is the child process
                            execvp(ball[0],ball);
                            perror("");
                            //printf("Unknown command.\n");
                            exit(0);
                            //printf("returning %d\n", cpid);
                        }
                        else { //this is the parent process
                            //pid =
                            waitpid(cpid, &status, 0);
                        }
                        //return cpid;
                    }
                }else if(pipe == 1){
                    printf("Pipe?\n");
                }else if(iored == 1){
                    printf("IO redirect\n");
                }else{

                }

                /*
printf("%d\n", h);
int i=0;
for(i=0;i<h;i++){
printf("%s\n", ball[i]);
printf("%d\n", i);
}//*/
            }else{
                printf("Too many arguments\n");
            }

        }

    }

    if (line)
        free(line);
    return 0;
}



      /*
//exit the terminal
if(strcmp(yarn,"exit")==0){
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
int cpid = fork();
if(cpid == -1) //fork failed
printf("fork failed. ...now what...?");
else if (cpid == 0) {//this is the child process
printf("%s\n", ptr);
//printf("returning %d\n", cpid);
return cpid;
}
else { //this is the parent process
//pid =
waitpid(cpid, &status, 0);

//printf("cpid=%d\n",cpid);
//printf("just recieved %d\n", pid);
}
break;
}

//list files in current directory
else if(strcmp(yarn,"ls")==0){
//ls();
break;
}
else if(strcmp(yarn,"cat")){
//cat(argc,argv);
break;
}

//else command not found
else {
printf("%s: Command not found.\n", yarn);
break;
}*/
