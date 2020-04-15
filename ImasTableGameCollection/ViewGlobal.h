//View�y��View Model�̒�`
#pragma once

#include<Siv3D.hpp>
#include<ratio>

//Scene�̃X�e�[�g
//V,VM�ɑ�������
enum class ViewState
{
	Title,
	Game,
	Setting,
	Help
};
using App = SceneManager<ViewState>;

//�G���[�X�e�[�g
enum class AppErrorState {
	None = 0U,
	HttpError,
};