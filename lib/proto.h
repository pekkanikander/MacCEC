/**
 * Protocol independent data structures.
 *
 * This file has been explicitly placed in public domain.
 */

/*
 * These data structures are optimised for handling protocols
 * that have short frames, where each frame starts with a
 * single 8-bit command code, followed by 0-16 bytes of
 * arguments.  Each frame is delimited by external means;
 * the frame itself does not carry any explicit length.
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
 * The basic element in the protocols, an 8-bits long char/frame.
 */

typedef unsigned char proto_char_t;

/**
 * Maximum size of a protocol frame.
 *
 * Allow a number of encoded P8 frames in one P8 serial frame.
 * A CEC message may be 16 bytes, requiring max 64 P8 serial bytes.
 * In addition that, there will be timeout setting etc, so we go for
 * a value longer that that 16*4 that fits into a power of 2.
 */

/* NB.  There is an assert in proto_dispatch.c checking the sizes. */
#define PROTO_FRAME_SIZE   128
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

static inline unsigned char
proto_frame_getc(proto_frame_t *f) {
    assert_frame_invariant(f);
    assert_frame_not_empty(f);

    return *(f->f_sta++);
}

static inline int
proto_frame_is_empty(const proto_frame_t *f) {
    assert_frame_invariant(f);
    return f->f_sta == f->f_end;
}

static inline void
proto_frame_init(proto_frame_t *f, int status) {
    f->f_end = f->f_sta = f->f_buf;
    f->f_status = status;
    assert_frame_invariant(f);
}

static inline long
proto_frame_len(const proto_frame_t *f) {
    assert_frame_invariant(f);
    return f->f_end - f->f_sta;
}

#define PROTO_FRAME(type, name, len, status)     \
    type _ ## name = {                           \
        .f_sta = _ ## name.f_buf,                \
        .f_end = _ ## name.f_buf + (len),        \
        .f_status = (status),                    \
    }, *name = &_ ## name

/**
 * A type used for representing protocol message lengths.
 *
 * Select a type that produces good code for your platform
 * and than can hold values 0...PROTO_FRAME_MAXLEN.
 * Usually an unsigned int is good, but for the 8/16-bit uCs,
 * where you may want to use unsigned short or unsigned char.
 */
typedef unsigned int proto_len_t;

/**
 * Represent any callback argument, like yesteryear's void pointer.
 */
typedef union {
    int                              cba_int;
    int                             *cba_intp;
    proto_frame_t                   *cba_frame;
    struct cec_callback_arg         *cba_cec_cba; /* Forward declaration of the type */
} proto_callback_arg_t;

/**
 * Callback function, called back by the proto_dispatch,
 * based on the code and dispatch table.
 */
typedef int (*proto_callback_t)(proto_char_t code, const proto_frame_t *frame,
                                proto_callback_arg_t cb_arg);

/*
 * Protocol dispatch.
 *
 * CMD -> dt_indices -> dt_callbacks -> callback
 */

#ifdef __GNUC__
/* GCC does support static initialization of flexible array members */
#define PROTO_INDEX_SIZE
#else
#warning "Spending memory on too big index tables"
#define PROTO_INDEX_SIZE 256
#endif

typedef unsigned char proto_dispatch_index_t;

typedef struct proto_dispatch_table {
    unsigned char                dt_number;      /* Number of callbacks in table */
    unsigned char                dt_code_mask;   /* Code mask before looking up dt_indices */
    const proto_callback_t       dt_error;       /* Index out of bounds */
    const proto_callback_t *     dt_callbacks;   /* The callbacks themselves */
    const proto_dispatch_index_t dt_indices[PROTO_INDEX_SIZE]; /* Map codes to indices */
} proto_dispatch_table_t;

extern int
proto_dispatch(const proto_char_t code,
               const proto_frame_t *frame,
               const proto_dispatch_table_t *table,
               proto_callback_arg_t *cb_arg_table);
