/*

Vladislav Zolotukhin
Revision History
-----------------------------------------------------------
Date      Reason
2023/11/22  Preliminary release
2023/11/27  Final update
-----------------------------------------------------------

*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include "iProduct.h"

namespace sdds {

		std::ostream& operator<<(std::ostream& ostr, const iProduct& product)
		{
			return product.display(ostr);
		}
		std::istream& operator>>(std::istream& istr, iProduct& product)
		{
			return product.read(istr);
		}
		bool iProduct::operator==(int sku) const
		{
			return false;
		}

		bool iProduct::operator==(const char* description) const
		{
			return false;
		}

}