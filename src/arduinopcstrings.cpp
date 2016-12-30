#include "arduinopcstrings.h"

namespace ArduinoPCStrings
{
    const char *CONFIG_FILE_NAME{"arduinopc.config"};

    #if defined(_WIN32) || defined(__CYGWIN__)
        const char *DEFAULT_CONFIGURATION_FILE{"C:/Program Files (x86)/ArduinoPC/config/arduinopc.config"};
        const char *BACKUP_CONFIGURATION_FILE{"C:/Cygwin64/opt/GitHub/ArduinoPC/config/arduinopc.config"};
        const char *LAST_CHANCE_CONFIGURATION_FILE{"D:/GitHub/ArduinoPC/config/arduinopc.config"};
    #else
        const char *DEFAULT_CONFIGURATION_FILE{static_cast<std::string>(static_cast<std::string>(getenv("HOME")) + "/.local/arduinopc/config/arduinopc.config").c_str()};
        const char *BACKUP_CONFIGURATION_FILE{"/usr/share/arduinopc/config/arduinopc.config"};
        const char *LAST_CHANCE_CONFIGURATION_FILE{"/opt/GitHub/ArduinoPC/config/arduinopc.config"};
    #endif

#if (defined(_WIN32) || defined(__CYGWIN__))
    const char *OPEN_SCRIPT_FILE_DEFAULT_DIRECTORY{"C:/Program Files(x86)/ArduinoPC/config/"};
    const char *BACKUP_OPEN_SCRIPT_FILE_DIRECTORY{"C:/Cygwin64/opt/GitHub/ArduinoPC/scripts/"};
    const char *LAST_CHANCE_OPEN_SCRIPT_FILE_DIRECTORY{"/opt/GitHub/ArduinoPC/scripts"};
#else
    const char *OPEN_SCRIPT_FILE_DEFAULT_DIRECTORY{static_cast<std::string>(static_cast<std::string>(getenv("HOME")) + "/.local/arduinopc/scripts/").c_str()};
    const char *BACKUP_OPEN_SCRIPT_FILE_DIRECTORY{"/usr/share/ArduinoPC/scripts/"};
    const char *LAST_CHANCE_OPEN_SCRIPT_FILE_DIRECTORY{"/opt/GitHub/ArduinoPC/scripts/"};
#endif

    const char *PIN_IN_HIGH_STATE_STRING{"This pin is currently written high (5v)"};
    const char *PIN_IN_LOW_STATE_STRING{"This pin is currently written low (0v)"};
    const char *PIN_IS_PULLED_UP_LABEL_STRING{"This pin is in pull up mode (via internal 10k resistor)"};
    const char *PIN_IS_NOT_PULLED_UP_LABEL_STRING{"This pin is in floating mode (not pulled up)"};
    const char *NATIVE_LINUX_SERIAL_PORT_NAME_BASE_STRING{"/dev/ttyS"};
    const char *NO_SERIAL_PORT_NAME_SPECIFIED_STRING{"WARNING: Ignore serial port switch accepted, but no serial port name was specified, skipping option"};
    const char *INVALID_SERIAL_PORT_NAME_STRING{"WARNING: Invalid serial port name specified after ignore serial port switch, skipping option ("};
    const char *BLUETOOTH_SERIAL_IDENTIFIER{"rfcomm"};
    const char *CLOSE_APPLICATION_WINDOW_TITLE{"Quit ArduinoPC?"};
    const char *CLOSE_APPLICATION_WINDOW_PROMPT{"Are you sure you'd like to quit?"};
    const char *MAIN_WINDOW_TITLE{"ArduinoPC - Main Screen"};
    const char *MANUAL_SCREEN_TITLE{"ArduinoPC - Manual IO Screen - "};
    const char *SERIAL_TERMINAL_WINDOW_TITLE_BASE{"ArduinoPC - Serial Terminal - "};
    const char *MANUAL_SCREEN_WINDOW_TITLE_BASE{"ArduinoPC - Manual IO Screen - "};
    const char *CAN_TERMINAL_WINDOW_TITLE_BASE{"ArduinoPC - CAN Terminal Screen - "};
    const char *KNOWN_BAD_SERIAL_PORT_NAME{"/dev/ttyS"};
    const char *SERIAL_SCRIPT_HAS_ERRORS_MESSAGE_BOX_TITLE{"Script Contains Errors"};
    const char *GENERIC_FAILED_STRING{"failed"};
    const char *GENERIC_SUCCESS_STRING{"success"};
    const char *ELIPSES_STRING{"..."};
    const char *SETTING_IO_TYPE_STRING{"Setting pin number "};
    const char *TO_IO_TYPE_STRING{" to IO type "};
    const char *HEXADECIMAL_BASE_STRING{"0x"};
    const char *CHANGE_ALIAS_ACTION_STRING{"Change alias"};
    const char *CHANGE_IO_TYPE_TO_ACTION_STRING{"Change IO type to "};
    const char *ALIAS_CHANGE_DIALOG_WINDOW_TITLE{"Change Pin Alias - "};
    const char *FAILED_TO_CHANGE_ALIAS_STRING{"Failed to change alias for pin number "};
    const char *SUCCESSFULLY_CHANGED_ALIAS_STRING{"Successfully changed alias for pin number "};
    const char *FAILED_TO_CHANGE_IO_TYPE_STRING{"Failed to change IO type for pin number "};
    const char *SUCCESSFULLY_CHANGED_IO_TYPE_STRING{"Successfully changed IO type for pin number "};

    const char *ANALOG_TO_DIGITAL_PB_RAW_TOOLTIP{"The analog to digital threshold is being displayed in raw mode (0 - 1024)"};
    const char *ANALOG_TO_DIGITAL_PB_RAW_STYLESHEET{"color: blue"};
    const char *ANALOG_TO_DIGITAL_PB_NORMAL_TOOLTIP{"The analog to digital threshold is being displayed in volts (0 - 5.0)"};
    const char *ANALOG_TO_DIGITAL_PB_NORMAL_STYLESHEET{"color: black"};
    const char *ANALOG_IO_DIGITAL_STATE_HIGH_STYLESHEET{"color: light-green"};
    const char *ANALOG_IO_DIGITAL_STATE_LOW_STYLESHEET{"color: black"};
    const char *ANALOG_IO_DIGITAL_PB_RAW_TEXT{"Raw"};
    const char *ANALOG_IO_DIGITAL_PB_NORMAL_TEXT{"Normal"};
    const char *NULL_ARDUINO_PASSED_TO_ADD_ARDUINO_STRING{"Null std::shared_ptr passed to ArduinoFactory::addArduino(int, std::shared_ptr<Arduino>)"};
    const char *SUCCESSFULLY_FETCHED_ANALOG_TO_DIGITAL_THRESHOLD_STRING{"Successfully fetched analog to digital threshold: "};
    const char *COULD_NOT_FETCH_ANALOG_TO_DIGITAL_THRESHOLD_STRING{"Could not fetch analog to digital threshold"};
    const char *EXCEPTION_CAUGHT_WHILE_FETCHING_ANALOG_TO_DIGITAL_THRESHOLD_STRING{"WARNING: An exception was caught while trying to fetch analog to digital threshold, the attached Arduino may not be available ("};
    const char *FAILED_TO_CHANGE_ANALOG_TO_DIGITAL_THRESHOLD_STRING{"Failed to change analog to digital threshold"};
    const char *SUCCESSFULLY_CHANGED_ANALOG_TO_DIGITAL_THRESHOLD_STRING{"Successfully changed analog to digital threshold to "};
    const char *EXCEPTION_TRYING_TO_CHANGE_ANALOG_TO_DIGITAL_THRESHOLD_STRING{"Exception caught while trying to change analog to digital threshold"};
    const char *INVALID_IO_TYPE_PASSED_TO_FIND_CORRECT_STRING{"Invalid IOType passed to findCorrectInsertionLocation(int, IOType): "};
    const char *NONEXISTANT_PIN_PASSED_TO_FIND_CORRECT_STRING{"Nonexistant pin number passed to findCorrectInsertionLocation(int, IOType): "};

    const char *RED_COLOR_STRING{"red"};
    const char *BLUE_COLOR_STRING{"blue"};
    const char *GREEN_COLOR_STRING{"green"};
    const char *GRAY_COLOR_STRING{"gray"};
    const char *ORANGE_COLOR_STRING{"orange"};

    const char *CAN_EMPTY_READ_SUCCESS_STRING{"{canread:1}"};
    const char *BLUETOOTH_MULTIPLIER_IDENTIFIER{"setbluetoothmultiplier("};
    const char *ALIAS_IDENTIFIER{"setalias("};
    const char *IO_TYPE_IDENTIFIER{"setiotype("};
    const char *INITIAL_STATE_IDENTIFIER{"setinitialstate("};
    const char *ARDUINO_UNO_IDENTIFIER{"arduino_uno"};
    const char *ARDUINO_MEGA_IDENTIFIER{"arduino_mega"};
    const char *ARDUINO_NANO_IDENTIFIER{"arduino_nano"};
    const char *NO_CONFIGURATION_FILE_WARNING_STRING{"WARNING: No configuration file found"};
    const char *UNABLE_TO_OPEN_CONFIGURATION_FILE_STRING_BASE{"WARNING: unable to open configuration file "};
    const char *UNABLE_TO_OPEN_CONFIGURATION_FILE_STRING_TAIL{", but the file exists (maybe a permission problem?), "};
    const char *TRYING_BACKUP_FILE_STRING{"trying backup file "};
    const char *FALL_BACK_ON_DEFAULTS_STRING{"falling back on default pin assignments"};
    const char *ERROR_OPENING_SERIAL_PORT_STRING{"WARNING: An exception was caught while trying to open serial port, the attached Arduino will not be available ("};
    const char *ERROR_OPENING_SERIAL_PORT_WINDOW_TITLE{"WARNING: Serial Port Open Failure"};
    const char *NO_SERIAL_PORTS_FOUND_STRING{"ERROR: No serial ports found, exiting program"};
    const char *NO_SERIAL_PORTS_FOUND_WINDOW_TITLE{"ERROR: No Serial Ports"};
    const char *SUCCESSFULLY_OPENED_SERIAL_PORT_STRING{"Successfully opened serial port "};
    const char *NO_CLOSING_PARENTHESIS_FOUND_STRING{"    No matching parenthesis found, ignoring option"};
    const char *NO_CLOSING_QUOTATION_MARKS_FOUND_STRING{"    No matching quotation marks found, ingoring option"};
    const char *NO_PARAMETER_SEPARATING_COMMA_STRING{"    No parameter separating comma found, ignoring option"};
    const char *NO_ALIAS_NAME_SPECIFIED_STRING{"    No alias name specified after pin number, ignoring option"};
    const char *NO_BLUETOOTH_MULTIPLIER_SPECIFIED_STRING{"    No bluetooth multiplier specified, ignoring option"};
    const char *BLUETOOTH_MULTIPLIER_IS_OUT_OF_RANGE_STRING{"    Bluetooth multiplier is out of valid range (0 < multiplier < 10), ignoring option"};
    const char *BLUETOOTH_MULTIPLIER_IS_NOT_A_NUMBER_STRING{"    Bluetooth multiplier is not a number, ignoring option"};
    const char *NO_STATE_SPECIFIED_STRING{"    No state specified after pin number, ignoring option"};
    const char *NO_IO_TYPE_SPECIFIED_STRING{"    No io type specified after pin number, ignoring option"};
    const char *EXPECTED_HERE_STRING{"^---expected here"};
    const char *HERE_STRING{"^---here"};
    const char *ALIASES_MUST_BE_IN_QUOTATIONS_STRING{"    Aliases must be enclosed in parentheses, ignoring option"};
    const char *PIN_NUMBER_NOT_AN_INTEGER_STRING{"    Pin number is not an integer, ignoring option"};
    const char *MAIN_WINDOW_STYLESHEET{"DIOStatePushButton { background-color: rgba(255, 255, 255, 0); }"
                                       "PullupStatePushButton { background-color: rgba(255, 255, 255, 0); }"
                                       "QLabel { background-color: rgba(255, 255, 255, 0); }"};
    const char *GENERIC_CONFIG_WARNING_BASE_STRING{"ERROR: line "};
    const char *GENERIC_CONFIG_WARNING_TAIL_STRING{" of configuration file:"};
    const char *CONFIG_EXPRESSION_MALFORMED_STRING{"    expression is malformed/has invalid syntax, ignoring option"};

    const char *EXCEPTION_IN_CONSTRUCTOR_STRING{"Standard exception caught in ConfigurationFileReader constructor: "};
    const char *COULD_NOT_FETCH_ARDUINO_STRING{"Could not fetch Arduino type"};
    const char *UNKNOWN_ARDUINO_TYPE_RECEIVED_STRING{"WARNING: Unknown arduino type returned from serial port while initializing IO, the attached Arduino will not be available ("};
    const char *SUCCESSFULLY_FETCHED_ARDUINO_STRING{"Successfully fetched Arduino type: "};
    const char *DID_NOT_RECEIVE_RESPONSE_FROM_SERIAL_PORT_STRING{"WARNING: Did not receive response from serial port, the attached Arduino will not be available ("};
    const char *EXCEPTION_CAUGHT_WHILE_FETCHING_ARDUINO_STRING{"WARNING: An exception was caught while trying to fetch Arduino type, the attached Arduino may not be available ("};

    const char *SUCCESSFULLY_FETCHED_FIRMWARE_VERSION_STRING{"Sucessfully fetched firmware version: "};
    const char *COULD_NOT_FETCH_FIRMWARE_VERSION_STRING{"Could not fetch firmware version"};
    const char *EXCEPTION_CAUGHT_WHILE_FETCHING_FIRMWARE_VERSION_STRING{"WARNING: An exception was caught while trying to fetch Arduino firmware version, the attached Arduino may need a firmware update ("};
    const char *SUCCESSFULLY_FETCHED_CAN_CAPABILITY_STRING{"Successfully fetched CAN capability status: "};
    const char *COULD_NOT_FETCH_CAN_CAPABILITY_STRING{"Could not fetch CAN capability status"};
    const char *EXCEPTION_CAUGHT_WHILE_FETCHING_CAN_CAPABILITY_STRING{"WARNING: An exception was caught while trying to fetch Arduino CAN capability status, the attached Arduino may need a firmware update ("};
    const char *CAN_BYTES_LINE_EDIT_ZERO_STRING{"00"};
    const char *CAN_ID_LINE_EDIT_ZERO_STRING{"000"};
    const char *INVALID_ARDUINO_PTR_AT_INDEX_STRING{"Invalid index for MainWindow::arduinoPtrAtIndex(int): "};
    const char *SUCCESSFULLY_INITIALIZED_CAN_BUS_STRING{"Successfully initialized CAN bus for "};
    const char *FAILED_TO_INITIALIZE_CAN_BUS_STRING{"Failed to initialize CAN bus for "};
    const char *SUCCESSFULLY_SET_CAN_AUTO_UPDATE_STRING{"Successfully set CAN auto update for "};
    const char *FAILED_TO_SET_CAN_AUTO_UPDATE_STRING{"Failed to set CAN auto update for "};
    const char *SUCCESSFULLY_CANCELED_CAN_AUTO_UPDATE_STRING{"Successfully canceled CAN auto update for "};
    const char *FAILED_TO_CANCEL_CAN_AUTO_UPDATE_STRING{"Failed to cancel CAN auto update for "};
    const char *SUCCESSFULLY_REMOVED_ALL_POSITIVE_CAN_MASKS_STRING{"Successfully removed all positive can masks"};
    const char *FAILED_TO_REMOVE_ALL_POSITIVE_CAN_MASKS_STRING{"Failed to remove all positive can masks"};
    const char *SUCCESSFULLY_REMOVED_ALL_NEGATIVE_CAN_MASKS_STRING{"Successfully removed all positive can masks"};
    const char *FAILED_TO_REMOVE_ALL_NEGATIVE_CAN_MASKS_STRING{"Failed to remove all positive can masks"};

    const char *STD_EXCEPTION_IN_IO_REPORT_STRING{"Standard exception caught in ioReport(MainWindow *): "};
    const char *STD_EXCEPTION_IN_SERIAL_REPORT_STRING{"Standard exception caught in serialTerminalReport(MainWindow *): "};
    const char *STD_EXCEPTION_IN_CAN_REPORT_STRING{"Standard exception caught in canTerminalReport(MainWindow *): "};
    const char *IO_REPORT_FUTURE_NOT_STARTED_ERROR_STRING{"ERROR: IOReport Future not started, cannot retrieve result"};
    const char *IO_REPORT_FUTURE_ALREADY_STARTED_ERROR_STRING{"ERROR: IOReport Future already running, cannot start"};
    const char *IO_REPORT_FUTURE_NOT_COMPLETED_ERROR_STRING{"ERROR: IOReport Future still running, cannot retrieve result"};
    const char *SERIAL_TERMINAL_FUTURE_NOT_STARTED_ERROR_STRING{"ERROR: Serial Terminal Future not started, cannot retrieve result"};
    const char *SERIAL_TERMINAL_FUTURE_ALREADY_STARTED_ERROR_STRING{"ERROR: Serial Terminal Future already running, cannot start"};
    const char *SERIAL_TERMINAL_FUTURE_NOT_COMPLETED_ERROR_STRING{"ERROR: Serial Terminal Future still running, cannot retrieve result"};
    const char *CAN_TERMINAL_FUTURE_NOT_STARTED_ERROR_STRING{"ERROR: Can Terminal Future not started, cannot retrieve result"};
    const char *CAN_TERMINAL_FUTURE_ALREADY_STARTED_ERROR_STRING{"ERROR: Can Terminal Future already running, cannot start"};
    const char *CAN_TERMINAL_FUTURE_NOT_COMPLETED_ERROR_STRING{"ERROR: Can Terminal Future still running, cannot retrieve result"};
    const char *DETECTED_SERIAL_PORT_STRING{"Detected serial port: "};
    const char *LOADING_PROGRAM_WIDGET_WINDOW_TITLE{"Loading ArduinoPC"};
    const char *CAN_BYTE_LINE_EDIT_REGEX{"/[0-9a-f]+/i"};
    const char *NTH_DATA_PACKET_BYTE_INDEX_OUT_OF_RANGE_STRING{"ERROR: Invalid index passed to nthDataPacket(int index) :"};
    const char *SUCCESSFULLY_ADDED_POSITIVE_CAN_MASK_STRING{"Successfully added positive can mask: "};
    const char *FAILED_TO_ADD_POSITIVE_CAN_MASK_STRING{"Failed to add positive can mask: "};
    const char *SUCCESSFULLY_ADDED_NEGATIVE_CAN_MASK_STRING{"Successfully added negative can mask: "};
    const char *FAILED_TO_ADD_NEGATIVE_CAN_MASK_STRING{"Failed to add negative can mask: "};
    const char *SUCCESSFULLY_REMOVED_POSITIVE_CAN_MASK_STRING{"Successfully removed positive can mask: "};
    const char *FAILED_TO_REMOVE_POSITIVE_CAN_MASK_STRING{"Failed to remove negative can mask: "};
    const char *SUCCESSFULLY_REMOVED_NEGATIVE_CAN_MASK_STRING{"Successfully removed negative can mask: "};
    const char *FAILED_TO_REMOVE_NEGATIVE_CAN_MASK_STRING{"Failed to remove negative can mask: "};

    const char *PINS_BY_SCREEN_INDEX_INVALID_INDEX_STRING{"ERROR: FlaggedIO::pinsByScreenIndex(int) passed invalid screen index: "};
    const char *CLEAR_PINS_BY_SCREEN_INDEX_INVALID_INDEX_STRING{"ERROR: FlaggedIO::clearPinsByScreenIndex(int) passed invalid screen index: "};
    const char *ADD_PIN_INVALID_INDEX_STRING{"ERROR: FlaggedIO::addPin(int) passed invalid screen index: "};
    const char *ADD_SCREEN_FLAGGED_IO_INDEX_ALREADY_EXISTS_STRING{"ERROR: FlaggedIO::addScreen(int) passed screen index that already exists: "};
    const char *REMOVE_SCREEN_FLAGGED_IO_INVALID_INDEX_STRING{"ERROR: FlaggedIO::removeScreen(int) passed invalid screen index: "};

    const char *IO_REPORT_BY_SCREEN_INDEX_INVALID_INDEX_STRING{"ERROR: SmartIOReportScheduler::reportByScreenIndex(int) passed invalid screen index: "};
    const char *ADD_REPORT_IO_INDEX_ALREADY_EXISTS_STRING{"ERROR: SmartIOReportScheduler::addReport(int) passed screen index that already exists: "};
    const char *REMOVE_SCREEN_IO_REPORT_INVALID_INDEX_STRING{"ERROR: SmartIOReportScheduler::removeScreen(int) passed invalid screen index: "};

    const char *SERIAL_REPORT_BY_SCREEN_INDEX_INVALID_INDEX_STRING{"ERROR: SmartSerialReportScheduler::reportByScreenIndex(int) passed invalid screen index: "};
    const char *ADD_REPORT_SERIAL_INDEX_ALREADY_EXISTS_STRING{"ERROR: SmartSerialReportScheduler::addReport(int) passed screen index that already exists: "};
    const char *REMOVE_SCREEN_SERIAL_REPORT_INVALID_INDEX_STRING{"ERROR: SmartSerialReportScheduler::removeScreen(int) passed invalid screen index: "};

    const char *CAN_REPORT_BY_SCREEN_INDEX_INVALID_INDEX_STRING{"ERROR: SmartCamReportScheduler::reportByScreenIndex(int) passed invalid screen index: "};
    const char *ADD_REPORT_CAN_INDEX_ALREADY_EXISTS_STRING{"ERROR: SmartCanReportScheduler::addReport(int) passed screen index that already exists: "};
    const char *REMOVE_SCREEN_CAN_REPORT_INVALID_INDEX_STRING{"ERROR: SmartCanReportScheduler::removeScreen(int) passed invalid screen index: "};

    const char *SERIAL_TERMINAL_BY_SCREEN_INDEX_INVALID_INDEX_STRING{"ERROR: SerialTerminalScheduler::serialTerminalByScreenIndex(int) passed invalid screen index: "};
    const char *REMOVE_SCREEN_SERIAL_TERMINAL_INVALID_INDEX_STRING{"ERROR: SerialTerminalScheduler::removeScreen(int) passed invalid screen index: "};
    const char *ADD_REPORT_SERIAL_TERMINAL_INDEX_ALREADY_EXISTS_STRING{"ERROR: SerialTerminalScheduler::addScreen(int) passed screen index that already exists: "};

    const char *CAN_TERMINAL_BY_SCREEN_INDEX_INVALID_INDEX_STRING{"ERROR: CanTerminalScheduler::canTerminalByScreenIndex(int) passed invalid screen index: "};
    const char *REMOVE_SCREEN_CAN_TERMINAL_INVALID_INDEX_STRING{"ERROR: CanTerminalScheduler::removeScreen(int) passed invalid screen index: "};
    const char *ADD_REPORT_CAN_TERMINAL_INDEX_ALREADY_EXISTS_STRING{"ERROR: CanTerminalScheduler::addScreen(int) passed screen index that already exists: "};

    const char *TIMER_BY_SCREEN_INDEX_INVALID_INDEX_STRING{"ERROR: IOUpdateTimerScheduler::timerByScreenIndex(int) passed invalid screen index: "};
    const char *ADD_TIMER_ALREADY_EXISTS_STRING{"ERROR: IOUpdateTimerScheduler::addScreen(int) passed screen index that already exists: "};
    const char *REMOVE_SCREEN_TIMER_INVALID_INDEX_STRING{"ERROR: IOUpdateTimerScheduler::removeScreen(int) passed invalid screen index: "};

    const char *ARDUINO_BY_SCREEN_INDEX_INVALID_INDEX_STRING{"ERROR: ArduinoScheduler::arduinoByScreenIndex(int) passed invalid screen index: "};
    const char *ADD_ARDUINO_ALREADY_EXISTS_STRING{"ERROR: ArduinoScheduler::addScreen(int) passed screen index that already exists: "};
    const char *REMOVE_SCREEN_ARDUINO_INVALID_INDEX_STRING{"ERROR: ArduinoScheduler::removeScreen(int) passed invalid screen index: "};

    const char *MANUAL_SCREEN_BY_SCREEN_INDEX_INVALID_INDEX_STRING{"ERROR: ManualScreenScheduler::manualScreenByScreenIndex(int) passed invalid screen index: "};
    const char *ADD_MANUAL_ALREADY_EXISTS_STRING{"ERROR: ManualScreenScheduler::addScreen(int) passed screen index that already exists: "};
    const char *REMOVE_SCREEN_MANUAL_INVALID_INDEX_STRING{"ERROR: ManualScreenScheduler::removeScreen(int) passed invalid screen index: "};

    const char *STATUS_VIEW_BY_SCREEN_INDEX_INVALID_INDEX_STRING{"ERROR: IOStatusViewScheduler::statusViewByScreenIndex(int) passed invalid screen index: "};
    const char *ADD_SCREEN_IO_VIEW_INDEX_ALREADY_EXISTS_STRING{"ERROR: IOStatusViewScheduler::addScreen(int) passed screen index that already exists: "};
    const char *REMOVE_SCREEN_IO_VIEW_INVALID_INDEX_STRING{"ERROR: IOStatusViewScheduler::removeScreen(int) passed invalid screen index: "};

    const char *USING_INITIAL_STATE_STRING{"Using initial state " };
    const char *FOR_PIN_NUMBER_STRING{" for pin number "};
    const char *USING_IO_TYPE_STRING{"Using IOType "};
    const char *USING_ALIAS_STRING{"Using alias "};
    const char *USING_BLUETOOTH_MULTIPLIER_STRING{"Using bluetooth multiplier "};

    const char *PINS_EXISTS_BY_SCREEN_INDEX_INVALID_INDEX_STRING{"Invalid screen index passed to FlaggedIO::pinsExistByScreenIndex(int): "};
    const char *FLAGGED_IO_MUTEX_SCREEN_INDEX_INVALID_STRING{"Invalid screen index passed to FlaggedIO::ioMutexAtScreenIndex(int): "};
    const char *FLAGGED_SERIAL_TERMINAL_MUTEX_SCREEN_INDEX_INVALID_STRING{"Invalid screen index passed to SerialTerminal::serialTerminalMutexAtScreenIndex(int): "};
    const char *FLAGGED_CAN_TERMINAL_MUTEX_SCREEN_INDEX_INVALID_STRING{"Invalid screen index passed to CanTerminal::canTerminalMutexAtScreenIndex(int): "};

    const char *DIGITAL_OUTPUT_LABEL{"Digital Output"};
    const char *DIGITAL_INPUT_LABEL{"Digital Input"};
    const char *ANALOG_INPUT_LABEL{"Analog Input"};
    const char *ANALOG_OUTPUT_LABEL{"Analog Output"};
    const char *PIN_NUMBER_LABEL{"Pin number "};
    const char *FAILED_SOFT_DIGITAL_READ_STRING{" failed softDigitalRead"};
    const char *FAILED_DIGITAL_WRITE_STRING{" failed digitalWrite"};
    const char *FAILED_CURRENT_PIN_MODE_STRING{" failed currentPinMode"};
    const char *FAILED_PIN_MODE_STRING{" failed pinMode"};
    const char *FAILED_PULL_UP_STRING{" failed pullup"};
    const char *FAILED_PULL_DOWN_STRING{" failed release from pullup"};
    const char *SUCCESSFULLY_WRITTEN_LOW_STRING{" was successfully written low"};
    const char *SUCCESSFULLY_WRITTEN_HIGH_STRING{" was successfully writtten high"};
    const char *SUCCESSFULLY_PULLED_UP_STRING{" was successfully pulled up"};
    const char *SUCCESSFULLY_PULLED_DOWN_STRING{" was successfully released from pullup"};
    const char *ANALOG_WRITE_SUCCESSFUL_BASE_STRING{" was successfully written to analog state: "};
    const char *ANALOG_WRITE_RAW_SUCCESSFUL_BASE_STRING{" was successfully written to raw analog state: "};
    const char *FAILED_ANALOG_WRITE_STRING{" failed analogWrite"};
    const char *FAILED_ANALOG_WRITE_RAW_STRING{" failed analogWriteRaw"};
    const char *ANALOG_OUTPUT_STATE_RAW_STRING{"This output is currently in raw mode"};
    const char *ANALOG_OUTPUT_STATE_NORMAL_STRING{"This output is currently in normal mode"};
    const char *ANALOG_OUTPUT_STATE_RAW_STYLESHEET{ "AnalogIORawStatePushButton { color: black; }" };
    const char *ANALOG_OUTPUT_STATE_NORMAL_STYLESHEET{ "AnalogIORawStatePushButton { color: blue; }" };

    const char *IO_REPORT_INVALID_DATA_STRING{"ioReportRequest(int) timed out or received invalid data"};
    const char *CAN_REPORT_INVALID_DATA_STRING{"canReportRequest(int) timed out or received invalid data"};
    const char *IO_MUTEX_INVALID_INDEX_STRING{"Invalid index passed to Arduino::ioMutexAtIndex(int): "};
    const char *SERIAL_PORT_INVALID_INDEX_STRING{"Invalid index passed to Arduino::serialPortAtIndex(int): "};

    const char *DIGITAL_INPUT_PULLUP_UP_STYLESHEET{ "PullupStatePushButton { color: red; }" };
    const char *DIGITAL_INPUT_PULLUP_DOWN_STYLESHEET{ "PullupStatePushButton { color: black; }" };
    const char *ARDUINO_UNO_LONG_NAME{"Arduino Uno"};
    const char *ARDUINO_NANO_LONG_NAME{"Arduino Nano"};
    const char *ARDUINO_MEGA_LONG_NAME{"Arduino Mega"};
    const char *PIN_LABEL_BASE{"Pin "};
    const char *ANALOG_IO_INITIAL_STATE_LABEL{"0.00"};
    const char *INVALID_ANALOG_STATE_BASE_STRING{"Invalid analog state: "};
    const char *INVALID_ANALOG_STATE_WINDOW_TITLE{"Invalid Analog State"};
    const char *ANALOG_OUTPUT_ENTRY_BOX_PENDING_CHANGE_STYLE_SHEET{"AnalogOutputEntryBox { color: blue; }"};
    const char *ANALOG_OUTPUT_ENTRY_BOX_STABLE_STYLE_SHEET{"AnalogOutputEntryBox { color: black; }"};


    const char *INVALID_PIN_ALIAS_STRING{"Invalid pin alias: "};
    const char *INVALID_ANALOG_PIN_BASE_STRING{"Invalid analog pin: "};
    const char *INVALID_GPIO_PIN_STRING{"Invalid gpio pin: "};
    const char *INVALID_PIN_NUMBER_STRING{"Invalid pin number: "};
    const char *UNSPECIFIED_IO_TYPE_TO_PARSE_IO_TYPE_STRING{"Unspecified IOType passed to Arduino::parseIOType(IOType)"};
    const char *INVALID_IO_TYPE_BASE_STRING{"Invalid IOType: "};
    const char *INVALID_IO_TYPE_TAIL_STRING{" passed to Arduino::parseIOTypeFromString(const std::string &)"};
    const char *INVALID_STATE_TO_PARSE_TO_DIGITAL_STATE_STRING{"ERROR: Invalid state passed to parseToDigitalState(const std::string &): "};
    const char *INVALID_STATE_TO_PARSE_TO_ANALOG_STATE_STRING{"ERROR: Invalid state passed to parseToAnalogState(const std::string &): "};
    const char *INVALID_STATE_TO_PARSE_TO_ANALOG_STATE_RAW_STRING{"ERROR: Invalid state passed to parseToAnalogStateRaw(const std::string &): "};
    const char *FIRMWARE_VERSION_UNKNOWN_STRING{" unknown"};
    const char *FIRMWARE_VERSION_BASE_STRING{"firmware version "};

    const char *NULL_GPIO_PTR_TO_DIGITAL_READ_STRING{"Null std::shared_ptr<GPIO> passed to digitalRead(const std::shared_ptr<GPIO> &)"};
    const char *NULL_GPIO_PTR_TO_DIGITAL_WRITE_STRING{"Null std::shared_ptr<GPIO> passed to digitalWrite(const std::shared_ptr<GPIO> &)"};
    const char *NULL_GPIO_PTR_TO_ANALOG_READ_STRING{"Null std::shared_ptr<GPIO> passed to analogRead(const std::shared_ptr<GPIO> &)"};
    const char *NULL_GPIO_PTR_TO_ANALOG_READ_RAW_STRING{"Null std::shared_ptr<GPIO> passed to analogReadRaw(const std::shared_ptr<GPIO> &)"};
    const char *NULL_GPIO_PTR_TO_ANALOG_WRITE_STRING{"Null std::shared_ptr<GPIO> passed to analogWrite(const std::shared_ptr<GPIO> &)"};
    const char *NULL_GPIO_PTR_TO_ANALOG_WRITE_RAW_STRING{"Null std::shared_ptr<GPIO> passed to analogWriteRaw(const std::shared_ptr<GPIO> &)"};
    const char *NULL_GPIO_PTR_TO_SOFT_DIGITAL_READ_STRING{"Null std::shared_ptr<GPIO> passed to softDigitalRead(const std::shared_ptr<GPIO> &)"};
    const char *NULL_GPIO_PTR_TO_SOFT_ANALOG_READ_STRING{"Null std::shared_ptr<GPIO> passed to softAnalogRead(const std::shared_ptr<GPIO> &)"};
    const char *NULL_GPIO_PTR_TO_SOFT_ANALOG_READ_RAW_STRING{"Null std::shared_ptr<GPIO> passed to softAnalogReadRaw(const std::shared_ptr<GPIO> &)"};
    const char *NULL_GPIO_PTR_TO_PIN_MODE_STRING{"Null std::shared_ptr<GPIO> passed to pinMode(const std::shared_ptr<GPIO> &)"};
    const char *NULL_GPIO_PTR_TO_CURRENT_PIN_MODE_STRING{"Null std::shared_ptr<GPIO> passed to currentPinMode(const std::shared_ptr<GPIO> &)"};

    const char *PASSED_TO_DIGITAL_READ_STRING_TAIL_STRING{" passed to digitalRead(const std::string &)"};
    const char *PASSED_TO_DIGITAL_WRITE_STRING_TAIL_STRING{" passed to digitalWrite(const std::string &, bool)"};
    const char *PASSED_TO_ANALOG_READ_STRING_TAIL_STRING{" passed to analogRead(const std::string &)"};
    const char *PASSED_TO_ANALOG_READ_RAW_STRING_TAIL_STRING{" passed to analogReadRaw(const std::string &)"};
    const char *PASSED_TO_ANALOG_WRITE_STRING_TAIL_STRING{" passed to analogWrite(const std::string &, double)"};
    const char *PASSED_TO_ANALOG_WRITE_RAW_STRING_TAIL_STRING{" passed to analogWriteRaw(const std::string &, int)"};
    const char *PASSED_TO_SOFT_DIGITAL_READ_STRING_TAIL_STRING{" passed to softDigitalRead(const std::string &)"};
    const char *PASSED_TO_SOFT_ANALOG_READ_STRING_TAIL_STRING{" passed to softAnalogRead(const std::string &)"};
    const char *PASSED_TO_SOFT_ANALOG_READ_RAW_STRING_TAIL_STRING{" passed to softAnalogReadRaw(const std::string &)"};
    const char *PASSED_TO_PIN_MODE_STRING_TAIL_STRING{" passed to pinMode(const std::string &, IOType)"};
    const char *PASSED_TO_CURRENT_PIN_MODE_STRING_TAIL_STRING{" passed to currentPinMode(const std::string &)"};

    const char *PASSED_TO_DIGITAL_READ_GPIO_TAIL_STRING{" passed to digitalRead(const GPIO &)"};
    const char *PASSED_TO_DIGITAL_WRITE_GPIO_TAIL_STRING{" passed to digitalWrite(const GPIO &, bool)"};
    const char *PASSED_TO_ANALOG_READ_GPIO_TAIL_STRING{" passed to analogRead(const GPIO &)"};
    const char *PASSED_TO_ANALOG_READ_RAW_GPIO_TAIL_STRING{" passed to analogReadRaw(const GPIO &)"};
    const char *PASSED_TO_ANALOG_WRITE_GPIO_TAIL_STRING{" passed to analogWrite(const GPIO &, double)"};
    const char *PASSED_TO_ANALOG_WRITE_RAW_GPIO_TAIL_STRING{" passed to analogWriteRaw(const GPIO &, int)"};
    const char *PASSED_TO_SOFT_DIGITAL_READ_GPIO_TAIL_STRING{" passed to softDigitalRead(const GPIO &)"};
    const char *PASSED_TO_SOFT_ANALOG_READ_GPIO_TAIL_STRING{" passed to softAnalogRead(const GPIO &)"};
    const char *PASSED_TO_SOFT_ANALOG_READ_RAW_GPIO_TAIL_STRING{" passed to softAnalogReadRaw(const GPIO &)"};
    const char *PASSED_TO_PIN_MODE_GPIO_TAIL_STRING{" passed to pinMode(const GPIO &, IOType)"};
    const char *PASSED_TO_CURRENT_PIN_MODE_GPIO_TAIL_STRING{" passed to currentPinMode(const GPIO &)"};

    const char *UNO_INVALID_ANALOG_STRING_TAIL_STRING{" passed to ArduinoUno::analogPin(const std::string&)"};
    const char *UNO_INVALID_ANALOG_INT_TAIL_STRING{" passed to ArduinoUno::analogPinFromNumber(int)"};

    const char *NANO_INVALID_ANALOG_STRING_TAIL_STRING{" passed to ArduinoNano::analogPin(const std::string&)"};
    const char *NANO_INVALID_ANALOG_INT_TAIL_STRING{" passed to ArduinoNano::analogPinFromNumber(int)"};

    const char *MEGA_INVALID_ANALOG_STRING_TAIL_STRING{" passed to ArduinoMega::analogPin(const std::string&)"};
    const char *MEGA_INVALID_ANALOG_INT_TAIL_STRING{" passed to ArduinoMega::analogPinFromNumber(int)"};

    const char *TERMINAL_RECEIVE_BASE_STRING{"Rx << "};
    const char *TERMINAL_TRANSMIT_BASE_STRING{"Tx >> "};
    const char *TERMINAL_DELAY_BASE_STRING{"Delay <> "};
    const char *TERMINAL_FLUSH_RX_BASE_STRING{"Flush vv "};
    const char *TERMINAL_FLUSH_TX_BASE_STRING{"Flush ^^ "};
    const char *TERMINAL_FLUSH_RX_TX_BASE_STRING{"Flush ^v "};
    const char *NO_SERIAL_PORTS_CONNECTED_STRING{"No serial ports connected"};
    const char *CONNECT_TO_SERIAL_PORT_TO_BEGIN_STRING{"Use the connect button to connect to a serial port and begin sending data"};
    const char *LOAD_SCRIPT_BUTTON_TEXT_STRING{"Load Script"};
    const char *OPEN_SCRIPT_FILE_CAPTION{"Open Script File"};
    const char *EMPTY_SCRIPT_STRING{"The loaded script does not contain any valid commands: "};
    const char *EMPTY_SCRIPT_WINDOW_TITLE_STRING{"Loaded Script Empty"};
    const char *FILE_DOES_NOT_EXIST_STRING{"The selected script file does not exist: "};
    const char *FILE_DOES_NOT_EXIST_WINDOW_TITLE_STRING{"Script Does Not Exist"};
    const char *EXECUTING_SCRIPT_STRING{"Executing script file: "};
    const char *FINISHED_EXECUTING_SCRIPT_STRING{"Finished executing script file: "};
    const char *DELAY_IDENTIFIER{"delay"};
    const char *DELAY_SECONDS_IDENTIFIER{"delayseconds("};
    const char *DELAY_MILLISECONDS_IDENTIFIER{"delaymilliseconds("};
    const char *DELAY_MICROSECONDS_IDENTIFIER{"delaymicroseconds("};
    const char *WRITE_IDENTIFIER{"write("};
    const char *READ_IDENTIFIER{"read("};
    const char *LOOP_IDENTIFIER{"loop("};
    const char *FLUSH_RX_IDENTIFIER{"flushrx"};
    const char *FLUSH_TX_IDENTIFIER{"flushtx"};
    const char *FLUSH_TX_RX_IDENTIFIER{"flushtxrx"};
    const char *FLUSH_RX_TX_IDENTIFIER{"flushrxtx"};
    const char *SECONDS_SUFFIX_STRING{"sec"};
    const char *MILLISECONDS_SUFFIX_STRING{"ms"};
    const char *MICROSECONDS_SUFFIX_STRING{"us"};
    const char *ACTION_LOAD_SCRIPT_ENABLED_TOOLTIP{""};
    const char *ACTION_LOAD_SCRIPT_DISABLED_TOOLTIP{"Connect to a serial port to load a script"};
    const char *SEND_BOX_ENABLED_TOOLTIP{""};
    const char *SEND_BOX_DISABLED_TOOLTIP{"Connect to a serial port to begin sending and receiving data"};
    const char *WRITE_PARAMETER_MUST_BE_IN_QUOTATIONS_STRING{"    Write() parameter must be enclosed in parentheses, ignoring option"};
    const char *DELAY_SECONDS_PARAMETER_NOT_AN_INTEGER_STRING{"    DelaySeconds() parameter is not an integer, ignoring option"};
    const char *DELAY_MILLISECONDS_PARAMETER_NOT_AN_INTEGER_STRING{"    DelayMilliseconds() parameter is not an integer, ignoring option"};
    const char *DELAY_MICROSECONDS_PARAMETER_NOT_AN_INTEGER_STRING{"    DelayMicroseconds() parameter is not an integer, ignoring option"};
    const char *SERIAL_COMMAND_TYPE_NOT_IMPLEMENTED_STRING{"WARNING: SerialCommandType not implemented, skipping command: "};
    const char *NULL_MAIN_WINDOW_PASSED_TO_EXECUTE_STRING{"WARNING: Null MainWindow passed to ScriptExecutor::execute(), skipping script execution"};
    const char *NULL_SERIAL_TERMINAL_PASSED_TO_EXECUTE_STRING{"WARNING: Null SerialTerminal passed to ScriptExecutor::execute(), skipping script execution"};
    const char *SCRIPT_FILE_DOES_NOT_EXISTS_STRING{"WARNING: Script file does not exist, skipping script: "};
    const char *UNABLE_TO_OPEN_SCRIPT_FILE_STRING{"WARNING: Unable to open script file, skipping script: "};
    const char *LOOP_COUNT_PARAMETER_NOT_AN_INTEGER_STRING{"LoopCount() parameter is not an integer, ignoring option"};
    const char *UNTERMINATED_LOOP_STRING{"ERROR: The script contains an unterminated loop,  skipping script execution"};
    const char *UNEXPECTED_LOOP_CLOSING_STRING{"ERROR: A loop closure was found, but no loop was currently being populated, ignoring option"};
    const char *CLOSING_LOOP_IDENTIFIER{"}"};
    const char *BEGINNING_INFINITE_LOOP_STRING{"***Beginning infinite loop***"};
    const char *INFINITE_LOOP_COUNT_TAIL_STRING{"/infinite)"};
    const char *LOOPS_TAIL_STRING{" loops***"};
    const char *BEGINNING_LOOPS_BASE_STRING{"***Starting "};
    const char *ENDING_LOOPS_BASE_STRING{"***Finished "};
    const char *BEGIN_LOOP_BASE_STRING{"Begin loop ("};
    const char *END_LOOP_BASE_STRING{"End loop ("};
    const char *CANCEL_SCRIPT_STRING{"Cancel Script"};
    const char *SEND_STRING{"Send"};
    const char *CANCELED_EXECUTING_SCRIPT_STRING{"Canceled execution of script file: "};
    const char *COMMAND_HISTORY_CONTEXT_MENU_STRING{"Command History"};

    const char *QUIT_PROMPT_STRING{"Are you sure you want to quit?"};
    const char *QUIT_PROMPT_WINDOW_TITLE_STRING{"Quit ArduinoPC?"};
    const char *INVALID_SETTINGS_DETECTED_STRING{"Invalid settings detected, please reselect serial port settings: "};
    const char *INVALID_SETTINGS_DETECTED_WINDOW_TITLE_STRING{"Invalid Serial Port Settings"};
    const char *SETTINGS_DIALOG_WINDOW_TITLE{"Serial Port Settings"};
    const char *SUCCESSFULLY_CLOSED_SERIAL_PORT_STRING{"Successfully closed serial port "};
}
