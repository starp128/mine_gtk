all:
	gcc `pkg-config --cflags gtk+-3.0` mine.c mine_gtk_ui.c `pkg-config --libs gtk+-3.0` -o mine
	gcc  mine.c console_ui.c  -o mine_console_ui
	gcc  mine.c mine_xlib_ui.c -lX11 -o mine_xlib_ui

tags:
	find . -name "*.[chCH]" -print | etags -
	@echo "using M-x visit-tags-table in emacs to navigate"

help:
	@echo "make or make all to build"
	@echo "make sure there is libgtk3 is in the box"
	@echo "if not please install it first using something like below"
	@echo "apt-get install libgtk-3-dev"
	@echo "make tags to build tag file for emacs"
	@echo "make clean to clean the target file and the backup files"

clean:
	rm -f mine mine_console_ui mine_xlib_ui
	rm -f *~
	rm -f \#*\#
	rm -f TAGS
	@echo "clean done"



