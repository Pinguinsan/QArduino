
#ifndef ARDUINOPC_ARDUINOPCSTRINGS_H
#define ARDUINOPC_ARDUINOPCSTRINGS_H

#include <QString>
#include <QFile>
#include <QDir>
#include <string>
#include <vector>

namespace ArduinoPCStrings
{
    extern const char *CONFIG_FILE_NAME;

    #if defined(_WIN32) || defined(__CYGWIN__)
        extern const char *DEFAULT_CONFIGURATION_FILE;
        extern const char *BACKUP_CONFIGURATION_FILE;
        extern const char *LAST_CHANCE_CONFIGURATION_FILE;
    #else
        extern const char *DEFAULT_CONFIGURATION_FILE;
        extern const char *BACKUP_CONFIGURATION_FILE;
        extern const char *LAST_CHANCE_CONFIGURATION_FILE;
    #endif

#if (defined(_WIN32) || defined(__CYGWIN__))
    extern const char *OPEN_SCRIPT_FILE_DEFAULT_DIRECTORY;
    extern const char *BACKUP_OPEN_SCRIPT_FILE_DIRECTORY;
    extern const char *LAST_CHANCE_OPEN_SCRIPT_FILE_DIRECTORY;
#else
    extern const char *OPEN_SCRIPT_FILE_DEFAULT_DIRECTORY;
    extern const char *BACKUP_OPEN_SCRIPT_FILE_DIRECTORY;
    extern const char *LAST_CHANCE_OPEN_SCRIPT_FILE_DIRECTORY;
#endif

    extern const char *PIN_IN_HIGH_STATE_STRING;
    extern const char *PIN_IN_LOW_STATE_STRING;
    extern const char *PIN_IS_PULLED_UP_LABEL_STRING;
    extern const char *PIN_IS_NOT_PULLED_UP_LABEL_STRING;
    extern const char *NATIVE_LINUX_SERIAL_PORT_NAME_BASE_STRING;
    extern const char *NO_SERIAL_PORT_NAME_SPECIFIED_STRING;
    extern const char *INVALID_SERIAL_PORT_NAME_STRING;
    extern const char *HEXADECIMAL_BASE_STRING;
    extern const char *BLUETOOTH_SERIAL_IDENTIFIER;
    extern const char *CLOSE_APPLICATION_WINDOW_TITLE;
    extern const char *CLOSE_APPLICATION_WINDOW_PROMPT;
    extern const char *MAIN_WINDOW_TITLE;
    extern const char *MANUAL_SCREEN_TITLE;
    extern const char *KNOWN_BAD_SERIAL_PORT_NAME;
    extern const char *SERIAL_SCRIPT_HAS_ERRORS_MESSAGE_BOX_TITLE;
    extern const char *ANALOG_TO_DIGITAL_PB_RAW_TOOLTIP;
    extern const char *ANALOG_TO_DIGITAL_PB_NORMAL_TOOLTIP;
    extern const char *SUCCESSFULLY_FETCHED_ANALOG_TO_DIGITAL_THRESHOLD_STRING;
    extern const char *COULD_NOT_FETCH_ANALOG_TO_DIGITAL_THRESHOLD_STRING;
    extern const char *EXCEPTION_CAUGHT_WHILE_FETCHING_ANALOG_TO_DIGITAL_THRESHOLD_STRING;
    extern const char *FAILED_TO_CHANGE_ANALOG_TO_DIGITAL_THRESHOLD_STRING;
    extern const char *SUCCESSFULLY_CHANGED_ANALOG_TO_DIGITAL_THRESHOLD_STRING;
    extern const char *EXCEPTION_TRYING_TO_CHANGE_ANALOG_TO_DIGITAL_THRESHOLD_STRING;
    extern const char *ANALOG_TO_DIGITAL_PB_RAW_STYLESHEET;
    extern const char *ANALOG_TO_DIGITAL_PB_NORMAL_STYLESHEET;
    extern const char *ANALOG_IO_DIGITAL_STATE_HIGH_STYLESHEET;
    extern const char *ANALOG_IO_DIGITAL_STATE_LOW_STYLESHEET;
    extern const char *ANALOG_IO_DIGITAL_PB_RAW_TEXT;
    extern const char *ANALOG_IO_DIGITAL_PB_NORMAL_TEXT;
    extern const char *GENERIC_FAILED_STRING;
    extern const char *GENERIC_SUCCESS_STRING;
    extern const char *ELIPSES_STRING;
    extern const char *SETTING_IO_TYPE_STRING;
    extern const char *TO_IO_TYPE_STRING;
    extern const char *CHANGE_ALIAS_ACTION_STRING;
    extern const char *CHANGE_IO_TYPE_TO_ACTION_STRING;
    extern const char *FAILED_TO_CHANGE_ALIAS_STRING;
    extern const char *SUCCESSFULLY_CHANGED_ALIAS_STRING;
    extern const char *FAILED_TO_CHANGE_IO_TYPE_STRING;
    extern const char *SUCCESSFULLY_CHANGED_IO_TYPE_STRING;
    extern const char *INVALID_IO_TYPE_PASSED_TO_FIND_CORRECT_STRING;
    extern const char *NONEXISTANT_PIN_PASSED_TO_FIND_CORRECT_STRING;

    extern const char *RED_COLOR_STRING;
    extern const char *BLUE_COLOR_STRING;
    extern const char *GREEN_COLOR_STRING;
    extern const char *GRAY_COLOR_STRING;
    extern const char *ORANGE_COLOR_STRING;
    extern const char *ALIAS_CHANGE_DIALOG_WINDOW_TITLE;
    extern const char *NULL_ARDUINO_PASSED_TO_ADD_ARDUINO_STRING;

    extern const char *CAN_EMPTY_READ_SUCCESS_STRING;
    extern const char *BLUETOOTH_MULTIPLIER_IDENTIFIER;
    extern const char *ALIAS_IDENTIFIER;
    extern const char *IO_TYPE_IDENTIFIER;
    extern const char *INITIAL_STATE_IDENTIFIER;
    extern const char *ARDUINO_UNO_IDENTIFIER;
    extern const char *ARDUINO_NANO_IDENTIFIER;
    extern const char *ARDUINO_MEGA_IDENTIFIER;
    extern const char *NO_CONFIGURATION_FILE_WARNING_STRING;
    extern const char *UNABLE_TO_OPEN_CONFIGURATION_FILE_STRING_BASE;
    extern const char *UNABLE_TO_OPEN_CONFIGURATION_FILE_STRING_TAIL;
    extern const char *TRYING_BACKUP_FILE_STRING;
    extern const char *FALL_BACK_ON_DEFAULTS_STRING;
    extern const char *ERROR_OPENING_SERIAL_PORT_STRING;
    extern const char *ERROR_OPENING_SERIAL_PORT_WINDOW_TITLE;
    extern const char *NO_SERIAL_PORTS_FOUND_STRING;
    extern const char *NO_SERIAL_PORTS_FOUND_WINDOW_TITLE;
    extern const char *SUCCESSFULLY_OPENED_SERIAL_PORT_STRING;
    extern const char *MANUAL_SCREEN_WINDOW_TITLE_BASE;
    extern const char *CAN_TERMINAL_WINDOW_TITLE_BASE;
    extern const char *MAIN_WINDOW_TITLE;
    extern const char *NO_CLOSING_PARENTHESIS_FOUND_STRING;
    extern const char *NO_CLOSING_QUOTATION_MARKS_FOUND_STRING;
    extern const char *NO_PARAMETER_SEPARATING_COMMA_STRING;
    extern const char *NO_ALIAS_NAME_SPECIFIED_STRING;

    extern const char *NO_BLUETOOTH_MULTIPLIER_SPECIFIED_STRING;
    extern const char *BLUETOOTH_MULTIPLIER_IS_OUT_OF_RANGE_STRING;
    extern const char *BLUETOOTH_MULTIPLIER_IS_NOT_A_NUMBER_STRING;
    extern const char *NO_STATE_SPECIFIED_STRING;
    extern const char *NO_IO_TYPE_SPECIFIED_STRING;
    extern const char *EXPECTED_HERE_STRING;
    extern const char *HERE_STRING;
    extern const char *ALIASES_MUST_BE_IN_QUOTATIONS_STRING;
    extern const char *PIN_NUMBER_NOT_AN_INTEGER_STRING;
    extern const char *MAIN_WINDOW_STYLESHEET;
    extern const char *GENERIC_CONFIG_WARNING_BASE_STRING;
    extern const char *GENERIC_CONFIG_WARNING_TAIL_STRING;
    extern const char *CONFIG_EXPRESSION_MALFORMED_STRING;
    extern const char *EXCEPTION_IN_CONSTRUCTOR_STRING;
    extern const char *LOADING_PROGRAM_WIDGET_WINDOW_TITLE;
    extern const char *SERIAL_TERMINAL_WINDOW_TITLE_BASE;
    extern const char *LOAD_SCRIPT_BUTTON_TEXT_STRING;
    extern const char *OPEN_SCRIPT_FILE_CAPTION;
    extern const char *OPEN_SCRIPT_FILE_DEFAULT_DIRECTORY;

    extern const char *COULD_NOT_FETCH_ARDUINO_STRING;
    extern const char *UNKNOWN_ARDUINO_TYPE_RECEIVED_STRING;
    extern const char *SUCCESSFULLY_FETCHED_ARDUINO_STRING;
    extern const char *DID_NOT_RECEIVE_RESPONSE_FROM_SERIAL_PORT_STRING;
    extern const char *EXCEPTION_CAUGHT_WHILE_FETCHING_ARDUINO_STRING;
    extern const char *SUCCESSFULLY_REMOVED_ALL_POSITIVE_CAN_MASKS_STRING;
    extern const char *FAILED_TO_REMOVE_ALL_POSITIVE_CAN_MASKS_STRING;
    extern const char *SUCCESSFULLY_REMOVED_ALL_NEGATIVE_CAN_MASKS_STRING;
    extern const char *FAILED_TO_REMOVE_ALL_NEGATIVE_CAN_MASKS_STRING;
    extern const char *NTH_DATA_PACKET_BYTE_INDEX_OUT_OF_RANGE;

    extern const char *SUCCESSFULLY_FETCHED_FIRMWARE_VERSION_STRING;
    extern const char *COULD_NOT_FETCH_FIRMWARE_VERSION_STRING;
    extern const char *EXCEPTION_CAUGHT_WHILE_FETCHING_FIRMWARE_VERSION_STRING;
    extern const char *SUCCESSFULLY_FETCHED_CAN_CAPABILITY_STRING;
    extern const char *COULD_NOT_FETCH_CAN_CAPABILITY_STRING;
    extern const char *EXCEPTION_CAUGHT_WHILE_FETCHING_CAN_CAPABILITY_STRING;
    extern const char *INVALID_ARDUINO_PTR_AT_INDEX_STRING;
    extern const char *CAN_BYTES_LINE_EDIT_ZERO_STRING;
    extern const char *CAN_ID_LINE_EDIT_ZERO_STRING;
    extern const char *NTH_DATA_PACKET_BYTE_INDEX_OUT_OF_RANGE_STRING;

    extern const char *STD_EXCEPTION_IN_IO_REPORT_STRING;
    extern const char *STD_EXCEPTION_IN_SERIAL_REPORT_STRING;
    extern const char *STD_EXCEPTION_IN_CAN_REPORT_STRING;
    extern const char *IO_REPORT_FUTURE_NOT_STARTED_ERROR_STRING;
    extern const char *IO_REPORT_FUTURE_ALREADY_STARTED_ERROR_STRING;
    extern const char *IO_REPORT_FUTURE_NOT_COMPLETED_ERROR_STRING;
    extern const char *SERIAL_TERMINAL_FUTURE_NOT_STARTED_ERROR_STRING;
    extern const char *SERIAL_TERMINAL_FUTURE_ALREADY_STARTED_ERROR_STRING;
    extern const char *SERIAL_TERMINAL_FUTURE_NOT_COMPLETED_ERROR_STRING;
    extern const char *CAN_TERMINAL_FUTURE_NOT_STARTED_ERROR_STRING;
    extern const char *CAN_TERMINAL_FUTURE_ALREADY_STARTED_ERROR_STRING;
    extern const char *CAN_TERMINAL_FUTURE_NOT_COMPLETED_ERROR_STRING;
    extern const char *PINS_BY_SCREEN_INDEX_INVALID_INDEX_STRING;

    extern const char *SUCCESSFULLY_ADDED_POSITIVE_CAN_MASK_STRING;
    extern const char *FAILED_TO_ADD_POSITIVE_CAN_MASK_STRING;
    extern const char *SUCCESSFULLY_ADDED_NEGATIVE_CAN_MASK_STRING;
    extern const char *FAILED_TO_ADD_NEGATIVE_CAN_MASK_STRING;
    extern const char *SUCCESSFULLY_REMOVED_POSITIVE_CAN_MASK_STRING;
    extern const char *FAILED_TO_REMOVE_POSITIVE_CAN_MASK_STRING;
    extern const char *SUCCESSFULLY_REMOVED_NEGATIVE_CAN_MASK_STRING ;
    extern const char *FAILED_TO_REMOVE_NEGATIVE_CAN_MASK_STRING;

    extern const char *ADD_SCREEN_FLAGGED_IO_INDEX_ALREADY_EXISTS_STRING;
    extern const char *CLEAR_PINS_BY_SCREEN_INDEX_INVALID_INDEX_STRING;
    extern const char *REMOVE_SCREEN_FLAGGED_IO_INVALID_INDEX_STRING;
    extern const char *ADD_PIN_INVALID_INDEX_STRING;

    extern const char *IO_REPORT_BY_SCREEN_INDEX_INVALID_INDEX_STRING;
    extern const char *ADD_REPORT_IO_INDEX_ALREADY_EXISTS_STRING;
    extern const char *REMOVE_SCREEN_IO_REPORT_INVALID_INDEX_STRING;

    extern const char *SERIAL_REPORT_BY_SCREEN_INDEX_INVALID_INDEX_STRING;
    extern const char *ADD_REPORT_SERIAL_INDEX_ALREADY_EXISTS_STRING;
    extern const char *REMOVE_SCREEN_SERIAL_REPORT_INVALID_INDEX_STRING;

    extern const char *CAN_REPORT_BY_SCREEN_INDEX_INVALID_INDEX_STRING;
    extern const char *ADD_REPORT_CAN_INDEX_ALREADY_EXISTS_STRING;
    extern const char *REMOVE_SCREEN_CAN_REPORT_INVALID_INDEX_STRING;

    extern const char *CAN_TERMINAL_BY_SCREEN_INDEX_INVALID_INDEX_STRING;
    extern const char *ADD_REPORT_CAN_TERMINAL_INDEX_ALREADY_EXISTS_STRING;
    extern const char *REMOVE_SCREEN_CAN_TERMINAL_INVALID_INDEX_STRING;

    extern const char *SERIAL_TERMINAL_BY_SCREEN_INDEX_INVALID_INDEX_STRING;
    extern const char *REMOVE_SCREEN_SERIAL_TERMINAL_INVALID_INDEX_STRING;
    extern const char *ADD_REPORT_SERIAL_TERMINAL_INDEX_ALREADY_EXISTS_STRING;

    extern const char *TIMER_BY_SCREEN_INDEX_INVALID_INDEX_STRING;
    extern const char *ADD_TIMER_ALREADY_EXISTS_STRING;
    extern const char *REMOVE_SCREEN_TIMER_INVALID_INDEX_STRING;

    extern const char *ARDUINO_BY_SCREEN_INDEX_INVALID_INDEX_STRING;
    extern const char *REMOVE_SCREEN_ARDUINO_INVALID_INDEX_STRING;
    extern const char *ADD_ARDUINO_ALREADY_EXISTS_STRING;

    extern const char *MANUAL_SCREEN_BY_SCREEN_INDEX_INVALID_INDEX_STRING;
    extern const char *REMOVE_SCREEN_MANUAL_INVALID_INDEX_STRING;
    extern const char *ADD_MANUAL_ALREADY_EXISTS_STRING;

    extern const char *STATUS_VIEW_BY_SCREEN_INDEX_INVALID_INDEX_STRING;
    extern const char *ADD_SCREEN_IO_VIEW_INDEX_ALREADY_EXISTS_STRING;
    extern const char *REMOVE_SCREEN_IO_VIEW_INVALID_INDEX_STRING;

    extern const char *PINS_EXISTS_BY_SCREEN_INDEX_INVALID_INDEX_STRING;
    extern const char *FLAGGED_IO_MUTEX_SCREEN_INDEX_INVALID_STRING;
    extern const char *FLAGGED_SERIAL_TERMINAL_MUTEX_SCREEN_INDEX_INVALID_STRING;
    extern const char *FLAGGED_CAN_TERMINAL_MUTEX_SCREEN_INDEX_INVALID_STRING;

    extern const char *DIGITAL_OUTPUT_LABEL;
    extern const char *DIGITAL_INPUT_LABEL;
    extern const char *ANALOG_INPUT_LABEL;
    extern const char *ANALOG_OUTPUT_LABEL;
    extern const char *PIN_NUMBER_LABEL;
    extern const char *FAILED_SOFT_DIGITAL_READ_STRING;
    extern const char *FAILED_DIGITAL_WRITE_STRING;
    extern const char *FAILED_CURRENT_PIN_MODE_STRING;
    extern const char *FAILED_PIN_MODE_STRING;
    extern const char *FAILED_PULL_UP_STRING;
    extern const char *FAILED_PULL_DOWN_STRING;
    extern const char *SUCCESSFULLY_WRITTEN_LOW_STRING;
    extern const char *SUCCESSFULLY_WRITTEN_HIGH_STRING;
    extern const char *SUCCESSFULLY_PULLED_UP_STRING;
    extern const char *SUCCESSFULLY_PULLED_DOWN_STRING;
    extern const char *INVALID_ANALOG_STATE_BASE_STRING;
    extern const char *INVALID_ANALOG_STATE_WINDOW_TITLE;
    extern const char *ANALOG_OUTPUT_ENTRY_BOX_PENDING_CHANGE_STYLE_SHEET;
    extern const char *ANALOG_OUTPUT_ENTRY_BOX_STABLE_STYLE_SHEET;
    extern const char *ANALOG_WRITE_SUCCESSFUL_BASE_STRING;
    extern const char *ANALOG_WRITE_RAW_SUCCESSFUL_BASE_STRING;
    extern const char *FAILED_ANALOG_WRITE_STRING;
    extern const char *FAILED_ANALOG_WRITE_RAW_STRING;
    extern const char *ANALOG_OUTPUT_STATE_RAW_STRING;
    extern const char *ANALOG_OUTPUT_STATE_NORMAL_STRING;
    extern const char *ANALOG_OUTPUT_STATE_RAW_STYLESHEET;
    extern const char *ANALOG_OUTPUT_STATE_NORMAL_STYLESHEET;

    extern const char *USING_INITIAL_STATE_STRING;
    extern const char *FOR_PIN_NUMBER_STRING;
    extern const char *USING_IO_TYPE_STRING;
    extern const char *USING_ALIAS_STRING;
    extern const char *USING_BLUETOOTH_MULTIPLIER_STRING;

    extern const char *TERMINAL_RECEIVE_BASE_STRING;
    extern const char *TERMINAL_SEND_BASE_STRING;

    extern const char *IO_REPORT_INVALID_DATA_STRING;
    extern const char *CAN_REPORT_INVALID_DATA_STRING;
    extern const char *IO_MUTEX_INVALID_INDEX_STRING;
    extern const char *SERIAL_PORT_INVALID_INDEX_STRING;

    extern const char *DIGITAL_INPUT_PULLUP_UP_STYLESHEET;
    extern const char *DIGITAL_INPUT_PULLUP_DOWN_STYLESHEET;
    extern const char *ARDUINO_UNO_LONG_NAME;
    extern const char *ARDUINO_NANO_LONG_NAME;
    extern const char *ARDUINO_MEGA_LONG_NAME;
    extern const char *PIN_LABEL_BASE;
    extern const char *ANALOG_IO_INITIAL_STATE_LABEL;

    extern const char *DIGITAL_INPUT_IDENTIFIER;
    extern const char *DIGITAL_OUTPUT_IDENTIFIER;
    extern const char *ANALOG_INPUT_IDENTIFIER;
    extern const char *ANALOG_OUTPUT_IDENTIFIER;
    extern const char *DIGITAL_INPUT_PULLUP_IDENTIFIER;
    extern const char *OPERATION_FAILURE_STRING;
    extern const char *OPERATION_SUCCESS_STRING;
    extern const char *CAN_BYTE_LINE_EDIT_REGEX;

    extern const char *SUCCESSFULLY_INITIALIZED_CAN_BUS_STRING;
    extern const char *FAILED_TO_INITIALIZE_CAN_BUS_STRING;
    extern const char *SUCCESSFULLY_SET_CAN_AUTO_UPDATE_STRING;
    extern const char *SUCCESSFULLY_CANCELED_CAN_AUTO_UPDATE_STRING;
    extern const char *FAILED_TO_SET_CAN_AUTO_UPDATE_STRING;
    extern const char *FAILED_TO_CANCEL_CAN_AUTO_UPDATE_STRING;

    extern const char *INVALID_PIN_ALIAS_STRING;
    extern const char *INVALID_ANALOG_PIN_BASE_STRING;
    extern const char *INVALID_GPIO_PIN_STRING;
    extern const char *INVALID_PIN_NUMBER_STRING;

    extern const char *INVALID_PIN_ALIAS_STRING;
    extern const char *INVALID_ANALOG_PIN_BASE_STRING;
    extern const char *INVALID_GPIO_PIN_STRING;
    extern const char *INVALID_PIN_NUMBER_STRING;
    extern const char *UNSPECIFIED_IO_TYPE_TO_PARSE_IO_TYPE_STRING;
    extern const char *INVALID_IO_TYPE_BASE_STRING;
    extern const char *INVALID_IO_TYPE_TAIL_STRING;
    extern const char *INVALID_STATE_TO_PARSE_TO_DIGITAL_STATE_STRING;
    extern const char *INVALID_STATE_TO_PARSE_TO_ANALOG_STATE_STRING;
    extern const char *INVALID_STATE_TO_PARSE_TO_ANALOG_STATE_RAW_STRING;
    extern const char *DETECTED_SERIAL_PORT_STRING;
    extern const char *FIRMWARE_VERSION_UNKNOWN_STRING;
    extern const char *FIRMWARE_VERSION_BASE_STRING;

    extern const char *NULL_GPIO_PTR_TO_DIGITAL_READ_STRING;
    extern const char *NULL_GPIO_PTR_TO_DIGITAL_WRITE_STRING;
    extern const char *NULL_GPIO_PTR_TO_ANALOG_READ_STRING;
    extern const char *NULL_GPIO_PTR_TO_ANALOG_READ_RAW_STRING;
    extern const char *NULL_GPIO_PTR_TO_ANALOG_WRITE_STRING;
    extern const char *NULL_GPIO_PTR_TO_ANALOG_WRITE_RAW_STRING;
    extern const char *NULL_GPIO_PTR_TO_SOFT_DIGITAL_READ_STRING;
    extern const char *NULL_GPIO_PTR_TO_SOFT_ANALOG_READ_STRING;
    extern const char *NULL_GPIO_PTR_TO_SOFT_ANALOG_READ_RAW_STRING;
    extern const char *NULL_GPIO_PTR_TO_PIN_MODE_STRING;
    extern const char *NULL_GPIO_PTR_TO_CURRENT_PIN_MODE_STRING;

    extern const char *PASSED_TO_DIGITAL_READ_STRING_TAIL_STRING;
    extern const char *PASSED_TO_DIGITAL_WRITE_STRING_TAIL_STRING;
    extern const char *PASSED_TO_ANALOG_READ_STRING_TAIL_STRING;
    extern const char *PASSED_TO_ANALOG_READ_RAW_STRING_TAIL_STRING;
    extern const char *PASSED_TO_ANALOG_WRITE_STRING_TAIL_STRING;
    extern const char *PASSED_TO_ANALOG_WRITE_RAW_STRING_TAIL_STRING;
    extern const char *PASSED_TO_SOFT_DIGITAL_READ_STRING_TAIL_STRING;
    extern const char *PASSED_TO_SOFT_ANALOG_READ_STRING_TAIL_STRING;
    extern const char *PASSED_TO_SOFT_ANALOG_READ_RAW_STRING_TAIL_STRING;
    extern const char *PASSED_TO_PIN_MODE_STRING_TAIL_STRING;
    extern const char *PASSED_TO_CURRENT_PIN_MODE_STRING_TAIL_STRING;

    extern const char *PASSED_TO_DIGITAL_READ_GPIO_TAIL_STRING;
    extern const char *PASSED_TO_DIGITAL_WRITE_GPIO_TAIL_STRING;
    extern const char *PASSED_TO_ANALOG_READ_GPIO_TAIL_STRING;
    extern const char *PASSED_TO_ANALOG_READ_RAW_GPIO_TAIL_STRING;
    extern const char *PASSED_TO_ANALOG_WRITE_GPIO_TAIL_STRING;
    extern const char *PASSED_TO_ANALOG_WRITE_RAW_GPIO_TAIL_STRING;
    extern const char *PASSED_TO_SOFT_DIGITAL_READ_GPIO_TAIL_STRING;
    extern const char *PASSED_TO_SOFT_ANALOG_READ_GPIO_TAIL_STRING;
    extern const char *PASSED_TO_SOFT_ANALOG_READ_RAW_GPIO_TAIL_STRING;
    extern const char *PASSED_TO_PIN_MODE_GPIO_TAIL_STRING;
    extern const char *PASSED_TO_CURRENT_PIN_MODE_GPIO_TAIL_STRING;

    extern const char *UNO_INVALID_ANALOG_STRING_TAIL_STRING;
    extern const char *UNO_INVALID_ANALOG_INT_TAIL_STRING;

    extern const char *NANO_INVALID_ANALOG_STRING_TAIL_STRING;
    extern const char *NANO_INVALID_ANALOG_INT_TAIL_STRING;

    extern const char *MEGA_INVALID_ANALOG_STRING_TAIL_STRING;
    extern const char *MEGA_INVALID_ANALOG_INT_TAIL_STRING;

    extern const char *TERMINAL_RECEIVE_BASE_STRING;
    extern const char *TERMINAL_TRANSMIT_BASE_STRING;
    extern const char *TERMINAL_DELAY_BASE_STRING;
    extern const char *NO_SERIAL_PORTS_CONNECTED_STRING;
    extern const char *CONNECT_TO_SERIAL_PORT_TO_BEGIN_STRING;
    extern const char *EMPTY_SCRIPT_STRING;
    extern const char *FILE_DOES_NOT_EXIST_STRING;
    extern const char *FILE_DOES_NOT_EXIST_WINDOW_TITLE_STRING;
    extern const char *EMPTY_SCRIPT_WINDOW_TITLE_STRING;
    extern const char *EXECUTING_SCRIPT_STRING;
    extern const char *FINISHED_EXECUTING_SCRIPT_STRING;

    extern const char *DELAY_IDENTIFIER;
    extern const char *DELAY_SECONDS_IDENTIFIER;
    extern const char *DELAY_MILLISECONDS_IDENTIFIER;
    extern const char *DELAY_MICROSECONDS_IDENTIFIER;
    extern const char *WRITE_IDENTIFIER;
    extern const char *READ_IDENTIFIER;
    extern const char *SECONDS_SUFFIX_STRING;
    extern const char *MILLISECONDS_SUFFIX_STRING;
    extern const char *MICROSECONDS_SUFFIX_STRING;
    extern const char *ACTION_LOAD_SCRIPT_ENABLED_TOOLTIP;
    extern const char *ACTION_LOAD_SCRIPT_DISABLED_TOOLTIP;
    extern const char *SEND_BOX_ENABLED_TOOLTIP;
    extern const char *SEND_BOX_DISABLED_TOOLTIP;

    extern const char *NO_CLOSING_PARENTHESIS_FOUND_STRING;
    extern const char *NO_CLOSING_QUOTATION_MARKS_FOUND_STRING;
    extern const char *NO_PARAMETER_SEPARATING_COMMA_STRING;
    extern const char *EXPECTED_HERE_STRING;
    extern const char *HERE_STRING;
    extern const char *WRITE_PARAMETER_MUST_BE_IN_QUOTATIONS_STRING;
    extern const char *DELAY_SECONDS_PARAMETER_NOT_AN_INTEGER_STRING;
    extern const char *DELAY_MILLISECONDS_PARAMETER_NOT_AN_INTEGER_STRING;
    extern const char *DELAY_MICROSECONDS_PARAMETER_NOT_AN_INTEGER_STRING;
    extern const char *GENERIC_CONFIG_WARNING_BASE_STRING;
    extern const char *GENERIC_CONFIG_WARNING_TAIL_STRING;
    extern const char *CONFIG_EXPRESSION_MALFORMED_STRING;
    extern const char *EXCEPTION_IN_CONSTRUCTOR_STRING;
    extern const char *SERIAL_COMMAND_TYPE_NOT_IMPLEMENTED_STRING;
    extern const char *NULL_SERIAL_TERMINAL_PASSED_TO_EXECUTE_STRING;
    extern const char *NULL_MAIN_WINDOW_PASSED_TO_EXECUTE_STRING;
    extern const char *SCRIPT_FILE_DOES_NOT_EXISTS_STRING;
    extern const char *UNABLE_TO_OPEN_SCRIPT_FILE_STRING;

    extern const char *TERMINAL_RECEIVE_BASE_STRING;
    extern const char *TERMINAL_TRANSMIT_BASE_STRING;
    extern const char *TERMINAL_DELAY_BASE_STRING;
    extern const char *TERMINAL_FLUSH_RX_BASE_STRING;
    extern const char *TERMINAL_FLUSH_TX_BASE_STRING;
    extern const char *TERMINAL_FLUSH_RX_TX_BASE_STRING;
    extern const char *NO_SERIAL_PORTS_CONNECTED_STRING;
    extern const char *CONNECT_TO_SERIAL_PORT_TO_BEGIN_STRING;
    extern const char *EMPTY_SCRIPT_STRING;
    extern const char *FILE_DOES_NOT_EXIST_STRING;
    extern const char *FILE_DOES_NOT_EXIST_WINDOW_TITLE_STRING;
    extern const char *EMPTY_SCRIPT_WINDOW_TITLE_STRING;
    extern const char *EXECUTING_SCRIPT_STRING;
    extern const char *FINISHED_EXECUTING_SCRIPT_STRING;
    extern const char *OPEN_SCRIPT_FILE_CAPTION;

    extern const char *DELAY_IDENTIFIER;
    extern const char *DELAY_SECONDS_IDENTIFIER;
    extern const char *DELAY_MILLISECONDS_IDENTIFIER;
    extern const char *DELAY_MICROSECONDS_IDENTIFIER;
    extern const char *WRITE_IDENTIFIER;
    extern const char *READ_IDENTIFIER;
    extern const char *LOOP_IDENTIFIER;
    extern const char *FLUSH_RX_IDENTIFIER;
    extern const char *FLUSH_TX_IDENTIFIER;
    extern const char *FLUSH_TX_RX_IDENTIFIER;
    extern const char *FLUSH_RX_TX_IDENTIFIER;
    extern const char *SECONDS_SUFFIX_STRING;
    extern const char *MILLISECONDS_SUFFIX_STRING;
    extern const char *MICROSECONDS_SUFFIX_STRING;
    extern const char *ACTION_LOAD_SCRIPT_ENABLED_TOOLTIP;
    extern const char *ACTION_LOAD_SCRIPT_DISABLED_TOOLTIP;
    extern const char *SEND_BOX_ENABLED_TOOLTIP;
    extern const char *SEND_BOX_DISABLED_TOOLTIP;

    extern const char *NO_CLOSING_PARENTHESIS_FOUND_STRING;
    extern const char *NO_CLOSING_QUOTATION_MARKS_FOUND_STRING;
    extern const char *NO_PARAMETER_SEPARATING_COMMA_STRING;
    extern const char *EXPECTED_HERE_STRING;
    extern const char *HERE_STRING;
    extern const char *WRITE_PARAMETER_MUST_BE_IN_QUOTATIONS_STRING;
    extern const char *DELAY_SECONDS_PARAMETER_NOT_AN_INTEGER_STRING;
    extern const char *DELAY_MILLISECONDS_PARAMETER_NOT_AN_INTEGER_STRING;
    extern const char *DELAY_MICROSECONDS_PARAMETER_NOT_AN_INTEGER_STRING;
    extern const char *GENERIC_CONFIG_WARNING_BASE_STRING;
    extern const char *GENERIC_CONFIG_WARNING_TAIL_STRING;
    extern const char *CONFIG_EXPRESSION_MALFORMED_STRING;
    extern const char *EXCEPTION_IN_CONSTRUCTOR_STRING;
    extern const char *SERIAL_COMMAND_TYPE_NOT_IMPLEMENTED_STRING;
    extern const char *NULL_SERIAL_TERMINAL_PASSED_TO_EXECUTE_STRING;
    extern const char *NULL_MAIN_WINDOW_PASSED_TO_EXECUTE_STRING;
    extern const char *SCRIPT_FILE_DOES_NOT_EXISTS_STRING;
    extern const char *UNABLE_TO_OPEN_SCRIPT_FILE_STRING;
    extern const char *LOOP_COUNT_PARAMETER_NOT_AN_INTEGER_STRING;
    extern const char *UNTERMINATED_LOOP_STRING;
    extern const char *UNEXPECTED_LOOP_CLOSING_STRING;
    extern const char *CLOSING_LOOP_IDENTIFIER;
    extern const char *BEGINNING_INFINITE_LOOP_STRING;
    extern const char *INFINITE_LOOP_COUNT_TAIL_STRING;
    extern const char *LOOPS_TAIL_STRING;
    extern const char *BEGINNING_LOOPS_BASE_STRING;
    extern const char *ENDING_LOOPS_BASE_STRING;
    extern const char *BEGIN_LOOP_BASE_STRING;
    extern const char *END_LOOP_BASE_STRING;
    extern const char *CANCEL_SCRIPT_STRING;
    extern const char *SEND_STRING;
    extern const char *CANCELED_EXECUTING_SCRIPT_STRING;
    extern const char *COMMAND_HISTORY_CONTEXT_MENU_STRING;

}

#endif // ARDUINOPC_ARDUINOPCSTRINGS_H
