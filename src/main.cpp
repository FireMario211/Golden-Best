// this is literally the mod
#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
using namespace geode::prelude;

// definitely not taken from Prism Menu
std::string getNodeName(cocos2d::CCObject* node) {
    #ifdef GEODE_IS_WINDOWS
        return typeid(*node).name() + 6;
    #else 
        {
            std::string ret;
            int status = 0;
            auto demangle = abi::__cxa_demangle(typeid(*node).name(), 0, 0, &status);
            if (status == 0) {
                ret = demangle;
            }
            free(demangle);

            return ret;
        }
    #endif
    }
class $modify(PlayLayer) {
    CCLabelBMFont* percentLabel;
    float oldScale;
    void updateProgressbar() {
        PlayLayer::updateProgressbar();
        if (m_fields->percentLabel == nullptr) {
            for (size_t i = 0; i < this->getChildrenCount(); i++) {
                auto obj = this->getChildren()->objectAtIndex(i);
                if (getNodeName(obj) == "cocos2d::CCLabelBMFont" && m_fields->percentLabel == nullptr) {
                    auto labelTest = static_cast<CCLabelBMFont*>(obj);
                    if (strlen(labelTest->getString()) < 10) {
                        m_fields->percentLabel = labelTest;
                        m_fields->oldScale = labelTest->getScale();
                        break;
                    }
                }
            }
        } else {
            auto showInPractice = Mod::get()->getSettingValue<bool>("practice-mode");
            if (!m_level->isPlatformer()) {
                if (!showInPractice && m_isPracticeMode) {
                    m_fields->percentLabel->setFntFile("bigFont.fnt");
                    m_fields->percentLabel->setScale(m_fields->oldScale);
                    return;
                }
                float percent = (this->m_player1->getPositionX() / m_levelLength) * 100;
                if (percent > m_level->m_normalPercent) {
                    m_fields->percentLabel->setFntFile("goldFont.fnt");
                    m_fields->percentLabel->setScale(m_fields->oldScale + 0.15F);
                } else {
                    m_fields->percentLabel->setFntFile("bigFont.fnt");
                    m_fields->percentLabel->setScale(m_fields->oldScale);
                }
            }
        }
    }
    void updateTimeLabel(int p0, int p1, bool p2) {
        PlayLayer::updateTimeLabel(p0,p1,p2);
        auto showInPractice = Mod::get()->getSettingValue<bool>("practice-mode");
        if (m_level->isPlatformer()) {
            if (!showInPractice && m_isPracticeMode) {
                m_fields->percentLabel->setFntFile("bigFont.fnt");
                m_fields->percentLabel->setScale(m_fields->oldScale);
                return;
            }
            if (p0 > m_level->m_bestTime) {
                m_fields->percentLabel->setFntFile("goldFont.fnt");
                m_fields->percentLabel->setScale(m_fields->oldScale + 0.15F);
            } else {
                m_fields->percentLabel->setFntFile("bigFont.fnt");
                m_fields->percentLabel->setScale(m_fields->oldScale);
            }
        }
    }
};
