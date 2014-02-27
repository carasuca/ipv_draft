// ipvcli.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <ipv/sdk/plugin.h>
#include <ipv/internal.h>

#include <Windows.h>

using namespace std;
using namespace ipv;

// TODO: move up somewhere?
#define tcin wcin
#define tcout wcout
#define tclog wclog

#define T IPV_TXT
#define IPV_S const IPV_CHAR*

#define IPV_VERSION_STR T(IPV_VERSION_MAJOR) ## T(".") ## T(IPV_VERSION_MAJOR)
#define IPV_BUILD

str_t filename(str_t s)
{
	s = s.substr(s.find_last_of(T('\\')) + 1);
	return s.substr(0, s.find_last_of(T('.')));
}

static const IPV_CHAR g_find_plugins[] = T("find_plugins");

namespace ipv
{
	struct plugin_base : plugin_i
	{
		FARPROC m_fn_ptr;

		virtual result plugin_i::run(int c = 0, IPV_CHAR* v[] = 0)
		{
			return reinterpret_cast<plugin_fn_ptr>(m_fn_ptr)(c, v);
		}

		virtual const void* data() const
		{
			return m_fn_ptr;
		}

		virtual bool data(const void*, size_t)
		{
			return false;
		}

		virtual size_t bytes() const
		{
			return sizeof(int);
		}

		virtual size_t type_id() const
		{
			return 10;
		}

		virtual const IPV_CHAR* type_name() const
		{
			return IPV_TXT("plugin");
		}

		plugin_base(FARPROC fn_ptr) : m_fn_ptr(fn_ptr) {}
	};

	bool set(const data_i&, cstr name)
	{
		return false;//instance().set(0, 0);
	}

	bool add(FARPROC fn_ptr, cstr name)
	{
		//return ipv::set(plugin_base(fn_ptr), name);
		return false;
	}
}

int find_plugins(int c = 0, IPV_CHAR* v[] = 0)
{
	//if ipv::has(T("plugin_dir"));
	//ipv::get()
	// NeedCurrentDirectoryForExePathA??

	WIN32_FIND_DATAA data;
	HANDLE handle = ::FindFirstFileA("*.dll", &data);

	for(BOOL res = handle != INVALID_HANDLE_VALUE; res; res = ::FindNextFileA(handle, &data))
	{
		HMODULE lib = ::LoadLibraryA(data.cFileName);
		FARPROC fp = ::GetProcAddress(lib, "wmain");
		if (fp) ipv::add(fp, str(data.cFileName));
	}
	return false;
}

struct ipvcli : framework_impl_base
{
	// TODO: change to multimap
	map<str_t, str_t> params;
	map<str_t, image16> images;
	map<str_t, plugin_i*> plugins;

	typedef IPV_CHAR char_t;
	// access parameters values, images and other data
	// some params can be reserved, like "log"
	bool framework_i::set(const data_i& val, const data_i* name) 
	{tcout << (!name || name->type_id()) << char_t('=') << val.type_id() << endl; return true;}
	bool get(data_i&, const data_i* = 0) {return false;}

	////bool framework_i::get(char_t* val, const char_t*)
	//{
	//	// TODO: switch to data_i
	//	// if string, get param
	//	// if image, get image
	//	// if plugin, get plugin
	//	tcin >> val;
	//	return false;
	//	//return cin >> 
	//}

	// execute a plugin
	bool run(const char_t* name)
	{
		try {
			auto it = plugins.find(name);
			if (it != plugins.end() && it->second) it->second->run();
			else log("Plugin not found.", __FUNCTION__);
		} 
		catch (const ipv::runtime_exception& e) {log(e.what(), __FUNCTION__); return false;}

		return false;
	}

	// communicate with user interface
	bool update(int percent) 
	{
		tcout << percent << IPV_TXT("%\r");
		return true;
	}

	void show(const char_t*) {cout << "not implemented" << endl;}
	void log(const char_t* m, const char_t* s) { tclog << T('[') << s << T(']') << m << endl;}
	void log(const char* m, const char* s) { clog << '[' << s << ']' << m << endl;}

	// helper to hold a copy of command line arguments
	struct cmdline : vector<str_t>
	{
		vector<char_t*> m_argv;

		cmdline(cstr);
		cmdline(int c, char_t*v[]) : vector(v, v + c)
		{
			for(auto it = begin(); it != end(); ++it)
				//if (!it->empty()) 
					m_argv.push_back(const_cast<char_t*>(it->c_str()));
		}

		char_t*const* argv() const {return &m_argv.front();}
		int argc() const {return m_argv.size();}
	};

	int framework_impl_i::run(int c, IPV_CHAR *v[])
	{
		for (str_t line; (tcout << T("ipv>")).good() && getline(tcin, line);)
		{
			// basic one word operations
			if (line.empty()) continue;
			if (line.back() == T(';')) line.pop_back();

			if (line == T("continue")) continue;
			if (line == T("break")) break;
			if (line == T("exit")) exit(HadToExit);

			if (line == T("version")) 
			{
				tcout << "ImProVise CLI " << IPV_VERSION_MAJOR << T('.') << IPV_VERSION_MINOR << endl;
				continue;
			}

			// parameterized operators
			typedef std::wistringstream iss_t;
			iss_t iss(line);
			str_t cmd, par;

			if (iss >> cmd >> par)
			{
				if (cmd == T("list")); else
				if (cmd == T("show")) true;
			}
			else tcout << line << "?\n";
			//::_wexecvpe(cmd.c_str(), v, NULL);
			if (cmd == T("run")) run(par.c_str());
		}

		return OK;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	return ipvcli().run(argc, argv);
}

