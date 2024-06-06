/* 

Vladislav Zolotukhin
Revision History
-----------------------------------------------------------
Date      Reason
2023/11/15  Preliminary release
2023/11/29  Final update
-----------------------------------------------------------

*/

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <cstring>
#include <ctime>
#include "Date.h"
#include "Utils.h"


namespace sdds {

	bool Date::validate() {
		bool valid = true;
		int i = 0;
		while (valid == true && i == 0) {
			//validation should stop checking as soon as it fails
			if (year < 2023 || year > MAX_YEAR) {
				this->State = "Invalid year in date";
				this->State = 1;
				//std::cout << this->State.getName();
				valid = false;
			}
			else if (month < 1 || month > 12) {
				this->State = "Invalid month in date";
				this->State = 2;
				valid = false;
			}
			else {
				int maxDays = ut.daysOfMon(month, year);
				if (day < 1 || day > maxDays) {
					this->State = "Invalid day in date";
					this->State = 3;
					valid = false;
				}
			}
			i++;
		}
		if (valid == true) this->State.clear(); //ONLY CLEAR THE STATE IF IT IS VALID!!!!
		return valid;
	}
	
	int Date::uniqueDate() {
		return year * 372 + month * 31 + day;
	}

	Date::Date() {
		
		ut.testMode(true);
		ut.getSystemDate(&year, &month, &day);
		format = true;
	}
	Date::Date(int year, int month, int day) {
		this->year = year;
		this->month = month;
		this->day = day;
		format = true;
		validate();
	}
	//I originally had the parameters for these as int src which caused errors in main.cpp due to ambiguity
	//They require comparisons between Date objects.
	bool Date::operator==(Date& src) {
		return (uniqueDate() == src.uniqueDate());
	}
	bool Date::operator!=(Date& src) {
		return (uniqueDate() != src.uniqueDate());
	}
	bool Date::operator>=(Date& src) {
		return (uniqueDate() >= src.uniqueDate());
	}
	bool Date::operator<=(Date& src) {
		return (uniqueDate() <= src.uniqueDate());
	}
	bool Date::operator>(Date& src) {
		return (uniqueDate() > src.uniqueDate());
	}
	bool Date::operator<(Date& src) {
		return (uniqueDate() < src.uniqueDate());
	}

	const Status& Date::state() const {
		return State; 
	}

	Date& Date::formatted(bool format) {
		this->format = format;
		return *this;
	}

	Date::operator bool() const {
		return State;
	}

	std::ostream& Date::write(std::ostream& ostr) const {
		/* receives and returns a reference of the ostream object 
		in which it inserts the date value as follows:
		If the Formatted attribute is true, it will print the date in the following format:
		YYYY/MM/DD, month and day are printed in 2 spaces padded with zero.
		If the Formatted attribute is false it will print the date in the following format:
		YYMMDD, month and day are printed in 2 spaces padded with zero.*/
		if (this->format)
		{
			//both year & month are padded with 0s and occupy 2 spaces
			ostr << std::setw(4) << std::setfill('0') << year << "/" << std::setw(2) << month << "/" << std::setw(2) << day;// << std::endl;
		}
		else {
			//to print last 2 digits of year, use remainder(%)
			ostr << std::setw(2) << std::setfill('0') << year % 100  << std::setw(2) << month << std::setw(2) << day;// << std::endl;
		}
		return ostr;
		
	}

	std::istream& Date::read(std::istream& istr) {
		/* extracts the date from input stream
		The date is entered as one integer value.
		If the value is a four digits integer then the Date is read as: 
		MMDD and the year is set to the current system year.
		If the value is a six digits integer then the date is read as: YYMMDD.
		Then the data is validated and if the validation fails the istream object 
		is set to a fail state. istream::setstate(ios::badbit);
		*/

		//first validate that istr can actually be assigned to an integer
		int tempDate = 0;
		if (istr >> tempDate)
		{
			if (tempDate > 999 && tempDate < 10000) { //if its 4 digits
			//MMDD, year set to current year
				ut.getSystemDate(&year, &month, &day); //initially calculated this myself with this->year = curTime->tm_year + 1900;
				this->month = tempDate / 100; //ex: 1231 / 100 will floor the result to 12 (December)
				this->day = tempDate % 100; //same as to display the year in 2 digits in "write" function
			}
			else if (tempDate > 99999 && tempDate < 1000000) { //if its 6 digits
			//YYMMDD
				this->year = 2000 + tempDate / 10000; //divide by 5 digits ex: 231231 / 10000 = floored to 23
				this->month = (tempDate / 100) % 100;  //really tricky, 231231 / 100 = 2312, 
				//to remove the leading digits get the remainder of % 100 like we would to get the day
				this->day = tempDate % 100;
				
			}
			else if (tempDate < 99 && tempDate > 0)
			{
				//If input is only 2 digits, no month has been entered therefore the Date is invalid
				month = 0;
				day = 0;
			}
			else {
				//if the istream isn't 2, 4 or 6 digits or not integers, do not do anything
				this->State = "Invalid date value";
				this->State = 0;
			}
			if (validate() == false) {
				istr.setstate(std::ios::badbit);
			}
		}
		else {
			this->State = "Invalid date value";
			this->State = 0;
		}
		return istr;
	}
	
	std::ostream& operator<<(std::ostream& ostr, const Date& date) {
		return date.write(ostr);
	}
	std::istream& operator>>(std::istream& istr, Date& date) {
		return date.read(istr);
	}

	int Date::readYear() const {
		return year;
	}
	int Date::readMonth() const {
		return month;
	}
	int Date::readDay() const {
		return day;
	}


}
