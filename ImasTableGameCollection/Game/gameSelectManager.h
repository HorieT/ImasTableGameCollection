//ゲーム選択マネージャー
#pragma once

#include "../ViewGlobal.h"
#include "GameSelectGlobal.h"

#include "gameSelectScene.h"
#include "gameManagerBase.h"
#include "Games/Games.h"



class gameSelectManager final : public App::Scene {
private:
    GamesManager manager;

public:
    gameSelectManager(const InitData& init) : IScene(init) {
        manager.get() = std::make_shared<bool>(false);
        manager.add<gameSelectScene>(Games::Select);
        manager.add<gameManagerBase<concentration>>(Games::Concentaration);
    }

    void update() override{
        manager.updateScene();
        if (*manager.get())changeScene(ViewState::Title, 50);
    }

    void draw() const override {
        manager.drawScene();
    }
};