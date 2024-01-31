// this is literally the mod
#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
using namespace geode::prelude;

class $modify(PlayLayer) {
    CCLabelBMFont* percentLabel;
    float oldScale;
    void updateProgressbar() {
        PlayLayer::updateProgressbar();
        if (m_fields->percentLabel == nullptr) {
            for (size_t i = 0; i < this->getChildrenCount(); i++) {
                auto obj = this->getChildren()->objectAtIndex(i);
                if (typeinfo_cast<CCLabelBMFont*>(obj) && m_fields->percentLabel == nullptr) {
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
                if (percent > (m_level->m_normalPercent + 1)) {
                    m_fields->percentLabel->setFntFile("goldFont.fnt");
                    m_fields->percentLabel->setScale(m_fields->oldScale + 0.15F);
                    m_fields->percentLabel->setAnchorPoint({0.0,0.45}); // since robert wont allow us to change the Y pos, we will just have to change the anchor point!
                    //m_fields->percentLabel->setPositionY(m_fields->yPosOLD + 8);
                    m_fields->percentLabel->setPositionY(8);
                } else {
                    m_fields->percentLabel->setFntFile("bigFont.fnt");
                    m_fields->percentLabel->setScale(m_fields->oldScale);
                    m_fields->percentLabel->setAnchorPoint({0,0.5});
                }
            }
        }
    }
    void updateTimeLabel(int p0, int p1, bool p2) {
        PlayLayer::updateTimeLabel(p0,p1,p2);
        if (m_fields->percentLabel == nullptr) return; // prevent crashes
        auto showInPractice = Mod::get()->getSettingValue<bool>("practice-mode");
        if (m_level->isPlatformer() && Mod::get()->getSettingValue<bool>("platforer-mode")) {
            if (!showInPractice && m_isPracticeMode) {
                m_fields->percentLabel->setFntFile("bigFont.fnt");
                m_fields->percentLabel->setScale(m_fields->oldScale);
                return;
            }
            if (p0 > m_level->m_bestTime) {
                m_fields->percentLabel->setFntFile("goldFont.fnt");
                m_fields->percentLabel->setScale(m_fields->oldScale + 0.15F);
                m_fields->percentLabel->setAnchorPoint({0.0,0.45});
            } else {
                m_fields->percentLabel->setFntFile("bigFont.fnt");
                m_fields->percentLabel->setScale(m_fields->oldScale);
                m_fields->percentLabel->setAnchorPoint({0,0.5});
            }
        }
    }
};
