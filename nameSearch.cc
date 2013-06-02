

#include "nameSearch.h"
#include <string.h>
#include <iostream>

std::fstream nameSearch::text_filePtr;
std::map<std::string,bool>nameSearch::list_LastName;
std::map<std::string,bool> nameSearch::list_FirstName;
uint32_t nameSearch::current_offset;
std::vector<uint32_t> nameSearch::listOffsets;
std::string nameSearch::token;
bool nameSearch::BOM_in=false;
char nameSearch::bom_bytes;


// Transitions table
nameSearch::machineElement nameSearch::transitions_table[2][3] = {
    {{nameSearch::STATE_0, nameSearch::read_next_token}, {nameSearch::STATE_0,  nameSearch::store_currentOffset},	{nameSearch::STATE_1,  nameSearch::store_currentOffset}},
    {{nameSearch::STATE_0, nameSearch::read_next_token}, {nameSearch::STATE_0,  nameSearch::read_next_token}, {nameSearch::STATE_1,  nameSearch::read_next_token}}
};

//Set initial state
nameSearch::state currentState=nameSearch::STATE_0;

nameSearch::nameSearch(const char* text_filename,const char* male_filename, const char* female_filename, const char* lastname_filename)
{
	
	
    if(loadNames(male_filename))
	throw "male_firstnames.dat";
    	
    if(loadNames(female_filename))
	throw "female_firstnames.dat";
    
    if(loadNames(lastname_filename))
	throw "lastnames.dat";
    
	
    
    text_filePtr.open(text_filename,std::ifstream::in);
   

    if ( (text_filePtr.rdstate() & std::ifstream::badbit ) != 0 ||  (text_filePtr.rdstate() & std::ifstream::failbit ) !=0 )
	throw  text_filename;
    
    

    current_offset=0;

    run_machine();
	
	
}

nameSearch::~nameSearch(){
    text_filePtr.close();
}


void nameSearch::run_machine(){
		
    read_next_token();
	
    while(!text_filePtr.eof()){
		
	if(isFirstName(token)){
	    evalEvent(FIRST_NAME_FOUND);

	}else if(isLastName(token)){
	    evalEvent(LAST_NAME_FOUND);
	}
	else{
	    evalEvent(ANY);
			
	}
		
		
    }

	
    //token that caused the termination, 
    if(isFirstName(token)){
	evalEvent(FIRST_NAME_FOUND);
		
    }else if(isLastName(token)){
	evalEvent(LAST_NAME_FOUND);
		
    }
    else{
	evalEvent(ANY);
		
    }	
		
	
	
}

/********** Actions *******/
void
nameSearch::read_next_token(void)
{
	
    if(current_offset==0){
	//Skipping BOM with a heuristic
	char a,b,c,d;
	a=text_filePtr.get();
	b=text_filePtr.get();
	c=text_filePtr.get();
	d=text_filePtr.get();
		
		
	if(a==(char)0xEF && b==(char)0xBB && c==(char)0xBF) {/* UTF-8 */
	    BOM_in=true;
	    bom_bytes=3;
	    token.erase(0, 3);
	}
	else if(a==(char)0xFF &&  b==(char)0xFE){/* UTF-16 little-endian */	
	    BOM_in=true;
	    bom_bytes=4;
	    token.erase(0, 4);
	}
	else if(a==(char)0xFE &&  b==(char)0xFF){/* UTF-16 big-endian */	
	    BOM_in=true;
	    bom_bytes=4;
	    token.erase(0, 4);
	}
	else if(a==(char)0xFF &&  b==(char)0xFE && c==(char)0x00 && d==(char)0x00){/* UTF-32 little-endian */	
	    BOM_in=true;
	    bom_bytes=16;
	    token.erase(0, 16);
	}
	else if(a==(char)0x00 &&  b==(char)0x00 && c==(char)0xFE && d==(char)0xFF){/* UTF-32 big-endian */	
	    BOM_in=true;
	    bom_bytes=16;
	    token.erase(0, 16);
	}
	else {//no BOM 
	    bom_bytes=0;
	    text_filePtr.seekg(0);
	}

    }
			

    text_filePtr >> token;
    //current_offset	= text_filePtr.tellg() >= 0 ? (uint32_t) text_filePtr.tellg() : current_offset;
	
    current_offset	= BOM_in ? (uint32_t) text_filePtr.tellg() - bom_bytes : (uint32_t) text_filePtr.tellg() ;
	


    //skipping eventual punctuations at the beginning of the token
    while(ispunct(token[0]))
	token.erase(0, 1);
	
    current_offset -= token.size();
	
    //skipping eventual punctuations at the end of the token
    while(ispunct(token[token.size()-1]))
	token.erase(token.size()-1, 1);
	
	
	
    std::transform(token.begin(), token.end(), token.begin(), ::tolower);
    
}

void
nameSearch::store_currentOffset(void)
{
    push_offset(current_offset);
}
/*******************/


/** Utils functions **/

bool
nameSearch::isFirstName(std::string name){
    return list_FirstName.find(name) != list_FirstName.end();
}

bool 
nameSearch::isLastName(std::string name){
    return list_LastName.find(name) != list_LastName.end();
}



void
nameSearch::push_offset(uint32_t offset){
    listOffsets.push_back(offset);
}

void
nameSearch::push_FirstName(std::string name){
    list_FirstName.insert(std::pair<std::string,bool>(name,true));
}

void
nameSearch::push_LastName(std::string name){
    list_LastName.insert(std::pair<std::string,bool>(name,true));
}

/*
  Triggers the action corresponding to the pair (currentState, incoming event E) and goes to the next state
*/
void
nameSearch::evalEvent(event E)
{
	
    state nextState;
    transition_action action;
	
    switch(E){
    case FIRST_NAME_FOUND:
	nextState= transitions_table[currentState][FIRST_NAME_FOUND].nextState;
	action= transitions_table[currentState][FIRST_NAME_FOUND].actionToTake;
	break;
    case LAST_NAME_FOUND:
	nextState= transitions_table[currentState][LAST_NAME_FOUND].nextState;
	action= transitions_table[currentState][LAST_NAME_FOUND].actionToTake;
	break;
    default:
	nextState= transitions_table[currentState][ANY].nextState;
	action= transitions_table[currentState][ANY].actionToTake;
		
    }

    	
    currentState=nextState;
    (*action)();
	
}


int 
nameSearch::loadNames(const char* namesFile)
{

    name_filePtr.open(namesFile, std::ifstream::in);
	
    if(!name_filePtr.is_open())
	return 1;
	
    std::string name;
	
    while(!name_filePtr.eof()){
	name_filePtr >> name;
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);
	push_FirstName(name);
		
    }
    name_filePtr.close();
    return 0;
    
}


std::vector<uint32_t> nameSearch::getOffsets(){ return listOffsets; }

