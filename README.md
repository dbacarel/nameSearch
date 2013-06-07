nameSearch
===========
Parse and return names offset within a text document

 
 >USAGE:
 
./namesSearch [text_file] [male_filename] [female_filename] [lastname_filename]
 
 
  > Grammar:
 
  NAME = FIRST_NAME | SURNAME | FIRST_NAME SPACE SURNAME
  
  FIRST_NAME = MALE_NAME | FEMALE_NAME
  
  MALE_NAME = name E List of male first names
  
  FEMALE_NAME = name E List of female first names
  
  SURNAME = surname E List of surnames

  SPACE = [ ]
 
 
  > Data Structures
 
  The main operation in this task is the names lookup.
  It requires an efficient data structures providing a fast lookup.
  Nonetheless I used here a std::map with an average insert/lookup complexity of O(logN)
 
 
  > Implementantion
 
  Implementation of a  finite states machine (Mealy) that parses the entire text with
  the attempt to match the rule NAME.
 
  (STATE_id, Event)-> NEXT_STATE
 
  - States:
 
  STATE_0: a lastname OR uninteresting token found
  STATE_1: a firstname found
 
  - Transitions:
	
  (STATE_0, LastName_found)	->	ACTION: store offset and stay in STATE_0  // e.g. "Mr. Smith"

  (STATE_0, Any_found)	->		 ACTION: read next token and stay in STATE_0 

  (STATE_0, FirstName_found)	->	ACTION: store offset and go to STATE_1 
// e.g. "Adam"+followed by middle names, lastname or any other token
 
  (STATE_1, FirstName_found)	->	ACTION: read next token and stay in STATE_1 
// e.g. "Michelle Pamela"

  (STATE_1, LastName_found)	->	ACTION: read next token and go to STATE_0 
// e.g. "Adam Smith" or "Michelle Pamela Smith" (FirstName)+ Lastname
 
 (STATE_1, Any_found)	->	ACTION: read next token and go to STATE_0
 // e.g. "Michelle and .."
 
 

 
