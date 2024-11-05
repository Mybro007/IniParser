#include <iostream>
#include "IniPrs.h"
#include <string>


int main()
{
	try 
	{
		IniPrs parser("input.ini");
		std::cout << parser.get_value<int>("Section1", "var1");
	}
	catch (const FileException& ex) 
	{
		std::cout << ex.what();
		return -1;
	}
	catch (const ValueException& ex)
	{
		std::cout << ex.what();
		return -1;
	}
	catch (const SymbolException& ex)
	{
		std::cout << ex.what();
		return -1;
	}
	return 0;
}