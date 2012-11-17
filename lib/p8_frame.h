/**
 * P8 Serial protocol.
 *
 * Implements the serial protocol framing.
 *
 * This file has been explicitly placed in public domain.
 */

/*
 * P8 framing
 */

enum p8_framing {
    P8_FRAME_ESC   = 0xfd,
    P8_FRAME_END   = 0xfe,
    P8_FRAME_BEGIN = 0xff,
};
#define P8_FRAME_ESC_OFFSET (P8_FRAME_BEGIN - P8_FRAME_ESC + 1)

enum p8_frame_error {
    P8_ERROR_FRAME_INCOMPLETE,    /* Incomplete frame, something is missing */
    P8_ERROR_FRAME_BEGIN_MISSING, /* Didn't find frame start when expecting */
    P8_ERROR_SPURIOUS_START,      /* Found frame start before end */
    P8_ERROR_FRAME_MALFORMED,     /* Frame didn't follow assumed format */
};

typedef proto_frame_t p8s_frame_t; /* Encoded serial frame */

extern int
p8_frame_error(enum p8_frame_error error, const p8s_frame_t *frame);

extern void p8_encode_cmd(p8s_frame_t *oframe, p8_code_t code, proto_char_t *params, proto_len_t len);
extern void p8_encode_tx(p8s_frame_t *oframe, p8_code_t code, proto_frame_t *params);
extern int  p8_decode(p8s_frame_t *iframe, p8_frame_t  *oframe);

