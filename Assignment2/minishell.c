#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/stat.h>

void sigintHandler(int sig_num) ;


int minish(void)
{
char command[50],stringforspaces[20][20],file[20];
int a,i,j,count,background,status;
pid_t pid;
int exec;
FILE *fp;
printf("minish>\a");
fgets(command,50,stdin);	
count=0;
background=0;
j=0;
int ret;
ret=strncmp(command,"exit", 4);
if(ret==0)
kill(0, SIGTERM);

    for(i=0;i<=(strlen(command));i++)
    {
       
        if(command[i]==' '|| command[i]=='\0' || command[i]=='\n' )
        {	
		
            stringforspaces[count][j]='\0';
            count++; 
            j=0;  
        }
	else
        {
		if(command[i]=='&')
		{
		background++;
		stringforspaces[count][j]='\0';
		count++;
		j=0;
		}
	   else{
		
            stringforspaces[count][j]=command[i];
		
            j++;}
        }
	
	
	
    }


ret=strncmp(command,"kill", 4);
if(ret==0){
int qwe;
qwe=kill(atoi(&stringforspaces[1]), SIGKILL);
if(qwe=-1)
printf("No Such process running\n");}
pid=fork();
	if(pid<0)
	 {
	 	printf("Fork Error");
	 }	
	if(pid==0)
	{
		if((background==0 && count==2) || (background>0 && count==4))
			{
			exec=execlp(stringforspaces[0], stringforspaces[0], (char *)NULL);
			if(exec==-1){printf("No such command\n");}
			}
		if((background==0 && count==3) || (background>0 && count==5))
			{
			exec=execlp(stringforspaces[0],stringforspaces[0],stringforspaces[1], (char *)NULL); 
			if(exec==-1){printf("No such command\n");}
			
			}
		if((background==0 && count==4) || (background>0 && count==6))
			{
			exec=execlp(stringforspaces[0],stringforspaces[0],stringforspaces[1],stringforspaces[2], (char *)NULL); 
			if(exec==-1){printf("No such command\n");}
						
			}
		if((background==0 && count==5) || (background>0 && count==7))
			{
			exec=execlp(stringforspaces[0],stringforspaces[0],stringforspaces[1],stringforspaces[2],stringforspaces[3], (char *)NULL); 
			if(exec==-1){printf("No such command\n");}
			
			}
		if((background==0 && count==6) || (background>0 && count==8))
			{
			exec=execlp(stringforspaces[0],stringforspaces[0],stringforspaces[1],stringforspaces[2],stringforspaces[3],stringforspaces[4], (char *)NULL); 
			if(exec==-1){printf("No such command\n");}
			
			}
		exit(0);
        }
	
	if(background==0){
	int status;
	int wait;
	signal(SIGCHLD, SIG_DFL);
	wait=waitpid(pid, &status, 0);
	if(wait==-1){printf("Error");}
	
	}
	else{
	background--;
	signal(SIGCHLD, SIG_IGN);
	}
	
return 0;	
}



void sigintHandler(int sig_num) 
{ 
    
    signal(SIGINT, sigintHandler); 
    printf("\n Cannot be terminated using Ctrl+C \n"); 
    fflush(stdout);
    minish(); 
} 
  

int main()
{
	signal(SIGINT, sigintHandler); 
	while(1)
	{
		minish();
	}
return 0;
}
