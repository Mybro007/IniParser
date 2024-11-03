#include <iostream>
#include "IniPrs.h"
#include <string>


int main()
{
	IniPrs parser("inut.ini");
	std::cout << parser.get_value<double>("Section1", "var1");
	return 0;
}