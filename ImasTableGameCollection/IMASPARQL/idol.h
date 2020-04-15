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

namespace imas {
#pragma region Parametar setting
	enum class ParametarName {
		Name = 0,
		Month,
		Date,
		Place,
		Age,
		Height,
		Weight,
		//Bust,
		//Waist,
		//Hip,
		Blood,
		Hand,
		Gender,
		Color,
		COUNT
	};
	static constexpr size_t ParametarName_COUNT = static_cast<size_t>(ParametarName::COUNT);

#pragma region Prameters concepts
	//�R���Z�v�g(���I�g�p�����邽��constexpr�֐�)
	//��`���Ȃ����͕s�K�i(COUNT������)
	constexpr bool is_in_range(ParametarName param) {
		return static_cast<int>(param) >= 0 && static_cast<int>(param) < ParametarName_COUNT;
	}

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
			(param == ParametarName::Weight);// ||
			//(param == ParametarName::Bust) ||
			//(param == ParametarName::Waist) ||
			//(param == ParametarName::Hip);
	}
	//bool�^
	constexpr bool is_boolean_parm([[maybe_unused]] ParametarName param) {
		return false;
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
	//�񋓌^(���ł�bool��)
	constexpr bool is_enum_parm(ParametarName param) {
		return is_boolean_parm(param) ||
			is_blood_parm(param) ||
			is_hand_parm(param) ||
			is_gender_parm(param);
	}

	//�ˋ�̌^
	//���I���͐����̂���
	enum class NumberParametarName {
	};
	enum class EnumParam {};

#pragma endregion

#pragma region enum, struct parametar
	enum class BloodType {
		A = 0,
		B,
		AB,
		O,
	};
	enum class Handedness
	{
		Right,
		Left
	};
	enum class Gender
	{
		Male,
		Female
	};
	struct idolColor{
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
#pragma endregion



	//�A�C�h���̃f�[�^����т�����擾����N�G���̃N���X
	class idol {
	private:
#pragma region Static object
#pragma region Parametar class
		class idolParamBase {
		private:
		protected:
			idolParamBase(sparql::objAcqPred predicate) : _predicate(predicate){}
			virtual ~idolParamBase(){}
			sparql::objAcqPred _predicate;
		public:
			sparql::objAcqPred get_predicate() { return _predicate; }
			virtual std::any get_data(web::json::value)const = 0;
			virtual bool is_arithmetic()const noexcept = 0;
			virtual const std::type_info& data_type()const noexcept = 0;
			virtual bool is_same_param(const web::json::value& l_op, const web::json::value& r_op)const noexcept = 0;
			//��肭�ǂ��̂ō폜 to_int()��to_double()�őΉ�
			//virtual std::partial_ordering is_spaceship_param(const web::json::value& l_op, const web::json::value& r_op)const = 0;
		};


		//�p�����[�^�p�N���X
		template<ParametarName P>
		class idolParam : public idolParamBase {
			static_assert(is_in_range(P), "P is out of range.");
		
		public:
			idolParam(sparql::objAcqPred pred): idolParamBase(pred) {}

#pragma region �߂�l�������I�Ȋe�p�����[�^�̃f�[�^�擾�֐�
			//wstring
			static std::wstring data_conv(web::json::value val) requires (is_string_parm(P)){
				return val.at(sparql::results::JSON_VALUE).as_string().c_str();
			}
			//int & double(���t�͂���ɓ��ꉻ)
			static int data_conv(web::json::value val) requires (is_arithmetic_parm(P)){
				if constexpr (P == ParametarName::Month) {
					//���K�\���g��Ȃ��Ă������
					std::wstring code_str = val.at(sparql::results::JSON_VALUE).as_string().c_str();
					try {
						return std::stoi(code_str.substr(2, 2));
					}
					catch (web::json::json_exception& e) {
						std::wostringstream ss;
						ss << e.what() << L"\n";
						OutputDebugString(ss.str().c_str());
						throw;
					}
					catch (std::invalid_argument&) {
						return -1;
					}
				}
				else if constexpr (P == ParametarName::Date) {
					std::wstring code_str = val.at(sparql::results::JSON_VALUE).as_string().c_str();
					try {
						return std::stoi(code_str.substr(5, 2));
					}
					catch (web::json::json_exception& e) {
						std::wostringstream ss;
						ss << e.what() << L"\n";
						OutputDebugString(ss.str().c_str());
						throw;
					}
					catch (std::invalid_argument&) {
						return -1;
					}
				}
				else {
					try {
						return std::stoi(val.at(sparql::results::JSON_VALUE).as_string().c_str());
					}
					catch (web::json::json_exception& e) {
						std::wostringstream ss;
						ss << e.what() << L"\n";
						OutputDebugString(ss.str().c_str());
						throw;
					}
					catch (std::invalid_argument&) {
						return -1;
					}
				}
			}
			//bool
			static bool data_conv(web::json::value val) requires (is_boolean_parm(P)){
				try {
					std::wstring value = val.at(sparql::results::JSON_VALUE).as_string().c_str();
					return (value == L"true");
					
				}
				catch (web::json::json_exception&) {
					return false;
				}
			}
			//���t�^
			static BloodType data_conv(web::json::value val) requires (is_blood_parm(P)){
				std::wstring value = val.at(sparql::results::JSON_VALUE).as_string().c_str();
				if (value == L"A")return BloodType::A;
				else if (value == L"B")return BloodType::B;
				else if (value == L"AB")return BloodType::AB;
				else if (value == L"O")return BloodType::O;
				throw web::json::json_exception(L"Cannot change argument to 'BloodType' type");
			}
			//������
			static Handedness data_conv(web::json::value val) requires (is_hand_parm(P)){
				std::wstring value = val.at(sparql::results::JSON_VALUE).as_string().c_str();
				if (value == L"right")return Handedness::Right;
				else if (value == L"left")return Handedness::Left;
				throw web::json::json_exception(L"Cannot change argument to 'Handedness' type");
			}
			//��
			static Gender data_conv(web::json::value val) requires (is_gender_parm(P)){
				std::wstring value = val.at(sparql::results::JSON_VALUE).as_string().c_str();
				if (value == L"male")return Gender::Male;
				else if (value == L"female")return Gender::Female;
				throw web::json::json_exception(L"Cannot change argument to 'Gender' type");
			}
			//�F
			static idolColor data_conv(web::json::value val) requires (is_color_parm(P)){
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
			virtual std::any get_data(web::json::value val)const override final{
				return std::any(data_conv(val));
			}

			//�召��r�\��(���I�`�F�b�N�ɂ���)
			virtual bool is_arithmetic()const noexcept override final {
				return is_arithmetic_parm(P);
			}

			//�ϊ���f�[�^�^
			virtual const std::type_info& data_type()const noexcept override final{
				return typeid(decltype(data_conv(web::json::value())));
			}

			//���l��r
			virtual bool is_same_param(const web::json::value& l_op, const web::json::value& r_op)const noexcept override final{
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
		static inline const std::array<idolParamBase*, ParametarName_COUNT> params{
			(new idolParam<ParametarName::Name>(sparql::objAcqPred(_name_iri, [](const std::wstring& arg) {return L"(LANG(" + arg + L") = 'ja')"; }))),
			(new idolParam<ParametarName::Month>(sparql::objAcqPred(_birth_date_iri))),
			(new idolParam<ParametarName::Date>(sparql::objAcqPred(_birth_date_iri))),
			(new idolParam<ParametarName::Place>(sparql::objAcqPred(_birth_place_iri))),
			(new idolParam<ParametarName::Age>(sparql::objAcqPred(_age_iri))),
			(new idolParam<ParametarName::Height>(sparql::objAcqPred(_height_iri))),
			(new idolParam<ParametarName::Weight>(sparql::objAcqPred(_weight_iri))),
			//(new idolParam<ParametarName::Bust>(sparql::objAcqPred(_bust_iri))),
			//(new idolParam<ParametarName::Waist>(sparql::objAcqPred(_waist_iri))),
			//(new idolParam<ParametarName::Hip>(sparql::objAcqPred(_hip_iri))),
			(new idolParam<ParametarName::Blood>(sparql::objAcqPred(_blood_iri))),
			(new idolParam<ParametarName::Hand>(sparql::objAcqPred(_hand_iri))),
			(new idolParam<ParametarName::Gender>(sparql::objAcqPred(_gender_iri))),
			(new idolParam<ParametarName::Color>(sparql::objAcqPred(_color_iri)))
		};
#pragma endregion
#pragma endregion
		//�N�G���p
		static std::vector<sparql::objAcqPred> get_param_predicate_list();

		//�擾�l(�B��̔�ÓI�����o�ϐ�)
		const web::json::value _values;


		//�R���X�g���N�^�B�؂�
		//json�̃`�F�b�N�͂����ł��Ȃ��̂�
		idol(web::json::value val) : _values(val) {}
	public:
		//�R�s�[�R���X�g���N�^�͖���(�����l�Ԃ͓�l�Ƌ����...)
		//idol(const idol& i) : _values(i.get_json_list()) {}
		//idol& operator=(const idol& i) { return *this; }

		//�p�����[�^�̎擾
		web::json::value get_json_value(ParametarName name) const;
		//�p�����[�^�̎擾
		std::wstring to_wstring(ParametarName name) const;
		//�p�����[�^�̎擾
		int to_int(ParametarName name) const;
		//�p�����[�^�̎擾(����Ȃ��C������)
		std::any get_data(ParametarName name) const;
		//�p�����[�^�̎擾
		template<ParametarName P>
		auto get_data() const {
			static_assert(is_in_range(P), "'P' is out of range.");
			try {
				return dynamic_cast<idolParam<P>*>(params.at(static_cast<size_t>(P)))->data_conv(_values.at(sparql::query::get_object(static_cast<int>(P))));
			}
			catch (web::json::json_exception&) {
				throw web::json::json_exception("This parameter does not exist.");//�p�����[�^����
			}
			catch (std::bad_cast& e) {//dynamic_cast���s array�̈ʒu���m�F���ׂ�
				std::wostringstream ss;
				ss << e.what();
				OutputDebugString(ss.str().c_str());
				throw;
			}
		}


#pragma region �ÓI�֐�
		//http reqest�𓊂��ăA�C�h�������󂯎��
		//idol���̂͂��̊֐��ȊO�Ő����ł��Ȃ�
		static pplx::task<std::vector<std::shared_ptr<const idol>>> get_idol_list();


		//�p�����[�^�̔�r
		static bool is_same_param(const idol& l_op, const idol& r_op, ParametarName name);
#pragma endregion
	};
} 