//�Q�[����VM�̃x�[�X�N���X
//gameManagerBase��GameSceneClass����ɑ����Čp�����邱��
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

	//��t���O��
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

	//�V�[���ړ��͈ꊇ�Ǘ�
	virtual void update_draw() = 0;
	void update() override final{
		//�I����ʂɖ߂� ���؂͐��؂̂��Ƃ�������Ȃ���
		if (_is_return_title) {
			_sound.RETURN_BUTTON.playOneShot(_setting.get_volume());
			getData() = true;
		}
		update_draw();
	}

	virtual void draw() const override = 0;
};