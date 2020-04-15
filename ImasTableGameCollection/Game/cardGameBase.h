//�J�[�h�Q�[���p�Ƀ`���[�������Q�[���̃x�[�X�N���X
#pragma once

#include "gameSceneBase.h"
#include <vector>
#include <ratio>

class cardGameSceneBase : public gameSceneBase {
protected:
#pragma region �錾
	using IdolCardRes = std::shared_ptr<const Idol>;//�����A�C�h���ŕ����̃J�[�h�ɂȂ邽�߃|�C���^�ŕ��
	using IdolCard = std::weak_ptr<const Idol>;

	class cardDeck {
	private:
		std::vector<IdolCardRes> _bill_resource;//�Q�[�����̎D�\�[�X
		imas::ParametarName _card_mark_param;
		imas::ParametarName _card_num_param;
		bool _bill_initlized = false;
	public:
		//num_param:���l�p�����[�^
		//same_val_num:�������l�J�[�h�̖���
		bool make_bill_resource(const std::vector<Idol>& all_idols, imas::ParametarName num_param, size_t same_val_num);

		//num_param:���l�p�����[�^
		//same_val_num:�������l�J�[�h�̖���
		//num_of_type : ��ސ�
		//�\�[�X�̎����l�̎�ނ��num_of_type�������ꍇ�\�z���s
		bool make_bill_resource(const std::vector<Idol>& all_idols, imas::ParametarName num_param, size_t same_val_num, size_t num_of_type);

		inline bool is_bill_initlized()const noexcept { return _bill_initlized; };
		std::vector<IdolCardRes> get_resource()const;
		imas::ParametarName get_mark()const;
		imas::ParametarName get_number()const;
	};
#pragma endregion

	cardDeck _card_resource;//�Q�[�����̎D�\�[�X
	std::vector<IdolCard> _mountain_bill;//�R�D
	std::vector<IdolCard> _field_bill;//��D
	std::vector<std::vector<IdolCard>> _hand_bill;//��D

	//����
	s3d::RoundRect draw_card(Vec2 point, double width, Vec2 distortion = Vec2(1.0, 1.0)) const;
	//�\��
	s3d::RoundRect draw_card(Vec2 point, double width, IdolCard idol, bool draw_num = false, bool draw_mark = false, Vec2 distortion = Vec2(1.0, 1.0)) const;
	//���ʁ��\��
	s3d::RoundRect draw_card_open(double time_s, double max_time_s, Vec2 point, double width, IdolCard idol, bool draw_num = false, bool draw_mark = false) const;
	//�\�ʁ�����
	s3d::RoundRect draw_card_close(double time_s, double max_time_s, Vec2 point, double width, IdolCard idol, bool draw_num = false, bool draw_mark = false) const;

public:
	cardGameSceneBase(const InitData& init) : gameSceneBase(init){

	}

	virtual void update_draw() override = 0;
	virtual void draw() const override = 0;
};