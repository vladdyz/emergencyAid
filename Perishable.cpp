/* 

Vladislav Zolotukhin
Revision History
-----------------------------------------------------------
Date      Reason
2023/11/29  Preliminary release
2023/12/02  Final update
-----------------------------------------------------------

*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <fstream>
#include <iomanip>
#include "Item.h"
#include "Perishable.h"
#include "Utils.h"
namespace sdds {
	Perishable::Perishable() {
		handling = nullptr;
	}
	Perishable::~Perishable() {
		if (handling != nullptr) {
			delete[] handling;
			handling = nullptr;
		}
	}
	Perishable::Perishable(const Perishable& src) : Item(src) {
		//copy the handling description
		handling = nullptr; //nullptr saves lives
		if (src.handling != nullptr) {
			ut.alocpy(handling, src.handling);
		}
		else handling = nullptr;
		//copy the date
		exp = src.exp;

	}
	Perishable& Perishable::operator=(const Perishable& src) {
		if (this != &src) {
			Item::operator=(src); //copy assignment operator call of base class
			delete[] handling;
			handling = nullptr; //forgot to do this, destroyed the heap & was stuck for HOURS 
			if (src.handling != nullptr) {
				ut.alocpy(handling, src.handling);
			}
			//copy the date object
			exp = src.exp;

		}
		return *this;
	}

	const Date& Perishable::expiry() const {
		return exp;
	}


	int Perishable::readSku(std::istream& istr) {
		m_sku = ut.getint(10000, 39999, "SKU: ");
		return m_sku;
	}

	std::ofstream& Perishable::save(std::ofstream& ofstr) const {
		if (m_state)
		{
			//if state is good, first call the base save function
			Item::save(ofstr);
			ofstr << '\t';
			//only print handling instructions if they exist
			if (handling != nullptr && handling[0] != '\0') {
				ofstr << handling << '\t';
			}
			else ofstr << '\t'; //if no handling, print two tabs!! 
			//If there isn't two tabs printed, the lines will not be read!!
			//This is why it wouldn't read the last two records entered into the file during program execution (the two Advils) 
			
			//writes an unformatted copy of the expiry Date
			ofstr << std::setw(2) << std::setfill('0') << exp.readYear() % 100 << std::setw(2) << exp.readMonth() << std::setw(2) << exp.readDay() << '\n';
		}
		return ofstr;
	}

	std::ifstream& Perishable::load(std::ifstream& ifstr) {
		//call load of the base (Item) class
		Item::load(ifstr);
		char temp[1000];
		ifstr.getline(temp, 999, '\t');
		ut.alocpy(handling, temp);
		//testing
		//std::cout << this->handling << std::endl;
		//std::cout << readDesc();
		if (ifstr.peek() == '\t')
		{
			ifstr.get();
		}
		exp.read(ifstr);
		ifstr.ignore(5000, '\n');
		if (ifstr.peek() == '\n')
		{
			ifstr.get();
		}
		if (ifstr.bad()) {
			m_state = "Input file stream read failed!";
		}
		return ifstr;
	}

	std::ostream& Perishable::display(std::ostream& ostr) const {
		//If state is bad, print state instead
		if (!m_state) {
			ostr << m_state;
		
		}
		else {
			//if in Linear format
			if (linear()) {
				ostr.fill(' '); //remove padding of leading zeroes
				Item::display(ostr); //call base class display first
				//if handling instructions not null/empty print asterisk, otherwise print space
				if (handling != nullptr && handling[0] != '\0') {
					ostr << "*";
				}
				else ostr << " ";
				ostr << exp;
			}
			//else if descriptive
			else {
				ostr << "Perishable ";
				Item::display(ostr); //display base class
				ostr << "Expiry date: ";
				ostr << exp;

				//if handling is not null/empty, print the instructions
				if (handling != nullptr && handling[0] != '\0') {
					ostr << std::endl;
					ostr << "Handling Instructions: " << handling << std::endl;
				}
				else ostr << std::endl;
			}
		}
		return ostr;
	}

	std::istream& Perishable::read(std::istream& istr) {
		Item::read(istr); //call base class read
		char temp[1000];
		if (handling != nullptr) {
			delete[] handling;
			handling = nullptr;
		}
		std::cout << "Expiry date (YYMMDD): ";
		exp.read(istr);
		if (istr.peek() == '\n')
		{
			istr.get();
		}
		std::cout << "Handling Instructions, ENTER to skip: ";
		//peeks and if the very first character is not '\n' it will read the instructions dynamically 
		//into the instructions attribute. otherwise, nothing is read and the attribute remains null.
		if (istr.peek() == '\n')
		{
			istr.get();
		}
		else {
			std::cin.getline(temp, 999, '\n');
			istr.clear();
			ut.alocpy(handling, temp); //copy instructions into handling
		}
		//if the istream object is in a fail state
		if (!istr) {
			m_state = "Perishable console date entry failed!";
		}
		return istr;
	}

	Perishable::operator bool() const {
		return readDesc() != nullptr;
		//The main function was printing empty Perishable objects 
		/*
		if (p) { cout... }
		resulting in:
		linear: ------------------------------------------------------------------------
		0 |  |    0 |    0 |    0.00 | 2023/12/09
		--------------------------------------------------------------------------------
		Had to make a function to recognize that empty objects should be skipped

		*/
	}

}