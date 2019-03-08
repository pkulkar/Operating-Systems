#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>


int main(int argc, char *argv[])
{

pid_t pid;
int height=atoi(argv[1]);
int child=atoi(argv[2]);
	printf( "%*s", 4*height,"");
	printf("(%d): Process starting\n",getpid());
	printf( "%*s", 4*height,"");
	printf("(%d): Parent's id= (%d)\n",getpid(),getppid());
	printf( "%*s", 4*height," ");	
	printf("(%d): Height in the tree = %d\n",getpid(),height);
	printf( "%*s", 4*height,"");	
	printf("(%d): Creating %d children at height %d\n",getpid(),child,height);
	printf( "%*s", 4*height,"");	
	if(height>1){
	for(int i=0;i<child;i++){
	pid=fork();
			if(pid < 0) {
				fprintf(stderr,"FORK FAILED");
				exit(1);
			}
			else if(pid==0) {
				int exec;
				char children[2],heightt[2];
				sprintf(children,"%d",child);
				sprintf(heightt,"%d",height-1);
				exec=execlp("./process","./process",heightt,children,(char *) NULL);				
				if(exec == -1) {
					fprintf(stderr, "Exec FAILED!");
				}
				exit(0);
			}		
		}
		while(child>0){
				wait(NULL);
				child--;
		}
		printf( "%*s", 4*height,"");
		printf("(%d):Terminating at height %d\n",getpid(),height);
		
		}
else
{
printf( "%*s", 4*height,"");
printf("(%d):Terminating at height %d\n",getpid(),height);
}
return 0;
}

