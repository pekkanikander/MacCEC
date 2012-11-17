/**
 * Protocol independent data structures.
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
 * XXX
 */


typedef unsigned char proto_char_t;

/**
 * XXX
 *
 * Allow a number of encoded P8 frames in one P8 serial frame.
 * A CEC message may be 16 bytes, requiring max 64 P8 serial bytes.
 * In addition that, there will be timeout setting etc, so we go for
 * longer that fits into a power of 2.
 */

#define PROTO_FRAME_SIZE 128

#define PROTO_FRAME_MAXLEN (PROTO_FRAME_SIZE - 2 * sizeof(proto_char_t *) - sizeof(int))

typedef struct proto_frame {
    proto_char_t *f_sta;    /* Start of frame, within in f_buf (or just past) */
    proto_char_t *f_end;    /* End of frame, within in f_buf (or just past) */
    int           f_status; /* Protocol specific */
    proto_char_t  f_buf[PROTO_FRAME_MAXLEN];
} proto_frame_t;

#define assert_frame_invariant(frame)                                      \
    assert(   (frame)->f_end >= (frame)->f_buf                             \
           && (frame)->f_end <= (frame)->f_buf + COUNT_OF((frame)->f_buf)),\
    assert(   (frame)->f_sta >= (frame)->f_buf                             \
           && (frame)->f_sta <= (frame)->f_buf + COUNT_OF((frame)->f_buf)),\
    assert(   (frame)->f_sta <= (frame)->f_end)

#define assert_frame_not_empty(frame) assert((frame)->f_sta <  (frame)->f_end)
#define assert_frame_is_empty(frame)  assert((frame)->f_sta == (frame)->f_end)

/**
 * A type used for representing protocol message lengths.
 *
 * Select a type that produces good code for your platform
 * and than can hold values 0...PROTO_FRAME_MAXLEN.
 */
typedef unsigned int proto_len_t;

/*
 * Represents any callback argument, like yesteryear's void *.
 * Notice the usage of struct pointers instead of typedefs,
 * avoiding including upper layer headers.
 */
typedef union {
    int                              cba_int;
    int                             *cba_intp;
    proto_frame_t                   *cba_frame;
} proto_callback_arg_t;

typedef int (*proto_callback_t)(proto_char_t code, const proto_frame_t *frame,
                                proto_callback_arg_t cb_arg);

typedef unsigned char proto_dispatch_index_t;

/*
 * Protocol dispatch.
 *
 * CMD -> dt_indices -> dt_callbacks -> callback
 */

#define PROTO_INDEX_SIZE 256

typedef struct proto_dispatch_table {
    unsigned char                dt_number;      /* Number of callbacks in table */
    const proto_dispatch_index_t dt_indices[PROTO_INDEX_SIZE]; /* Map codes to indices */
    const proto_callback_t       dt_error;       /* Index out of bounds */
    const proto_callback_t *     dt_callbacks;   /* The callbacks themselves */
} proto_dispatch_table_t;


extern int
proto_dispatch(const proto_char_t code,
               const proto_frame_t *frame,
               const proto_dispatch_table_t *table,
               proto_callback_arg_t *cb_arg_table);
