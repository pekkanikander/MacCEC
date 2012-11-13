/**
 * P8 Serial protocol.
 *
 * Implements the serial protocol codes.
 *
 * This file has been explicitly placed in public domain.
 */

#include "p8.h"
#include "p8_codes.h"

/**
 * Describes the message code points used in P8 messages.
 *
 * Part of the information in this table is guessed from
 * protocol traces, part from reading the original source code.
 */

const p8_code_type_t p8_code_types[] = {
    { P8_CT_NONE },

    { P8_CT_H2A_ACKED, 0, 0 },

    { P8_CT_A2H,       0,  0,  0, 16 }, /* NB. Be liberal in what you accept */
    { P8_CT_A2H,       0,  0,  0, 16 }, /* NB. Be liberal in what you accept */
    { P8_CT_A2H,       0,  0,  0, 16 }, /* NB. Be liberal in what you accept */

    { P8_CT_A2H,       0,  0,  1,  1 }, /* RX_START */
    { P8_CT_A2H,       0,  0,  1,  1 }, /* RX_NEXT */
    { P8_CT_A2H,       0,  0,  0, 16 }, /* RX_FAIL.  TBD Too liberal, I presume. */

    { P8_CT_A2H,       0,  0,  0,  1 }, /* May contain the accepted command. */
    { P8_CT_A2H,       0,  0,  0,  1 }, /* May contain the rejected command. */
    { P8_CT_H2A_ACKED, 1,  1,  0,  0 },

    { P8_CT_H2A_ACKED, 1,  1,  0,  0 }, /* TX */
    { P8_CT_H2A_TXEOM, 1,  1,  0,  0 }, /* TX + EOM */
    { P8_CT_H2A_ACKED, 1,  1,  0,  0 }, /* TX signal free time */
    { P8_CT_H2A_ACKED, 1,  1,  0,  0 }, /* Expected ACK polarity, multicast/unicast */
    { P8_CT_H2A_ACKED, 1,  1,  0,  0 }, /* TBD.  Not used in the libcec code */

    { P8_CT_A2H,       0,  0,  0, 16 }, /* TX succeeded.  TBD too liberal. */
    { P8_CT_A2H,       0,  0,  0, 16 }, /* TX succeeded.  TBD too liberal. */
    { P8_CT_A2H,       0,  0,  0, 16 }, /* TX succeeded.  TBD too liberal. */
    { P8_CT_A2H,       0,  0,  0, 16 }, /* TX succeeded.  TBD too liberal. */
    { P8_CT_A2H,       0,  0,  0, 16 }, /* TX succeeded.  TBD too liberal. */

    { P8_CT_QUERY,     0,  0,  2,  2 }, /* Firmware version */
    { P8_CT_H2A,       0,  0,  0,  0 }, /* Enter bootloader -- TBD check ACK status */
    { P8_CT_QUERY,     0,  0,  4,  4 }, /* Boot date */
    { P8_CT_H2A_ACKED, 1,  1,  0,  0 }, /* Controlled mode */
};

const unsigned int p8_code_type_count = COUNT_OF(p8_code_types);
