#include <iostream>
#include "IniPrs.h"
#include <string>


int main()
{
	IniPrs parser("input.ini");
	std::cout << parser.get_value<std::string>("Section2", "var2");
	return 0;
}