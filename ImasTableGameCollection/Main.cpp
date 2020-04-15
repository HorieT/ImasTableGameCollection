
# include <Siv3D.hpp> // OpenSiv3D v0.4.2
#include "Resource.h"
#include "settingFile.h"
#include "titleScene.h"
#include "settingScene.h"
#include "helpScene.h"
#include "Game/gameSelectManager.h"


void Main()
{
	
	//リソース類の読み込み
	[[maybe_unused]]const soundResource& sound = soundResource::get_instance();
	const settingFile& setting = settingFile::get_instance();

	//メイン制御シーン
	App scene_manager;
	scene_manager.add<titleScene>(ViewState::Title);
	scene_manager.add<gameSelectManager>(ViewState::Game);
	scene_manager.add<settingScene>(ViewState::Setting);
	scene_manager.add<helpScene>(ViewState::Help);


	//リサイズ
	Scene::Resize(
		Size(textureResource::WINDOW_DEF_WIDTH, 
			static_cast<int>(textureResource::WINDOW_DEF_WIDTH / textureResource::WindowAspectRatio::to_double)));
	Window::Resize(
		Size(static_cast<int>(setting.get_window_width()), 
			static_cast<int>(setting.get_window_width() / textureResource::WindowAspectRatio::to_double)), 
		WindowResizeOption::KeepSceneSize);
	Window::SetTitle(TEXT_TITLE);
	

	while (System::Update())
	{
		if (!scene_manager.update()) break;
	}
}