#pragma once

#include <iostream>
#include <string>

namespace imas {
	//�p�����[�^
	enum class ParametarName {
		Name = 0,
		Title,
		Month,
		Date,
		Place,
		Age,
		Height,
		Weight,
		Bust,
		Waist,
		Hip,
		Blood,
		Hand,
		Gender,
		Color,
		COUNT
	};
	static constexpr size_t ParametarName_COUNT = static_cast<size_t>(ParametarName::COUNT);
	constexpr bool is_in_range(ParametarName param) {
		return static_cast<int>(param) >= 0 && static_cast<int>(param) < ParametarName_COUNT;
	}

	//�A�C�h���N���X�̑O���錾
	class idol;


#pragma region enum, struct parametar
	template<typename T>
	concept EnumTypeParam = requires {
		T::NAME;
		requires std::is_enum_v<T::Type>;
		T::literals;
	};


	struct Title final{
		static constexpr ParametarName NAME = ParametarName::Title;
		enum class Type {
			None = -1,
			First = 0,
			NamcoAS,
			Production1054,
			DearlyStars,
			Cinderella,
			Million,
			Production315,
			Production283,
		};
		static inline const std::array<std::wstring, 7> literals{
			L"1st Vision",
			L"765AS",
			L"1054Pro",
			L"DearlyStars",
			L"CinderellaGirls"
			L"MillionStars",
			L"315ProIdols",
			L"283Pro"
		};
	};
	struct Blood {
		static constexpr ParametarName NAME = ParametarName::Blood;
		enum class Type {
			None = -1,
			A = 0,
			B,
			AB,
			O,
		};
		static inline const std::array<std::wstring, 4> literals{
			L"A",
			L"B",
			L"AB",
			L"O",
		};
	};
	struct Handedness {
		static constexpr ParametarName NAME = ParametarName::Hand;
		enum class Type {
			None = -1,
			Right = 0,
			Left
		};
		static inline const std::array<std::wstring, 2> literals{
			L"right",
			L"left",
		};
	};
	struct Gender {
		static constexpr ParametarName NAME = ParametarName::Gender;
		enum class Type {
			None = -1,
			Male = 0,
			Female
		};
		static inline const std::array<std::wstring, 2> literals{
			L"male",
			L"female",
		};
	};
	struct idolColor {
		uint8_t red;
		uint8_t green;
		uint8_t blue;
	};
	constexpr bool operator==(const idolColor& a, const idolColor& b) {
		return (a.red == b.red) && (a.green == b.green) && (a.blue == b.blue);
	}
	constexpr bool operator!=(const idolColor& a, const idolColor& b) {
		return !(a == b);
	}
#pragma endregion


#pragma region Prameters concepts
	//�R���Z�v�g(���I�g�p�����邽��constexpr�֐�)
	//��`���Ȃ����͕s�K�i(COUNT������)

	//wstring�^
	constexpr bool is_string_parm(ParametarName param) {
		return (param == ParametarName::Name) ||
			(param == ParametarName::Place);
	}
	//�����^
	constexpr bool is_integral_param(ParametarName param) {
		return (param == ParametarName::Month) ||
			(param == ParametarName::Date) ||
			(param == ParametarName::Age);
	}
	//���������_�^(�����p�����[�^�����Ȃ��̂ōŏI�I�ɂ�int�Ŏ��o��)
	constexpr bool is_floating_point_param(ParametarName param) {
		return (param == ParametarName::Height) ||
			(param == ParametarName::Weight) ||
			(param == ParametarName::Bust) ||
			(param == ParametarName::Waist) ||
			(param == ParametarName::Hip);
	}
	//���t�^
	constexpr bool is_title_parm(ParametarName param) {
		return param == ParametarName::Title;
	}
	//���t�^
	constexpr bool is_blood_parm(ParametarName param) {
		return param == ParametarName::Blood;
	}
	//������
	constexpr bool is_hand_parm(ParametarName param) {
		return param == ParametarName::Hand;
	}
	//��
	constexpr bool is_gender_parm(ParametarName param) {
		return param == ParametarName::Gender;
	}
	//�F
	constexpr bool is_color_parm(ParametarName param) {
		return param == ParametarName::Color;
	}

	//�傫��������
	//���l�^
	constexpr bool is_arithmetic_parm(ParametarName param) {
		return is_integral_param(param) || is_floating_point_param(param);
	}
	//�񋓌^
	constexpr bool is_enum_parm(ParametarName param) {
		return is_title_parm(param) ||
			is_blood_parm(param) ||
			is_hand_parm(param) ||
			is_gender_parm(param);
	}
#pragma endregion




	//�B���p�N���X&���ʉ��x�[�X
	class idolBase {
		friend class idol;
	private:
#pragma region parameter class
		//�p�����[�^���ʉ��x�[�X�N���X
		class idolParamBase {
		private:
		protected:
			idolParamBase(std::wstring name,sparql::objAcqPred predicate) : _name(name), _predicate(predicate) {}
			virtual ~idolParamBase() {}
			const std::wstring _name;
			sparql::objAcqPred _predicate;
			static inline std::wstring to_string(web::json::value val) {
				return val.at(sparql::results::JSON_VALUE).as_string().c_str();
			}
		public:
			std::wstring get_name(){ return _name; }
			sparql::objAcqPred get_predicate() { return _predicate; }
			sparql::objAcqPred get_predicate(std::initializer_list<std::shared_ptr<sparql::filter>> filters) { 
				auto filter_list = _predicate.query_filter;
				for (auto f : filters)filter_list.push_back(f);
				return sparql::objAcqPred(_predicate.predicate, filter_list);
			}
			sparql::objAcqPred get_predicate(std::vector<std::shared_ptr<sparql::filter>> filters) {
				auto filter_list = _predicate.query_filter;
				for (auto f : filters)filter_list.push_back(f);
				return sparql::objAcqPred(_predicate.predicate, filter_list);
			}



			virtual std::any get_data(web::json::value)const = 0;
			virtual bool is_arithmetic()const noexcept = 0;
			virtual const std::type_info& data_type()const noexcept = 0;
			virtual bool is_same_param(const web::json::value& l_op, const web::json::value& r_op)const noexcept = 0;
		};


		//�p�����[�^�N���X
		template<ParametarName P>
		class idolParam : public idolParamBase {
			static_assert(is_in_range(P), "P is out of range.");

		public:
			idolParam(std::wstring name, sparql::objAcqPred pred) : idolParamBase(name, pred) {}

#pragma region �߂�l�������I�Ȋe�p�����[�^�̃f�[�^�擾�֐� �y�ыt�ϊ�
			//wstring
			static std::wstring data_conv(web::json::value val) requires (is_string_parm(P)) {
				return to_string(val);
			}
			static std::wstring data_to_string(std::wstring val) requires (is_string_parm(P)) {
				return val;
			}
			//int & double(���t�͂���ɓ��ꉻ)
			static int data_conv(web::json::value val) requires (is_arithmetic_parm(P)) {
				if constexpr (P == ParametarName::Month) {
					//���K�\���g��Ȃ��Ă������
					std::wstring code_str = to_string(val);
					try {
						return std::stoi(code_str.substr(2, 2));
					}
					catch (std::invalid_argument&) {
						return -1;
					}
				}
				else if constexpr (P == ParametarName::Date) {
					std::wstring code_str = to_string(val);
					try {
						return std::stoi(code_str.substr(5, 2));
					}
					catch (std::invalid_argument&) {
						return -1;
					}
				}
				else {
					try {
						return std::stoi(to_string(val));
					}
					catch (std::invalid_argument&) {
						return -1;
					}
				}
			}
			static std::wstring data_to_string(int val) requires (is_arithmetic_parm(P)) {
				return std::to_wstring(val);
			}
			//�^�C�g��
			static Title::Type data_conv(web::json::value val) requires (is_title_parm(P)) {
				std::wstring value = to_string(val);
				for (int i = 0; auto l : Title::literals) {
					if (l == value)return static_cast<Title::Type>(i);
					++i;
				}
				return Title::Type::None;
			}
			static std::wstring data_to_string(Title::Type val) requires (is_arithmetic_parm(P)) {//�ꊇ�����ɂ�����
				return Title::literals.at(static_cast<size_t>(val));
			}
			//���t�^
			static Blood::Type data_conv(web::json::value val) requires (is_blood_parm(P)) {
				std::wstring value = to_string(val);
				for (int i = 0; auto l : Blood::literals) {
					if (l == value)return static_cast<Blood::Type>(i);
					++i;
				}
				return Blood::Type::None;
			}
			//������
			static Handedness::Type data_conv(web::json::value val) requires (is_hand_parm(P)) {
				std::wstring value = to_string(val);
				for (int i = 0; auto l : Handedness::literals) {
					if (l == value)return static_cast<Handedness::Type>(i);
					++i;
				}
				return Handedness::Type::None;
			}
			//��
			static Gender::Type data_conv(web::json::value val) requires (is_gender_parm(P)) {
				std::wstring value = to_string(val);
				for (int i = 0; auto l : Gender::literals) {
					if (l == value)return static_cast<Gender::Type>(i);
					++i;
				}
				return Gender::Type::None;
			}
			//�F
			static idolColor data_conv(web::json::value val) requires (is_color_parm(P)) {
				try {
					std::wstring code_str = val.at(sparql::results::JSON_VALUE).as_string().c_str();
					return idolColor{
						static_cast<uint8_t>(std::stoi(code_str.substr(0, 2), nullptr, 16)),
						static_cast<uint8_t>(std::stoi(code_str.substr(2, 2), nullptr, 16)),
						static_cast<uint8_t>(std::stoi(code_str.substr(4, 2), nullptr, 16)) };
				}
				catch (web::json::json_exception& e) {
					std::wostringstream ss;
					ss << e.what() << L"\n";
					OutputDebugString(ss.str().c_str());
					throw;
				}
				catch (std::invalid_argument&) {
					return idolColor{ 0xFF, 0xFF, 0xFF };
				}
			}
#pragma endregion

			//json�̃��e�����^����̃f�[�^�擾�֐�
			virtual std::any get_data(web::json::value val)const override final {
				return std::any(data_conv(val));
			}

			//�召��r�\��(���I�`�F�b�N�ɂ���)
			virtual bool is_arithmetic()const noexcept override final {
				return is_arithmetic_parm(P);
			}

			//�ϊ���f�[�^�^
			virtual const std::type_info& data_type()const noexcept override final {
				return typeid(decltype(data_conv(web::json::value())));
			}

			//���l��r
			virtual bool is_same_param(const web::json::value& l_op, const web::json::value& r_op)const noexcept override final {
				try {
					return data_conv(l_op) == data_conv(r_op);
				}
				catch (...) {
					return false;
				}
			}
		};
#pragma endregion



#pragma region �萔��
		static inline const std::wstring _URL = L"https://sparql.crssnky.xyz/spql/imas/query?output=json&force-accept=text/plain&query=";
#pragma region PREFIX
		static inline const sparql::prefix _schema_prefix{ L"<http://schema.org/>" };
		static inline const sparql::prefix _rdfs_prefix{ L"<http://www.w3.org/2000/01/rdf-schema#>" };
		static inline const sparql::prefix _rdf_prefix{ L"<http://www.w3.org/1999/02/22-rdf-syntax-ns#>" };
		static inline const sparql::prefix _xsd_prefix{ L"<http://www.w3.org/2001/XMLSchema#>" };
		static inline const sparql::prefix _foaf_prefix{ L"<http://xmlns.com/foaf/0.1/>" };
		static inline const sparql::prefix _math_prefix{ L"<http://www.w3.org/2005/xpath-functions/math#>" };
		static inline const sparql::prefix _imas_prefix{ L"<https://sparql.crssnky.xyz/imasrdf/URIs/imas-schema.ttl#>" };
		static inline const sparql::prefix _imasrdf_prefix{ L"<https://sparql.crssnky.xyz/imasrdf/RDFs/detail/>" };
#pragma endregion

#pragma region IRI
		static inline const sparql::iri _type_iri{ _rdf_prefix , L"type" };
		static inline const sparql::iri _idol_iri{ _imas_prefix , L"Idol" };
		static inline const sparql::iri _name_iri{ _schema_prefix , L"name" };
		static inline const sparql::iri _title_iri{ _imas_prefix , L"Title" };
		static inline const sparql::iri _birth_date_iri{ _schema_prefix , L"birthDate" };
		static inline const sparql::iri _birth_place_iri{ _schema_prefix , L"birthPlace" };
		static inline const sparql::iri _age_iri{ L"<http://xmlns.com/foaf/0.1/age>" };
		static inline const sparql::iri _height_iri{ _schema_prefix , L"height" };
		static inline const sparql::iri _weight_iri{ _schema_prefix , L"weight" };
		static inline const sparql::iri _bust_iri{ _imas_prefix , L"Bust" };
		static inline const sparql::iri _waist_iri{ _imas_prefix , L"Waist" };
		static inline const sparql::iri _hip_iri{ _imas_prefix , L"Hip" };
		static inline const sparql::iri _blood_iri{ _imas_prefix , L"BloodType" };
		static inline const sparql::iri _hand_iri{ _imas_prefix , L"Handedness" };
		static inline const sparql::iri _gender_iri{ _schema_prefix , L"gender" };
		static inline const sparql::iri _color_iri{ _imas_prefix , L"Color" };
#pragma endregion

		//�p�����[�^�̐ݒ�
		//���I�����N�̂��� 
		//iri�▼�O���N���X���Ƃɓ��ꃁ���o�����ׂ����������e�����邢�̂őË�
		static inline const std::array<std::shared_ptr<idolParamBase>, ParametarName_COUNT> params{
			std::shared_ptr<idolParamBase>(new idolParam<ParametarName::Name>(L"���O", sparql::objAcqPred(_name_iri, std::shared_ptr<sparql::filter>(new sparql::langFilter(sparql::langFilter::Type::ja))))),
			std::shared_ptr<idolParamBase>(new idolParam<ParametarName::Title>(L"�^�C�g��", sparql::objAcqPred(_title_iri))),
			std::shared_ptr<idolParamBase>(new idolParam<ParametarName::Month>(L"�a����", sparql::objAcqPred(_birth_date_iri))),
			std::shared_ptr<idolParamBase>(new idolParam<ParametarName::Date>(L"�a����(��)", sparql::objAcqPred(_birth_date_iri))),
			std::shared_ptr<idolParamBase>(new idolParam<ParametarName::Place>(L"�o�g�n", sparql::objAcqPred(_birth_place_iri))),
			std::shared_ptr<idolParamBase>(new idolParam<ParametarName::Age>(L"�N��", sparql::objAcqPred(_age_iri))),
			std::shared_ptr<idolParamBase>(new idolParam<ParametarName::Height>(L"�g��", sparql::objAcqPred(_height_iri))),
			std::shared_ptr<idolParamBase>(new idolParam<ParametarName::Weight>(L"�̏d", sparql::objAcqPred(_weight_iri))),
			std::shared_ptr<idolParamBase>(new idolParam<ParametarName::Bust>(L"�o�X�g", sparql::objAcqPred(_bust_iri))),
			std::shared_ptr<idolParamBase>(new idolParam<ParametarName::Waist>(L"�E�G�X�g", sparql::objAcqPred(_waist_iri))),
			std::shared_ptr<idolParamBase>(new idolParam<ParametarName::Hip>(L"�q�b�v", sparql::objAcqPred(_hip_iri))),
			std::shared_ptr<idolParamBase>(new idolParam<ParametarName::Blood>(L"���t�^", sparql::objAcqPred(_blood_iri))),
			std::shared_ptr<idolParamBase>(new idolParam<ParametarName::Hand>(L"������", sparql::objAcqPred(_hand_iri))),
			std::shared_ptr<idolParamBase>(new idolParam<ParametarName::Gender>(L"����", sparql::objAcqPred(_gender_iri))),
			std::shared_ptr<idolParamBase>(new idolParam<ParametarName::Color>(L"�C���[�W�J���[", sparql::objAcqPred(_color_iri)))
		};


		static std::shared_ptr<idolParamBase> get_params(ParametarName name) {
			if (!is_in_range(name))throw std::out_of_range("'name' is out of range.");
			return params.at(static_cast<size_t>(name));
		}
#pragma endregion
	};
}