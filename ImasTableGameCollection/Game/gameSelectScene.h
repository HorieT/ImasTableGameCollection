//ゲーム選択のVMクラス
#pragma once

#include "../Resource.h"
#include "GameSelectGlobal.h"
#include "Games/Games.h"


class gameSelectScene : public GamesManager::Scene {
private:
    //model
    const fontResource& _font;
    const colorResource& _color;
    const soundResource& _sound;
    const settingFile& _setting;
    
public:
    gameSelectScene(const InitData& init) : 
        IScene(init), 
        _font(fontResource::get_instance()), 
        _color(colorResource::get_instance()),
        _sound(soundResource::get_instance()),
        _setting(settingFile::get_instance()) {

    }

    void update() override {
        if (SimpleGUI::ButtonAt(U"神経衰弱", Vec2(Scene::Center().x, Scene::Center().y))) {
            _sound.ENTER_BUTTON.playOneShot(_setting.get_volume());
            changeScene(Games::Concentaration, 50);
        }
        if (SimpleGUI::ButtonAt(TEXT_RETURN, Scene::Size() + textureResource::RETURN_BUTTON_OFFSET)) {
            _sound.RETURN_BUTTON.playOneShot(_setting.get_volume());
            getData() = true;
        }
    }

    void draw() const override {
        Scene::SetBackground(_color.GAME_SELECT);
        _font.GAME_TITLE(TEXT_GAME_SELECTT).drawAt(Scene::Center().x, 90);
    }
};