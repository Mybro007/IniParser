#include <iostream>
#include "IniPrs.h"
#include <string>


int main()
{
	IniPrs parser("input.ini");
	std::cout << parser.get_value<int>("Section1", "var1");
	return 0;
}