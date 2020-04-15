#pragma once

#include <iostream>

namespace sparql {
	//PREFIXを行うためのIRI補助用クラス
	class prefix
	{
	private:
		const std::wstring _iri;
	public:
		prefix(std::wstring iri_text) : _iri(iri_text) {}

		inline std::wstring get_iri()const noexcept { return _iri; }
	};

	//IRIクラス
	class iri {
	private:
		const prefix _prefix;
		const std::wstring _iri;
	public:
		iri(std::wstring iri_text): _prefix(L""), _iri(iri_text) {}
		iri(prefix label, std::wstring local_part): _prefix(label), _iri(local_part) {}

		inline bool has_using_prefix() const noexcept { return (!_prefix.get_iri().empty()); }
		inline prefix get_prefix() const noexcept { return _prefix; }
		inline std::wstring get_local_part() const noexcept { return _iri; }
	};


	static inline bool operator==(const prefix& op1, const prefix& op2) {
		return op1.get_iri() == op2.get_iri();
	}
	static inline bool operator!=(const prefix& op1, const prefix& op2) {
		return !(op1 == op2);
	}
}