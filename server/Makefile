objects = main.o server.o sem_open.o sem_wait.o sem_post.o sem_getvalue.o  shm_open.o ../include/common.o
header = server.h sem_h.h shm_h.h ../include/chatroom.h
program : server

server : $(objects)
	cc -o $@ $^

$(objects) : $(header)


.PHONY : clean cleanobj

clean :
	-rm *.o server
cleanobj :
	-rm *.o
