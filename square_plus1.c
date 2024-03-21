#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <ctype.h>
#include <stdbool.h>

// used this for pipe things: https://www.geeksforgeeks.org/c-program-demonstrate-fork-and-pipe/

int main(){

    int parentToChild[2];
    int childToParent[2];
    int childToChild[2];

    pipe(parentToChild);
    pipe(childToChild);
    pipe(childToParent);

    pid_t pid1 = fork();

    if (pid1 == 0) {

        close(parentToChild[1]);
        close(childToChild[0]);
        close(childToParent[0]);
        close(childToParent[1]);
        
        int number = 0;

        if (read (parentToChild[0], &number, sizeof(int)) > 0) {

            number = number * number;
            write(childToChild[1], &number, sizeof(int));

        }

        close(parentToChild[0]);
        close(childToChild[1]);

        exit(EXIT_SUCCESS);
    }

    pid_t pid2 = fork();

    if (pid2 == 0) { 

        close(childToChild[1]);
        close(childToParent[0]);
        close(parentToChild[0]);
        close(parentToChild[1]);

        int number = 0;
        if (read(childToChild[0], &number, sizeof(int)) > 0) {

            number = number + 1;

            write(childToParent[1], &number, sizeof(int)); 

        }

        close(childToChild[0]);
        close(childToParent[1]);

        exit(EXIT_SUCCESS);

    }

    close(parentToChild[0]);
    close(childToChild[0]);
    close(childToChild[1]);
    close(childToParent[1]);
    
    int number = 0;

    while (scanf("%d", &number) != EOF) {

        write(parentToChild[1], &number, sizeof(int)); 
        read(childToParent[0], &number, sizeof(int));  
        printf("%d\n", number); 

    }


    close(parentToChild[1]);
    close(childToParent[0]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 0;


}