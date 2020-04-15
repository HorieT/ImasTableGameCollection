//�Q�[���Z���N�^�̐ݒ�
#pragma once

#include<Siv3D.hpp>

//�Q�[���B
//
enum class Games {
    Select,
    Concentaration,
};
using GamesManager = SceneManager<Games, bool>;

//�Q�[���̒��g
enum class GameState {
    Start,
    Rule,
    Game,
    Result,
};
using GameStateManager = SceneManager<GameState, bool>;