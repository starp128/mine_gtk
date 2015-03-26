all:
	gcc `pkg-config --cflags gtk+-3.0` mine.c mine_gtk_ui.c `pkg-config --libs gtk+-3.0` -o mine

tags:
	find . -name "*.[chCH]" -print | etags -
	@echo "using M-x visit-tags-table in emacs to navigate"

help:
	@echo "make or make all to build the project"
	@echo "make sure there is libgtk3 is in the box"
	@echo "if not please install it first using something like below"
	@echo "apt-get install libgtk-3-dev"
	@echo "make tags to build tag file for emacs"
	@echo "make clean to clean the target file and the backup files"

clean:
	rm -f mine
	rm -f *~
	rm -f \#*\#
	rm -f TAGS
	@echo "clean done"



