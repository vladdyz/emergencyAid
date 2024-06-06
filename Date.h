/* 

Vladislav Zolotukhin
Revision History
-----------------------------------------------------------
Date      Reason
2023/11/15  Preliminary release
2023/11/27  Final update
-----------------------------------------------------------

*/

#ifndef SDDS_DATE_H_
#define SDDS_DATE_H_

#include "Status.h"

namespace sdds {

#define MAX_YEAR 2030

	class Date {
		int year; //must be between current year and 2030
		int month; //must be between 1-12
		int day; //must be valid depending on month
		Status State;
		bool format; //if true prints YYYY/MM/DD, false prints YYMMDD
		bool validate(); //validates year/month/day
		int uniqueDate(); //returns year * 372 + mon * 31 + day
	public:
		Date();
		Date(int year, int month, int day);
		bool operator==(Date&);
		bool operator!=(Date&);
		bool operator>=(Date&);
		bool operator<=(Date&);
		bool operator>(Date&);
		bool operator<(Date&);
		const Status& state() const; //return a constant reference to state (a query)
		Date& formatted(bool); //sets the format
		operator bool() const; //If the Date object is casted to a boolean, the state of the date object is returned.
		std::ostream& write(std::ostream& ostr) const;
		std::istream& read(std::istream& istr);
		//query functions
		int readYear() const;
		int readMonth() const;
		int readDay() const;

	};

	//Helpers
	std::ostream& operator<<(std::ostream& ostr, const Date& date);
	std::istream& operator>>(std::istream& istr, Date& date);


}

#endif //SDDS_DATE_H_