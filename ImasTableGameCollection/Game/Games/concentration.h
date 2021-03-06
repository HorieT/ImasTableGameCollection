//神経衰弱のようなもの
#pragma once

#include "../cardGameBase.h"

class concentration : public cardGameSceneBase {
private:
	enum class Turn {
		OpenFirst = 0U,
		OpeningFirst,
		OpenSecond,
		OpeningSecond,
		CardCheck,
		CloseCard,
		EraseCard
	};
	static constexpr int _row = 4;
	static constexpr int _column = 6;
	static constexpr double _card_size = 80.0;
	static constexpr double _card_offset = _card_size / 5.0;
	static constexpr double _card_open_animation_time = 0.6;
	static constexpr double _card_close_animation_time = _card_open_animation_time;
	static constexpr double _card_check_time = 1.0;

	//ルール
	static inline imas::ParametarName card_num = imas::ParametarName::Age;
	static inline Array<String> num_params;
	static inline Array<imas::ParametarName> num_params_index;
	static inline Array<bool> title_filter;
	static inline Array<String> titles;
	static inline bool is_show_num = true;

	bool error_flag = false;

	imas::idol::FilteringParams params;
	pplx::task<std::vector<Idol>> get_all_ldols;
	
	Turn game_turn = Turn::OpenFirst;
	Stopwatch _card_open_animation;
	Stopwatch _card_close_animation;
	Stopwatch _card_check;
	IdolCard _opened_card;
	IdolCard _opening_card;
	std::vector<IdolCard> _maked_pair_card;
	int _turn_count = 0;
public:
	concentration(const InitData& init) : cardGameSceneBase(init){
		using namespace sparql;
		sparql::objAcqPred::Filters filters;
		for (int i = 0; auto t_filter : title_filter) {
			if (!t_filter)filters.push_back((objAcqPred::Filter)(new regexFilter(imas::Title::literals.at(i), false)));
			++i;
		}

		params.push_back({ imas::ParametarName::Name, objAcqPred::Filters()});
		params.push_back({ imas::ParametarName::Color, objAcqPred::Filters() });
		params.push_back({ imas::ParametarName::Title,  filters });
		params.push_back({ card_num, objAcqPred::Filters() });
		get_all_ldols = imas::idol::get_idol_list(params);
		
		//テスト用　4*6
		_card_open_animation.reset();
		_card_close_animation.reset();
		_card_check.reset();
	}

	virtual void update_draw() override {
		if (_card_resource.is_bill_initlized()) {
			//ゲーム内容
			
			//
			_font.GAME_FRONT_TEXT(U"ターン数 : " + Format(_turn_count)).draw(10, 25);

			if (_maked_pair_card.size() == 24) {//クリア//輪郭
				for (int i = 0; i < _font.OUTLINE_DIVISION_TIMES; ++i) {
					const double theta = i * 360_deg / _font.OUTLINE_DIVISION_TIMES;
					const Vec2 pos = OffsetCircular(Scene::Center(), _font.OUTLINE_WIDTH, theta);
					_font.GAME_TITLE(TEXT_CLEAR).drawAt(pos, _color.GAME_CLEAR_BASE);
				}
				_font.GAME_TITLE(TEXT_CLEAR).drawAt(Scene::Center(), _color.GAME_CLEAR);
			}
			else {
				//カード描画
				const double offset_x = (_card_size + _card_offset);
				const double offset_y = (_card_size / _texture.CARD_ASPECT_RATIO + _card_offset);
				const double origin_x = offset_x * (_column - 1) / 2.0;
				const double origin_y = offset_y * (_row - 1) / 2.0;
				for (int i = 0; const auto & card : _field_bill) {
					//ペア除外
					if (std::none_of(_maked_pair_card.begin(), _maked_pair_card.end(), [&](IdolCard idol) {return idol.lock() == card.lock(); })) {
						s3d::RoundRect overlay;
						//めくられ
						if (_opened_card.lock() == card.lock() && (game_turn != Turn::CloseCard && game_turn != Turn::EraseCard))
							overlay = draw_card(Scene::Center() - Vec2(origin_x - (i % _column) * offset_x, origin_y - (i / _column) * offset_y), _card_size, card, is_show_num);
						//めくられてない
						else if (_opening_card.lock() != card.lock() && _opened_card.lock() != card.lock())
							overlay = draw_card(Scene::Center() - Vec2(origin_x - (i % _column) * offset_x, origin_y - (i / _column) * offset_y), _card_size);
						//ダミー
						else
							overlay = s3d::RoundRect();

						//ターン処理
						switch (game_turn) {
						case Turn::OpenFirst://選択中
						case Turn::OpenSecond:
							if (_opened_card.lock() != card.lock()) {
								if (overlay.leftClicked()) {
									_card_open_animation.restart();
									_opening_card = card;
									_sound.CARD_TURN_OVER.playOneShot(_setting.get_volume());
									pre_increment_enum(game_turn);
								}
								else if (overlay.mouseOver()) {
									overlay.draw(_color.SELECTED_ITEM_OVERLAY);
								}
							}
							break;
						case Turn::OpeningFirst://めくる
						case Turn::OpeningSecond://めくる
							if (_opening_card.lock() == card.lock())//めくり中
								draw_card_open(
									_card_open_animation.sF(),
									_card_open_animation_time,
									Scene::Center() - Vec2(origin_x - (i % _column) * offset_x, origin_y - (i / _column) * offset_y),
									_card_size,
									card,
									is_show_num);

							//めくりアニメーション用ストップウォッチ
							if (_card_open_animation.sF() > _card_open_animation_time) {
								_card_open_animation.reset();
								if (game_turn == Turn::OpeningFirst) {
									_opened_card = _opening_card;
									_opening_card = IdolCard();
								}
								else {
									_card_check.restart();
								}
								pre_increment_enum(game_turn);
							}
							break;
						case Turn::CardCheck://確かめる
							if (_opening_card.lock() == card.lock())
								draw_card(Scene::Center() - Vec2(origin_x - (i % _column) * offset_x, origin_y - (i / _column) * offset_y), _card_size, card, is_show_num);
							if (_card_check.sF() > _card_check_time) {
								//ペア
								if (imas::idol::is_same_param(**_opening_card.lock(), **_opened_card.lock(), _card_resource.get_number())) {
									_maked_pair_card.push_back(_opening_card);
									_maked_pair_card.push_back(_opened_card);
									game_turn = Turn::EraseCard;
								}
								else {//じゃない
									_card_close_animation.restart();
									_sound.CARD_TURN_OVER.playOneShot(_setting.get_volume());
									game_turn = Turn::CloseCard;
								}
							}
							break;
						case Turn::CloseCard://戻す
							if (_opening_card.lock() == card.lock() || _opened_card.lock() == card.lock())//もどし中
								draw_card_close(
									_card_close_animation.sF(),
									_card_close_animation_time,
									Scene::Center() - Vec2(origin_x - (i % _column) * offset_x, origin_y - (i / _column) * offset_y),
									_card_size,
									card,
									is_show_num);

							//戻しアニメーション用ストップウォッチ
							if (_card_close_animation.sF() > _card_close_animation_time) {
								_card_close_animation.reset();
								_opening_card = IdolCard();
								_opened_card = IdolCard();
								game_turn = Turn::OpenFirst;
								++_turn_count;
							}
							break;
						case Turn::EraseCard://消す
							_opening_card = IdolCard();
							_opened_card = IdolCard();
							game_turn = Turn::OpenFirst;
							++_turn_count;
							break;
						default:
							break;
						}
					}
					++i;
				}
			}
		}
		else {
			if (error_flag) {
				_font.GAME_FRONT_TEXT(U"ネットワークに接続できません。").drawAt(Scene::Center(), Palette::Black);
			}
			else if (get_all_ldols.is_done()) {//山札作成中
				try {
					if (_card_resource.make_bill_resource(get_all_ldols.get(), card_num, 2, _row * _column / 2)) {
						for (const auto& card : _card_resource.get_resource()) {
							_field_bill.push_back(card);
						}
						shuffle(_field_bill.begin(), _field_bill.end(), std::mt19937(std::random_device()()));
					}
					else {
						_font.GAME_FRONT_TEXT(U"ゲームが開始できません。\nルール設定が実現できない可能性があります。").drawAt(Scene::Center(), Palette::Black);
					}
				}
				catch (web::json::json_exception& e) {
					std::wostringstream ss;
					ss << e.what();
					OutputDebugString(ss.str().c_str());
					error_flag = true;
				}
				catch (...) {
					error_flag = true;
				}
			}
			else _font.GAME_FRONT_TEXT(TEXT_LOADING).drawAt(Scene::Center(), Palette::Black);
		}


		_is_return_title = SimpleGUI::ButtonAt(TEXT_RETURN, Scene::Size() + textureResource::RETURN_BUTTON_OFFSET);
	}

	void draw() const override {
		Scene::SetBackground(_color.GAME_BACKGROUND);
	}

#pragma region コンセプト既定

	static String title() { return U"神経衰弱"; }
	static String rule() { return U"めくって同じ奴でペアを作る。"; }

	static void rule_setting() {
		//初期化
		if (title_filter.size() == 0) {
			for (size_t i = 0; i < imas::ParametarName_COUNT; ++i) {
				auto name = static_cast<imas::ParametarName>(i);
				if (imas::is_arithmetic_parm(name)) {
					num_params << Unicode::FromWString(imas::idol::get_param_name(name));
					num_params_index << name;
				}
			}
			for (auto name : imas::Title::literals) {
				titles << Unicode::FromWString(name);
				title_filter.push_back(true);
			}
		}
		size_t num_index = 0;
		for (int i = 0; auto num : num_params_index) {
			if (num == card_num) {
				num_index = i;
				break;
			}
			++i;
		}

		SimpleGUI::CheckBoxAt(is_show_num, U"ナンバーの可視化", Scene::Center() + Vec2(0, -200));
		if (SimpleGUI::RadioButtonsAt(num_index, num_params, Scene::Center() + Vec2(-200, 0), 200)) {
			card_num = num_params_index.at(num_index);
		}
		for (int i = 0; const auto & title_name : titles) {
			SimpleGUI::CheckBox(title_filter.at(i), title_name, Scene::Center() + Vec2(150, -150 + i * 45));
			++i;
		}
	}
#pragma endregion
};