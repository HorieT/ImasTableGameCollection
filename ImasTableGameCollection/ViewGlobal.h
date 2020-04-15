//View及びView Modelの定義
#pragma once

#include<Siv3D.hpp>
#include<ratio>

//Sceneのステート
//V,VMに相当する
enum class ViewState
{
	Title,
	Game,
	Setting,
	Help
};
using App = SceneManager<ViewState>;

//エラーステート
enum class AppErrorState {
	None = 0U,
	HttpError,
};