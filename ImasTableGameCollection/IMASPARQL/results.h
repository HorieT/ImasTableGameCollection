#pragma once


#include "query.h"

namespace sparql {
	//一般化したJSON値の型
	template<typename T>
	concept JsonValueType = std::is_same_v<T, int> || std::is_same_v<T, double> || std::is_same_v<T, std::optional<bool>> || std::is_same_v<T, std::wstring>;

	//クエリ結果処理クラス
	class results
	{
	public:
		static inline const std::wstring JSON_HEAD = L"head";
		static inline const std::wstring JSON_VARS = L"vars";
		static inline const std::wstring JSON_BOOLEAN = L"boolean";
		static inline const std::wstring JSON_RESULTS = L"results";
		static inline const std::wstring JSON_BINDINGS = L"bindings";
		static inline const std::wstring JSON_VALUE = L"value";


		//一つの主語に連なるクエリ結果のリスト抽出
		static web::json::value& get_result_unit_list(web::json::value& val);
	};
}