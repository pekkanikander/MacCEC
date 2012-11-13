
TESTS = \
	p8_open \
	p8_ping \
	p8_ping2 \
	p8_getfw \
	p8_disp \
	cec_vendor_id1 \
	cec_vendor_id2 \
	cec_vendor_id3 \
#

CFLAGS = -I../libccec
LDFLAGS = -L../libccec -framework CoreFoundation -framework IOKit
LDLIBS = -lccec

all:	$(TESTS)

clean:
	rm -f $(TESTS)

tests:
	for t in $(TESTS); do ./$$t; done

# $(TESTS):: ../libccec/libccec.a ../libccec/p8.h
