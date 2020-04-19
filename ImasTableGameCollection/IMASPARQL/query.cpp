#include "query.h"

using namespace std;
using namespace sparql;
using namespace web;
using namespace web::json;


[[nodiscard]] wstring query::make_iri_and_prefix(const iri& iri_obj, vector<prefix>& prefixs, wstring& prefix_text) {
	if (iri_obj.has_using_prefix()) {
		int label_count = 0;
		for (auto& label : prefixs) {
			if (label == iri_obj.get_prefix()) {//事前定義したPREFIXとの衝突
				return make_prefix_label(label_count) + iri_obj.get_local_part();
			}
			++label_count;
		}
		//衝突無し
		prefixs.push_back(iri_obj.get_prefix());
		prefix_text += PREFIX + L" " + make_prefix_label(static_cast<int>(prefixs.size()) - 1) + iri_obj.get_prefix().get_iri();
		return make_prefix_label(static_cast<int>(prefixs.size()) - 1) + iri_obj.get_local_part();
	}
	else {//PREFIX無し
		return iri_obj.get_local_part();
	}
}

utility::string_t query::create(iri subject_type, iri subject, vector<objAcqPred> predicates) {
	vector<prefix> prefixs;
	wstring prefix_text;
	wstring query_text = SELECT + L" ";
	wstring filter_text;
	for (int i = 0; i < predicates.size(); ++i) { query_text += make_object(i); }
	query_text += L" " + WHERE + L"{" + make_subject(0) + L" ";

	//主語の作成
	//主語の述語部
	query_text += make_iri_and_prefix(subject_type, prefixs, prefix_text) + L" ";
	//主語の目的語部
	query_text += make_iri_and_prefix(subject, prefixs, prefix_text);

	//述語・目的語の作成
	for (int i = 0; auto & p : predicates) {
		query_text += L";" + make_iri_and_prefix(p.predicate, prefixs, prefix_text);
		query_text += L" " + make_object(i);

		//FILTER部分
		for (const auto& f : p.query_filter) {
			filter_text += FILTER + L"(" + f->get_filter_text(make_object(i)) + L").";
		}
		++i;
	}

	query_text += L"." + filter_text + L"}";

	return uri::encode_data_string(prefix_text + query_text);
}