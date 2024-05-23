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
    struct Fields {
        bool initialized = false;
        CCLabelBMFont* goldenLabel;
        float oldScale;
        float oldAnchorPoint = -1.0F;
    };

    void updateProgressbar() {
        PlayLayer::updateProgressbar();
        if (!m_fields->initialized) {
            if (dontChangeFont) {
                m_fields->goldenLabel = CCLabelBMFont::create("0%", "goldFont.fnt");
                m_fields->goldenLabel->setScale(m_percentageLabel->getScale() + 0.15F);
                m_fields->goldenLabel->setVisible(false);
                m_fields->goldenLabel->setAnchorPoint({0.5,m_percentageLabel->getAnchorPoint().y-0.05F});
                m_fields->goldenLabel->setZOrder(16);
                m_percentageLabel->getParent()->addChild(m_fields->goldenLabel);
            }
            m_fields->oldScale = m_percentageLabel->getScale();
            m_fields->initialized = true;
            return;
        }

        if (m_fields->goldenLabel != nullptr) {
            m_fields->goldenLabel->setPosition(m_percentageLabel->getPosition());
            m_fields->goldenLabel->setString(m_percentageLabel->getString());
            m_fields->goldenLabel->setAnchorPoint({m_percentageLabel->getAnchorPoint().x, m_fields->goldenLabel->getAnchorPoint().y});
        }
        auto showInPractice = Mod::get()->getSettingValue<bool>("practice-mode");
        auto showInTestMode = Mod::get()->getSettingValue<bool>("test-mode");
        if (!m_level->isPlatformer()) {
            float anchorPointX = m_percentageLabel->getAnchorPoint().x;
            #ifndef GEODE_IS_IOS
            if ((!showInPractice && m_isPracticeMode) || (!showInTestMode && m_isTestMode)) {
                if (dontChangeFont) {
                    m_fields->goldenLabel->setVisible(false);
                    m_percentageLabel->setVisible(true);
                } else {
                    #ifndef GEODE_IS_IOS
                    m_percentageLabel->setFntFile("bigFont.fnt");
                    m_percentageLabel->setScale(m_fields->oldScale);
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
                m_fields->oldAnchorPoint = m_percentageLabel->getAnchorPoint().y;
            }
            if (percent > (m_level->m_normalPercent)) {
                if (!dontChangeFont) {
                    #ifndef GEODE_IS_IOS
                    m_percentageLabel->setFntFile("goldFont.fnt");
                    m_percentageLabel->setScale(m_fields->oldScale + 0.15F);
                    m_percentageLabel->setAnchorPoint({anchorPointX,m_fields->oldAnchorPoint-0.05F}); // since robert wont allow us to change the Y pos, we will just have to change the anchor point!
                    #endif
                } else {
                    m_fields->goldenLabel->setVisible(true);
                    m_percentageLabel->setScale(0);
                }
            } else {
                if (dontChangeFont) {
                    m_fields->goldenLabel->setVisible(false);
                    m_percentageLabel->setScale(m_fields->oldScale);
                } else {
                    #ifndef GEODE_IS_IOS
                    m_percentageLabel->setFntFile("bigFont.fnt");
                    m_percentageLabel->setScale(m_fields->oldScale);
                    m_percentageLabel->setAnchorPoint({anchorPointX,m_fields->oldAnchorPoint});
                    #endif
                }
            }
        }
    }
#ifndef GEODE_IS_IOS
    void updateTimeLabel(int p0, int p1, bool p2) {
        PlayLayer::updateTimeLabel(p0,p1,p2);
        if (m_percentageLabel == nullptr) return; // prevent crashes
        float actualTime = p0 + (static_cast<float>(p1) / 100.F);
        auto showInPractice = Mod::get()->getSettingValue<bool>("practice-mode");
        auto showInTestMode = Mod::get()->getSettingValue<bool>("test-mode");
        if (m_level->isPlatformer() && Mod::get()->getSettingValue<bool>("platformer-mode")) {
            float anchorPointX = m_percentageLabel->getAnchorPoint().x;
            if ((!showInPractice && m_isPracticeMode) || (!showInTestMode && m_isTestMode)) {
                m_percentageLabel->setFntFile("bigFont.fnt");
                m_percentageLabel->setScale(m_fields->oldScale);
                return;
            }
            if (m_fields->oldAnchorPoint == -1.0F) {
                m_fields->oldAnchorPoint = m_percentageLabel->getAnchorPoint().y;
            }

            if (actualTime < (static_cast<float>(m_level->m_bestTime) / 1000.F) || m_level->m_bestTime == 0) {
                m_percentageLabel->setFntFile("goldFont.fnt");
                m_percentageLabel->setScale(m_fields->oldScale + 0.15F);
                m_percentageLabel->setAnchorPoint({anchorPointX,m_fields->oldAnchorPoint-0.05F});
            } else {
                m_percentageLabel->setFntFile("bigFont.fnt");
                m_percentageLabel->setScale(m_fields->oldScale);
                m_percentageLabel->setAnchorPoint({anchorPointX,m_fields->oldAnchorPoint});
            }
        }
    }
#endif
};
