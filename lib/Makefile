#
# This file has been explicitly placed in public domain.
#

PROTO_OBJ = \
	proto_dispatch.o \
#
P8_OBJ = \
	p8_error.o \
	p8_codes.o \
	p8_frame_decode.o \
	p8_frame_error.o \
	p8_frame_encode.o \
	p8_dispatch.o \
	p8_command.o \
	p8_cec_rx.o \
	p8_cec_tx.o \
#
CEC_OBJ = \
	cec_rx_char_cb.o \
	cec_rx_error.o \
	cec_tx_error.o \
	cec_open.o \
	cec_command.o \
#
PLATFORM_OBJ = \
	p8_detect_apple.o \
	p8_io_unix.o \
	p8_io.o \
#

CFLAGS += -Wall -g

OBJ = $(PROTO_OBJ) $(P8_OBJ) $(PLATFORM_OBJ) $(CEC_OBJ)

all:	libccec.a

clean:
	rm -f libccec.a
	rm -f $(OBJ)

libccec.a:	$(OBJ)
	ar -cr $@ $(OBJ)
	ranlib $@
