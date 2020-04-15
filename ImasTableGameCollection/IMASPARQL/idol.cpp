#include "idol.h"


using namespace std;
using namespace sparql;
using namespace imas;
using namespace web;
using namespace web::json;
using namespace web::http;


#pragma region param class

#pragma endregion

vector<objAcqPred> idol::get_param_predicate_list() {
	vector<sparql::objAcqPred> predicates;
	for (auto& obj : params) {
		predicates.push_back(obj->get_predicate());
	}
	return predicates;
}

value idol::get_json_value(ParametarName name) const {
	if (!is_in_range(name))throw out_of_range("'name' is out of range.");
	try {
		//return _values.at(L"object1");
		return _values.at(query::get_object(static_cast<int>(name)));
	}
	catch (json_exception&) {
		throw json_exception("This parameter does not exist.");//パラメータ欠損
	}
}

wstring idol::to_wstring(ParametarName name) const {
	if (!is_in_range(name))throw out_of_range("'name' is out of range.");
	try {
		return get_json_value(name).at(sparql::results::JSON_VALUE).as_string().c_str();
	}
	catch (json_exception&) {
		throw json_exception("This parameter does not exist.");//パラメータ欠損
	}
}

int idol::to_int(ParametarName name) const {
	if (!is_arithmetic_parm(name))throw logic_error("'name' isn't arithmetic.");
	return any_cast<int>(get_data(name));
}

any idol::get_data(ParametarName name) const {
	if (!is_in_range(name))throw out_of_range("'name' is out of range.");
	try {
		return any(params.at(static_cast<size_t>(name))->get_data(_values.at(query::get_object(static_cast<int>(name)))));
	}
	catch (json_exception&) {
		throw json_exception("This parameter does not exist.");//パラメータ欠損
	}
}

pplx::task<vector<shared_ptr<const idol>>> idol::get_idol_list() {
	return pplx::create_task([]
	{
		const utility::string_t query(uri::encode_uri(_URL) + query::create(_type_iri, _idol_iri, get_param_predicate_list()));
		client::http_client client(uri{ query });

		//#ifdef _DEBUG
		wostringstream ss;
		ss << query;
		OutputDebugString(ss.str().c_str());
		//#endif
		return client.request(methods::GET);
	}).then([](http_response response)
	{
		if (response.status_code() == status_codes::OK)
		{
			try {
				// レスポンスをJSONとして解析する
				return response.extract_json(true);
			}
			catch (json_exception&) {
				throw;//有効なデータが帰ってきていない
			}
		}
		throw exception("http respnce error.");//ここどう処理すべきかわからん とりあえず例外投げる
	}).then([](value json)
	{
		vector<shared_ptr<const idol>>idols;
		try {
			const auto& idols_val = json.at(results::JSON_RESULTS).at(results::JSON_BINDINGS).as_array();

			for (const auto& val : idols_val) {
				idols.push_back(shared_ptr<const idol>(new idol(val)));
			}
		}
		catch (json_exception&) {
			throw;//クエリは通ってる　ここで例外投げられることはない？？
		}
		return idols;
	});
}

bool idol::is_same_param(const idol& l_op, const idol& r_op, ParametarName name) {
	if (!is_in_range(name))throw std::out_of_range("'name' is out of range.");
	try {
		return params.at(static_cast<size_t>(name))->is_same_param(l_op.get_json_value(name), r_op.get_json_value(name));
	}
	catch (json_exception&) {
		return false;
	}
}