#include "headers.h"

void prmpt(int lewn,char *myh)
{
	char *usr,*path,hostbuffer[256],cwd[256],*base,*loc;
	int i,n;
	gethostname(hostbuffer,sizeof(hostbuffer));
	usr=getenv("USER");
	getcwd(cwd,sizeof(cwd));
	// printf("%lu\n",sizeof(cwd));
	for(i=0;i<lewn;i++)
		if(myh[i]!=cwd[i])
			break;
	printf("<%s@%s:",usr,hostbuffer);
	if(i==lewn)
	{
		printf("~");
		for(;cwd[i]!='\0';i++)
			printf("%c",cwd[i]);
		printf("> ");
	}
	else
		printf("%s> ",cwd);
}