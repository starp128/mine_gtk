#include "mine.h"

uint8_t m[10][10]={{0,0,0,0,0,0,0,0,0,0},
		   {0,0,1,1,1,1,0,0,0,0},
		   {0,1,0,0,0,0,1,0,0,0},
		   {0,1,0,0,0,0,0,0,0,0},
		   {0,1,0,0,0,0,0,1,0,0},
		   {0,0,0,0,0,0,0,0,0,0},
		   {0,0,1,1,1,1,1,0,0,0},
		   {0,0,1,1,1,1,1,0,0,0},
		   {0,0,1,1,1,1,1,0,0,0},
		   {0,0,1,1,1,1,1,0,0,0}
};

  


enum mine_status s=clicked_on_digged;

void print_map()
{
	uint8_t i,j;
	printf("mine_map\n");
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			printf("%2d",mine_map[i][j]);
		}
		printf("\n");
	}
	printf("\n=====mine_number======\n");
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			printf("%2d",mine_number[i][j]);
		}
		printf("\n");
	}
	printf("\n=====should not see========\n");
}
void print_result()

{
	uint8_t i,j;
	printf("\n");
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			if(mine_digged[i][j])
			{
				printf("%2d",mine_number[i][j]);
			}
			else
			{
				if(mine_marked[i][j])
					printf("%2s","M");
				else
				{
					printf("%2s","*");
				}

			}
		}
		printf("\n");
	}
 
}
void print_status()
{
	printf("<----total %d marked %d---->\n",mine_count,mine_count_marked);
}

int main()
{
	uint32_t i,j;
	char cmd_buf[20];
	char cmd;
	char dum;

	/* uint8_t* p; */
	/* p=(uint8_t*)&m[0][0];//we the address of the 2-dem arry */
  
	/* row=10; */
	/* col=10; */
	/* init_mine_map(p); */
	/* //init_mine_map_with_default(); */
  

	/* initial with 5 bombs exist */
	init_mine_map_rand(1);
	print_map();
	print_status();
	print_result();
  
	printf("select by modey\n inter 'd' for dig\n inter 'm' for mark\n");
	while(1)
	{
		printf("slect mode('m'or'd') ->");
		scanf("%c",&cmd);
		while( (cmd!='d')&&(cmd!='m'))
		{
			scanf("%c",&cmd);
		}
	 
		if(cmd=='d')
		{
			printf("select the point(n,m)->");
			scanf("%d,%d",&i,&j);
			// printf("dig %d,%d \n",i,j);

			s=dig((uint8_t)i,(uint8_t)j);

			switch (s)
			{
			case clicked_on_digged:
				printf("nothing happen\n");
				break;
			case clicked_on_gameover:
				printf("game over\n");
				break;
			case clicked_on_empty:
				print_result();
				break;
			case clicked_on_number:
				print_result();
				break;
			default:
				printf("error\n");
			}
		}
		if(cmd=='m')
		{
			printf("select the point(m,n)->");
			scanf("%d,%d",&i,&j);
			//printf("mark %d,%d \n",i,j);

			s=mark((uint8_t)i,(uint8_t)j);

			switch (s)
			{
			case clicked_on_nothing:
				printf("nothing happen\n");
				break;
			case clicked_on_unmark:
				printf("unmark\n");
				break;
			case clicked_on_error:
				printf("error\n");
				break;
			case clicked_on_secceed:
				printf("succeed\n");
				getchar();
				return;
			case clicked_on_mark:
				printf("mark\n");
				break;
			default:
				printf("error\n");
			}
		}
	}

	return 0;
}
