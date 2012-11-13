/**
 * P8 Serial protocol.
 *
 * This file has been explicitly placed in public domain.
 */

/**
 * The number of items in a constant table.
 *
 * From http://stackoverflow.com/questions/1598773/\
 * is-there-a-standard-function-in-c-that-would-return-the-length-of-an-array:
 *
 * In this version if a pointer is mistakenly passed as the argument,
 * the compiler will complain in some cases - specifically if the
 * pointer's size isn't evenly divisible by the size of the object the
 * pointer points to. In that situation a divide-by-zero will cause
 * the compiler to error out. Actually at least one compiler
 * gives a warning instead of an error.
 *
 * That macro doesn't close the door on using it erroneously, but it
 * comes close in straight C.
 */

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((unsigned int)(!(sizeof(x) % sizeof(0[x])))))

/**
 * The maximum length of a P8 message.
 *
 * XXX: Check from the original source
 */
#define P8_FRAME_BUFFER_LENGTH 256

/**
 * A type used for representing P8 message lengths.
 *
 * Select a type that produces good code for your platform
 * and than can hold values 0...P8_FRAME_BUFFER_LENGTH.
 */
typedef unsigned int p8_len_t;

/**
 * A type used for representing P8 parameter bytes.
 */
typedef unsigned char p8_param_t;

typedef enum p8_code p8_code_t;
typedef struct p8_io_buffer p8_io_buffer_t;;

struct cec_rx_buffer;

#if 1
#define DEBUG(...) printf(__VA_ARGS__)
#include <stdio.h>
#else
#define DEBUG(...)
#endif

/**
 * Sends a P8 command, and waits for an ACK or NACK.
 */
extern int
p8_command(int fd, enum p8_code code,
           const p8_param_t *params, p8_len_t param_len,
           p8_io_buffer_t *pib,
           struct cec_rx_buffer *crb);
