#include "results.h"

using namespace std;
using namespace sparql;
using namespace web::json;

value& results::get_result_unit_list(::value& val) {
	try {
		return val.at(JSON_RESULTS).at(JSON_BINDINGS);
	}
	catch (json_exception&) {
		throw;
	}
}
