#include"cardGameBase.h"

using namespace std;
using namespace sparql;
using namespace imas;


#pragma region protected member

s3d::RoundRect cardGameSceneBase::draw_card(Vec2 point, double width, Vec2 distortion) const {
	//˜c‚Ý
	auto mat_scale = Mat3x2::Scale(distortion, point);
	const Transformer2D t_card(mat_scale, true);

	//‰æ‘œ
	_texture.CARD_BACK.scaled(width / _texture.CARD_BACK.width()).drawAt(point);

	//ƒI[ƒo[ƒŒƒC
	return s3d::RoundRect(
		Arg::center(point),
		width,
		width / _texture.CARD_ASPECT_RATIO,
		width / textureResource::CardRoundingRatio::to_double);
}

s3d::RoundRect cardGameSceneBase::draw_card(Vec2 point, double width, IdolCard idol, bool draw_num, bool draw_mark, Vec2 distortion) const{
	//kŽÚ&˜c‚Ý
	auto mat_scale = Mat3x2::Scale(distortion * (width / textureResource::GAME_CARD_SIZE_BASE), point);
	const Transformer2D t_card(mat_scale, true);

	//“y‘ä
	const double height = textureResource::GAME_CARD_SIZE_BASE / _texture.CARD_ASPECT_RATIO;
	const double round = textureResource::GAME_CARD_SIZE_BASE / textureResource::CardRoundingRatio::to_double;
	s3d::RoundRect(
		Arg::center(point),
		textureResource::GAME_CARD_SIZE_BASE,
		height,
		round).draw(_color.GAME_CARD_BASE);
	
	//ƒeƒLƒXƒg
	const auto& idol_obj = *idol.lock();
	imas::idolColor color;
	try {
		color = idol_obj->get_data<ParametarName::Color>();
	}
	catch (web::json::json_exception&) {
		color = { 0 };
	}

	const auto& glyphs = _font.GAME_CARD.getVerticalGlyphs(Unicode::FromWString(idol_obj->to_string(ParametarName::Name)));
	double start_y = point.y - (((glyphs.size() > 5) ? 5 : glyphs.size()) - 0.5) * glyphs.front().yAdvance / 2.0;
	Vec2 penPos(point.x + ((glyphs.size() > 5) ? (glyphs.front().xAdvance / 2) : 0), start_y);
	for (int cnt = 0; const auto & glyph : glyphs) {
		//—ÖŠs
		for (int i = 0; i < _font.OUTLINE_DIVISION_TIMES;++i) {
			const double theta = i * 360_deg / _font.OUTLINE_DIVISION_TIMES;
			const Vec2 pos = OffsetCircular(penPos, _font.OUTLINE_WIDTH, theta);
			glyph.texture.drawAt(pos, Color(0));
		}
		glyph.texture.drawAt(penPos, Color(color.red, color.green, color.blue));
		penPos.y += glyph.yAdvance;
		++cnt;
		if (cnt == 5) {
			penPos.x -= glyphs.front().xAdvance;
			penPos.y = start_y;
		}
	}
	if (draw_num || draw_mark) {
		Vec2 draw_point(textureResource::GAME_CARD_SIZE_BASE / 2.0 - _font.GAME_CARD_MARKS_OFFSET,
			height / 2.0 - _font.GAME_CARD_MARKS_OFFSET);
		if (draw_num) {
			String num = (idol_obj->to_int(_card_resource.get_number()) == -1) ? (U"-") : Unicode::FromWString(idol_obj->to_string(_card_resource.get_number()));
			_font.GAME_CARD_MARKS(num).drawAt(point - draw_point, Color(0));
		}
		if(draw_mark)_font.GAME_CARD_MARKS(Unicode::FromWString(idol_obj->to_string(_card_resource.get_mark()))).drawAt(point - draw_point, Color(0));
		{
			auto mat_rotate = Mat3x2::Rotate(180_deg, point);
			const Transformer2D t_marks(mat_rotate, true);
			if (draw_num) {
				String num = (idol_obj->to_int(_card_resource.get_number()) == -1) ? (U"-") : Unicode::FromWString(idol_obj->to_string(_card_resource.get_number()));
				_font.GAME_CARD_MARKS(num).drawAt(point - draw_point, Color(0));
			}
			if (draw_mark)_font.GAME_CARD_MARKS(Unicode::FromWString(idol_obj->to_string(_card_resource.get_mark()))).drawAt(point - draw_point, Color(0));
		}
	}

	//ƒI[ƒo[ƒŒƒC (ŠO‚Ì”ä—¦‚É’¼‚·)
	return s3d::RoundRect(
		Arg::center(point),
		width,
		width / _texture.CARD_ASPECT_RATIO,
		width / textureResource::CardRoundingRatio::to_double);
}


s3d::RoundRect cardGameSceneBase::draw_card_open(double time_s, double max_time_s, Vec2 point, double width, IdolCard idol, bool draw_num, bool draw_mark) const {
	const double distortion = (Cos(time_s / max_time_s * 360_deg) + 1.0) / 2.0;
	
	if (time_s > max_time_s / 2) {//•\–Ê
		return draw_card(point, width, idol, draw_num, draw_mark, Vec2(distortion, 1.0));
	}
	else {//— –Ê
		return draw_card(point, width, Vec2(distortion, 1.0));
	}
}

s3d::RoundRect cardGameSceneBase::draw_card_close(double time_s, double max_time_s, Vec2 point, double width, IdolCard idol, bool draw_num, bool draw_mark) const {
	const double distortion = (Cos(time_s / max_time_s * 360_deg) + 1.0) / 2.0;

	if (time_s > max_time_s / 2) {//— –Ê
		return draw_card(point, width, Vec2(distortion, 1.0));
	}
	else {//•\–Ê
		return draw_card(point, width, idol, draw_num, draw_mark, Vec2(distortion, 1.0));
	}
}

#pragma region cardDeck class

#pragma region private member

#pragma endregion
#pragma region public member
bool cardGameSceneBase::cardDeck::make_bill_resource(const vector<Idol>& all_idols, ParametarName num_param, size_t same_val_num) {
	if (!is_arithmetic_parm(num_param))throw logic_error("'num_param' isn't arithmetic.");
	_card_num_param = num_param;
	vector<Idol> copy_idols(all_idols);

	//–Ê“|‚È‚Ì‚ÅƒAƒ‹ƒSƒŠƒYƒ€‹l‚ß‚Ä‚È‚¢‚Å‚·
	shuffle(copy_idols.begin(), copy_idols.end(), mt19937(random_device()()));
	stable_sort(copy_idols.begin(), copy_idols.end(), [=](Idol a, Idol b) {return (*a).to_int(num_param) < (*b).to_int(num_param); });
	{
		int comp_val = (*copy_idols.at(0)).to_int(num_param);
		auto begin_it = copy_idols.begin();
		for (auto it = copy_idols.begin(); it != copy_idols.end(); ++it) {
			int val = (*it)->to_int(num_param);
			if (comp_val != val) {
				auto sames_it = begin_it;
				for (size_t i = 0; i < same_val_num; ++i) {
					_bill_resource.push_back(make_shared<Idol>(*sames_it));
					++sames_it;
					if (sames_it == it) sames_it = begin_it;
				}
				comp_val = val;
				begin_it = it;
			}
		}
	}
	_bill_initlized = true;
	return true;
}

bool cardGameSceneBase::cardDeck::make_bill_resource(const vector<Idol>& all_idols, ParametarName num_param, size_t same_val_num, size_t num_of_type) {
	if (!is_arithmetic_parm(num_param))throw logic_error("'num_param' isn't arithmetic.");

	_card_num_param = num_param;
	vector<Idol> copy_idols(all_idols);
	vector<IdolCardRes> rturn_cards;

	//–Ê“|‚È‚Ì‚ÅƒAƒ‹ƒSƒŠƒYƒ€‹l‚ß‚Ä‚È‚¢‚Å‚·
	shuffle(copy_idols.begin(), copy_idols.end(), mt19937(random_device()()));
	vector<vector<Idol>> same_idols;
	for (auto& idol : copy_idols) {
		bool no_sames = true;
		for (auto& sames : same_idols) {
			if (idol::is_same_param(*sames.front(), *idol, num_param)) {
				sames.push_back(idol);
				no_sames = false;
				break;
			}
		}
		if (no_sames) {
			same_idols.push_back(vector<Idol>());
			same_idols.back().push_back(idol);
		}
	}
	if (num_of_type > same_idols.size()) {
		return false;
	}

	for (size_t outer = 0; outer < num_of_type; ++outer) {
		for (size_t inner = 0; inner < same_val_num; ++inner) {
			_bill_resource.push_back(make_shared<Idol>(same_idols.at(outer).at(inner % same_idols.at(outer).size())));
		}
	}
	_bill_initlized = true;
	return true;
}


std::vector<cardGameSceneBase::IdolCardRes> cardGameSceneBase::cardDeck::get_resource()const {
	if (!_bill_initlized)throw std::exception("The bill has not been initialized.");
	return _bill_resource;
}
imas::ParametarName cardGameSceneBase::cardDeck::get_mark()const {
	if (!_bill_initlized)throw std::exception("The bill has not been initialized.");
	return _card_mark_param;
}
imas::ParametarName cardGameSceneBase::cardDeck::get_number()const {
	if (!_bill_initlized)throw std::exception("The bill has not been initialized.");
	return _card_num_param;
}
#pragma endregion
#pragma endregion
#pragma endregion



#pragma region public member

#pragma endregion

