#include<stdio.h>
#include<stdlib.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "mine.h"

#define pix_of_bmp 20 
#define num_of_bmp 20

int xlib_map[num_of_bmp][num_of_bmp]={0,};

#define RECORD(n,x,y) xlib_map[x][y]=n;
#define BLACK 10
#define FLAG 11
#define BOMB 12
#define WHITE 13
#define HEART 14
int init_map(int to)
{
	int i,j;
	for(i=0; i<num_of_bmp; i++)
		for(j=0; j<num_of_bmp; j++)
		{
			xlib_map[i][j] = to;
		}
	
}
#define SHOW(bitmap,x,y)   XCopyPlane(display, bitmap, win, gc,	\
			       0,0,				\
			       pix_of_bmp, pix_of_bmp,		\
			       x*20,y*20,			\
			       1);


#define SHOWALL() 	for(i=0; i<num_of_bmp; i++)	\
		for(j=0; j<num_of_bmp; j++)		\
			SHOW(bmp[xlib_map[i][j]],i,j);    


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
	
	char **p;
	int tmp_number;
	p = XListFonts(display, "*", 100, &tmp_number);
	printf("there are total %d fonts\n",tmp_number);
	for (i=0; i<tmp_number; i++) {
		printf("%s\n", p[i]);
	}
	XFreeFontNames(p);
	
	p = XGetFontPath(display, &tmp_number);
	printf("there are total %d fonts search pathes\n",tmp_number);
	for (i=0; i<tmp_number; i++) {
		printf("%s\n", p[i]);
	}
	XFreeFontPath(p);
	
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
	Pixmap bitmap;
	unsigned int bitmap_width, bitmap_heitgh;
	int hotspot_x, hotspot_y;
	int rc;
	rc = XReadBitmapFile(display, win, "./bmp/heart.bmp",
			     &bitmap_width, &bitmap_heitgh,
			     &bitmap,
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
	Pixmap bitmap_1;
	Pixmap bitmap_2, bitmap_3, bitmap_4, bitmap_5,
		bitmap_6, bitmap_7, bitmap_8, bitmap_9,
		bitmap_black, bitmap_flag, bitmap_bomb, bitmap_white,
		bitmap_heart;
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
	//	sleep(5);
	/* for(i=0; i< 14; i++) */
	/* { */
	/* 	init_map(i); */
	/* 	SHOWALL(); */
	/* 	sleep(2); */
	/* } */
	row = 20;
	col = 20;
	init_mine_map_rand(3);
	
	/*******************event loop**********************/
	XSelectInput(display, win, ExposureMask | ButtonPressMask |KeyPressMask);
	XEvent an_event;
	int done=0;
	int event_count=0;
	while(!done) {
		XNextEvent(display, &an_event);
		switch (an_event.type) {
		case Expose:
			SHOWALL();
			printf("expose\n");
			break;
		case ButtonPress:
			/* XDrawString(display, win, rev_gc, j, 50, "your mine", 9); */
			/* j++; */
		       	/* XDrawString(display, win, gc, j, 50, "your mine", 9); */
			/* printf("button Press\n"); */

			/* init_map(event_count%14); */
			/* SHOWALL(); */
			/* event_count++; */
			handle_button_down(display, win,gc,
					   (XButtonEvent*)&an_event.xbutton,
					   bmp);
			break;
		case KeyPress:
//			done = 1;
			break;
		default:
			break;
		}
	}
	XFreeGC(display, gc);
	XCloseDisplay(display);
	return 0;
}

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
			printf("game over\n");
			return;
		case clicked_on_empty:
//			print_result_xlib(display, win, gc, bmp);
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

			break;
		case clicked_on_number:
//			print_result_xlib(display, win, gc, bmp);
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

			break;
		default:
			printf("error\n");
		}
		
		/* map[x][y]++; */
		/* map[x][y]=map[x][y]%14; */
		/* SHOW(bmp[map[x][y]],x,y); */
		/* printf("painted\n"); */
		break;
	case Button3:
		xlib_map[x][y]=BLACK;
		SHOW(bmp[xlib_map[x][y]],x,y);
		break;
	}
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
				SHOW(bmp[mine_number[i][j]-1],i,j);
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
