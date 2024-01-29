#ifndef CoD_h
#define CoD_h 

#include<string>
#include <iostream>


	struct conDebug{
	
		std::string name;
		std::string sufix;
		std::string prefix;
		
		template<typename t>
		void operator()(t out){
			std::cout << sufix << " " << out << " " << prefix; 
			std::cout << '\n';
		}
		
		conDebug(std::string n = "DEBUG", std::string sfx = "", std::string pfx = "");
		
		~conDebug();
	};



#endif