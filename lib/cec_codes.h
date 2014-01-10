/**
 * CEC protocol.
 *
 * A simple C API for CEC.
 * Defines the CEC protocol codes.
 *
 * This file has been explicitly placed in public domain.
 */

typedef enum cec_code {
    CEC_FEATURE_ABORT            = 0x00,
    CEC_IMAGE_VIEW_ON            = 0x04,
    CEC_TUNER_STEP_INCREMENT     = 0x05,
    CEC_TUNER_STEP_DECREMENT     = 0x06,
    CEC_TUNER_DEVICE_STATUS      = 0x07,
    CEC_GIVE_TUNER_DEVICE_STATUS = 0x08,
    CEC_RECORD_ON                = 0x09,
    CEC_RECORD_STATUS            = 0x0A,
    CEC_RECORD_OFF               = 0x0B,
    CEC_TEXT_VIEW_ON             = 0x0D,
    CEC_RECORD_TV_SCREEN         = 0x0F,
    CEC_GIVE_DECK_STATUS         = 0x1A,
    CEC_DECK_STATUS              = 0x1B,
    CEC_SET_MENU_LANGUAGE        = 0x32,
    CEC_CLEAR_ANALOGUE_TIMER     = 0x33,
    CEC_SET_ANALOGUE_TIMER       = 0x34,
    CEC_TIMER_STATUS             = 0x35,
    CEC_STANDBY                  = 0x36,
    CEC_PLAY                     = 0x41,
    CEC_DECK_CONTROL             = 0x42,
    CEC_TIMER_CLEARED_STATUS     = 0x43,
    CEC_USER_CONTROL_PRESSED     = 0x44,
    CEC_USER_CONTROL_RELEASED    = 0x45,
    CEC_GIVE_OSD_NAME            = 0x46,
    CEC_SET_OSD_NAME             = 0x47,
    CEC_SET_OSD_STRING           = 0x64,
    CEC_SET_TIMER_TITLE          = 0x67,
    CEC_SYSTEM_AUDIO_MODE_REQUEST= 0x70,
    CEC_GIVE_AUDIO_STATUS        = 0x71,
    CEC_SET_SYSTEM_AUDIO_MODE    = 0x72,
    CEC_REPORT_AUDIO_STATUS      = 0x7A,
    CEC_GIVE_SYSTEM_AUDIO_MODE   = 0x7D,
    CEC_SYSTEM_AUDIO_MODE_STATUS = 0x7E,
    CEC_ROUTING_CHANGE           = 0x80,
    CEC_ROUTING_INFORMATION      = 0x81,
    CEC_ACTIVE_SOURCE            = 0x82,
    CEC_GIVE_PHYSICAL_ADDRESS    = 0x83,
    CEC_REPORT_PHYSICAL_ADDRESS  = 0x84,
    CEC_REQUEST_ACTIVE_SOURCE    = 0x85,
    CEC_SET_STREAM_PATH          = 0x86,
    CEC_DEVICE_VENDOR_ID         = 0x87,
    CEC_VENDOR_COMMAND           = 0x89,
    CEC_REMOTE_BUTTON_DOWN       = 0x8A,
    CEC_REMOTE_BUTTON_UP         = 0x8B,
    CEC_GET_DEVICE_VENDOR_ID     = 0x8C,
    CEC_MENU_REQUEST             = 0x8D,
    CEC_MENU_STATUS              = 0x8E,
    CEC_GIVE_DEVICE_POWER_STATUS = 0x8F,
    CEC_REPORT_POWER_STATUS      = 0x90,
    CEC_GET_MENU_LANGUAGE        = 0x91,
    CEC_SELECT_ANALOGUE_SERVICE  = 0x92,
    CEC_SELECT_DIGITAL_SERVICE   = 0x93,
    CEC_SET_DIGITAL_TIMER        = 0x97,
    CEC_CLEAR_DIGITAL_TIMER      = 0x99,
    CEC_SET_AUDIO_RATE           = 0x9A,
    CEC_VERSION                  = 0x9E,
    CEC_GET_VERSION              = 0x9F,
    CEC_INACTIVE_SOURCE          = 0x9D,
    CEC_VENDOR_COMMAND_WITH_ID   = 0xA0,
    CEC_CLEAR_EXTERNAL_TIMER     = 0xA1,
    CEC_SET_EXTERNAL_TIMER       = 0xA2,
    CEC_ABORT_MESSAGE            = 0xFF,
} cec_code_t;

enum cec_abort_reason {
    CEC_ABORT_UNRECOGNIZED_OPCODE = 0,
    CEC_ABORT_INCORRECT_MODE = 1,
    CEC_ABORT_NO_SOURCE = 2,
    CEC_ABORT_INVALID_OPERAND = 3,
    CEC_ABORT_REFUSED = 4,
};

enum cec_version {
    CEC_VERSION_11  = 0x00,
    CEC_VERSION_12  = 0x01,
    CEC_VERSION_12a = 0x02,
    CEC_VERSION_13  = 0x03,
    CEC_VERSION_13a = 0x04,
};

enum cec_menu_request_type {
    CEC_MENU_ACTIVATE = 0,
    CEC_MENU_DEACTIVATE = 1,
    CEC_MENU_QUERY = 2,
};

enum cec_power_status {
    CEC_POWER_ON = 0,
    CEC_POWER_STANDBY = 1,
    CEC_POWER_TRANSITION_TO_ON = 2,
    CEC_POWER_TRANSITION_TO_OFF = 3,
};

typedef struct cec_code_type {
    cec_tx_flags_t ct_tx_flags;
} cec_code_type_t;
