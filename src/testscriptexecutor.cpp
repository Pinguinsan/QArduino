#include "serialterminal/serialscriptexecutor.h"

SerialScriptExecutor::SerialScriptExecutor(const std::string &scriptFilePath) :
    m_scriptReader{std::make_shared<SerialScriptReader>(scriptFilePath)}
{

}

void SerialScriptExecutor::setScriptFilePath(const std::string &scriptFilePath)
{
    this->m_scriptReader.reset();
    this->m_scriptReader = std::make_shared<SerialScriptReader>(scriptFilePath);
}

std::shared_ptr<SerialScriptReader> SerialScriptExecutor::scriptReader() const
{
    return this->m_scriptReader;
}

void SerialScriptExecutor::execute(SerialTerminal *serialTerminal, std::shared_ptr<Arduino> arduino,
                             const std::function<void(SerialTerminal *, const std::string &)> &printRxResult,
                             const std::function<void(SerialTerminal *, const std::string &)> &printTxResult,
                             const std::function<void(SerialTerminal *, DelayType, int)> &printDelayResult,
                             const std::function<void(SerialTerminal *, FlushType)> &printFlushResult,
                             const std::function<void(SerialTerminal *, LoopType, int, int)> &printLoopResult)
{
    using namespace ArduinoPCStrings;
    using namespace GeneralUtilities;
    using namespace MathUtilities;
    if (!serialTerminal) {
        throw std::runtime_error(NULL_SERIAL_TERMINAL_PASSED_TO_EXECUTE_STRING);
    }
    if (this->m_scriptReader->hasInvalidCommands()) {
        QString errorString{""};
        for (auto &it : *this->m_scriptReader->invalidCommandErrorMessages()) {
            int i = 0;
            for (auto &innerIt : *(it.errorMessages())) {
                errorString.append(toQString(innerIt) + "\n");
                if(isEvenlyDivisibleBy(i, 4)) {
                    errorString.append("\n");
                }
                i++;
            }
        }
        QMessageBox::information(0, SERIAL_SCRIPT_HAS_ERRORS_MESSAGE_BOX_TITLE, errorString);
        return;
    }
    int loop {false};
    int loopCount{0};
    std::vector<SerialCommand> loopSerialCommands;
    for (auto &it : (*this->m_scriptReader->commands())) {
        serialTerminal->application()->processEvents();
        if (serialTerminal->cancelScript()) {
            return;
        }
        try {
            if (!loop) {
                if (it.commandType() == SerialCommandType::WRITE) {
                    arduino->writeRawString(it.commandArgument());
                    printTxResult(serialTerminal, it.commandArgument());
                } else if (it.commandType() == SerialCommandType::READ) {
                    printRxResult(serialTerminal, arduino->readRawString());
                } else if (it.commandType() == SerialCommandType::DELAY_SECONDS) {
                    printDelayResult(serialTerminal, DelayType::SECONDS, std::stoi(it.commandArgument()));
                    delaySecondsWithUpdate(std::stoi(it.commandArgument()), serialTerminal);
                } else if (it.commandType() == SerialCommandType::DELAY_MILLISECONDS) {
                    printDelayResult(serialTerminal, DelayType::MILLISECONDS, std::stoi(it.commandArgument()));
                    delayMillisecondsWithUpdate(std::stoi(it.commandArgument()), serialTerminal);
                } else if (it.commandType() == SerialCommandType::DELAY_MICROSECONDS) {
                    printDelayResult(serialTerminal, DelayType::MICROSECONDS, std::stoi(it.commandArgument()));
                    delayMicrosecondsWithUpdate(std::stoi(it.commandArgument()), serialTerminal);
                } else if (it.commandType() == SerialCommandType::FLUSH_RX) {
                    printFlushResult(serialTerminal, FlushType::RX);
                    arduino->flushRX();
                } else if (it.commandType() == SerialCommandType::FLUSH_TX) {
                    printFlushResult(serialTerminal, FlushType::TX);
                    arduino->flushTX();
                } else if (it.commandType() == SerialCommandType::FLUSH_RX_TX) {
                    printFlushResult(serialTerminal, FlushType::RX_TX);
                    arduino->flushRXTX();
                } else if (it.commandType() == SerialCommandType::LOOP_START) {
                    loop = true;
                    loopCount = std::stoi(it.commandArgument());
                    loopSerialCommands.clear();
                } else {
                    throw std::runtime_error(SERIAL_COMMAND_TYPE_NOT_IMPLEMENTED_STRING + it.commandArgument());
                }
            } else {
                if (it.commandType() == SerialCommandType::LOOP_END) {
                    loop =  false;
                    doLoop(serialTerminal, arduino, printRxResult, printTxResult, printDelayResult, printFlushResult, printLoopResult, loopSerialCommands, loopCount);
                    loopSerialCommands.clear();
                } else {
                    loopSerialCommands.emplace_back(it);
                }
            }
        } catch (std::exception &e) {
            throw std::runtime_error(e.what());
        }
    }
}


void SerialScriptExecutor::doLoop(SerialTerminal *serialTerminal, std::shared_ptr<Arduino> arduino,
                const std::function<void(SerialTerminal *, const std::string &)> &printRxResult,
                const std::function<void(SerialTerminal *, const std::string &)> &printTxResult,
                const std::function<void(SerialTerminal *, DelayType, int)> &printDelayResult,
                const std::function<void(SerialTerminal *, FlushType)> &printFlushResult,
                const std::function<void(SerialTerminal *, LoopType, int, int)> &printLoopResult,
                const std::vector<SerialCommand> &commands,
                int loopCount)
{
    using namespace ArduinoPCStrings;
    using namespace GeneralUtilities;
    if (loopCount == -1) {
        for (int i = 0; true; i++) {
            printLoopResult(serialTerminal, LoopType::START, i, -1);
            for (auto &it : commands) {
                serialTerminal->application()->processEvents();
                if (serialTerminal->cancelScript()) {
                    return;
                }
                try {
                    if (it.commandType() == SerialCommandType::WRITE) {
                        arduino->writeRawString(it.commandArgument());
                        printTxResult(serialTerminal, it.commandArgument());
                    } else if (it.commandType() == SerialCommandType::READ) {
                        printRxResult(serialTerminal, arduino->readRawString());
                    } else if (it.commandType() == SerialCommandType::DELAY_SECONDS) {
                       printDelayResult(serialTerminal, DelayType::SECONDS, std::stoi(it.commandArgument()));
                       delaySecondsWithUpdate(std::stoi(it.commandArgument()), serialTerminal);
                   } else if (it.commandType() == SerialCommandType::DELAY_MILLISECONDS) {
                       printDelayResult(serialTerminal, DelayType::MILLISECONDS, std::stoi(it.commandArgument()));
                       delayMillisecondsWithUpdate(std::stoi(it.commandArgument()), serialTerminal);
                   } else if (it.commandType() == SerialCommandType::DELAY_MICROSECONDS) {
                       printDelayResult(serialTerminal, DelayType::MICROSECONDS, std::stoi(it.commandArgument()));
                       delayMicrosecondsWithUpdate(std::stoi(it.commandArgument()), serialTerminal);
                   } else if (it.commandType() == SerialCommandType::FLUSH_RX) {
                        printFlushResult(serialTerminal, FlushType::RX);
                        arduino->flushRX();
                    } else if (it.commandType() == SerialCommandType::FLUSH_TX) {
                        printFlushResult(serialTerminal, FlushType::TX);
                        arduino->flushTX();
                    } else if (it.commandType() == SerialCommandType::FLUSH_RX_TX) {
                        printFlushResult(serialTerminal, FlushType::RX_TX);
                        arduino->flushRXTX();
                    }
                } catch (std::exception &e) {
                    throw std::runtime_error(e.what());
                }
            }
            printLoopResult(serialTerminal, LoopType::END, i, -1);
        }
    } else {
        for (int i = 0 ; i < loopCount; i++) {
            printLoopResult(serialTerminal, LoopType::START, i, loopCount);
            for (auto &it : commands) {
                serialTerminal->application()->processEvents();
                if (serialTerminal->cancelScript()) {
                    return;
                }
                try {
                    if (it.commandType() == SerialCommandType::WRITE) {
                        arduino->writeRawString(it.commandArgument());
                        printTxResult(serialTerminal, it.commandArgument());
                    } else if (it.commandType() == SerialCommandType::READ) {
                        printRxResult(serialTerminal, arduino->readRawString());
                    }
                    else if (it.commandType() == SerialCommandType::DELAY_SECONDS) {
                        printDelayResult(serialTerminal, DelayType::SECONDS, std::stoi(it.commandArgument()));
                        delaySecondsWithUpdate(std::stoi(it.commandArgument()), serialTerminal);
                    } else if (it.commandType() == SerialCommandType::DELAY_MILLISECONDS) {
                        printDelayResult(serialTerminal, DelayType::MILLISECONDS, std::stoi(it.commandArgument()));
                        delayMillisecondsWithUpdate(std::stoi(it.commandArgument()), serialTerminal);
                    } else if (it.commandType() == SerialCommandType::DELAY_MICROSECONDS) {
                        printDelayResult(serialTerminal, DelayType::MICROSECONDS, std::stoi(it.commandArgument()));
                        delayMicrosecondsWithUpdate(std::stoi(it.commandArgument()), serialTerminal);
                    } else if (it.commandType() == SerialCommandType::FLUSH_RX) {
                        printFlushResult(serialTerminal, FlushType::RX);
                        arduino->flushRX();
                    } else if (it.commandType() == SerialCommandType::FLUSH_TX) {
                        printFlushResult(serialTerminal, FlushType::TX);
                        arduino->flushTX();
                    } else if (it.commandType() == SerialCommandType::FLUSH_RX_TX) {
                        printFlushResult(serialTerminal, FlushType::RX_TX);
                        arduino->flushRXTX();
                    }
                } catch (std::exception &e) {
                    throw std::runtime_error(e.what());
                }
            }
            if (i+1 != loopCount) {
                printLoopResult(serialTerminal, LoopType::END, i, loopCount);
            }
        }
    }
    printLoopResult(serialTerminal, LoopType::END, loopCount-1, loopCount);
}


void SerialScriptExecutor::delaySecondsWithUpdate(int howLong, SerialTerminal *serialTerminal)
{
    int millisecondsToDelay{howLong * 1000};
    auto startTime{std::chrono::high_resolution_clock::now()};
    auto endTime{std::chrono::high_resolution_clock::now()};
    long long int elapsedTime{0};

    do {
        serialTerminal->application()->processEvents();
        if (serialTerminal->cancelScript()) {
            return;
        }
        endTime = std::chrono::high_resolution_clock::now();
        elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    } while (elapsedTime <= millisecondsToDelay);
}


void SerialScriptExecutor::delayMillisecondsWithUpdate(int howLong, SerialTerminal *serialTerminal)
{
    auto startTime{std::chrono::high_resolution_clock::now()};
    auto endTime{std::chrono::high_resolution_clock::now()};
    long long int elapsedTime{0};
    do {
        serialTerminal->application()->processEvents();
        if (serialTerminal->cancelScript()) {
            return;
        }
        endTime = std::chrono::high_resolution_clock::now();
        elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    } while (elapsedTime <= howLong);
}

void SerialScriptExecutor::delayMicrosecondsWithUpdate(int howLong, SerialTerminal *serialTerminal)
{
    auto startTime{std::chrono::high_resolution_clock::now()};
    auto endTime{std::chrono::high_resolution_clock::now()};
    long long int elapsedTime{0};

    do {
        serialTerminal->application()->processEvents();
        if (serialTerminal->cancelScript()) {
            return;
        }
        endTime = std::chrono::high_resolution_clock::now();
        elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
    } while (elapsedTime <= howLong);
}
