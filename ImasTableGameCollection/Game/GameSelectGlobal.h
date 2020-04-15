//ゲームセレクタの設定
#pragma once

#include<Siv3D.hpp>

//ゲーム達
//
enum class Games {
    Select,
    Concentaration,
};
using GamesManager = SceneManager<Games, bool>;

//ゲームの中身
enum class GameState {
    Start,
    Rule,
    Game,
    Result,
};
using GameStateManager = SceneManager<GameState, bool>;