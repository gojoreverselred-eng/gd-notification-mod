#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(MyPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;
        
        // Create instant win button
        auto winBtn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_plainBtn_001.png"),
            this,
            menu_selector(MyPlayLayer::onInstantWin)
        );
        
        auto label = CCLabelBMFont::create("WIN", "bigFont.fnt");
        label->setScale(0.5f);
        winBtn->addChild(label);
        label->setPosition(winBtn->getContentSize() / 2);
        
        auto menu = this->getChildByID("geode-menu");
        if (!menu) {
            menu = CCMenu::create();
            menu->setID("geode-menu");
            menu->setPosition(30, 30);
            this->addChild(menu);
        }
        
        menu->addChild(winBtn);
        winBtn->setPosition(0, 0);
        
        return true;
    }
    
    void onInstantWin(CCObject*) {
        this->playPlatformerEndAnimationToPos({0, 105}, true);
    }
};
