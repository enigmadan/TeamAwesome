#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>


//forks and execvps
    //our goto function to run a command
void mainline(char **ball) {
    int status;
    int cpid = fork();
    if(cpid == -1) //fork failed
        printf("fork failed. ...now what...?");
    else if (cpid == 0) {//this is the child process
        execvp(ball[0],ball);//run command
        perror("");//print possible error
        exit(0);//if something failed exit child anyway
    }
    else { //this is the parent process
        waitpid(cpid, &status, 0);//wait for specific child to finish
    }
}

//our piping algorithm
    //if a "|" is found, we get sent here
void piedPiper(char **ball){
    char *theory[10];//a theory of strings
    int i = 0;
    //iterate through ball until we get to the "|"
    while(strcmp(ball[i],"|") !=0){
        i++;
    }
    //erase the "|"
    ball[i] = NULL;
    //increment
    i++;
    int j = 0;
    //move the remainder of ball to theory
    while(ball[i] != NULL){
        theory[j] = ball[i];
        //remove string from ball
        ball[i] = NULL;
        //increment
        i++;
        j++;
    }
    //finish theory off (just in case)
    theory[j] = NULL;
    //begin actual piping stuff
    int fid[2];
    pipe(fid);
    //FORK!
    int cpid = fork();
    if(cpid==0){
        //just a child
        //close std out
        close(1);
        //close one end of the pipe
        close(fid[0]);
        //connect other end of the pipe to output place
        dup2(fid[1], 1);
        //execute stuff
        execvp(ball[0], ball);
        perror("");
    }else{
        // else we are in the parent so we fork again in order 
        // to create a sibling to play with
        int cpid2 = fork();
        if(cpid2 == 0){//child
            //close std in
            close(0);
            //close our other opening
            close(fid[1]);
            //open our end of the pipe
            dup2(fid[0], 0);
            //execute string theory and stuff
            execvp(theory[0], theory);
            perror("");
        }else{
            //wait for children to finish
            wait(&cpid);
            //set stuff right again
            dup2(0,fid[0]);
            dup2(1,fid[1]);
            wait(&cpid2);
        }
    }
}

//our redirect function
//does redirection including ">&"
void redirect(char **ball){
    int i = 0;
    int inbool = 0;
    //while loop to check what's happenin'
    while(ball[i] != NULL){
        //boolean values...
        int outbool = 0;
        int errbool = 0;
        int combobool = 0;
        //a rope to hold a string
        char *rope[1];
        //if in then
        if(strcmp(ball[i],"<") ==0){
            //in is true
            inbool = 1;
            //overwrite the "<" with whatever we want to bring in
            ball[i] = ball[i+1];
            //increment
            i++;
            //newly empty space
            ball[i] = NULL;
            //increment
            i++;
        //if output (normal, not error) then
        }else if(strcmp(ball[i],">") ==0 || strcmp(ball[i],"1>") ==0){
            //out is true
            outbool = 1;
            //remove characters
            ball[i] = NULL;
            //increment
            i++;
            //set rope
            rope[0]= ball[i];
        //if there's a want for an error we can do that too!
        }else if(strcmp(ball[i],"2>") ==0){
            //error is true
            errbool = 1;
            //get rid of the "2>"
            ball[i] = NULL;
            //increment
            i++;
            //put string in the rope
            rope[0]= ball[i];
        //normal output ~and~ error? no problem!
        }else if(strcmp(ball[i],">&") ==0){
            //combo is true
            combobool = 1;
            //get rid of ">&"
            ball[i] = NULL;
            //increment
            i++;
            //put a string into a rope
            rope[0]= ball[i];
        //if none of the above
        }else{
            //check again!
            i++;
        }
        //if out
        if(outbool == 1){
            //OFKR!
            int cpid = fork();
            //no child left behind
            if(cpid == -1) //fork failed
                printf("fork failed. ...now what...?");
            //such a child
            else if (cpid == 0) {
                //open string in the rope write only and 
                //create it if it doesn't exist
                //also give us r/w priveleges
                int out = open(rope[0], O_WRONLY|O_CREAT, 0666);
                //redirect output from stdout (at 1) to out 
                //which represents our file
                dup2(out,1);
                //execute shtuff
                execvp(ball[0],ball);
                perror("");
                //close our out (which describes our file quite well)
                close(out);
                //exit child in case something didn't work
                exit(0);
            }else { //this is the parent process
                //wait for your children
                wait(&cpid);
                //inbool is now false
                inbool = 0;
            }
        }
        //You really want an error?  okay then... here you go
        else if(errbool == 1){
            //!KROF
            int cpid = fork();
            //no failure! Only good learning experiences
            if(cpid == -1)
                printf("fork failed. ...now what...?");
            //Mama?
            else if (cpid == 0) {
                //err is very complimentary in its description 
                //of the filename in rope[0]
                int err = open(rope[0], O_WRONLY|O_CREAT, 0600);
                //open err in the err stream
                dup2(err,2);
                //do dat execute thing
                execvp(ball[0],ball);
                //close 'err up captain!
                close(err);
            }else { //this is the parent process (yeah... no jokes here)
                //wait for child
                wait(&cpid);
                //inbool is no longer true
                inbool = 0;
            }
        }
        //Porque no los dos?
        else if(combobool == 1){
            //KFRO!
            int cpid = fork();
            //failure is not allowed!
            if(cpid == -1) //fork failed
                printf("fork failed. ...now what...?");
            //no children allowed... but we'll 
            //make an exception for you
            else if (cpid == 0) {
                //open file for both out and err
                int out = open(rope[0], O_WRONLY|O_CREAT, 0666);
                int err = open(rope[0], O_WRONLY|O_CREAT, 0600);
                //set up out in std out and err in std err
                dup2(out,1);
                dup2(err,2);
                //just do it
                execvp(ball[0],ball);
                //close those descriptors
                close(out);
                close(err);
                //parenting 101, don't leave your child unattended
            }else{
                //wait patiently for your children
                wait(&cpid);
                //inbool is false again (if it wasn't already)
                inbool = 0;
            }
        }
        else{
            //nothing to see here...
        }
    }
    //if in, the whole in, and nothing but the in
    if(inbool == 1){
        //we have a function that does this sort of thing
        mainline(ball);
    }
}

//finally made it to main...
int main(int argc, char *argv[]){
    //getting the current working directory
    char *cwd = NULL;
    //a pointer
    char *ptr;
    //also a ptr
    char *ptr_too = malloc(40*sizeof(char));
    //a string
    char *foldertmp;
    //and another string
    char *folder;
    //get cwd and current folder name
    ptr = getcwd(cwd,(size_t)pathconf(".", _PC_PATH_MAX));//get the new cwd
    //copy that folder name into the also pointer
    strcpy(ptr_too,ptr);
    //do fancy things to get the current folder name
    foldertmp = strtok(ptr_too, "/");
    while(foldertmp != NULL){
        folder = foldertmp;
        foldertmp = strtok(NULL, "/");
    }
    //our big while loop
    while(1) {
        //one line
        char *line = NULL;
        // what else would we name our string?
        char *yarn = NULL;
        //a length
        size_t len = 0;
        //something we needed but don't really understand...
        ssize_t read;
        //a ball of string(s)!
        char *ball[40];
        ball[0] = NULL;
        //print out our shell command instantiator
        printf("%s[tosh %s]$%s ", "\x1B[36m", folder, "\x1B[0m");
        //increment through a typed line to check for things
        if((read = getline(&line, &len, stdin)) != -1){
            //not an int I swear!
            int pipeTrue = 0;
            //neither is this
            int iored = 0;
            line = strtok(line, "\n"); //remove the newline character
            yarn = strtok(line, " "); //tokenize by spaces
            //declare incrementor h
            int h = 0;
            //until we get to the end of the input or 19 arguments, whichever comes first...
            while(yarn!=NULL && h<19){
                //if there's a pipe
                if(strcmp(yarn,"|")==0){
                    //then it's true
                    pipeTrue = 1;
                }
                //if there is some io redirection wanting to happen, 
                //well, then you've come to the right place
                if(strcmp(yarn,"<")==0||strcmp(yarn,">")==0||
                    strcmp(yarn,"1>")==0||
                    strcmp(yarn,"2>")==0||
                    strcmp(yarn,">&")==0){
                    //this has gotta be true
                    iored = 1;
                }
                //lets add some string to our ball
                ball[h] = yarn;
                //get the next argument
                yarn = strtok(NULL, " ");
                //increment our incrementor h
                h++;
            }
            //set the next part of ball to NULL just in case...
            ball[h] = NULL;
            //if we don't have too many arguments
            if(h<19){
                //and if we aren't piping or redirecting
                if(pipeTrue == 0 && iored == 0){
                    //if ball is nuthin', just loop
                    if(ball[0]==NULL){
                    //if ball is exit, free and exit    
                    }else if(strcmp(ball[0],"exit")==0){
                        if(read)
                            close(read);
                        if(line)
                            free(line);
                        if(yarn)
                            free(yarn);
                        if(cwd)
                            free(cwd);
                        if(ptr)
                            free(ptr);
                        if(ptr_too)
                            free(ptr_too);
                        return 0;
                    //if ball is cd, well try to change directory
                    }else if(strcmp(ball[0],"cd")==0){
                        if (chdir(ball[1]) != 0) //if chdir failed,
                            perror(""); //print the error
                        else //else chdir worked,
                            ptr = getcwd(cwd,(size_t)pathconf(".", _PC_PATH_MAX));//get the new cwd
                        //copy from one pointer to an also pointer
                        strcpy(ptr_too,ptr);
                        //get the new current folder
                        foldertmp = strtok(ptr_too, "/");
                        while(foldertmp != NULL){
                            folder = foldertmp;
                            foldertmp = strtok(NULL, "/");
                        }
                    //don't recognize a command?  pipe it through!
                    }else{
                        mainline(ball);
                    }
                //if we wanna pipe, go on up
                }else if(pipeTrue == 1){
                    piedPiper(ball);
                //if we wanna redirect, well,
                //that's up top too
                }else if(iored == 1){
                    redirect(ball);
                }else{
                    //should never get here...
                }
            //more than 19 arguments?
            }else{
                printf("Too many arguments\n");
            }

        }
        //free some stuff before looping
        if(read)
            close(read);
        if(line)
            free(line);
        if(yarn)
            free(yarn);
    }
    //we should never get here, but if we did...
    if(cwd)
        free(cwd);
    if(ptr)
        free(ptr);
    if(ptr_too)
        free(ptr_too);
    return 0;
}
