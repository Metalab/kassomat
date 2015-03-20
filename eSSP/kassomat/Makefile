OBJS=main.o

CFLAGS=-g -I../inc
LDFLAGS=-L../lib/bin
LIBS=-litlssp

all: essp-kassomat

essp-kassomat: $(OBJS)
	$(CC) $(LDFLAGS) $(LIBS) -o essp-kassomat $(OBJS)

clean:
	rm -rf $(OBJS) essp-kassomat
