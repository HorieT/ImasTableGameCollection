//ê›íËâÊñ ÇÃÉNÉâÉX
#pragma once

#include"Resource.h"
#include "ViewGlobal.h"
#include "settingFile.h"


class settingScene : public App::Scene {
private:
    const fontResource& _font;
    const colorResource& _color;
    const textureResource& _textur;
    const soundResource& _sound;

    settingFile& _setting;
    double _volume;
    double _window_whidth;
    Audio _dumyy{};

public:
    settingScene(const InitData& init) :
        IScene(init),
        _font(fontResource::get_instance()),
        _color(colorResource::get_instance()),
        _textur(textureResource::get_instance()),
        _sound(soundResource::get_instance()),
        _setting(settingFile::get_instance()){
        
        _volume = _setting.get_volume();
    }


    void update() override {
        const auto& sample = _textur.VOLUME__UP_ICON.drawAt(Scene::Center() + Size(140, 0));
        SimpleGUI::SliderAt(TEXT_VOLUME, _volume, Scene::CenterF());
        if (sample.mouseOver()) {
            sample.draw(_color.SELECTED_ITEM_OVERLAY);
        }
        if (sample.leftClicked()) {
            _dumyy.playOneShot(_volume);
        }

        if (SimpleGUI::ButtonAt(TEXT_ENTER, Size(textureResource::ENTER_BUTTON_OFFSET.x, Scene::Height() + textureResource::ENTER_BUTTON_OFFSET.y))) {
            _setting.set_volume(_volume);
            _sound.ENTER_BUTTON.playOneShot(_setting.get_volume());
            changeScene(ViewState::Title, 50);
        }
        if (SimpleGUI::ButtonAt(TEXT_RETURN, Scene::Size() + textureResource::RETURN_BUTTON_OFFSET)) {
            _sound.RETURN_BUTTON.playOneShot(_setting.get_volume());
            changeScene(ViewState::Title, 50);
        }
    }

    void draw() const override {
        Scene::SetBackground(_color.TITLE);
        _font.SCENE_TITLE(TEXT_SETTING).drawAt(Scene::Center().x, 50);
    }
};