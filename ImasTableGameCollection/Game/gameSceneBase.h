//ゲームのVMのベースクラス
//gameManagerBaseのGameSceneClass制約に則って継承すること
#pragma once

#include "../Resource.h"
#include "../settingFile.h"
#include "../IMASPARQL/idol.h"
#include "GameSelectGlobal.h"


class gameSceneBase : public GameStateManager::Scene {
protected:
	using Idol = std::shared_ptr<const imas::idol>;

	//Model
	const fontResource& _font;
	const colorResource& _color;
	const textureResource& _texture;
	const soundResource& _sound;
	const settingFile& _setting;

	//基幹フラグ類
	bool _is_return_title = false;


public:
	gameSceneBase(const InitData& init) 
		: IScene(init), 
		_font(fontResource::get_instance()), 
		_color(colorResource::get_instance()),
		_texture(textureResource::get_instance()),
		_sound(soundResource::get_instance()),
		_setting(settingFile::get_instance()){
		
    }

	//シーン移動は一括管理
	virtual void update_draw() = 0;
	void update() override final{
		//選択画面に戻る 雛菜は雛菜のことしかしらないよ
		if (_is_return_title) {
			_sound.RETURN_BUTTON.playOneShot(_setting.get_volume());
			getData() = true;
		}
		update_draw();
	}

	virtual void draw() const override = 0;
};