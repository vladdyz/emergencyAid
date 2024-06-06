/* 

Vladislav Zolotukhin
Revision History
-----------------------------------------------------------
Date      Reason
2023/11/22  Preliminary release
2023/11/29  Version 1.0 update
2023/12/02  Final update
-----------------------------------------------------------

*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <string>
#include "Item.h"
#include "iProduct.h"
#include "Utils.h"

namespace sdds {

	bool Item::linear() const {
		return m_linear;
	}
	Item::Item() {
		m_price = 0;
		m_qty = 0;
		m_qtyNeeded = 0;
		m_sku = 0;
		m_description = nullptr;
		m_linear = false;
		m_state.clear();
	}
	Item::~Item() {
		delete[] m_description;
		m_description = nullptr;
	}
	Item::Item(const Item& src) {
		m_description = nullptr;
		*this = src;
	}
	Item& Item::operator=(const Item& src) {
		if (this != &src) {
			if (m_description != nullptr) {
				delete[] m_description;
			}
			//shallow copies
			m_price = src.m_price;
			m_qty = src.m_qty;
			m_qtyNeeded = src.m_qtyNeeded;
			m_sku = src.m_sku;
			m_linear = src.m_linear;
			m_state = src.m_state;
			//deep copies
			if (src.m_description != nullptr) {
				m_description = new char[strlen(src.m_description) + 1];
				strcpy(m_description, src.m_description);
			}
			else m_description = nullptr;
		}
		return *this;
	}
	int Item::qtyNeeded() const
	{
		return m_qtyNeeded;
	}

	int Item::qty() const
	{
		return m_qty;
	}

	Item::operator double() const
	{
		return m_price;
	}

	Item::operator bool() const
	{
		return m_state;
	}
	int Item::operator-=(int qty)
	{
		this->m_qty -= qty;
		return m_qty;
	}

	int Item::operator+=(int qty)
	{
		this->m_qty += qty;
		return m_qty;
	}
	void Item::linear(bool linear)
	{
		m_linear = linear;
	}
	void Item::clear() {
		m_state = nullptr;
	}
	bool Item::operator==(int sku)const {
		return m_sku == sku;
	}
	bool Item::operator==(const char* desc) const
	{
		return strstr(m_description, desc);
	}
	std::ofstream& Item::save(std::ofstream& ofstr) const {
		if (m_state)
		{
			ofstr << m_sku << '\t' << m_description << '\t' << m_qty << '\t' << m_qtyNeeded << '\t'/* << std::ios::fixed << std::setprecision(2)*/ << m_price; //<< std::endl;
			//Check if we're writing an item or a perishable
			//If its an item, add a newline at the end, else return to derived class save
			if (m_sku >= 40000 && m_sku <= 99999) ofstr << '\n';
		}
		return ofstr;
	}
	std::ifstream& Item::load(std::ifstream& ifstr) {
		char temp[900];
		ifstr >> m_sku;
		ifstr.ignore(5000, '\t');
		ifstr.getline(temp, 899, '\t');
		ut.alocpy(m_description, temp);
		ifstr >> m_qty;
		ifstr.ignore(5000, '\t');
		ifstr >> m_qtyNeeded;
		ifstr.ignore(5000, '\t');
		ifstr >> m_price;
		//check if its another tab (meaning a Perishable item) or the end of the record
		if (ifstr.peek() == '\n')
		{
			ifstr.get();
		}
		else ifstr.ignore(5000, '\t');
		if (ifstr.bad()) {
			m_state = "Input file stream read failed!";
		}
		return ifstr;
	}
	std::ostream& Item::display(std::ostream& ostr) const {
		//If state is bad, print state instead
		if (!m_state) {
			ostr << m_state;
		}
		else {
			//if in Linear format
			if (m_linear) {
				ostr.fill(' ');
				ostr.width(5);
				ostr << m_sku << " | ";
				int i = 0;
				//only print first 35 chars of description
				//adding a nullptr check
				if (m_description != nullptr)
				{
					for (i = 0; i < 35 && m_description[i] != '\0'; i++) {
						ostr << m_description[i];
					}
					//Fill any remaining space with whitespace
					while (i < 35) {
						ostr << " ";
						i++;
					}
				}
				ostr << " | ";
				ostr.setf(std::ios::right);
				ostr.width(4);
				ostr << m_qty << " | ";
				ostr.unsetf(std::ios::right);
				ostr.setf(std::ios::right);
				ostr.width(4);
				ostr << m_qtyNeeded << " | ";
				ostr.unsetf(std::ios::right);
				ostr.setf(std::ios::right);
				ostr.width(7);
				ostr << std::fixed << std::setprecision(2) << m_price << " |";
				ostr.unsetf(std::ios::right);
			}
			//If in descriptive format
			else {
				ostr << "AMA Item:" << std::endl;
				ostr << m_sku << ": " << m_description << std::endl;
				ostr << "Quantity Needed: " << m_qtyNeeded << std::endl;
				ostr << "Quantity Available: " << m_qty << std::endl;
				ostr << "Unit Price: $" << m_price << std::endl;
				ostr << "Needed Purchase Fund: $" << std::fixed << std::setprecision(2) << m_price * (m_qtyNeeded - m_qty) << std::endl;
			}
		}
	return ostr;
	}
	int Item::readSku(std::istream& istr) {
		m_sku = ut.getint(40000, 99999, "SKU: ");
		return m_sku;
	}
	std::istream& Item::read(std::istream& istr) {
		char temp[900];
		std::cout << "AMA Item:" << std::endl;
		std::cout << "SKU: " << m_sku << std::endl;
		std::cout << "Description: ";
		//Added peek before the getline because of the newline left in the buffer
		//Any leading whitespace caused the program to skip the input here
		if (istr.peek() == '\n')
		{
			istr.get();
		}
		std::cin.getline(temp, 899,'\n');
		istr.clear();
		ut.alocpy(m_description,temp);
		m_qtyNeeded = ut.getint(1, 9999, "Quantity Needed: ");
		m_qty = ut.getint(0, m_qtyNeeded, "Quantity On Hand: ");
		m_price = ut.getdouble(0.00, 9999.00, "Unit Price: $");
		if (!istr) {
			m_state = "Console entry failed!";
		}
		return istr;
	}
	const char* Item::readDesc() const {
		return m_description;
	}
	int Item::getSKU() const {
		return m_sku;
	}
}