CC = g++
CFLAG = -w
OBJ_DIR = objs
OBJS = $(OBJ_DIR)\thumb.o $(OBJ_DIR)\test.o $(OBJ_DIR)\machine.o

vm : test thumb machine
	$(CC) $(CFLAG) $(OBJS) -o vm 

makeobjdir :
	if test ! -d $(OBJ_DIR) ;\
	then \
		mkdir $(OBJ_DIR) ;\
	fi
test : makeobjdir
	$(CC) $(CFLAG) -c test.cpp -o $(OBJ_DIR)\test.o

thumb : makeobjdir
	$(CC) $(CFLAG) -c devices\thumb.cpp -o $(OBJ_DIR)\thumb.o	

machine : makeobjdir
	$(CC) $(CFLAG) -c devices\machine.cpp -o $(OBJ_DIR)\machine.o	