OBJS=main.o ssp_helpers.o ssp.o

CFLAGS=-g -I../inc -I../../hiredis -I../../libevent-2.0.22-stable/include -Wall -Werror -pedantic
LDFLAGS=-L../lib/bin/shared -L../../hiredis -L../../libevent-2.0.22-stable/.libs
LIBS=-litlssp -lhiredis -levent

all: essp-kassomat

run:
	ln -fs ../../hiredis/libhiredis.dylib libhiredis.0.12.dylib
	DYLD_LIBRARY_PATH=../lib/bin/shared:../../libevent-2.0.22-stable/.libs ./essp-kassomat

essp-kassomat: $(OBJS)
	$(CC) $(LDFLAGS) $(LIBS) -o essp-kassomat $(OBJS)

clean:
	rm -rf $(OBJS) essp-kassomat
