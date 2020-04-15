//カードゲーム用にチューンしたゲームのベースクラス
#pragma once

#include "gameSceneBase.h"
#include <vector>
#include <ratio>

class cardGameSceneBase : public gameSceneBase {
protected:
#pragma region 宣言
	using IdolCardRes = std::shared_ptr<const Idol>;//同じアイドルで複数のカードになるためポインタで包む
	using IdolCard = std::weak_ptr<const Idol>;

	class cardDeck {
	private:
		std::vector<IdolCardRes> _bill_resource;//ゲーム内の札ソース
		imas::ParametarName _card_mark_param;
		imas::ParametarName _card_num_param;
		bool _bill_initlized = false;
	public:
		//num_param:数値パラメータ
		//same_val_num:同じ数値カードの枚数
		bool make_bill_resource(const std::vector<Idol>& all_idols, imas::ParametarName num_param, size_t same_val_num);

		//num_param:数値パラメータ
		//same_val_num:同じ数値カードの枚数
		//num_of_type : 種類数
		//ソースの持つ数値の種類よりnum_of_typeが多い場合構築失敗
		bool make_bill_resource(const std::vector<Idol>& all_idols, imas::ParametarName num_param, size_t same_val_num, size_t num_of_type);

		inline bool is_bill_initlized()const noexcept { return _bill_initlized; };
		std::vector<IdolCardRes> get_resource()const;
		imas::ParametarName get_mark()const;
		imas::ParametarName get_number()const;
	};
#pragma endregion

	cardDeck _card_resource;//ゲーム内の札ソース
	std::vector<IdolCard> _mountain_bill;//山札
	std::vector<IdolCard> _field_bill;//場札
	std::vector<std::vector<IdolCard>> _hand_bill;//手札

	//裏面
	s3d::RoundRect draw_card(Vec2 point, double width, Vec2 distortion = Vec2(1.0, 1.0)) const;
	//表面
	s3d::RoundRect draw_card(Vec2 point, double width, IdolCard idol, bool draw_num = false, bool draw_mark = false, Vec2 distortion = Vec2(1.0, 1.0)) const;
	//裏面→表面
	s3d::RoundRect draw_card_open(double time_s, double max_time_s, Vec2 point, double width, IdolCard idol, bool draw_num = false, bool draw_mark = false) const;
	//表面→裏面
	s3d::RoundRect draw_card_close(double time_s, double max_time_s, Vec2 point, double width, IdolCard idol, bool draw_num = false, bool draw_mark = false) const;

public:
	cardGameSceneBase(const InitData& init) : gameSceneBase(init){

	}

	virtual void update_draw() override = 0;
	virtual void draw() const override = 0;
};