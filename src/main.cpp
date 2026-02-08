#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

bool autoWinEnabled = false;
bool noclipEnabled = false;
int fakeAttempts = 1;
float jumpHeight = 1.0f;

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
        
        auto menu = this->getChildByID("center-button-menu");
        if (menu) {
            menu->addChild(voidBtn);
            menu->updateLayout();
        }
        
        return true;
    }
    
    void onVoidMenu(CCObject*) {
        auto popup = createQuickPopup(
            "Void X Menu",
            "",
            "Cancel", "OK",
            [](FLAlertLayer* alert, bool btn2) {
                if (!btn2) return;
            }
        );
        
        auto layer = popup->m_mainLayer;
        
        auto autoWinToggle = CCMenuItemToggler::createWithStandardSprites(
            this, menu_selector(MyPauseLayer::onToggleAutoWin), 0.6f
        );
        autoWinToggle->toggle(autoWinEnabled);
        autoWinToggle->setPosition({-100, 50});
        
        auto autoWinLabel = CCLabelBMFont::create("Auto Win (Legit)", "bigFont.fnt");
        autoWinLabel->setScale(0.4f);
        autoWinLabel->setPosition({20, 50});
        
        auto noclipToggle = CCMenuItemToggler::createWithStandardSprites(
            this, menu_selector(MyPauseLayer::onToggleNoclip), 0.6f
        );
        noclipToggle->toggle(noclipEnabled);
        noclipToggle->setPosition({-100, 20});
        
        auto noclipLabel = CCLabelBMFont::create("Noclip", "bigFont.fnt");
        noclipLabel->setScale(0.4f);
        noclipLabel->setPosition({20, 20});
        
        auto attemptLabel = CCLabelBMFont::create("Fake Attempts:", "bigFont.fnt");
        attemptLabel->setScale(0.4f);
        attemptLabel->setPosition({-60, -10});
        
        auto attemptInput = TextInput::create(100, "Number");
        attemptInput->setString(std::to_string(fakeAttempts));
        attemptInput->setPosition({60, -10});
        attemptInput->setCallback([](const std::string& text) {
            try {
                fakeAttempts = std::stoi(text);
            } catch(...) {}
        });
        
        auto jumpLabel = CCLabelBMFont::create("Jump Height:", "bigFont.fnt");
        jumpLabel->setScale(0.4f);
        jumpLabel->setPosition({-60, -40});
        
        auto jumpInput = TextInput::create(100, "1.0");
        jumpInput->setString(fmt::format("{:.1f}", jumpHeight));
        jumpInput->setPosition({60, -40});
        jumpInput->setCallback([](const std::string& text) {
            try {
                jumpHeight = std::stof(text);
            } catch(...) {}
        });
        
        auto btnMenu = CCMenu::create();
        btnMenu->addChild(autoWinToggle);
        btnMenu->addChild(noclipToggle);
        btnMenu->setPosition(layer->getContentSize() / 2);
        layer->addChild(btnMenu);
        
        layer->addChild(autoWinLabel);
        layer->addChild(noclipLabel);
        layer->addChild(attemptLabel);
        layer->addChild(attemptInput);
        layer->addChild(jumpLabel);
        layer->addChild(jumpInput);
    }
    
    void onToggleAutoWin(CCObject* sender) {
        autoWinEnabled = !static_cast<CCMenuItemToggler*>(sender)->isToggled();
    }
    
    void onToggleNoclip(CCObject* sender) {
        noclipEnabled = !static_cast<CCMenuItemToggler*>(sender)->isToggled();
    }
};
