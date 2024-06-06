/* 

Vladislav Zolotukhin
Revision History
-----------------------------------------------------------
Date      Reason
2023/11/15  Preliminary release
2023/11/20  Update for version 1.0
2023/11/27  Final update
-----------------------------------------------------------

*/
#ifndef SDDS_STATUS_H_
#define SDDS_STATUS_H_
#include <iostream>

//I had a circular dependancy here by including "Date.h", since Date.h also includes Status.h
//This caused quite a lot of errors relating to using the Status class inside of Date
//objects, such as "c++ unknown override specifier". Fortunately it was an easy fix, I simply forgot
//that I had the line here.

namespace sdds {

		class Status {
			char* name;
			int code;
			friend class Date; //I tried inheritance (class Status : public Date) but it did not seem appropriate
		public:
			Status();
			Status(char* name);
			~Status();
			Status(const Status&); //copy constructor
			Status& operator=(const Status&); //copy assignment operator
			int getCode() const;
			const char* getName() const;

			//Assignment operators
			Status& operator=(int);
			Status& operator=(const char*);


			//Type conversion overloads
			operator int() const; //returns code
			operator const char* () const; //returns name
			operator bool() const; //returns true if object uninitialized (name = null)

			Status& clear();
			//helper insertion operator overload
			//code borrowed and modified from my HealthCard.cpp module (specifically, the 'print' function)
			friend std::ostream& operator<<(std::ostream& ostr, const Status& status);
			//I had to declare this as a friend because of ambiguity conflicts with 
			//std::ostream& operator<<(std::ostream& ostr, const Date& date)
			//in Date.h, as lines such as "cout << D.state() << endl;" would be unclear to which << overload
			//should be called.
		};
}

#endif // SDDS_STATUS_H_