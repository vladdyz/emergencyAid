/* 

Vladislav Zolotukhin
Revision History
-----------------------------------------------------------
Date      Reason
2023/11/22  Preliminary release
2023/11/27  Final update
-----------------------------------------------------------

*/
#ifndef SDDS_IPRODUCT_H
#define SDDS_IPRODUCT_H
namespace sdds {
	class iProduct {
	public:
		virtual ~iProduct() {};
		virtual std::ostream& display(std::ostream&)const = 0;
		virtual int readSku(std::istream& istr) = 0;
		virtual int operator-=(int) = 0;
		virtual int operator+=(int) = 0;
		virtual operator double()const = 0;
		virtual operator bool()const = 0;
		virtual int qtyNeeded()const = 0;
		virtual int qty()const = 0;
		virtual void linear(bool isLinear) = 0;
		virtual std::ofstream& save(std::ofstream&) const = 0;
		virtual std::ifstream& load(std::ifstream&) = 0;
		virtual std::istream& read(std::istream&) = 0;
		virtual bool operator==(int)const = 0;
		virtual bool operator==(const char*) const = 0;
		virtual int getSKU() const = 0; //final addition, returns the sku number
	};
	std::ostream& operator<<(std::ostream& os, const iProduct& product);
	std::istream& operator>>(std::istream& is, iProduct& product);
}
#endif