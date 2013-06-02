

#include <string>
#include <fstream>
#include <vector>
#include <map>


class nameSearch
{



 public:
    nameSearch();
    nameSearch(const char* text_filename,const char* male_name_filename, const char* female_name_filename, const char* lastname_filename);
    ~nameSearch();
	
    typedef void (*transition_action)(void);

    typedef enum 
    {
	STATE_0,
	STATE_1
    } state;

    typedef enum
    {

	ANY,
	LAST_NAME_FOUND,
	FIRST_NAME_FOUND
    } event;

           
    typedef struct{
	state nextState;
	transition_action actionToTake;
    } machineElement ;



    static std::vector<uint32_t> getOffsets();
    

    





 private: 
    static	void read_next_token(void);
    static  void store_currentOffset(void);
    static  void run_machine();
	
    static bool isFirstName(std::string name);
    static bool isLastName(std::string name);
	
    static void push_offset(uint32_t offset);
    static void evalEvent(event E);
	
	
    static std::map<std::string,bool> list_LastName;
    static std::map<std::string,bool> list_FirstName;
    static std::vector<uint32_t> listOffsets;
	
    static std::fstream text_filePtr;
    static uint32_t current_offset;
    static state current_state;
    static std::string token;
	
    static machineElement transitions_table[2][3];

	
   
	
    void push_FirstName(std::string name);
    void push_LastName(std::string name);
    int loadNames(const char* namesFile);

	

    std::fstream name_filePtr;
	
    /*
     * http://www.unicode.org/unicode/faq/utf_bom.html
     * 00 00 FE FF  = UTF-32, big-endian
     * FF FE 00 00  = UTF-32, little-endian
     * FE FF        = UTF-16, big-endian
     * FF FE        = UTF-16, little-endian
     * EF BB BF     = UTF-8
     *
     */

    //Flag for byte of marks
    static bool BOM_in;
    static char bom_bytes;
	 
    


};





 


     


    
