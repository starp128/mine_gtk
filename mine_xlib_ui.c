#include<stdio.h>
#include<stdlib.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "mine.h"

#define pix_of_bmp 20 
#define num_of_bmp 20

#define BLACK 10
#define FLAG 11
#define BOMB 12
#define WHITE 13
#define HEART 14

int aaa[20][20] = {
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0},
	{0,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0},
	{0,0,1,1,1,0,0,1,1,1,1,1,0,0,1,1,1,0,0,0},
	{0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,0,0},
	{0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0},
	{0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0},
	{0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0},
	{0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0},
	{0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0},
	{0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0},
	{0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
	{0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0},
	{0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0},
	{0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

/* number and location of the heart fianlly */
int heart_number =0 ;
int heart_loc[400][2];

int bomb_loc[400][2]; /* location of the bomb */

int xlib_map[num_of_bmp][num_of_bmp]={0,};

int init_map(int to)
{
	int i,j;
	for(i=0; i<num_of_bmp; i++)
		for(j=0; j<num_of_bmp; j++)
		{
			xlib_map[i][j] = to;
		}
}

/* draw a 20 pixel x 20 pixel element */
#define SHOW(bitmap,x,y)   XCopyPlane(display, bitmap, win, gc,	\
			       0,0,				\
			       pix_of_bmp, pix_of_bmp,		\
			       x*20,y*20,			\
			       1);


#define SHOWALL() 	for(i=0; i<num_of_bmp; i++)	\
		for(j=0; j<num_of_bmp; j++)		\
			SHOW(bmp[xlib_map[i][j]],i,j);    



int LoadBmp(Display *display, Window win, char* p, Pixmap* bitmap)
{
	unsigned int bitmap_width, bitmap_heitgh;
	int hotspot_x, hotspot_y;
	int rc;
	rc = XReadBitmapFile(display, win, p,
			     &bitmap_width, &bitmap_heitgh,
			     bitmap,
			     &hotspot_x, &hotspot_y);
	switch (rc) {
	case BitmapOpenFailed:
		fprintf(stderr, "XReadBitmapFile\n");
		exit(1);
		break;
        case BitmapFileInvalid:
		fprintf(stderr,
			"XReadBitmapFile doesn't contain a valid bitmap.\n");
		exit(1);
		break;
        case BitmapNoMemory:
		fprintf(stderr, "XReadBitmapFile - not enough memory.\n");
		exit(1);
		break;
	}
	return 0;
}


int print_result_xlib(Display* display, Window win, GC gc, Pixmap* bmp)
{
	int i, j;
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			if(mine_digged[i][j])
			{
				SHOW(bmp[mine_number[i][j]],i,j);
			}
			else
			{
				if(mine_marked[i][j]) {
					SHOW(bmp[FLAG],i,j);
				}
				else
				{
					SHOW(bmp[BLACK],i,j);
				}

			}
		}
	}
}

int flash(Display* display, Window win, GC gc, Pixmap* bmp)
{
	int i, j;
	int loc[400][2];
	int count =0;
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			if(mine_marked[i][j]) {
				loc[count][0] = i;
				loc[count][1] = j;
				count++;
			}

		}
	}
	for(i=0; i<count; i++)
		SHOW(bmp[FLAG], loc[i][0], loc[i][1]);
	XFlush(display);
	usleep(300000);
	for(i=0; i<count; i++)
		SHOW(bmp[WHITE], loc[i][0], loc[i][1]);
	XFlush(display);
	usleep(300000);

	for(i=0; i<count; i++)
		SHOW(bmp[FLAG], loc[i][0], loc[i][1]);
	XFlush(display);
	usleep(300000);

	for(i=0; i<count; i++)
		SHOW(bmp[WHITE], loc[i][0], loc[i][1]);
	XFlush(display);
	usleep(300000);

	for(i=0; i<count; i++)
		SHOW(bmp[BOMB], loc[i][0], loc[i][1]);
	XFlush(display);
	usleep(300000);

	for(i=0; i<count; i++)
		SHOW(bmp[WHITE], loc[i][0], loc[i][1]);
	XFlush(display);
	usleep(300000);

	for(i=0; i<count; i++)
		SHOW(bmp[HEART], loc[i][0], loc[i][1]);
	XFlush(display);
	usleep(300000);
}

void make_heart(Display* display, Window win, GC gc, Pixmap* bmp)
{
	int i;
	int m,n;
	int k;
	/* for(i=0; i<40; i++) */
	/* { */
	/* 	move_one_step(); */
	/* 	for(m=0; m<20; m++) */
	/* 		for(n=0;n<20;n++) */
	/* 			SHOW(bmp[WHITE], m,n); */
	/* 	XFlush(display); */
	/* 	for(k=0;k<heart_number;k++) */
	/* 		SHOW(bmp[HEART],bomb_loc[k][0],bomb_loc[k][1]); */
	/* 	XFlush(display); */
	/* 	usleep(300000); */
	/* } */
	int done=0;
	while(!done)
	{
		done = move_one_step();
		for(m=0; m<20; m++)
			for(n=0;n<20;n++)
				SHOW(bmp[WHITE], m,n);
		XFlush(display);
		for(k=0;k<heart_number;k++)
			SHOW(bmp[HEART],bomb_loc[k][0],bomb_loc[k][1]);
		XFlush(display);
//		usleep(1000000);
		sleep(1);
	}
}

/* If do not need move, return TURE. */
int move_one_step()
{
	int i;
	int x, y; //src
	int xx, yy; //dst
	int flag_ok=1;
	for(i=0; i<heart_number; i++)
	{
		x=bomb_loc[i][0];
		y=bomb_loc[i][1];
		xx = heart_loc[i][0];
		yy = heart_loc[i][1];
		if(x<xx)
		{
			x++;
			flag_ok = 0; //need move ,means not ok
		}
		else if(x>xx)
		{
			x--;
			flag_ok = 0;
		}
		else
		{
			x=x;
		}
		if(y<yy)
		{
			y++;
			flag_ok = 0;
		}
		else if(y>yy)
		{
			y--;
			flag_ok = 0;
		}
		else
		{
			y=y;
		}
		bomb_loc[i][0] = x;
		bomb_loc[i][1] = y;
	}
	return flag_ok;
}
handle_button_down(Display* display, Window win, GC gc, XButtonEvent* button_event
		   ,Pixmap* bmp)
{
	int x, y;
	enum mine_status s = clicked_on_digged;
	int i,j;

	/* invert the pixel under the mouse. */
	x = button_event->x/pix_of_bmp;
	y = button_event->y/pix_of_bmp;
	switch (button_event->button) {
	case Button1:
		s = dig(x,y);
		switch (s)
		{
		case clicked_on_digged:
			printf("nothing happen\n");
			break;
		case clicked_on_gameover:
			SHOW(bmp[BOMB], x, y);
			printf("game over\n");
			return;
		case clicked_on_empty:
			print_result_xlib(display, win, gc, bmp);
			break;
		case clicked_on_number:
			print_result_xlib(display, win, gc, bmp);
			break;
		default:
			printf("error\n");
		}
		break;
	case Button3:
		s = mark(x, y);
		switch(s)
		{
		case clicked_on_nothing:
			break;
		case clicked_on_unmark:
			SHOW(bmp[BLACK], x, y);
			break;
		case clicked_on_mark:
			SHOW(bmp[FLAG], x, y);
			break;
		case clicked_on_secceed:
			SHOW(bmp[FLAG], x, y);
			flash(display, win, gc, bmp);
			make_heart(display, win, gc, bmp);
			printf("succeed\n");
			break;
		default:
			printf("mark error\n");
		}
		break;
	default:
		printf("ingored, button 1 to dig, button to mark\n");
	}
}

int record_heart_and_return_number()
{
	int i,j;
	int tmp =0;
	for( i=0; i<20; i++)
		for(j=0; j<20; j++)
		{
			if(aaa[i][j])
			{
				heart_loc[tmp][0] = j;
				heart_loc[tmp][1] = i;
				tmp++;
			}
		}
	return tmp;
}

/* Remember where the bombs is */
int record_bomb()
{
	int i,j;
	int count=0;
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			if(mine_map[i][j]) {
				bomb_loc[count][0] = i;
				bomb_loc[count][1] = j;
				printf("init bomb_loc [%d] = %d,%d\n",
				       count, bomb_loc[count][0],bomb_loc[count][1]);
				count++;
			}
		}
	}
}

/*  */
void super_man(Display* display, Window win, GC gc, Pixmap* bmp)
{
	int i, j;
	enum mine_status s = clicked_on_digged;
	for(i = 0; i< 20; i++)
		for(j=0; j<20; j++)
		{
			if(mine_map[i][j]) {
				s = mark(i, j);
				switch(s)
				{
				case clicked_on_nothing:
					break;
				case clicked_on_unmark:
					break;
				case clicked_on_mark:
					SHOW(bmp[FLAG], i, j);
					XFlush(display);
					usleep(30000);
					break;
				case clicked_on_secceed:
					SHOW(bmp[FLAG], i, j);
					flash(display, win, gc, bmp);
					make_heart(display, win, gc, bmp);
					return;
				}
			}
		}
}

int main(int argc, char** argv)
{
	int i,j;
	Display* display;
	int display_width, display_height;
	char *display_name = getenv("DISPLAY");

	display = XOpenDisplay(display_name);

	/*******************query some information**********************/

	int screen_number = XDefaultScreen(display);
	unsigned long black_pixel;
	unsigned long white_pixel;
	black_pixel = XBlackPixel(display, XDefaultScreen(display));
	white_pixel = XWhitePixel(display, XDefaultScreen(display));
	printf("Default screen number is %d\n", screen_number);
	printf("black_pixel %ld white pixel %ld\n", black_pixel, white_pixel);

	int screen_count = XScreenCount(display);
	char* verdor = XServerVendor(display);
	Window root_window = XRootWindow(display, screen_number);
	printf("screen_count %d\n", screen_count);
	printf("vendor: %s\n", verdor);
	
	/*******************window**********************/	
	Window win;
	int width_win=pix_of_bmp*num_of_bmp;
	int height_win = pix_of_bmp*num_of_bmp;

	win = XCreateSimpleWindow(display, RootWindow(display, screen_number),
				  100, 100, width_win, height_win, 10,
				  BlackPixel(display, screen_number),
				  WhitePixel(display, screen_number));
	XMapWindow(display, win);

	/*******************draw fonts**********************/
	
	/* char **p; */
	/* int tmp_number; */
	/* p = XListFonts(display, "*", 100, &tmp_number); */
	/* printf("there are total %d fonts\n",tmp_number); */
	/* for (i=0; i<tmp_number; i++) { */
	/* 	printf("%s\n", p[i]); */
	/* } */
	/* XFreeFontNames(p); */
	
	/* p = XGetFontPath(display, &tmp_number); */
	/* printf("there are total %d fonts search pathes\n",tmp_number); */
	/* for (i=0; i<tmp_number; i++) { */
	/* 	printf("%s\n", p[i]); */
	/* } */
	/* XFreeFontPath(p); */
	
	/*******************GC       **********************/
	GC gc;
	GC rev_gc;
	XGCValues values;
	/* gc = XDefaultGC(display, screen_number); */
	/* XCopyGC(display, gc, (unsigned long)0, rev_gc); */
	gc = XCreateGC(display, win, 0, &values);
	rev_gc = XCreateGC(display, win, 0, &values);
	XSetForeground(display, gc, BlackPixel(display, screen_number));
	XSetBackground(display, gc, WhitePixel(display, screen_number));
	XSetForeground(display, rev_gc, WhitePixel(display, screen_number));
	XSetBackground(display, rev_gc, BlackPixel(display, screen_number));

	/*******************Pixmap    **********************/
	Pixmap bmp[15];
	LoadBmp(display, win, "./bmp/0.bmp", &bmp[0]);
	LoadBmp(display, win, "./bmp/1.bmp", &bmp[1]);
	LoadBmp(display, win, "./bmp/2.bmp", &bmp[2]);
	LoadBmp(display, win, "./bmp/3.bmp", &bmp[3]);
	LoadBmp(display, win, "./bmp/4.bmp", &bmp[4]);
	LoadBmp(display, win, "./bmp/5.bmp", &bmp[5]);
	LoadBmp(display, win, "./bmp/6.bmp", &bmp[6]);
	LoadBmp(display, win, "./bmp/7.bmp", &bmp[7]);
	LoadBmp(display, win, "./bmp/8.bmp", &bmp[8]);
	LoadBmp(display, win, "./bmp/9.bmp", &bmp[9]);
	LoadBmp(display, win, "./bmp/black.bmp", &bmp[10]);
	LoadBmp(display, win, "./bmp/flag.bmp", &bmp[11]);
	LoadBmp(display, win, "./bmp/bomb.bmp", &bmp[12]);
	LoadBmp(display, win, "./bmp/white.bmp", &bmp[13]);
	LoadBmp(display, win, "./bmp/heart.bmp", &bmp[14]);

	/*******************something not xlib**********************/
	init_map(BLACK);
	row = 20;
	col = 20;
	heart_number = record_heart_and_return_number();
	printf("heart_number = %d \n", heart_number);
	init_mine_map_rand(heart_number);
	record_bomb();
	
	/*******************event loop**********************/
	XSelectInput(display, win, ExposureMask | ButtonPressMask |KeyPressMask);
	XEvent an_event;
	int done=0;
	while(!done) {
		XNextEvent(display, &an_event);
		switch (an_event.type) {
		case Expose:
			print_result_xlib(display, win, gc, bmp);
			printf("expose\n");
			break;
		case ButtonPress:
			handle_button_down(display, win,gc,
					   (XButtonEvent*)&an_event.xbutton,
					   bmp);
			break;
		case KeyPress:
			done = 1;
			super_man(display, win, gc, bmp);
			break;
		default:
			break;
		}
	}
	sleep(5);
	XFreeGC(display, gc);
	XCloseDisplay(display);
	return 0;
}
