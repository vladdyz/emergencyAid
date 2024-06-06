/* 

Vladislav Zolotukhin
Revision History
-----------------------------------------------------------
Date      Reason
2023/11/22  Preliminary release
2023/11/29  Final update
-----------------------------------------------------------

*/

#ifndef SDDS_ITEM_H
#define SDDS_ITEM_H
#include "iProduct.h"
#include "Status.h"
namespace sdds {
	class Item : public iProduct {
		double m_price;
		int m_qty;
		int m_qtyNeeded;
		char* m_description;
		bool m_linear;
	protected:
		Status m_state;
		int m_sku;
		bool linear() const; //return linear flag of item
	public:
		Item();
		virtual ~Item();
		//copy constructor & copy assingment operator
		Item(const Item&); 
		Item& operator=(const Item&);
		int qtyNeeded()const; //virtual iProduct overrides returning Item attributes
		int qty()const;
		operator double()const; 
		virtual operator bool() const;
		int operator-=(int); //reduce or increase qty
		int operator+=(int);
		void linear(bool); //set linear flag true or false
		void clear(); //reset state
		bool operator==(int)const; //iProduct overrides, check for match in sku/description
		bool operator==(const char*) const;
		virtual std::ofstream& save(std::ofstream& ofstr) const; //write info if state is good
		virtual std::ifstream& load(std::ifstream& ifstr); //read info into corresponding attributes
		virtual std::ostream& display(std::ostream& ostr) const;
		virtual int readSku(std::istream& istr); //reads and validates the sku
		virtual std::istream& read(std::istream& istr); //reads and validates all the other variables
		const char* readDesc() const;
		int getSKU() const; //final addition, returns the sku number
	};


}
#endif