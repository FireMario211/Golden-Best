// this is literally the mod
// TODO: (and remove me after) find m_percentageLabel for imac, m1, Android32, Android64, and iOS
#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
using namespace geode::prelude;

class $modify(PlayLayer) {
    struct Fields {
        bool initialized = false;
        float oldScale;
        float oldAnchorPoint = -1.0F;
        bool doCustomColor;
        ccColor3B customColor;

        CCLabelBMFont* percentLabel;
    };

    void updateProgressbar() {
        PlayLayer::updateProgressbar();
        if (!m_fields->initialized) {
            m_fields->doCustomColor = Mod::get()->getSettingValue<bool>("enable-colors");
            m_fields->customColor = Mod::get()->getSettingValue<ccColor3B>("custom-color");
        }
        if (!m_fields->initialized && m_percentageLabel) {
            m_fields->percentLabel = m_percentageLabel;
            m_fields->oldScale = m_percentageLabel->getScale();
            m_fields->initialized = true;
            return;
        }
        if (!m_fields->initialized && m_fields->percentLabel == nullptr) {
            for (size_t i = 0; i < this->getChildrenCount(); i++) {
                auto obj = this->getChildren()->objectAtIndex(i);
                if (typeinfo_cast<CCLabelBMFont*>(obj) && m_fields->percentLabel == nullptr) {
                    auto labelTest = static_cast<CCLabelBMFont*>(obj);
                    auto labelEnding = labelTest->getString() + (strlen(labelTest->getString()) - 1);
                    if (strcmp(labelEnding, "%") == 0) {
                        m_fields->percentLabel = labelTest;
                        m_fields->oldScale = labelTest->getScale();
                        m_fields->initialized = true;
                        break;
                    }
                }
            }
            return;
        }
        auto showInPractice = Mod::get()->getSettingValue<bool>("practice-mode");
        auto showInTestMode = Mod::get()->getSettingValue<bool>("test-mode");
        if (!m_level->isPlatformer()) {
            float anchorPointX = m_fields->percentLabel->getAnchorPoint().x;
            if ((!showInPractice && m_isPracticeMode) || (!showInTestMode && m_isTestMode)) {
                if (m_fields->doCustomColor) {
                    m_fields->percentLabel->setColor(ccWHITE);
                } else {
                    m_fields->percentLabel->setFntFile("bigFont.fnt");
                    m_fields->percentLabel->setScale(m_fields->oldScale);
                }
                return;
            }
            float percent = static_cast<float>(PlayLayer::getCurrentPercentInt());
            if (m_fields->oldAnchorPoint == -1.0F) {
                m_fields->oldAnchorPoint = m_fields->percentLabel->getAnchorPoint().y;
            }
            if (percent > (m_level->m_normalPercent)) {
                if (m_fields->doCustomColor) {
                    m_fields->percentLabel->setColor(m_fields->customColor);
                } else {
                    m_fields->percentLabel->setFntFile("goldFont.fnt");
                    m_fields->percentLabel->setScale(m_fields->oldScale + 0.15F);
                    m_fields->percentLabel->setAnchorPoint({anchorPointX,m_fields->oldAnchorPoint-0.05F}); // since robert wont allow us to change the Y pos, we will just have to change the anchor point!
                }
            } else {
                if (m_fields->doCustomColor) {
                    m_fields->percentLabel->setColor(ccWHITE);
                } else {
                    m_fields->percentLabel->setFntFile("bigFont.fnt");
                    m_fields->percentLabel->setScale(m_fields->oldScale);
                    m_fields->percentLabel->setAnchorPoint({anchorPointX,m_fields->oldAnchorPoint});
                }
            }
        }
    }
    void updateTimeLabel(int p0, int p1, bool p2) {
        PlayLayer::updateTimeLabel(p0,p1,p2);
        if (m_fields->percentLabel == nullptr) return; // prevent crashes
        float actualTime = p0 + (static_cast<float>(p1) / 100.F);
        auto showInPractice = Mod::get()->getSettingValue<bool>("practice-mode");
        auto showInTestMode = Mod::get()->getSettingValue<bool>("test-mode");
        if (m_level->isPlatformer() && Mod::get()->getSettingValue<bool>("platformer-mode")) {
            float anchorPointX = m_fields->percentLabel->getAnchorPoint().x;
            if ((!showInPractice && m_isPracticeMode) || (!showInTestMode && m_isTestMode)) {
                if (m_fields->doCustomColor) {
                    m_fields->percentLabel->setColor(ccWHITE);
                } else {
                    m_fields->percentLabel->setFntFile("bigFont.fnt");
                    m_fields->percentLabel->setScale(m_fields->oldScale);
                }
                return;
            }
            if (m_fields->oldAnchorPoint == -1.0F) {
                m_fields->oldAnchorPoint = m_fields->percentLabel->getAnchorPoint().y;
            }

            if (actualTime < (static_cast<float>(m_level->m_bestTime) / 1000.F) || m_level->m_bestTime == 0) {
                if (m_fields->doCustomColor) {
                    m_fields->percentLabel->setColor(m_fields->customColor);
                } else {
                    m_fields->percentLabel->setFntFile("goldFont.fnt");
                    m_fields->percentLabel->setScale(m_fields->oldScale + 0.15F);
                    m_fields->percentLabel->setAnchorPoint({anchorPointX,m_fields->oldAnchorPoint-0.05F});
                }
            } else {
                if (m_fields->doCustomColor) {
                    m_fields->percentLabel->setColor(ccWHITE);
                } else {
                    m_fields->percentLabel->setFntFile("bigFont.fnt");
                    m_fields->percentLabel->setScale(m_fields->oldScale);
                    m_fields->percentLabel->setAnchorPoint({anchorPointX,m_fields->oldAnchorPoint});
                }
            }
        }
    }
};
