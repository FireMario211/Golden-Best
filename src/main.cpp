// this is literally the mod
#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
using namespace geode::prelude;

class $modify(PlayLayer) {
    CCLabelBMFont* percentLabel;
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
                        m_fields->oldScale = labelTest->getScale();
                        break;
                    }
                }
            }
        } else {
            auto showInPractice = Mod::get()->getSettingValue<bool>("practice-mode");
            if (!m_level->isPlatformer()) {
                float anchorPointX = m_fields->percentLabel->getAnchorPoint().x;
                if (!showInPractice && m_isPracticeMode) {
                    m_fields->percentLabel->setFntFile("bigFont.fnt");
                    m_fields->percentLabel->setScale(m_fields->oldScale);
                    return;
                }
                /* 
                   pfVar2 = (float *)(**(code **)(**(int **)&(this->GJBaseGameLayer_data).field_0x738 + 100))
                              (local_14);
  fVar6 = *(float *)&this->field_0x2aa0;
  fVar5 = *pfVar2;
  iVar4 = *(int *)(*(int *)&(this->GJBaseGameLayer_data).field_0x4a0 + 0x414);
  if (0 < iVar4) {
    fVar5 = (float)*(int *)&(this->GJBaseGameLayer_data).field_0x200;
    fVar6 = (float)iVar4;
  }
  local_30 = (fVar5 / fVar6) * 100.0;
  if (local_30 < 0.0) {
LAB_006e5591:
    local_30 = 0.0;
LAB_006e5599:
    fVar6 = 0.01;
    if (0.01 <= local_30) goto LAB_006e54f1;
  }
  else {
    if (local_30 == 100.0) {
      if (local_30 < 0.0) goto LAB_006e5591;
      goto LAB_006e5599;
    }
    local_30 = 100.0;
LAB_006e54f1:
    fVar6 = local_30;
  }
  fVar5 = *(float *)&this[1].field_0xd4;
  if ((fVar6 / 100.0) * fVar5 <= fVar5) {
    fVar6 = 0.01;
    if (0.01 <= local_30) {
      fVar6 = local_30;
    }
    fVar5 = (fVar6 / 100.0) * fVar5;
  }
  iVar4 = **(int **)&this[1].field_0xd0;
                 */
                //m_unknownD00
                //m_unknownDB4
                //float percent = (m_player1->getPositionX() / m_endPortal->getSpawnPos().x) * 100;
                float percent = static_cast<float>(PlayLayer::getCurrentPercentInt());
                if (m_fields->oldAnchorPoint == -1.0F) {
                    m_fields->oldAnchorPoint = m_fields->percentLabel->getAnchorPoint().y;
                }
                
                //std::cout << "end = " << this->m_endPortal->getSpawnPos().x << " , player = " << m_player1->getPositionX() << ", normalPercent = " << m_level->m_normalPercent << ", divided = " << percent << std::endl;
                if (percent > (m_level->m_normalPercent)) {
                    m_fields->percentLabel->setFntFile("goldFont.fnt");
                    m_fields->percentLabel->setScale(m_fields->oldScale + 0.15F);
                    m_fields->percentLabel->setAnchorPoint({anchorPointX,m_fields->oldAnchorPoint-0.05F}); // since robert wont allow us to change the Y pos, we will just have to change the anchor point!
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
        if (m_level->isPlatformer() && Mod::get()->getSettingValue<bool>("platformer-mode")) {
            float anchorPointX = m_fields->percentLabel->getAnchorPoint().x;
            if (!showInPractice && m_isPracticeMode) {
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
};
