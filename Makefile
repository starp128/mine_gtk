all:
	gcc `pkg-config --cflags gtk+-3.0` mine.c mine_gtk_ui.c `pkg-config --libs gtk+-3.0` -o mine

clean:
	rm mine *~

