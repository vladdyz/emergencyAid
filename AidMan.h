/* 

Vladislav Zolotukhin
Revision History
-----------------------------------------------------------
Date      Reason
2023/11/15  Preliminary release
2023/12/03  Final update
-----------------------------------------------------------

*/
#include "Menu.h" 
#include "iProduct.h"

#ifndef SDDS_AIDMAN_H
#define SDDS_AIDMAN_H
namespace sdds {
	const int sdds_max_num_items = 100;

	class AidMan {
		char* fName;
		Menu m;
		unsigned int menu();
		iProduct* ip[sdds_max_num_items];
		int iPnum;
	public:
		AidMan();
		AidMan(const char* filename);
		~AidMan();
		void run();
		void printMenuItem(int); 
		void save() const;
		void deallocate();
		void load();
		int list(const char*);
		int search(int sku) const;
		void addItem();
		void remove(int index = -1);
		void qtyUpdate();
		void sort();
		bool sortHelper(iProduct*, iProduct*); 
		void ship(const char*);
	};
	

}
#endif //SDDS_AIDMAN_H
