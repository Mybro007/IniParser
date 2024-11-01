#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <list>

typedef std::pair <std::string, std::string> Entry;
typedef std::list <Entry> Entries;
typedef std::pair <std::string, Entries> Section;
typedef std::list <Section> IniData;

class IniPrs
{
	IniData iniData;
	Section sectn;
	Entries entries;
	Entry entr;
	std::string buf_;
	unsigned int str_counter;

	char* loadTextFile(std::string fname);
	bool isSpace(char* s);
	char* dropSpace(char* s);
	char* matchSymbol(char* s, char c);
	bool isAlpha(char* s);
	bool isNumerous(char* s);
	char* getToken(char* s, std::string& token);
	char* getString(char* s, std::string& str);
	char* getGroupName(char* s, std::string& group);
	char* getNameStr(char* s, std::string& name_vl, std::string& str);
	void makeEntry(std::string name_vl, std::string str);
	void makeEntries();
	void makeSection(std::string group);
	void makeGroup();
	std::string get_string_value(const std::string& name_section, const std::string& name_value);
public:
	IniPrs(std::string flname);
	template<typename T>
	T get_value(const std::string& name_section, const std::string& name_value)
	{
		static_assert(sizeof(T) == -1, "not implemented type for get_value");
	}

};

template<>
std::string IniPrs::get_value(const std::string& name_section, const std::string& name_value);

template<>
int IniPrs::get_value(const std::string& name_section, const std::string& name_value);

class ValueException : std::exception
{
public:
	const char* what() const noexcept override;
};

class FileException : std::exception
{
public:
	const char* what() const noexcept override;
};