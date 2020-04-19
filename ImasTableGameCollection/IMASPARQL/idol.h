//SPARQLからアイドルのリストをとってくるクラス
//汚い
#pragma once

#include <iostream>
#include <any>
#include <type_traits>
#include <functional>
#include <array>
#include <optional>
#include <concepts>
#include <compare>
#include <regex>
#include<Windows.h>
#include "sparql.h"
#include "idolParamGlobal.h"



namespace imas {
	//アイドルのデータクラス
	class idol{
	public:
		using Params = std::vector<ParametarName>;
		using ParamsList = std::initializer_list<ParametarName>;
		using ParamAndFilter = std::pair<ParametarName, sparql::objAcqPred::Filters>;
		using FilteringParams = std::vector<ParamAndFilter>;

	private:

		//設定パラメータ
		const Params _my_params;
		
		//取得値
		const web::json::value _values;


		//コンストラクタ隠ぺい
		//jsonのチェックはここでやらないので
		idol(web::json::value val, Params params) : _values(val), _my_params(params) {}


		//クエリ用静的関数
		static std::vector<sparql::objAcqPred> get_param_predicate_list(FilteringParams params) {
			std::vector<sparql::objAcqPred> predicates;
			for (size_t i = 0; auto param : params) {
				predicates.push_back(idolBase::get_params(param.first)->get_predicate(param.second));
				++i;
			}
			return predicates;
		}


	public:
		//コピーコンストラクタは無し
		//idol(const idol& i) : _values(i.get_json_list()) {}
		//idol& operator=(const idol& i) { return *this; }


		//パラメータ設定の取得
		Params get_setting_parm() const{
			return _my_params;
		}

		//パラメータの存在
		bool check_param(ParametarName name) const{
			return std::any_of(_my_params.begin(), _my_params.end(), [=](ParametarName use_p) {return use_p == name; });
		}

		//パラメータの取得
		web::json::value get_json_value(ParametarName name) const {
			if (!check_param(name))throw std::out_of_range("'name' is unused.");
			try {
				const auto it = _my_params.begin();
				return _values.at(sparql::query::get_object(static_cast<int>(std::find_if(it, _my_params.end(), [=](ParametarName use_p) {return use_p == name; }) - it)));
			}
			catch (web::json::json_exception&) {
				throw web::json::json_exception("This parameter does not exist.");//パラメータ欠損
			}
		}

		//パラメータの取得
		std::wstring to_string(ParametarName name) const {
			if (!check_param(name))throw std::out_of_range("'name' is unused.");
			try {
				return get_json_value(name).at(sparql::results::JSON_VALUE).as_string().c_str();
			}
			catch (web::json::json_exception&) {
				throw web::json::json_exception("This parameter does not exist.");//パラメータ欠損
			}
		}

		//パラメータの取得
		std::any get_data(ParametarName name) const {
			if (!check_param(name))throw std::out_of_range("'name' is unused.");
			try {
				return std::any(idolBase::get_params(name)->get_data(get_json_value(name)));
			}
			catch (web::json::json_exception&) {
				throw web::json::json_exception("This parameter does not exist.");//パラメータ欠損
			}
		}

		//パラメータの取得
		int to_int(ParametarName name) const {
			if (!check_param(name))throw std::out_of_range("'name' is unused.");
			if (!is_arithmetic_parm(name))throw std::logic_error("'name' isn't arithmetic.");
			return std::any_cast<int>(get_data(name));
		}

		//パラメータの取得
		template<ParametarName P>
		auto get_data() const {
			static_assert(is_in_range(P), "'P' is out of range.");
			if (!check_param(P))throw std::out_of_range("'name' is unused.");

			try {
				return dynamic_cast<idolBase::idolParam<P>*>(idolBase::get_params(P).get())->data_conv(get_json_value(P));
			}
			catch (web::json::json_exception&) {
				throw web::json::json_exception("This parameter does not exist.");//パラメータ欠損
			}
			catch (std::bad_cast& e) {//dynamic_cast失敗 idolBase等確認すべし
				std::wostringstream ss;
				ss << e.what();
				OutputDebugString(ss.str().c_str());
				throw;
			}
		}


		//http reqestを投げてアイドル情報を受け取る
		//idol実体はこの関数以外で生成できない
		static pplx::task<std::vector<std::shared_ptr<const idol>>> get_idol_list(FilteringParams params) {
			return pplx::create_task([=]
			{
				const utility::string_t query(web::http::uri::encode_uri(idolBase::_URL) + sparql::query::create(idolBase::_type_iri, idolBase::_idol_iri, get_param_predicate_list(params)));
				web::http::client::http_client client(web::uri{ query });

				//#ifdef _DEBUG
				std::wostringstream ss;
				ss << query;
				OutputDebugString(ss.str().c_str());
				//#endif
				return client.request(web::http::methods::GET);
			}).then([](web::http::http_response response)
			{
				if (response.status_code() == web::http::status_codes::OK)
				{
					try {
						// レスポンスをJSONとして解析する
						return response.extract_json(true);
					}
					catch (web::json::json_exception&) {
						throw;//有効なデータが帰ってきていない
					}
				}
				throw std::exception("http respnce error.");//ここどう処理すべきかわからん とりあえず例外投げる
			}).then([=](web::json::value json)
			{
				std::vector<std::shared_ptr<const idol>>idols;
				try {
					Params ps;
					for (const auto& p : params) ps.push_back(p.first);
					const auto& idols_val = json.at(sparql::results::JSON_RESULTS).at(sparql::results::JSON_BINDINGS).as_array();

					for (const auto& val : idols_val) {
						idols.push_back(std::shared_ptr<const idol>(new idol(val, ps)));
					}
				}
				catch (web::json::json_exception&) {
					throw;//クエリは通ってる　ここで例外投げられることはない？？
				}
				return idols;
			});
		}

		//パラメータの型を気にせずに比較できる関数
		//どちらかまたは両方のアイドルに該当パラメータが無い場合はfalse
		static bool is_same_param(const idol& l_op, const idol& r_op, ParametarName name) {
			if (!is_in_range(name))throw std::out_of_range("'name' is out of range.");
			try {
				return idolBase::get_params(name)->is_same_param(l_op.get_json_value(name), r_op.get_json_value(name));
			}
			catch (web::json::json_exception&) {
				return false;
			}
		}
	};
} 