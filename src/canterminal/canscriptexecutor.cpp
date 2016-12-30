#include "canterminal/canscriptexecutor.h"

CanScriptExecutor::CanScriptExecutor(const std::string &scriptFilePath) :
    m_scriptReader{std::make_shared<CanScriptReader>(scriptFilePath)}
{

}

void CanScriptExecutor::setScriptFilePath(const std::string &scriptFilePath)
{
    this->m_scriptReader.reset();
    this->m_scriptReader = std::make_shared<CanScriptReader>(scriptFilePath);
}

std::shared_ptr<CanScriptReader> CanScriptExecutor::scriptReader() const
{
    return this->m_scriptReader;
}

void CanScriptExecutor::execute(CanTerminal *canTerminal, std::shared_ptr<Arduino> arduino,
                             const std::function<void(CanTerminal *, const std::string &)> &printRxResult,
                             const std::function<void(CanTerminal *, const std::string &)> &printTxResult,
                             const std::function<void(CanTerminal *, DelayType, int)> &printDelayResult,
                             const std::function<void(CanTerminal *, FlushType)> &printFlushResult,
                             const std::function<void(CanTerminal *, LoopType, int, int)> &printLoopResult)
{
    using namespace ArduinoPCStrings;
    using namespace GeneralUtilities;
    if (!canTerminal) {
        throw std::runtime_error(NULL_SERIAL_TERMINAL_PASSED_TO_EXECUTE_STRING);
    }
    int loop {false};
    int loopCount{0};
    std::vector<CanCommand> loopCanCommands;
    for (auto &it : (*this->m_scriptReader->commands())) {
        canTerminal->application()->processEvents();
        if (canTerminal->cancelScript()) {
            return;
        }
        try {
            if (!loop) {
                if (it.commandType() == CanCommandType::WRITE) {
                    arduino->writeRawString(it.commandArgument());
                    printTxResult(canTerminal, it.commandArgument());
                } else if (it.commandType() == CanCommandType::READ) {
                    printRxResult(canTerminal, arduino->readRawString());
                } else if (it.commandType() == CanCommandType::DELAY_SECONDS) {
                    printDelayResult(canTerminal, DelayType::SECONDS, std::stoi(it.commandArgument()));
                    delaySecondsWithUpdate(std::stoi(it.commandArgument()), canTerminal);
                } else if (it.commandType() == CanCommandType::DELAY_MILLISECONDS) {
                    printDelayResult(canTerminal, DelayType::MILLISECONDS, std::stoi(it.commandArgument()));
                    delayMillisecondsWithUpdate(std::stoi(it.commandArgument()), canTerminal);
                } else if (it.commandType() == CanCommandType::DELAY_MICROSECONDS) {
                    printDelayResult(canTerminal, DelayType::MICROSECONDS, std::stoi(it.commandArgument()));
                    delayMicrosecondsWithUpdate(std::stoi(it.commandArgument()), canTerminal);
                } else if (it.commandType() == CanCommandType::FLUSH_RX) {
                    printFlushResult(canTerminal, FlushType::RX);
                    arduino->flushRX();
                } else if (it.commandType() == CanCommandType::FLUSH_TX) {
                    printFlushResult(canTerminal, FlushType::TX);
                    arduino->flushTX();
                } else if (it.commandType() == CanCommandType::FLUSH_RX_TX) {
                    printFlushResult(canTerminal, FlushType::RX_TX);
                    arduino->flushRXTX();
                } else if (it.commandType() == CanCommandType::LOOP_START) {
                    loop = true;
                    loopCount = std::stoi(it.commandArgument());
                    loopCanCommands.clear();
                } else {
                    throw std::runtime_error(SERIAL_COMMAND_TYPE_NOT_IMPLEMENTED_STRING + it.commandArgument());
                }
            } else {
                if (it.commandType() == CanCommandType::LOOP_END) {
                    loop =  false;
                    doLoop(canTerminal, arduino, printRxResult, printTxResult, printDelayResult, printFlushResult, printLoopResult, loopCanCommands, loopCount);
                    loopCanCommands.clear();
                } else {
                    loopCanCommands.emplace_back(it);
                }
            }
        } catch (std::exception &e) {
            throw std::runtime_error(e.what());
        }
    }
}


void CanScriptExecutor::doLoop(CanTerminal *canTerminal, std::shared_ptr<Arduino> arduino,
                const std::function<void(CanTerminal *, const std::string &)> &printRxResult,
                const std::function<void(CanTerminal *, const std::string &)> &printTxResult,
                const std::function<void(CanTerminal *, DelayType, int)> &printDelayResult,
                const std::function<void(CanTerminal *, FlushType)> &printFlushResult,
                const std::function<void(CanTerminal *, LoopType, int, int)> &printLoopResult,
                const std::vector<CanCommand> &commands,
                int loopCount)
{
    using namespace ArduinoPCStrings;
    using namespace GeneralUtilities;
    if (loopCount == -1) {
        for (int i = 0; true; i++) {
            printLoopResult(canTerminal, LoopType::START, i, -1);
            for (auto &it : commands) {
                canTerminal->application()->processEvents();
                if (canTerminal->cancelScript()) {
                    return;
                }
                try {
                    if (it.commandType() == CanCommandType::WRITE) {
                        arduino->writeRawString(it.commandArgument());
                        printTxResult(canTerminal, it.commandArgument());
                    } else if (it.commandType() == CanCommandType::READ) {
                        printRxResult(canTerminal, arduino->readRawString());
                    } else if (it.commandType() == CanCommandType::DELAY_SECONDS) {
                       printDelayResult(canTerminal, DelayType::SECONDS, std::stoi(it.commandArgument()));
                       delaySecondsWithUpdate(std::stoi(it.commandArgument()), canTerminal);
                   } else if (it.commandType() == CanCommandType::DELAY_MILLISECONDS) {
                       printDelayResult(canTerminal, DelayType::MILLISECONDS, std::stoi(it.commandArgument()));
                       delayMillisecondsWithUpdate(std::stoi(it.commandArgument()), canTerminal);
                   } else if (it.commandType() == CanCommandType::DELAY_MICROSECONDS) {
                       printDelayResult(canTerminal, DelayType::MICROSECONDS, std::stoi(it.commandArgument()));
                       delayMicrosecondsWithUpdate(std::stoi(it.commandArgument()), canTerminal);
                   } else if (it.commandType() == CanCommandType::FLUSH_RX) {
                        printFlushResult(canTerminal, FlushType::RX);
                        arduino->flushRX();
                    } else if (it.commandType() == CanCommandType::FLUSH_TX) {
                        printFlushResult(canTerminal, FlushType::TX);
                        arduino->flushTX();
                    } else if (it.commandType() == CanCommandType::FLUSH_RX_TX) {
                        printFlushResult(canTerminal, FlushType::RX_TX);
                        arduino->flushRXTX();
                    }
                } catch (std::exception &e) {
                    throw std::runtime_error(e.what());
                }
            }
            printLoopResult(canTerminal, LoopType::END, i, -1);
        }
    } else {
        for (int i = 0 ; i < loopCount; i++) {
            printLoopResult(canTerminal, LoopType::START, i, loopCount);
            for (auto &it : commands) {
                canTerminal->application()->processEvents();
                if (canTerminal->cancelScript()) {
                    return;
                }
                try {
                    if (it.commandType() == CanCommandType::WRITE) {
                        arduino->writeRawString(it.commandArgument());
                        printTxResult(canTerminal, it.commandArgument());
                    } else if (it.commandType() == CanCommandType::READ) {
                        printRxResult(canTerminal, arduino->readRawString());
                    }
                    else if (it.commandType() == CanCommandType::DELAY_SECONDS) {
                        printDelayResult(canTerminal, DelayType::SECONDS, std::stoi(it.commandArgument()));
                        delaySecondsWithUpdate(std::stoi(it.commandArgument()), canTerminal);
                    } else if (it.commandType() == CanCommandType::DELAY_MILLISECONDS) {
                        printDelayResult(canTerminal, DelayType::MILLISECONDS, std::stoi(it.commandArgument()));
                        delayMillisecondsWithUpdate(std::stoi(it.commandArgument()), canTerminal);
                    } else if (it.commandType() == CanCommandType::DELAY_MICROSECONDS) {
                        printDelayResult(canTerminal, DelayType::MICROSECONDS, std::stoi(it.commandArgument()));
                        delayMicrosecondsWithUpdate(std::stoi(it.commandArgument()), canTerminal);
                    } else if (it.commandType() == CanCommandType::FLUSH_RX) {
                        printFlushResult(canTerminal, FlushType::RX);
                        arduino->flushRX();
                    } else if (it.commandType() == CanCommandType::FLUSH_TX) {
                        printFlushResult(canTerminal, FlushType::TX);
                        arduino->flushTX();
                    } else if (it.commandType() == CanCommandType::FLUSH_RX_TX) {
                        printFlushResult(canTerminal, FlushType::RX_TX);
                        arduino->flushRXTX();
                    }
                } catch (std::exception &e) {
                    throw std::runtime_error(e.what());
                }
            }
            if (i+1 != loopCount) {
                printLoopResult(canTerminal, LoopType::END, i, loopCount);
            }
        }
    }
    printLoopResult(canTerminal, LoopType::END, loopCount-1, loopCount);
}


void CanScriptExecutor::delaySecondsWithUpdate(int howLong, CanTerminal *canTerminal)
{
    int millisecondsToDelay{howLong * 1000};
    auto startTime = std::chrono::high_resolution_clock::now();
    auto endTime = std::chrono::high_resolution_clock::now();
    long long int elapsedTime{0};

    do {
        canTerminal->application()->processEvents();
        if (canTerminal->cancelScript()) {
            return;
        }
        endTime = std::chrono::high_resolution_clock::now();
        elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    } while (elapsedTime <= millisecondsToDelay);
}


void CanScriptExecutor::delayMillisecondsWithUpdate(int howLong, CanTerminal *canTerminal)
{
    auto startTime = std::chrono::high_resolution_clock::now();
    auto endTime = std::chrono::high_resolution_clock::now();
    long long int elapsedTime{0};
    do {
        canTerminal->application()->processEvents();
        if (canTerminal->cancelScript()) {
            return;
        }
        endTime = std::chrono::high_resolution_clock::now();
        elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    } while (elapsedTime <= howLong);
}

void CanScriptExecutor::delayMicrosecondsWithUpdate(int howLong, CanTerminal *canTerminal)
{
    auto startTime = std::chrono::high_resolution_clock::now();
    auto endTime = std::chrono::high_resolution_clock::now();
    long long int elapsedTime{0};

    do {
        canTerminal->application()->processEvents();
        if (canTerminal->cancelScript()) {
            return;
        }
        endTime = std::chrono::high_resolution_clock::now();
        elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
    } while (elapsedTime <= howLong);
}
