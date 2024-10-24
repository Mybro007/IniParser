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
	unsigned int str_counter{ 0 };

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
public:
	IniPrs(std::string flname);
	template <typename T>
	T get_value(std::string name_section, std::string name_value);
};

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