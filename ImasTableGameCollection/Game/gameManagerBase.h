//ゲームマネジャー
#pragma once

#include <type_traits>
#include "../Resource.h"
#include "../settingFile.h"
#include"GameSelectGlobal.h"



template<class T>
concept GameSceneClass = std::is_base_of_v<gameSceneBase, T> &&
requires {
    {T::title()} -> std::same_as<String>;
    {T::rule()} -> std::same_as<String>;
};

template<GameSceneClass T>
class gameManagerBase final: public GamesManager::Scene {
protected:
    GameStateManager manager;

public:
#pragma region 共通シーン
    //スタート画面
    class startScene final : public GameStateManager::Scene {
    private:
        //model
        const fontResource& _font;
        const colorResource& _color;
        const soundResource& _sound;
        const settingFile& _setting;
    public:
        startScene(const InitData& init) : 
            IScene(init),
            _font(fontResource::get_instance()), 
            _color(colorResource::get_instance()),
            _sound(soundResource::get_instance()),
            _setting(settingFile::get_instance()){

        }

        void update() override {
            if (SimpleGUI::ButtonAt(TEXT_GAME_START, Vec2(Scene::Center().x, Scene::Center().y - 30))) {
                _sound.ENTER_BUTTON.playOneShot(_setting.get_volume());
                changeScene(GameState::Game);
            }
            if (SimpleGUI::ButtonAt(TEXT_RULE, Vec2(Scene::Center().x, Scene::Center().y + 30))) {
                _sound.ENTER_BUTTON.playOneShot(_setting.get_volume());
                changeScene(GameState::Rule);
            }
            if (SimpleGUI::ButtonAt(TEXT_RETURN, Scene::Size() + textureResource::RETURN_BUTTON_OFFSET)) {
                _sound.RETURN_BUTTON.playOneShot(_setting.get_volume());
                getData() = true;
            }
        }

        void draw() const override {
            Scene::SetBackground(_color.GAME_RULE_OPTION);
            _font.GAME_TITLE(T::title()).drawAt(Vec2(Scene::Center().x, Scene::Center().y - 100));
        }
    };
    //ルール画面
    class ruleScene final : public GameStateManager::Scene {
    private:
        //model
        const fontResource& _font;
        const colorResource& _color;
        const soundResource& _sound;
        const settingFile& _setting;
    public:
        ruleScene(const InitData& init) :
            IScene(init),
            _font(fontResource::get_instance()),
            _color(colorResource::get_instance()),
            _sound(soundResource::get_instance()),
            _setting(settingFile::get_instance()) {
        }

        void update() override {
            if (SimpleGUI::ButtonAt(TEXT_RETURN, Scene::Size() + textureResource::RETURN_BUTTON_OFFSET)) {
                _sound.RETURN_BUTTON.playOneShot(_setting.get_volume());
                changeScene(GameState::Start);
            }
        }

        void draw() const override {
            Scene::SetBackground(_color.GAME_RULE_OPTION);
            _font.GAME_TITLE(T::title() + U" のルール").drawAt(Vec2(Scene::Center().x, 50));
        }
    };
#pragma endregion

    gameManagerBase(const InitData& init) : IScene(init) {
        manager.get() = std::make_shared<bool>(false);
        manager.add<startScene>(GameState::Start);
        manager.add<ruleScene>(GameState::Rule);
        manager.add<T>(GameState::Game);
    }

    void update() override {
        manager.updateScene();
        if (*manager.get())changeScene(Games::Select);
    }

    void draw() const override {
        manager.drawScene();
    }
};