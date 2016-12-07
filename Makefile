CC=gcc
RM=del
CFLAGS=-O3
all: practice.cgi greeting.cgi account.cgi make_acc.cgi admin.exe
practice.cgi: gregutils.o practice.o 
	$(CC) practice.o gregutils.o -o practice.cgi
greeting.cgi: gregutils.o greeting.o 
	$(CC) gregutils.o greeting.o -o greeting.cgi
account.cgi: gregutils.o create_acc.o
	$(CC) gregutils.o create_acc.o -o create_acc.cgi
make_acc.cgi: make_acc.o gregutils.o
	$(CC) make_acc.o gregutils.o -o make_acc.cgi
character_setup.cgi: character_setup.o gregutils.o
	$(CC) character_setup.o gregutils.o -o character_setup.cgi
admin.exe: admin.o gregutils.o linked_list.o player.o
	$(CC) -g admin.o gregutils.o linked_list.o player.o -o admin.exe
clean:
	$(RM) *.o *.cgi