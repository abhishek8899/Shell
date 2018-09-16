#include "headers.h"

char *root,myhome[256];
int ln,ind,chd[10000];
volatile sig_atomic_t signalis=0;

void sigintHandler() {
	signalis=1;
}

void clockti(char ** arg)
{
	char tm[256],dt[256];
	int dew;
	if(arg[1]==NULL)
		sscanf("1","%d",&dew);
	else if(arg[2]==NULL)
		sscanf(arg[1],"%d",&dew);	
	else
		sscanf(arg[2],"%d",&dew);	
	while(signalis==0)
	{
		FILE *kj=fopen("/proc/driver/rtc","r");
		if(kj==NULL)
		{
			perror("Error: ");
			exit(-1);
		}
		for(int g=0;g<3;g++)
			fscanf(kj,"%s",tm);
		for(int g=0;g<3;g++)
			fscanf(kj,"%s",dt);
		printf("%s %s\n",dt,tm);
		sleep(dew);
	}
	signalis=0;
}

void cmd()
{
	signal(SIGINT,sigintHandler);
    char *cmds,*ar,*dir;
	while(1)
	{
		char* com[256];
		int i=0,and=0,smc=0;
		for(i=0;i<256;i++)
			com[i]=NULL;
		for(i=0;i<ind;i++)
		{
			int status;
			if (waitpid(chd[i],&status,WNOHANG)!=0)
			{
				printf("%d exited successfully\n",chd[i]);
				ind--;
				for(int j=i;j<ind;j++)
					chd[j]=chd[j+1];
				i--;
			}
		}
		prmpt(ln,myhome);
		cmds=takeinput();
		ar=strtok(cmds," \t\n");
		cmds=NULL;
		i=0;
		while(ar!=NULL)
		{
			com[i++]=ar;
			if(strcmp(ar,"&")==0)
			{
				and=1;
				com[--i]=NULL;
				break;
			}
			if(strcmp(ar,";")==0)
				smc=1;
			ar=strtok(NULL," \t\n");
		}
		ar=NULL;
		char *arg[i+1];
		int j;
		for(j=0;j<i;j++)
			arg[j]=com[j];
		arg[j]=NULL;
		
		if(i==0)
			continue;

		if(strcmp(com[0],"exit")==0)
			break;

		pid_t pid,wpid;
		int state;
		
		if(smc==1)
		{
			if (execvp(arg[0],arg) == -1)
				perror("lsh");
			continue;
		}

		if(and==0 && strcmp(com[0],"remindme")!=0)
		{

			if(strcmp(com[0],"pwd")==0)
			{
				pwdi();
				continue;
			}

			if(strcmp(com[0],"pinfo")==0)
			{
				p_inf(arg,ln,myhome);
				continue;
			}

			if(strcmp(com[0],"cd")==0)
			{
				if(i==1)
				{
					chdir(myhome);
					continue;
				}
				cdd(arg,myhome);
				continue;
			}

			if(strcmp(com[0],"echo")==0)
			{
				echo(arg,i);
				continue;
			}

			if(strcmp(com[0],"clock")==0)
			{
				clockti(arg);
				continue;
			}

			if(strcmp(com[0],"ls")==0)
			{
				if(i==1)
				{
					ls(".","n");
					continue;
				}
				if(i==2)
				{
					if(strcmp(com[1],"-a")==0 || strcmp(com[1],"-l")==0 || strcmp(com[1],"-la")==0 || strcmp(com[1],"-al")==0)
						ls(".",com[1]);
					else
						ls(com[1],"n");
					continue;
				}
				if(i==3 && ((strcmp(com[1],"-a")==0 && strcmp(com[2],"-l")==0) || (strcmp(com[2],"-a")==0 && strcmp(com[1],"-l")==0)))
				{
					ls(".","-la");
					continue;
				}
				if(i==4 && ((strcmp(com[1],"-a")==0 && strcmp(com[2],"-l")==0) || (strcmp(com[2],"-a")==0 && strcmp(com[1],"-l")==0)))
				{
					ls(com[3],"-la");
					continue;
				}
				ls(com[2],com[1]);
				continue;
			}
			
			pid_t pid, wpid;
  			int status;

			pid = fork();
			if (pid == 0)
			{
				if (execvp(arg[0],arg) == -1)
				{
					//printf("deewc\n");
			    	perror("lsh");
			    }
			    exit(EXIT_FAILURE);
			}
			else if (pid < 0)
			{
			  	// Error forking
			    perror("lsh");
			}
			else
			{
			    do
			    {
			      wpid = waitpid(pid, &status, WUNTRACED);
			    } 
			    while (!WIFEXITED(status) && !WIFSIGNALED(status));
			}
		}
		else
		{
			pid_t pid, wpid;
  			int status;
  			//printf("ewflwepprgfunrougoungeuoge\n");
			pid = fork();
			if (pid == 0)
			{
				// Child prcs
				if(strcmp(com[0],"remindme")==0)
				{
					int ed;
					sscanf(com[1],"%d",&ed);
					sleep(ed);
					printf("\n\nReminder: ");
					int lkre=2;
					while(arg[lkre]!=NULL)
						printf("%s ",arg[lkre++]);
					printf("\n");
					_exit(0);
				}
				if (execvp(arg[0],arg) == -1)
				{
			    	perror("lsh");
			    }
			    exit(EXIT_FAILURE);
			}
			else if (pid < 0)
			{
			  	// Error forking
			    perror("lsh");
			}
			else
			{
				chd[ind++]=pid;
				printf("Started process %d successfully\n",chd[ind-1]);

			}
		}
	}
}

int main()
{
	root=getenv("PWD");
	char* dir=malloc(sizeof(char)*256);;
	getcwd(dir,256);
	ln=strlen(dir);
	for(int i=0;i<ln;i++)
		myhome[i]=dir[i];
	free(dir);
	cmd();
	return 0;
}
