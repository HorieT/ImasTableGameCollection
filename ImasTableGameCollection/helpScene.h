//�w���v�̃V�[��
#pragma once

#include"Resource.h"
#include "ViewGlobal.h"
#include "settingFile.h"


class helpScene : public App::Scene {
private:
    const fontResource& _font;
    const colorResource& _color;
    const textureResource& _textur;
    const soundResource& _sound;

    const settingFile& _setting;

public:
    helpScene(const InitData& init) :
        IScene(init),
        _font(fontResource::get_instance()),
        _color(colorResource::get_instance()),
        _textur(textureResource::get_instance()),
        _sound(soundResource::get_instance()),
        _setting(settingFile::get_instance()) {

    }


    void update() override {
        if (SimpleGUI::ButtonAt(TEXT_RETURN, Scene::Size() + textureResource::RETURN_BUTTON_OFFSET)) {
            _sound.RETURN_BUTTON.playOneShot(_setting.get_volume());
            changeScene(ViewState::Title);
        }
    }

    void draw() const override {
        Scene::SetBackground(_color.TITLE);
        _font.HELP_TEXT(
          U"�쐬��: �S�Ί��L(�@���瑁,�^�ǐ���,�ڑ򂠂��ВS��)\ntwitter: @tekkamaki200\nGitHub: HorieT\n\n�o�O�̕񍐂�ӌ�������܂�����,twitter�����|�W�g���̕��ɂ��A�����������B").drawAt(Scene::Center());
    }
};