/**
 * P8 Serial protocol.
 *
 * Implements the serial protocol codes.
 *
 * This file has been explicitly placed in public domain.
 */

#include "proto.h"

#include "p8.h"
#include "p8_codes.h"

/**
 * Describes the message code points used in P8 messages.
 *
 * Part of the information in this table is guessed from
 * protocol traces, part from reading the original source code.
 */

const p8_code_type_t p8_code_types[] = {
    [P8_CMD_NONE]          = { P8_CT_NONE },

    [P8_CMD_PING]          = { P8_CT_H2A_ACKED, 0, 0 },

    [P8_IND_ERROR_TIMEOUT] = { P8_CT_A2H,       0,  0,  0, 16 }, /* NB. Be liberal. */
    [P8_IND_ERROR_HIGH]    = { P8_CT_A2H,       0,  0,  0, 16 }, /* NB. Be liberal. */
    [P8_IND_ERROR_LOW]     = { P8_CT_A2H,       0,  0,  0, 16 }, /* NB. Be liberal. */

    [P8_IND_RX_START]      = { P8_CT_A2H,       0,  0,  1,  1 },
    [P8_IND_RX_NEXT]       = { P8_CT_A2H,       0,  0,  1,  1 },
    [P8_IND_RX_FAILED]     = { P8_CT_A2H,       0,  0,  0, 16 }, /* TBD Too liberal, I presume. */

    [P8_IND_ACK]           = { P8_CT_A2H,       0,  0,  0,  1 }, /* May contain the accepted cmd. */
    [P8_IND_NACK]          = { P8_CT_A2H,       0,  0,  0,  1 }, /* May contain the rejected cmd. */
    [P8_CMD_SET_ACK_MASK]  = { P8_CT_H2A_ACKED, 1,  1,  0,  0 },

    [P8_CMD_TX]            = { P8_CT_H2A_ACKED, 1,  1,  0,  0 },
    [P8_CMD_TX_EOM]        = { P8_CT_H2A_TXEOM, 1,  1,  0,  0 },
    [P8_CMD_TX_SET_IDLE]   = { P8_CT_H2A_ACKED, 1,  1,  0,  0 }, /* Signal free time */
    [P8_CMD_TX_SET_ACK_P]  = { P8_CT_H2A_ACKED, 1,  1,  0,  0 }, /* ACK polarity, multicast/unicast */
    [P8_CMD_TX_SET_TIMEOUT]= { P8_CT_H2A_ACKED, 1,  1,  0,  0 }, /* TBD.  */

    [P8_IND_TX_SUCCEEDED]  = { P8_CT_A2H,       0,  0,  0, 16 }, /* TBD too liberal. */
    [P8_IND_TX_FAIL_LINE]  = { P8_CT_A2H,       0,  0,  0, 16 }, /* TBD too liberal. */
    [P8_IND_TX_FAIL_ACK]   = { P8_CT_A2H,       0,  0,  0, 16 }, /* TBD too liberal. */
    [P8_IND_TX_TIMEOUT_D]  = { P8_CT_A2H,       0,  0,  0, 16 }, /* TBD too liberal. */
    [P8_IND_TX_TIMEOUT_L]  = { P8_CT_A2H,       0,  0,  0, 16 }, /* TBD too liberal. */

    [P8_QRY_FIRMWARE]      = { P8_CT_QUERY,     0,  0,  2,  2 }, /* Firmware version */
    [P8_SET_CONTROLLED]    = { P8_CT_H2A_ACKED, 1,  1,  0,  0 }, /* Controlled mode */
};

const unsigned int P8_CODE_TYPE_COUNT = COUNT_OF(p8_code_types);
