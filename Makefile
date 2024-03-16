.PHONY: openrc systemd
CC=gcc
CFLAGS=-lb64 -Iinc -O2

SRCS = src/*

sddc: ${SRCS}
	${CC} ${SRCS} -o sddc ${CFLAGS}

install: sddc
	cp sddc /usr/local/bin/sddc

openrc: openrc/sddc
	cp openrc/sddc /etc/init.d/sddc
	rc-update add sddc default
	rc-service sddc start

systemd: systemd/sddc.service
	cp systemd/sddc.service /etc/systemd/system/sddc.service
	systemctl daemon-reload
	systemctl enable sddc
	systemctl start sddc

clean:
	rm -rf sddc
