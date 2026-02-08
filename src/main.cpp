#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

bool autoWinEnabled = false;
bool noclipEnabled = false;
int fakeAttempts = 1;

class $modify(MyPlayLayer, PlayLayer) {
    void update(float dt) {
        PlayLayer::update(dt);
        
        if (autoWinEnabled) {
            this->handleButton(true, 1, true);
        }
        
        if (noclipEnabled && m_player1) {
            m_player1->m_isOnGround = true;
            m_player1->m_isDead = false;
            if (m_player2) {
                m_player2->m_isOnGround = true;
                m_player2->m_isDead = false;
            }
        }
    }
    
    void resetLevel() {
        PlayLayer::resetLevel();
        
        if (fakeAttempts > 0 && m_level) {
            m_level->m_attempts = fakeAttempts - 1;
        }
    }
};

class $modify(MyPauseLayer, PauseLayer) {
    bool init(bool p0) {
        if (!PauseLayer::init(p0)) return false;
        
        auto voidBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Void X", "bigFont.fnt", "GJ_button_01.png"),
            this,
            menu_selector(MyPauseLayer::onVoidMenu)
        );
        voidBtn->setID("void-x-button");
        
        auto menu = this->getChildByID("left-button-menu");
        if (!menu) {
            menu = CCMenu::create();
            menu->setID("left-button-menu");
            menu->setPosition({30, 160});
            this->addChild(menu);
        }
        
        menu->addChild(voidBtn);
        menu->updateLayout();
        
        return true;
    }
    
    void onVoidMenu(CCObject*) {
        auto popup = FLAlertLayer::create(
            "Void X Menu",
            "",
            "OK"
        );
        
        auto layer = popup->m_mainLayer;
        auto winSize = layer->getContentSize();
        
        auto toggleMenu = CCMenu::create();
        toggleMenu->setPosition({0, 0});
        
        auto autoWinToggle = CCMenuItemToggler::createWithStandardSprites(
            this, menu_selector(MyPauseLayer::onToggleAutoWin), 0.8f
        );
        autoWinToggle->toggle(autoWinEnabled);
        autoWinToggle->setPosition({winSize.width / 2 - 80, winSize.height / 2 + 30});
        
        auto autoWinLabel = CCLabelBMFont::create("Auto Win (Hold)", "bigFont.fnt");
        autoWinLabel->setScale(0.5f);
        autoWinLabel->setPosition({winSize.width / 2 + 20, winSize.height / 2 + 30});
        autoWinLabel->setAnchorPoint({0, 0.5f});
        
        auto noclipToggle = CCMenuItemToggler::createWithStandardSprites(
            this, menu_selector(MyPauseLayer::onToggleNoclip), 0.8f
        );
        noclipToggle->toggle(noclipEnabled);
        noclipToggle->setPosition({winSize.width / 2 - 80, winSize.height / 2});
        
        auto noclipLabel = CCLabelBMFont::create("Noclip", "bigFont.fnt");
        noclipLabel->setScale(0.5f);
        noclipLabel->setPosition({winSize.width / 2 + 20, winSize.height / 2});
        noclipLabel->setAnchorPoint({0, 0.5f});
        
        auto attemptLabel = CCLabelBMFont::create("Fake Attempts:", "bigFont.fnt");
        attemptLabel->setScale(0.4f);
        attemptLabel->setPosition({winSize.width / 2 - 60, winSize.height / 2 - 30});
        attemptLabel->setAnchorPoint({0, 0.5f});
        
        auto attemptInput = TextInput::create(80, "1");
        attemptInput->setString(std::to_string(fakeAttempts));
        attemptInput->setPosition({winSize.width / 2 + 40, winSize.height / 2 - 30});
        attemptInput->setCallback([](const std::string& text) {
            try {
                fakeAttempts = std::stoi(text);
            } catch(...) {
                fakeAttempts = 1;
            }
        });
        
        toggleMenu->addChild(autoWinToggle);
        toggleMenu->addChild(noclipToggle);
        
        layer->addChild(toggleMenu);
        layer->addChild(autoWinLabel);
        layer->addChild(noclipLabel);
        layer->addChild(attemptLabel);
        layer->addChild(attemptInput);
        
        popup->show();
    }
    
    void onToggleAutoWin(CCObject* sender) {
        autoWinEnabled = !static_cast<CCMenuItemToggler*>(sender)->isToggled();
    }
    
    void onToggleNoclip(CCObject* sender) {
        noclipEnabled = !static_cast<CCMenuItemToggler*>(sender)->isToggled();
    }
};
