//クエリのフィルタクラス
#pragma once

#include "iri.h"


namespace sparql {
	class filter {
	protected:
		bool _match;
	
		std::wstring get_match_flag() const{
			return (std::wstring)(_match ? L" " : L"!");
		}
	public:
		filter(bool match) : _match(match){}
		bool get_match() { return _match; }
		void set_match(bool m) { _match = m; }
		virtual std::wstring get_filter_text(std::wstring) const= 0;
	};


	//正規表現フィルタ
	class regexFilter : public filter{
	private:
		const std::wstring _value;

	public:
		regexFilter(std::wstring val, bool match = true) : filter(match), _value(val) {	}


		virtual std::wstring get_filter_text(std::wstring arg) const override {
			return get_match_flag() + L"regex(" + arg + L", '" + _value + L"')";
		}
	};


	//言語型フィルタ
	class langFilter : public filter {
	public:
		enum class Type{
			ja,
			en
		};


	private:
		const Type _type;

		std::wstring get_literal() const {
			switch (_type)
			{
			case Type::ja:
				return L"'ja'";
			case Type::en:
				return L"'en'";
			default:
				throw std::logic_error("no match 'type'");
				break;
			}
		}

	
	public:
		langFilter(Type type, bool match = true) : filter(match), _type(type){	}
		
			
		virtual std::wstring get_filter_text(std::wstring arg) const override {
			return get_match_flag() + L"LANG(" + arg + L")=" + get_literal();
		}
	};
}