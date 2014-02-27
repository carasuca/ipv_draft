#pragma once

#include <string>
#include <sstream> // arbitrary type <-> string conversion

namespace ipv
{
	struct auto_str : std::wstring
	{
		typedef std::string str_t;
		typedef std::wstring wstr_t, base_t;
		typedef const str_t& cstr_t;
		typedef const wstr_t& cwstr_t;
		typedef auto_str my_t;
		typedef std::wostringstream oss_t;
		typedef std::wistringstream iss_t;

	private:
		// ansi string accessor, so you can use auto_str with 
		// ansi stream << >> operators and ansi("will be converted to unicode")
		struct ansi_proxy : str_t
		{
			ansi_proxy(auto_str& target) 
				: str_t(const_cast<const auto_str&>(target).ansi())
				, _target(target) {} 

			// perform ansi->unicode conversion
			~ansi_proxy() {_target.ansi(*this);}

		private:
			auto_str& _target;
		};

		friend struct ansi_proxy;

	public:
		auto_str(cwstr_t ws = wstr_t()) : wstr_t(ws) {}
		auto_str(cstr_t s) {ansi(s);}
		auto_str(const wchar_t wc[]) : wstr_t(wc) {}
		auto_str(const char c[]) {ansi(c);}

		template<typename T>
		auto_str(const T& type_to_str) : wstr_t(static_cast<oss_t&>(oss_t() << type_to_str).str()) {}

		// the only place for unicode<->ansi conversion
		const str_t& ansi() const {return _ansi.assign(begin(), end());}
		my_t& ansi(cstr_t s) {assign(s.begin(), s.end()); return *this;}
		ansi_proxy ansi() {return ansi_proxy(*this);}

		// access null terminated character arrays
		operator const char*() const {return ansi().c_str();}
		operator const wchar_t*() const {return c_str();}

		template <typename T>
		bool as(T& t) const {return !static_cast<iss_t&>(iss_t(*this) >> t).fail();}

		// access wide characters
		const wchar_t& operator[](int i) const {return at(i);}
		wchar_t& operator[](int i) {return at(i);}

		// TODO: insert stuff separated by space, except strings, which are separated by '\n'
		//template<typename T>
		//operator<<

		// TODO: extract stuff separated by space, except strings, which are separated by '\n'
		//template<typename T>
		//operator>>



	private:

		// local char buffer
		mutable str_t _ansi;
	};

	typedef auto_str str_t;
	typedef const str_t& cstr_t;
}
