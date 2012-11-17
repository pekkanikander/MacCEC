/**
 * Get firmware version, testing encode and decode, and the code table.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include <proto.h>

#include <p8.h>
#include <p8_codes.h>
#include <p8_frame.h>
#include <p8_io.h>
#include <p8_dispatch.h>

static int replied = 0;
static char fw_vers[2];

int callback(proto_char_t code,
             const proto_frame_t *frame,
             proto_callback_arg_t dummy) {
    const proto_char_t *params = frame->f_sta;
    proto_len_t param_len = frame->f_end - frame->f_sta;
    assert(code < P8_CODE_TYPE_COUNT);
    assert(code == P8_QRY_FIRMWARE_VERS);
    assert(param_len == p8_code_types[P8_QRY_FIRMWARE_VERS].ct_a2h_min_plen);
    assert(param_len == p8_code_types[P8_QRY_FIRMWARE_VERS].ct_a2h_max_plen);
    replied = 1;
    memcpy(fw_vers, params, param_len);
    return 0;
}

proto_callback_t callbacks[] = { callback };

const proto_dispatch_table_t dt = {
    .dt_number = 1,
    .dt_indices = { 0, },       /* All zeros */
    .dt_error = callback,
    .dt_callbacks = callbacks,
};

int main(void) {
    int fd, len, rv;

    if ((fd = p8_open()) < 0) {
        perror("open");
        exit(1);
    }

    p8s_frame_t sframe = {
        .f_sta = sframe.f_buf,
        .f_end = sframe.f_buf,
    };

    if ((fd = p8_open()) < 0) {
        perror("open");
        exit(1);
    }

    p8_encode_cmd(&sframe, P8_QRY_FIRMWARE_VERS, NULL, 0);
    len = write(fd, sframe.f_buf, sframe.f_end - sframe.f_sta);
    if (len != sframe.f_end - sframe.f_sta) {
        perror("write");
        exit(1);
    }

    sleep(1);                  /* NB.  Avoid repetive reads.  Ugly. */

    sframe.f_end = sframe.f_sta = sframe.f_buf;
    len = read(fd, sframe.f_buf, sizeof(sframe.f_buf));
    sframe.f_end += len;

    proto_callback_arg_t cba_table[] = { { .cba_int = 0 } };

    while (sframe.f_end > sframe.f_sta) {
        rv = p8_dispatch(&sframe, &dt, cba_table);
        if (rv) break;
    }

    printf("Firmware version = %2.2x.%2.2x\n", fw_vers[0], fw_vers[1]);

    return 0;
}
