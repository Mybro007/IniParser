#include "IniPrs.h"

typedef std::pair <std::string, std::string> Entry;
typedef std::list <Entry> Entries;
typedef std::pair <std::string, Entries> Section;
typedef std::list <Section> IniData;
	
char* IniPrs::loadTextFile(std::string fname)
	{
		std::ifstream fl(fname);
		if (!fl)
		{
			throw FileException();
		}
		fl.seekg(0, fl.end);
		long int size = fl.tellg();
		fl.seekg(0, std::ios::beg);
		char* buf = new char[size + 1];
		char current;
		int i = 0;
		while(!fl.eof())
		{
			current = fl.get();
			if (current == ';')
			{
				while (current != '\n')
				{
					current = fl.get();
				}
			}
			if (current == 'ÿ')
			{
				break;
			}
			buf[i] = current;
			i++;
		}
		buf[i] = '\0';
		return buf;
	}

bool IniPrs::isSpace(char* s)
	{
		return (*s == ' ' || *s == '\n');
	}

char* IniPrs::dropSpace(char* s)
	{
		while ((isSpace(s) || *s == ';') && *s != '\0')
		{
			if (*s == ';')
			{
				while (*s != '\n' && *s != '\0')
					s++;
			}
			if (*s == '\n')
				str_counter++;
			if (*s == '\0')
				return s;
			s++;
		}
		return s;
	}

char* IniPrs::matchSymbol(char* s, char c)
	{
		s = dropSpace(s);
		if (*s == c)
		{
			s++;
			return s;
		}
		else
		{
			std::cout << "(" << str_counter + 1 << ") " << "'" << c << "' is expected, but " << *s << "received\n";
			exit(0);
		}
	}

bool IniPrs::isAlpha(char* s)
	{
		return ((*s >= 'a' && *s <= 'z') || (*s >= 'A' && *s <= 'Z'));
	}

bool IniPrs::isNumerous(char* s)
	{
		return (*s >= '0' && *s <= '9');
	}

char* IniPrs::getToken(char* s, std::string& token)
	{
		s = dropSpace(s);
		char* start = s;
		char c;
		while ((isNumerous(s) || *s == '_' || isAlpha(s)) && *s != '\0')
			s++;
		if (*s != '\0')
		{
			c = *s;
			*s = '\0';
			token.append(start);
			*s = c;
		}
		else
		{
			token.append(start);
		}
		return s;
	}

char* IniPrs::getString(char* s, std::string& str)
	{
		s = dropSpace(s);
		char* start = s;
		char c = *s;
		while (*s != '\n' && *s!=';' && *s!='\0')
		{
			if (*s == 0x0a)
			{
				str_counter++;
			}
			s++;
			while (*s == ' ')
			{
				s++;
			}
		}

		if (*s != c)
		{
			c = *s;
			*s = '\0';
			str.append(start);
			*s = c;
		}
		else
		{
			std::cout << "(" << str_counter << ")" << "'" << c << "' missing\n";
			exit(0);
		}
		if (*s != '\0')
		{
			s++;
		}
		return s;
	}

char* IniPrs::getGroupName(char* s, std::string& group)
	{
		s = matchSymbol(s, '[');
		s = getToken(s, group);
		s = matchSymbol(s, ']');
		return s;
	}

char* IniPrs::getNameStr(char* s, std::string& name_vl, std::string& str)
	{
		s = getToken(s, name_vl);
		s = matchSymbol(s, '=');
		s = getString(s, str);
		return s;
	}

void IniPrs::makeEntry(std::string name_vl, std::string str)
	{
		entr.first = name_vl;
		entr.second = str;
	}

void IniPrs::makeEntries()
	{
		entries.push_back(entr);
	}

void IniPrs::makeSection(std::string group)
	{
		sectn.first = group;
		sectn.second = entries;
	}

void IniPrs::makeGroup()
	{
		iniData.push_back(sectn);
		entries.clear();
	}

IniPrs::IniPrs(std::string flname) : str_counter(0)
	{
		buf_ = loadTextFile(flname);
		char* s;
		s = &buf_[0];
		std::string name_vl;
		std::string str;
		std::string group;

		while (*s != '\0')
		{
			s = dropSpace(s);
			if (*s == '[')
			{
				if (entries.size() != 0)
				{
					makeSection(group);
					makeGroup();
					group.clear();
				}
				s = getGroupName(s, group);
			}
			else if (isAlpha(s) || isNumerous(s))
			{
				s = getNameStr(s, name_vl, str);
				makeEntry(name_vl, str);
				name_vl.clear();
				str.clear();
				makeEntries();
			}
			else if (*s != '\0')
			{
				std::cout << str_counter + 1 << "Error: name or chapter expected\n";
			}
		}
		makeSection(group);
		makeGroup();
		group.clear();
	}

IniPrs::~IniPrs() = default;

std::string IniPrs::get_string_value(const std::string& name_section, const std::string& name_value)
	{
		std::string answ = "";
		for (auto it = iniData.begin(); it != iniData.end(); ++it)
		{
			if (it->first == name_section)
			{
				for (auto ts = it->second.begin(); ts != it->second.end(); ++ts)
				{
					if (ts->first == name_value)
					{
						answ = ts->second;
					}
				}
			}
		}
		if (!answ.empty())
		{
			return answ;
		}
		else
		{
			throw ValueException();
		}
	}

template<>
std::string IniPrs::get_value(const std::string& name_section, const std::string& name_value)
{
	
	return get_string_value(name_section, name_value);
	
}

template<>
int IniPrs::get_value(const std::string& name_section, const std::string& name_value)
{
	if (get_string_value(name_section, name_value) != "- 1")
	{
		std::string str_value = get_string_value(name_section, name_value);
		int answ = atoi(str_value.c_str());
		return answ;
	}
	else return -1;
}

template<>
double IniPrs::get_value(const std::string& name_section, const std::string& name_value)
{
	if (get_string_value(name_section, name_value) != "-1")
	{
		std::string str_value = get_string_value(name_section, name_value);
		double answ = std::stod(str_value.c_str());
		return answ;
	}
	else return -1;
}

const char* FileException :: what() const noexcept  
{
	return "Can't open this .ini file.\n";
}

const char* ValueException :: what() const noexcept  
{
	return "No such variables in this file.\n";
}
