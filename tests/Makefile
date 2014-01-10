
TESTS = \
	p8_open \
	p8_ping \
	p8_ping2 \
	p8_getfw \
	p8_disp \
	p8_cec_vendor_id1 \
	p8_cec_vendor_id3 \
	cec_poll \
	cec_logical_initialise
#

CFLAGS = -I../lib -Wall -g
LDFLAGS = -L../lib -framework CoreFoundation -framework IOKit
LDLIBS = -lmaccec

all:	$(TESTS)

clean:
	rm -f $(TESTS)
	rm -rf *.dSYM

tests:
	for t in $(TESTS); do echo ==== $$t ====; ./$$t; done

