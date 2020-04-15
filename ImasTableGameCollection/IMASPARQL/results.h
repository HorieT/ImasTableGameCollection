#pragma once


#include "query.h"

namespace sparql {
	//��ʉ�����JSON�l�̌^
	template<typename T>
	concept JsonValueType = std::is_same_v<T, int> || std::is_same_v<T, double> || std::is_same_v<T, std::optional<bool>> || std::is_same_v<T, std::wstring>;

	//�N�G�����ʏ����N���X
	class results
	{
	public:
		static inline const std::wstring JSON_HEAD = L"head";
		static inline const std::wstring JSON_VARS = L"vars";
		static inline const std::wstring JSON_BOOLEAN = L"boolean";
		static inline const std::wstring JSON_RESULTS = L"results";
		static inline const std::wstring JSON_BINDINGS = L"bindings";
		static inline const std::wstring JSON_VALUE = L"value";


		//��̎��ɘA�Ȃ�N�G�����ʂ̃��X�g���o
		static web::json::value& get_result_unit_list(web::json::value& val);
	};
}