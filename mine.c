#include<stdio.h>
#include<stdint.h>
#include"mine.h"


const uint8_t map[8][8]={{0,0,0,0,0,0,0,0},
						 {0,0,1,0,0,0,0,0},
						 {0,0,0,0,0,0,0,0},
						 {0,1,0,0,0,0,0,0},
						 {0,0,0,0,0,0,0,0},
						 {0,0,0,0,1,0,0,0},
						 {0,0,0,0,0,0,0,0},
						 {0,0,0,0,0,0,1,0}};

const uint8_t map2[8][8]={{0,0,0,0,0,0,0,0},
						 {0,0,1,1,1,1,0,0},
						 {0,1,0,0,0,0,1,0},
						 {0,1,0,0,0,0,0,0},
						 {0,1,0,0,0,0,0,1},
						 {0,0,0,0,0,0,0,0},
						 {0,0,1,1,1,1,1,0},
						 {0,0,0,0,0,0,0,0}};

uint8_t mine_count=0;
uint8_t mine_count_marked=0;

uint8_t row=8,col=8;
uint8_t mine_map[MAX_RAW][MAX_COL];
uint8_t mine_number[MAX_RAW][MAX_COL];
uint8_t mine_digged[MAX_RAW][MAX_COL];
uint8_t mine_marked[MAX_RAW][MAX_COL];

void ifnomine(uint8_t i,uint8_t j)
{
  uint8_t m,n;

  mine_digged[i][j]=1;//use this flag in case of dead iteration
  for(m=((i==0)?i:i-1);m<=((i==row-1)?i:i+1);m++)
	for(n=((j==0)?j:j-1);n<=((j==col-1)?j:j+1);n++)
	  {
		if(mine_digged[m][n])
		  continue;
		else
		  {
			if(mine_number[m][n])
			  {
				mine_digged[m][n]=1;
				continue;
			  }
			else
			  {
				ifnomine(m,n);
			  }
		  }
	  }
}

//event we accept,dig event
enum mine_status dig(uint8_t i,uint8_t j)
{
  if(mine_digged[i][j]||mine_marked[i][j])
	return clicked_on_digged; //nothing happen
  else if(mine_number[i][j]==9)
	return clicked_on_gameover; //dig a bomb
  else if(mine_number[i][j]==0)
	{
	  ifnomine(i,j);
	  return clicked_on_empty;//you dig one step
	}
  else if(1<=mine_number[i][j]<=8)
	{
	  mine_digged[i][j]=1;
	  return clicked_on_number;
	}
  else
	{
	  return clicked_on_error;//error.should not run here
	}
}

uint8_t ifsucceed()
{
  uint8_t i,j;
  for(i=0;i<row;i++)
	for(j=0;j<col;j++)
	  {
		if(mine_map[i][j]!=mine_marked[i][j])
		  return 0;
	  }

  return 1;
}
enum mine_status mark(uint8_t i,uint8_t j)
{
 if(mine_digged[i][j])
   return clicked_on_nothing; //nothing happen
 else if(mine_marked[i][j])
   {
	 mine_marked[i][j]=0; //dig a bomb
	 mine_count_marked--;
	 return clicked_on_unmark;
   }
 else if(mine_marked[i][j]==0)
   {
	 mine_marked[i][j]=1;
	 mine_count_marked++;
	 if(ifsucceed())
	   return clicked_on_secceed;
	 return clicked_on_mark;
   }
 else 
   return clicked_on_error;
}


void init_mine_map_with_default()
{
  uint8_t i,j;
  uint8_t m,n;
  row=8;
  col=8;

  for(i=0;i<row;i++)
	  for(j=0;j<col;j++)
		{
		  mine_map[i][j]=map[i][j];
		  mine_digged[i][j]=0;
		  mine_number[i][j]=0;
		  mine_marked[i][j]=0;
		}
 
 for(i=0;i<row;i++)
	  for(j=0;j<col;j++)
		{
		  if(mine_map[i][j])
			{
			  mine_number[i][j]=9;
			  continue;
			}
		  for(m=((i==0)?i:i-1);m<=((i==row-1)?i:i+1);m++)
		  	for(n=((j==0)?j:j-1);n<=((j==col-1)?j:j+1);n++)
		  	  {
		  		mine_number[i][j]+=mine_map[m][n];
		  	  }
 
		}
}
/* void init_mine_map(uint8_t** map) */
/* { */
/*   uint8_t i,j; */
/*   uint8_t m,n; */

/*   for(i=0;i<row;i++) */
/* 	  for(j=0;j<col;j++) */
/* 		{ */
/* 		  mine_map[i][j]=map[i][j]; */
/* 		  mine_digged[i][j]=0; */
/* 		  mine_number[i][j]=0; */
/* 		  mine_marked[i][j]=0; */
/* 		} */

/*  for(i=0;i<row;i++) */
/* 	  for(j=0;j<col;j++) */
/* 		{ */
/* 		  if(mine_map[i][j]) */
/* 			{ */
/* 			  //9 stands for a mine here,for there is no more than 8 mine around */
/* 			  mine_number[i][j]=9; */
/* 			  continue; */
/* 			} */
/* 		  for(m=((i==0)?i:i-1);m<=((i==row-1)?i:i+1);m++) */
/* 		  	for(n=((j==0)?j:j-1);n<=((j==col-1)?j:j+1);n++) */
/* 		  	  { */
/* 		  		mine_number[i][j]+=mine_map[m][n]; */
/* 		  	  } */
/*  		} */
/*  } */


void init_mine_map(uint8_t* map_head)
{
  uint8_t i,j;
  uint8_t m,n;

  for(i=0;i<row;i++)
	  for(j=0;j<col;j++)
		{
		  mine_map[i][j]=map_head[i*col+j]; //do in the compiler's way 
		  mine_digged[i][j]=0;
		  mine_number[i][j]=0;
		  mine_marked[i][j]=0;
		}

 for(i=0;i<row;i++)
	  for(j=0;j<col;j++)
		{
		  if(mine_map[i][j])
			{
			  //9 stands for a mine here,for there is no more than 8 mine around
			  mine_number[i][j]=9;
			  continue;
			}
		  for(m=((i==0)?i:i-1);m<=((i==row-1)?i:i+1);m++)
		  	for(n=((j==0)?j:j-1);n<=((j==col-1)?j:j+1);n++)
		  	  {
		  		mine_number[i][j]+=mine_map[m][n];
		  	  }
 		}
 }
void init_mine_map_rand(uint32_t count)
{
  uint32_t tmp;
  uint8_t i,j,m,n;
  uint8_t index_x,index_y;
  mine_count=count;
  mine_count_marked=0;

  srand((unsigned int)time(NULL));
  for(i=0;i<row;i++)
	  for(j=0;j<col;j++)
		{
		  mine_map[i][j]=0; 
		  mine_digged[i][j]=0;
		  mine_number[i][j]=0;
		  mine_marked[i][j]=0;
		}

  for(tmp=0;tmp<count;tmp++)
	{
	  index_x=rand()%row;
	  index_y=rand()%col;//generate a point
	  if(mine_map[index_x][index_y]==1)
		tmp--;
	  else
		mine_map[index_x][index_y]=1;
	}



 for(i=0;i<row;i++)
	  for(j=0;j<col;j++)
		{
		  if(mine_map[i][j])
			{
			  //9 stands for a mine here,for there is no more than 8 mine around
			  mine_number[i][j]=9;
			  continue;
			}
		  for(m=((i==0)?i:i-1);m<=((i==row-1)?i:i+1);m++)
		  	for(n=((j==0)?j:j-1);n<=((j==col-1)?j:j+1);n++)
		  	  {
		  		mine_number[i][j]+=mine_map[m][n];
		  	  }
 		}


}
