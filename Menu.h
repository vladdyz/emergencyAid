/* 

Vladislav Zolotukhin
Revision History
-----------------------------------------------------------
Date      Reason
2023/11/15  Preliminary release
2023/12/02  Final Update
-----------------------------------------------------------

*/

#define MAXOPTIONS 15


#ifndef SDDS_MENU_H
#define SDDS_MENU_H
namespace sdds {
	class Menu {
		char* menuContent;
		int numOpt;
	public:
		Menu();
		Menu(const char* content);
		~Menu();
		unsigned int run() const; //return int must be unsigned
		char* Content() const; //query function to return the menuContent

	};
}
#endif //SDDS_MENU_H