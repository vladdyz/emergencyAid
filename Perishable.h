/* 

Vladislav Zolotukhin
Revision History
-----------------------------------------------------------
Date      Reason
2023/11/29  Initial release
-----------------------------------------------------------

*/

#ifndef SDDS_PERISHABLE_H
#define SDDS_PERISHABLE_H
#include "Item.h"
#include "Date.h"
namespace sdds {
	class Perishable : public Item {
		Date exp;
		char* handling; 
	public:
		Perishable();
		~Perishable();
		Perishable(const Perishable&);
		Perishable& operator=(const Perishable&);
		const Date& expiry() const;
		int readSku(std::istream& istr);
		std::ofstream& save(std::ofstream& ofstr) const;
		virtual std::ifstream& load(std::ifstream& ifstr);
		std::ostream& display(std::ostream& ostr) const;
		std::istream& read(std::istream& istr);
		operator bool() const;
	};


}
#endif //SDDS_PERISHABLE_H



