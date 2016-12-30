#include "manualscreen/manualscreenscheduler.h"

std::shared_ptr<ManualScreen> ManualScreenScheduler::manualScreenByScreenIndex(int screenIndex) const
{
    using namespace ArduinoPCStrings;
    if (this->m_manualScreens.find(screenIndex) == this->m_manualScreens.end()) {
        throw std::runtime_error(MANUAL_SCREEN_BY_SCREEN_INDEX_INVALID_INDEX_STRING + std::to_string(screenIndex));
    } else {
        return this->m_manualScreens.at(screenIndex);
    }
}

void ManualScreenScheduler::removeManualScreen(int screenIndex)
{
    using namespace ArduinoPCStrings;
    if (this->m_manualScreens.find(screenIndex) == this->m_manualScreens.end()) {
        throw std::runtime_error(REMOVE_SCREEN_MANUAL_INVALID_INDEX_STRING + std::to_string(screenIndex));
    } else {
        this->m_manualScreens.erase(screenIndex);
    }
}

int ManualScreenScheduler::numberOfScreens() const
{
    return this->m_manualScreens.size();
}

void ManualScreenScheduler::addManualScreen(int screenIndex,
                                             std::shared_ptr<ArduinoPCIcons> apciPtr,
                                             std::shared_ptr<QDesktopWidget> qDesktopWidget,
                                             std::shared_ptr<GlobalLogger> globalLogger,
                                             std::shared_ptr<Arduino> arduino,
                                             QWidget *parent)
{
    using namespace ArduinoPCStrings;
    if (this->m_manualScreens.find(screenIndex) != this->m_manualScreens.end()) {
        throw std::runtime_error(ADD_MANUAL_ALREADY_EXISTS_STRING + std::to_string(screenIndex));
    } else {
        this->m_manualScreens.emplace(std::make_pair(screenIndex, std::make_shared<ManualScreen>(screenIndex,
                                                                                                 apciPtr,
                                                                                                 qDesktopWidget,
                                                                                                 globalLogger,
                                                                                                 arduino,
                                                                                                 parent)));
    }
}

std::vector<std::shared_ptr<ManualScreen>> ManualScreenScheduler::manualScreens() const
{
    std::vector<std::shared_ptr<ManualScreen>> returnVector;
    for (auto &it : this->m_manualScreens) {
        returnVector.emplace_back(it.second);
    }
    return returnVector;
}
