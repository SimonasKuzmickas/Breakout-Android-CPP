#pragma once

#include "../scene/IScene.h"
#include "../ui/UILayoutHandler.h"
#include "../ui/UILayoutGameOver.h"
#include "SceneId.h"

namespace Breakout::Scenes {

// GameOverScene: Displays the game over UI
// transitions to Menu or retries GameScene
class GameOverScene : public IScene {
public:
    explicit GameOverScene(AppContext* context)
            : appContext(context) {}

    void onStart() override {
        auto layout = std::make_shared<UILayoutGameOver>(appContext);
        addComponent(layout);

        layout->onMenuPressed.addListener([this]() {
            sceneManager->requestChange((int)SceneId::Menu);
        });

        layout->onRetryPressed.addListener([this]() {
            sceneManager->requestChange((int)SceneId::GameOver);
        });
    }

    void onUpdate() override {}
    void onDestroy() override {}

private:
    AppContext* appContext;
};

}
