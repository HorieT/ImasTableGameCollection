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
	static constexpr imas::ParametarName _card_num_param = imas::ParametarName::Age;
	static constexpr int _row = 4;
	static constexpr int _column = 6;
	static constexpr double _card_size = 80.0;
	static constexpr double _card_offset = _card_size / 5.0;
	static constexpr double _card_open_animation_time = 0.6;
	static constexpr double _card_close_animation_time = _card_open_animation_time;
	static constexpr double _card_check_time = 1.0;


	static inline imas::idol::FilteringParams params;
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
		params.push_back({ imas::ParametarName::Name, objAcqPred::Filters()});
		params.push_back({ imas::ParametarName::Color, objAcqPred::Filters() });
		//params.push_back({ imas::ParametarName::Title, objAcqPred::FilterList{(objAcqPred::Filter)(new regexFilter(L"MillionStars"))} });//テスト
		params.push_back({ _card_num_param, objAcqPred::Filters() });
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
						overlay = draw_card(Scene::Center() - Vec2(origin_x - (i % _column) * offset_x, origin_y - (i / _column) * offset_y), _card_size, card, true);
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
								true);

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
							draw_card(Scene::Center() - Vec2(origin_x - (i % _column) * offset_x, origin_y - (i / _column) * offset_y), _card_size, card, true);
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
								true);

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
		else {
			//山札作成中
			if (get_all_ldols.is_done()) {
				try {
					if (_card_resource.make_bill_resource(get_all_ldols.get(), _card_num_param, 2, _row * _column / 2)) {
						for (const auto& card : _card_resource.get_resource()) {
							_field_bill.push_back(card);
						}
						shuffle(_field_bill.begin(), _field_bill.end(), std::mt19937(std::random_device()()));
					}
				}
				catch (web::json::json_exception& e) {
					std::wostringstream ss;
					ss << e.what();
					OutputDebugString(ss.str().c_str());
				}
			}
			_font.GAME_FRONT_TEXT(TEXT_LOADING).drawAt(Scene::Center(), Palette::Black);
		}


		_is_return_title = SimpleGUI::ButtonAt(TEXT_RETURN, Scene::Size() + textureResource::RETURN_BUTTON_OFFSET);
	}

	void draw() const override {
		Scene::SetBackground(_color.GAME_BACKGROUND);
	}

	static String title() { return U"神経衰弱"; }
	static String rule() { return U"めくって同じ奴でペアを作る。"; }
};