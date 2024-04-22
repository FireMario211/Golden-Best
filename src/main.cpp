// this is literally the mod
#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
using namespace geode::prelude;

#ifdef GEODE_IS_IOS
bool dontChangeFont = true;
#else
bool dontChangeFont = false;
#endif

class $modify(PlayLayer) {
    CCLabelBMFont* percentLabel;
    CCLabelBMFont* goldenLabel;
    float oldScale;
    float oldAnchorPoint = -1.0F;
    void updateProgressbar() {
        PlayLayer::updateProgressbar();
        if (m_fields->percentLabel == nullptr) {
            for (size_t i = 0; i < this->getChildrenCount(); i++) {
                auto obj = this->getChildren()->objectAtIndex(i);
                if (typeinfo_cast<CCLabelBMFont*>(obj) && m_fields->percentLabel == nullptr) {
                    auto labelTest = static_cast<CCLabelBMFont*>(obj);
                    auto labelEnding = labelTest->getString() + (strlen(labelTest->getString()) - 1);
                    if (strcmp(labelEnding, "%") == 0) {
                        m_fields->percentLabel = labelTest;
                        if (dontChangeFont) {
                            m_fields->goldenLabel = CCLabelBMFont::create("0%", "goldFont.fnt");
                            m_fields->goldenLabel->setScale(labelTest->getScale() + 0.15F);
                            m_fields->goldenLabel->setVisible(false);
                            m_fields->goldenLabel->setAnchorPoint({0.5,m_fields->percentLabel->getAnchorPoint().y-0.05F});
                            m_fields->goldenLabel->setZOrder(16);
                            m_fields->percentLabel->getParent()->addChild(m_fields->goldenLabel);
                        }
                        m_fields->oldScale = labelTest->getScale();
                        break;
                    }
                }
            }
        } else {
            if (m_fields->goldenLabel != nullptr) {
                m_fields->goldenLabel->setPosition(m_fields->percentLabel->getPosition());
                m_fields->goldenLabel->setString(m_fields->percentLabel->getString());
                m_fields->goldenLabel->setAnchorPoint({m_fields->percentLabel->getAnchorPoint().x, m_fields->goldenLabel->getAnchorPoint().y});
            }
            auto showInPractice = Mod::get()->getSettingValue<bool>("practice-mode");
            auto showInTestMode = Mod::get()->getSettingValue<bool>("test-mode");
            if (!m_level->isPlatformer()) {
                float anchorPointX = m_fields->percentLabel->getAnchorPoint().x;
                #ifndef GEODE_IS_IOS
                if ((!showInPractice && m_isPracticeMode) || (!showInTestMode && m_isTestMode)) {
                    if (dontChangeFont) {
                        m_fields->goldenLabel->setVisible(false);
                        m_fields->percentLabel->setVisible(true);
                    } else {
                        #ifndef GEODE_IS_IOS
                        m_fields->percentLabel->setFntFile("bigFont.fnt");
                        m_fields->percentLabel->setScale(m_fields->oldScale);
                        #endif
                    }
                    return;
                }
                #endif
#ifdef GEODE_IS_IOS
                float percent = static_cast<int>(PlayLayer::getCurrentPercent());
#else
                float percent = static_cast<float>(PlayLayer::getCurrentPercentInt());
#endif
                if (m_fields->oldAnchorPoint == -1.0F) {
                    m_fields->oldAnchorPoint = m_fields->percentLabel->getAnchorPoint().y;
                }
                if (percent > (m_level->m_normalPercent)) {
                    if (!dontChangeFont) {
                        #ifndef GEODE_IS_IOS
                        m_fields->percentLabel->setFntFile("goldFont.fnt");
                        m_fields->percentLabel->setScale(m_fields->oldScale + 0.15F);
                        m_fields->percentLabel->setAnchorPoint({anchorPointX,m_fields->oldAnchorPoint-0.05F}); // since robert wont allow us to change the Y pos, we will just have to change the anchor point!
                        #endif
                    } else {
                        m_fields->goldenLabel->setVisible(true);
                        m_fields->percentLabel->setScale(0);
                    }
                } else {
                    if (dontChangeFont) {
                        m_fields->goldenLabel->setVisible(false);
                        m_fields->percentLabel->setScale(m_fields->oldScale);
                    } else {
                        #ifndef GEODE_IS_IOS
                        m_fields->percentLabel->setFntFile("bigFont.fnt");
                        m_fields->percentLabel->setScale(m_fields->oldScale);
                        m_fields->percentLabel->setAnchorPoint({anchorPointX,m_fields->oldAnchorPoint});
                        #endif
                    }
                }
            }
        }
    }
#ifndef GEODE_IS_IOS
    void updateTimeLabel(int p0, int p1, bool p2) {
        PlayLayer::updateTimeLabel(p0,p1,p2);
        if (m_fields->percentLabel == nullptr) return; // prevent crashes
        float actualTime = p0 + (static_cast<float>(p1) / 100.F);
        auto showInPractice = Mod::get()->getSettingValue<bool>("practice-mode");
        auto showInTestMode = Mod::get()->getSettingValue<bool>("test-mode");
        if (m_level->isPlatformer() && Mod::get()->getSettingValue<bool>("platformer-mode")) {
            float anchorPointX = m_fields->percentLabel->getAnchorPoint().x;
            if ((!showInPractice && m_isPracticeMode) || (!showInTestMode && m_isTestMode)) {
                m_fields->percentLabel->setFntFile("bigFont.fnt");
                m_fields->percentLabel->setScale(m_fields->oldScale);
                return;
            }
            if (m_fields->oldAnchorPoint == -1.0F) {
                m_fields->oldAnchorPoint = m_fields->percentLabel->getAnchorPoint().y;
            }

            if (actualTime < (static_cast<float>(m_level->m_bestTime) / 1000.F) || m_level->m_bestTime == 0) {
                m_fields->percentLabel->setFntFile("goldFont.fnt");
                m_fields->percentLabel->setScale(m_fields->oldScale + 0.15F);
                m_fields->percentLabel->setAnchorPoint({anchorPointX,m_fields->oldAnchorPoint-0.05F});
            } else {
                m_fields->percentLabel->setFntFile("bigFont.fnt");
                m_fields->percentLabel->setScale(m_fields->oldScale);
                m_fields->percentLabel->setAnchorPoint({anchorPointX,m_fields->oldAnchorPoint});
            }
        }
    }
#endif
};
