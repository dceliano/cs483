#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(){
	//Ex01 - Fork
	pid_t ret_val;
	int x = 0;

	ret_val = fork();
	if (ret_val != 0){
		//fork returned a nonzero value (typically pid of child)
		//in parent runs this code
		pid_t pid_term = wait(&x);
		printf("In parent...pid of terminated process is %d\n", pid_term);
		printf("My process ID: %d\n", getpid()); //print my pid
		printf("My parent's id: %d\n", getppid()); //print parent's pid
	}
	else{
		//fork returned 0 - child runs this code
		printf("In child...\n");
		printf("My process ID: %d\n", getpid());
		printf("My parent's ID: %d\n", getppid());
	}
	printf("You should see this message two times - once from the parent and once from child prior to termination.\n");
	return 0;
}
