OBJS=main.o ssp_helpers.o linux.o

CFLAGS=-g -I../inc -I../../hiredis
LDFLAGS=-L../lib/bin/shared -L../../hiredis
LIBS=-litlssp -lhiredis

all: essp-kassomat

run:
	ln -fs ../../hiredis/libhiredis.dylib libhiredis.0.12.dylib
	DYLD_LIBRARY_PATH=../lib/bin/shared ./essp-kassomat

essp-kassomat: $(OBJS)
	$(CC) $(LDFLAGS) $(LIBS) -o essp-kassomat $(OBJS)

clean:
	rm -rf $(OBJS) essp-kassomat
