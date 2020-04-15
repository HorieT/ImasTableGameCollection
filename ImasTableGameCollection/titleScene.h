//タイトルのVMクラス
#pragma once

#include"Resource.h"
#include "ViewGlobal.h"
#include "settingFile.h"


class titleScene : public App::Scene{
private:
    const fontResource& _font;
    const colorResource& _color;
    const textureResource& _textur;
    const soundResource& _sound;

    const settingFile& _setting;

public:
	titleScene(const InitData& init) : 
        IScene(init), 
        _font(fontResource::get_instance()), 
        _color(colorResource::get_instance()),
        _textur(textureResource::get_instance()),
        _sound(soundResource::get_instance()),
        _setting(settingFile::get_instance()){
        
	}


    void update() override {
        const auto& setting_icon = _textur.CONFIG_ICON.drawAt(Vec2(Scene::Center().x, Scene::Center().y + 170));
        const auto& help_icon = _textur.QUESTION_ICON.drawAt(Vec2(Scene::Size().x - 40, 40));
        
        if (SimpleGUI::ButtonAt(U"Start", Vec2(Scene::Center().x, Scene::Center().y + 90))) {
            _sound.ENTER_BUTTON.playOneShot(_setting.get_volume());
            changeScene(ViewState::Game, 50);
        }

        if (setting_icon.mouseOver()) {
            setting_icon.draw(_color.SELECTED_ITEM_OVERLAY);
        }
        if (setting_icon.leftClicked()) {
            changeScene(ViewState::Setting, 50);
        }

        if (help_icon.mouseOver()) {
            help_icon.draw(_color.SELECTED_ITEM_OVERLAY);
        }
        if (help_icon.leftClicked()) {
            changeScene(ViewState::Help, 50);
        }
    }

    void draw() const override {
        Scene::SetBackground(_color.TITLE);
        _font.TITLE(TEXT_TITLE).drawAt(Scene::CenterF());
    }
};