//外のリソースファイルにまとめようと思ったけど面倒だった
//constexprなstringはよ
#pragma once

#include <Siv3D.hpp>
#include <ratio>
#include <concepts>
#pragma region 定数テキスト
#define TEXT_VERSION U"ver:b1.00.00"
#define TEXT_TITLE U"IM@Sアソビ大全"
#define TEXT_ENTER U"決定"
#define TEXT_RETURN U"戻る"
#define TEXT_SETTING U"設定"
#define TEXT_VOLUME U"音量"
#define TEXT_GAME_SELECTT U"ゲーム選択"
#define TEXT_LOADING U"ロード中"
#define TEXT_GAME_START U"ゲームスタート"
#define TEXT_RULE U"ルール"
#define TEXT_RULE_SETTING U"ルール設定"
#define TEXT_CLEAR U"クリア"
#pragma endregion



//インラインでFontを用意したかったができないようなのでシングルトン化
//そんなに量食わないはずなので全部読み込んでおく
struct fontResource {
private:
	fontResource() {}
	static inline std::unique_ptr<fontResource> instance = nullptr;

public:
	static const fontResource& get_instance() {
		return (instance == nullptr) ? *(instance = std::make_unique<fontResource>(fontResource())) : *instance;
	}

	static constexpr int OUTLINE_DIVISION_TIMES = 12;
	static constexpr double OUTLINE_WIDTH = 1.0;
	static constexpr double GAME_CARD_MARKS_OFFSET = 1.5;

	const Font TITLE{ 60 };
	const Font APP_VERSION{ 20 };
	const Font SCENE_TITLE{ 40 };
	const Font HELP_TEXT{ 20 };
	const Font GAME_TITLE{ 40 };
	const Font GAME_RULE{ 12 };
	const Font GAME_FRONT_TEXT{ 30 };
	const Font GAME_CARD{ 21 };
	const Font GAME_CARD_MARKS{ 17 };
};

//色
struct colorResource {
private:
	colorResource() {}
	static inline std::unique_ptr<colorResource> instance = nullptr;

public:
	static const colorResource& get_instance() {
		return (instance == nullptr) ? *(instance = std::make_unique<colorResource>(colorResource())) : *instance;
	}

	const Color TITLE{ 0xE0, 0xB0, 0xD0 };
	const Color SETTING{ TITLE };
	const Color SELECTED_ITEM_OVERLAY{ 0x66, 0xCC, 0xFF , 0x66};
	const Color GAME_SELECT{ 0x0, 0x33, 0x66 };
	const Color GAME_RULE_OPTION{ 0x66, 0x00, 0x66};
	const Color GAME_BACKGROUND{ 0x00, 0x66, 0x33};
	const Color GAME_CARD_BASE{ 0xFF, 0xFF, 0xFF };
	const Color GAME_CLEAR{ 0xFF, 0x20, 0x20 };
	const Color GAME_CLEAR_BASE{ 0xFF, 0xFF, 0x66 };
};


//テクスチャ&レイアウト
struct textureResource {
private:
	textureResource() {
	}
	static inline std::unique_ptr<textureResource> instance = nullptr;

public:
	static const textureResource& get_instance() {
		return (instance == nullptr) ? *(instance = std::make_unique<textureResource>(textureResource())) : *instance;
	}

	//ratioの拡張
	template<intmax_t N, intmax_t D = 1>
	class origRatio : public std::ratio<N, D> {
	public:
		static constexpr double to_double = static_cast<double>(N) / static_cast<double>(D);
	};


	using WindowAspectRatio = origRatio<10, 7>;
	static constexpr int WINDOW_DEF_WIDTH = 1000;
	

	static constexpr Size ENTER_BUTTON_OFFSET{ 70, -40 };
	static constexpr Size RETURN_BUTTON_OFFSET{ -70, -40 };


	const Texture VOLUME__UP_ICON{ Icon(0xF028, 40) }; 
	const Texture CONFIG_ICON{ Icon(0xF013, 40) };
	const Texture QUESTION_ICON{ Icon(0xF059, 40) };

	const Texture CARD_BACK{ Resource(U"card_back.png"), TextureDesc::Mipped };
	const double CARD_ASPECT_RATIO = static_cast<double>(CARD_BACK.width()) / static_cast<double>(CARD_BACK.height()); 
	using CardRoundingRatio = origRatio<134, 7>;//目測で頑張る
	static constexpr double GAME_CARD_SIZE_BASE = 100.0;
};


//音
struct soundResource {
private:
	soundResource() {
	}
	static inline std::unique_ptr<soundResource> instance = nullptr;

public:
	static const soundResource& get_instance() {
		return (instance == nullptr) ? *(instance = std::make_unique<soundResource>(soundResource())) : *instance;
	}

	static constexpr double DEF_VOLUME = 1.0;

	const Audio RETURN_BUTTON{ Resource(U"cancel2.mp3") };
	const Audio ENTER_BUTTON{ Resource(U"cursor4.mp3") };
	const Audio CARD_TURN_OVER{ Resource(U"_sound_various_mp3_card-turn-over1.mp3") };
};



//その他
template<typename T>
concept EnumType = std::is_enum_v<T>;

template<EnumType T>
inline T& pre_increment_enum(T& e) {
	return e = static_cast<T>(static_cast<int>(e) + 1);
}

template<EnumType T>
inline T& pre_idecrement_enum(T& e) {
	return e = static_cast<T>(static_cast<int>(e) - 1);
}