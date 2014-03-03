#include<gtk/gtk.h>
#include"mine.h"
enum mine_status s=clicked_on_nothing;

#define IMAGE_RED   "./image/red20.png"
#define IMAGE_BLACK "./image/black20.png"
#define FLAG        "./image/flag20.png"
#define BOMB        "./image/bomb.png"
gchar* image_file[9]={"./image/number0.png","./image/number1.png",
					  "./image/number2.png",
					  "./image/number3.png","./image/number4.png",
					  "./image/number5.png","./image/number6.png",
					  "./image/number7.png","./imgae/number8.png"};


enum button_status
  {
	button_status_init,
	button_status_marked,
	button_status_nothing,
	button_status_numbered
  };

struct supper_button
{
  GtkWidget* button;
  GtkWidget* image;
  enum button_status status;
};

struct supper_button my_supper_button[8][8];

void action()
{
  gint i,j;
  GtkWidget* image;
  for(i=0;i<row;i++)
	for(j=0;j<col;j++)
	  {
		if(my_supper_button[i][j].status==button_status_init)
		  {
			if(mine_digged[i][j])
			  {
				if(mine_number[i][j])
				  {
					gtk_container_remove(GTK_CONTAINER(my_supper_button[i][j].button),my_supper_button[i][j].image);
					image=gtk_image_new_from_file(image_file[mine_number[i][j]]);
					gtk_container_add(GTK_CONTAINER(my_supper_button[i][j].button),image);
					gtk_widget_show(image);
					my_supper_button[i][j].status=button_status_numbered;
				  }
				else
				  {
					gtk_container_remove(GTK_CONTAINER(my_supper_button[i][j].button),my_supper_button[i][j].image);
					
					my_supper_button[i][j].status=button_status_nothing;
					
				  }
			  }
		  }
	  }	/* end for */
}
void action_mark(gint i,gint j,gint flag)
{
  GtkWidget* image;
  if(flag==0)
	{
	  //umark
	  gtk_container_remove(GTK_CONTAINER(my_supper_button[i][j].button),my_supper_button[i][j].image);
	  image=gtk_image_new_from_file(IMAGE_RED);
	  gtk_container_add(GTK_CONTAINER(my_supper_button[i][j].button),image);
	  gtk_widget_show(image);
	  my_supper_button[i][j].status=button_status_init;
	  my_supper_button[i][j].image=image;

	}
  else
	{
	  gtk_container_remove(GTK_CONTAINER(my_supper_button[i][j].button),my_supper_button[i][j].image);
	  image=gtk_image_new_from_file(FLAG);
	  gtk_container_add(GTK_CONTAINER(my_supper_button[i][j].button),image);
	  gtk_widget_show(image);
	  my_supper_button[i][j].status=button_status_marked;
	  my_supper_button[i][j].image=image;

	}
}
void action_game_over(gint i,gint j)
{
  GtkWidget *image;
  gtk_container_remove(GTK_CONTAINER(my_supper_button[i][j].button),my_supper_button[i][j].image);
  image=gtk_image_new_from_file(BOMB);
  gtk_container_add(GTK_CONTAINER(my_supper_button[i][j].button),image);
  gtk_widget_show(image);
  my_supper_button[i][j].status=button_status_marked;
  my_supper_button[i][j].image=image;
}
void on_button_clicked(GtkWidget* button,GdkEvent *event,gpointer userdata)
{
  GtkWidget* image;
  gint n=(gint)userdata;
  gint index_x,index_y;
  gint button_number=0;//1 means left 3 mean right
  
  index_x=n/8;
  index_y=n%8;
  button_number=event->button.button;
  //  g_print("button %d\n",event->button.button);

  g_print("button %d x=%d,y=%d\n",button_number,index_x,index_y);

  //left click 
  if(button_number==1)
	{
	  s=dig((uint8_t)index_x,(uint8_t)index_y);
	  switch(s)
		{
		case clicked_on_digged:
		  break;
		case clicked_on_gameover:
		  action_game_over(index_x,index_y);
		  g_print("GAME OVER\n");
		  break;
		case clicked_on_empty:
		  action();
		  break;
		case clicked_on_number:
		  action();
		  break;
		default:
		  g_print("error");
		}
	}
  //right clice
  if(button_number==3)
	{
	  s=mark((uint8_t)index_x,(uint8_t)index_y);
	  switch(s)
		{
		case clicked_on_nothing:
		  break;
		case clicked_on_unmark:
		  action_mark(index_x,index_y,0);
		  break;
		case clicked_on_mark:
		  action_mark(index_x,index_y,1);
		  break;
		case clicked_on_secceed:
		  action_mark(index_x,index_y,1);
		  g_printf("succeed\n");
		  break;
		default:
		  g_printf("mark error\n");
		}
	}
}

GtkWidget* creat_box_table(gint row,gint col)
{
  GtkWidget* table;
  GtkWidget* eventbox;
  gint i=0,j=0;
  gchar str[10];
  GtkWidget* image;

  table=gtk_table_new(row,col,TRUE);//set homoge**
  for(i=0;i<row;i++)
  	for(j=0;j<col;j++)
  	  {
		eventbox=gtk_event_box_new();
		g_signal_connect(G_OBJECT(eventbox),"button-press-event",G_CALLBACK(on_button_clicked),(gpointer)(i*row+j));
		
		image=gtk_image_new_from_file(IMAGE_RED);
		gtk_container_add(GTK_CONTAINER(eventbox),image);

		gtk_table_attach_defaults(GTK_TABLE(table),eventbox,j,j+1,i,i+1);

		my_supper_button[i][j].button=eventbox;
		my_supper_button[i][j].image=image;
		my_supper_button[i][j].status=button_status_init;

		
  	  }
 
  return table;
}

GtkWidget* creat_main_window()
{
  GtkWidget* window;  
  window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(gtk_main_quit),NULL);
  gtk_window_set_title(GTK_WINDOW(window),"bomb");
  gtk_window_set_default_size(GTK_WINDOW(window),200,200);
  gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);  
  return window;
}

int main(int argc,char *argv[])
{
  GtkWidget* window;
  GtkWidget* frame;
  GtkWidget* table;
  gint i,j;
  
  row=8;
  col=8;
  init_mine_map_rand(3);
  

  gtk_init(&argc,&argv);
  window=creat_main_window();


  frame=gtk_frame_new("frame");
  gtk_container_add(GTK_CONTAINER(window),frame);

  table=creat_box_table(row,col); //creat a 5X5 table of botton;
  gtk_container_add(GTK_CONTAINER(frame),table);
  gtk_widget_show_all(window);
  
  gtk_main();
}

