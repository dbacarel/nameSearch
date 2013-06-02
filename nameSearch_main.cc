
#include "nameSearch.h"
#include <iostream>
#include <stdexcept>

int
main(int argc,char** argv)
{

    if(argc < 5)
	{
	    std::cout << "Usage: " << argv[0] <<  "<text_file> <maleNames_file> <femaleNames_file> <lastname_file>" << std::endl;
	    return 1;
	    
	}
    
        
    try	
    {	
	nameSearch nS(argv[1],argv[2],argv[3],argv[4]);
	std::vector<uint32_t> off=nS.getOffsets();

	for(size_t i=0;i<off.size();i++)
	    std::cout << off[i] << " ";
    }
    
    catch(const char * E)
    {
	std::cerr << "Cannot open '"<< E << "'" << std::endl;
	return 1;
    }
    
    
  
    
				       
    return 0;
    
}

