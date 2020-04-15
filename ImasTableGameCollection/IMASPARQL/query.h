#pragma once

#include <vector>
#include <any>
#include <type_traits>
#include <functional>
#include <array>
#include <optional>
#include <concepts>
#include <compare>
#include <regex>
#include <cpprest/http_client.h>
#include "iri.h"


namespace sparql {
	//�N�G���쐬�⏕�̂��߂̖ړI��擾�p�q��N���X
	class objAcqPred {
	public:
		objAcqPred(iri predicate, const std::function<std::wstring(const std::wstring&)>& filter = nullptr) : predicate(predicate), query_filter(filter) {}
		iri predicate;
		//�N�G����FILTER���|����
		const std::function<std::wstring(const std::wstring&)> query_filter;
	};


	//�N�G���쐬�N���X
	class query {
	private:
#pragma region Key
		static inline const std::wstring PREFIX = L"PREFIX";
		static inline const std::wstring SELECT = L"SELECT";
		static inline const std::wstring WHERE = L"WHERE";
		static inline const std::wstring FILTER = L"FILTER";
		static inline const std::wstring PREFIX_LABEL = L"label";
#pragma endregion
		//�߂�l�FPREFIX�錾�L�[���[�h��label����Ȃ�IRI
		[[nodiscard]] static std::wstring make_iri_and_prefix(const iri& iri_obj, std::vector<prefix>& prefixs, std::wstring& prefix_text);

	public:
#pragma region Key arg
		static inline const std::wstring SUBJECT = L"subject";
		static inline const std::wstring PREDICATE = L"predicate";
		static inline const std::wstring OBJECT = L"object";

		static inline std::wstring make_subject(int count) {
			return L"?" + SUBJECT + std::to_wstring(count);
		}
		static inline std::wstring make_object(int count) {
			return L"?" + OBJECT + std::to_wstring(count);
		}
		static inline std::wstring make_prefix_label(int count) {
			return PREFIX_LABEL + std::to_wstring(count) + L":";
		}
		static inline std::wstring get_object(int count) {
			return OBJECT + std::to_wstring(count);
		}
#pragma endregion


		//�N�G���̍쐬(�q��-�ړI��̃��X�g)
		static utility::string_t create(iri subject_type, iri subject, std::vector<objAcqPred> predicates);

	};
}