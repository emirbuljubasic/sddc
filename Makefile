CC=gcc
CFLAGS=-lb64 -Iinc -O2

SRCS = src/*

install: all
	cp sddc /usr/local/bin/sddc
	cp openrc/sddc /etc/init.d/sddc
	rc-update add sddc default
	rc-service sddc start

all: ${SRCS}
	${CC} ${SRCS} -o sddc ${CFLAGS} 

clean:
	rm -rf sddc
