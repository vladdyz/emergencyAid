/* 

Vladislav Zolotukhin
Revision History
-----------------------------------------------------------
Date      Reason
2023/11/15  Preliminary release
2023/12/03  Updated for Final Project
-----------------------------------------------------------

*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <cstring>
#include <string>
#include <fstream>
#include "AidMan.h"
#include "Date.h"
#include "Item.h"
#include "Perishable.h"
#include "Menu.h"
#include "Utils.h"

namespace sdds {

	Perishable temp;
	Item temp2;
	
	unsigned int AidMan::menu() {
		Date currentDate;
		std::cout << "Aid Management System" << std::endl;
		std::cout << "Date: " << currentDate << std::endl;
		std::cout << "Data file: ";
		if (fName != nullptr)
			std::cout << fName << std::endl;
		else std::cout << "No file" << std::endl;
		std::cout << "---------------------------------" << std::endl;
		//std::cout << std::endl;
		return m.run();
	}

	AidMan::AidMan() : m("List Items\tAdd Item\tRemove Item\tUpdate Quantity\tSort\tShip Items\tNew/Open Aid Database") {
		fName = nullptr;
		iPnum = 0;
		for (int i = 0; i < sdds_max_num_items; ++i) {
			ip[i] = nullptr; 
		}
		

	}

	AidMan::AidMan(const char* filename) : m ("List Items\tAdd Item\tRemove Item\tUpdate Quantity\tSort\tShip Items\tNew/Open Aid Database") {
		fName = nullptr;
		iPnum = 0;
		if (filename != nullptr && filename[0] != '\0') {
			fName = new char[strlen(filename) + 1];
			strcpy(fName, filename);
		}
	}

	AidMan::~AidMan() {
		if (fName != nullptr) {
			delete[] fName;
			fName = nullptr;
		}
		for (int i = 0; i < iPnum; i++) {
			delete ip[i];
		}
	}

	void AidMan::run() {
		int menuSelect = 7;
		do
		{
			menuSelect = this->menu();
			if (menuSelect != 0 && menuSelect != 7 && !fName) menuSelect = 7;
			if (menuSelect != 0) {
				//Had to create a couple custom functions (printMenuItem and Content query)
				//in order to print out the selected menu item substring from the menuContent string
				std::cout << "\n****";
				printMenuItem(menuSelect);
				std::cout << "****" << std::endl;
				if (menuSelect == 7) load();
				if (menuSelect == 1) list(nullptr);
				if (menuSelect == 2) addItem(); 
				if (menuSelect == 3) remove();
				if (menuSelect == 4) qtyUpdate();
				if (menuSelect == 5) sort();
				if (menuSelect == 6) ship("shippingOrder.txt");


			}
		} while (menuSelect != 0);
		std::cout << "Exiting Program!" << std::endl;
		save();
		
	}

	void AidMan::printMenuItem(int menuSelect) {
		//Idea for this function taken from SFT workshops
		//Essentially, uses the query function to get the menuContent attribute from the Menu class
		//Then using the unsigned integer return from the menu() function call, identify which "line"
		//the client has selected from the menu, and print only that part of the whole menuContent string
		//use tabs as delimiting characters and counters for the iteration, when the # of tabs matches
		//the parameter integer, start printing characters from there until reaching either the next tab
		//or null terminator 
		char* selectedLine = nullptr; //holds the string for the name of the option selected from the menu
		bool start = false; //identifies the starting point in the menuContent where the word should be printed from
		int tabCounter = 1; //must match the selected line integer (menuSelect)
		char* menuString = m.Content();

		for (int i = 0; menuString[i] != '\0'; i++)
		{
			if (tabCounter == menuSelect && !start) {
				start = true;
				selectedLine = &menuString[i];
			}
			if (start && (menuString[i] == '\t' || menuString[i + 1] == '\0')) {
				start = false;
				if (menuString[i + 1] == '\0') {
					for (; selectedLine <= &menuString[i]; ++selectedLine) {
						std::cout << *selectedLine; 
					}
				}
				else {
					for (; selectedLine < &menuString[i]; ++selectedLine) {
						std::cout << *selectedLine;
					}
				}
				break;
			}
			if (menuString[i] == '\t')
				tabCounter++; //keep track of the tabs
		}
	}
	void AidMan::save() const {
		std::ofstream file(fName, std::ios::trunc); //overwrite the contents, not add
		file.clear();
		if (file && iPnum > 0) {
			for (int i = 0; i < iPnum; i++)
			{

				ip[i]->save(file);
			}
		}	
		file.close();
	}

	void AidMan::deallocate() {
		for (int i = 0; i < iPnum; i++) {
			delete[] ip[i];
		}
		iPnum = 0;
	}

	void AidMan::load() {
		int choice = 0, flag = 0;
		char sku;
		std::string filename;
		//save existing iProducts
		if (fName != nullptr) save();
		//deallocate resources
		if (fName != nullptr) {
			delete[] fName;
			fName = nullptr;
		}
		if (iPnum > 0) {
			for (int i = 0; i < iPnum; i++) {
				delete[] ip[i]; 
			}
		}
		iPnum = 0;
		std::cout << "Enter file name: ";
		std::cin >> filename; //using a string instead of a char array
		//dynamically create fName
		fName = new char[filename.length() + 1];
		std::strcpy(const_cast<char*>(fName), filename.c_str());
		//std::cout << "check"; //testing
		std::ifstream file(fName, std::ios::app);
			if (file.is_open()) {
				while (iPnum < sdds_max_num_items && !file.eof() && flag == 0) {
					//peek at the first char of the record
					//1-3: Perishable object
					//4-9: Item object
					//Anything else: Invalid istr
					file.clear();
						sku = file.peek();
						if (sku >= '1' && sku <= '3') {
							ip[iPnum] = new Perishable;
							if (ip[iPnum]->load(file)) {
								iPnum++;
							}
							else {
								delete ip[iPnum];
								//flag = 1;
							}
						}
						else if (sku >= '4' && sku <= '9') {
							ip[iPnum] = new Item;
							if (ip[iPnum]->load(file)) {
								iPnum++;
							}
							else {
								delete ip[iPnum];
								//flag = 1;
							}
						}
						else {
							//Debugging
							//std::cout << sku << std::endl;
							file.setstate(std::ios::failbit);
							flag = 1;
						}
					//}
				}
				
				std::cout << iPnum << " records loaded!\n" << std::endl;
				//For debugging purposes

				/*
				for (int i = 0; i < iPnum; i++) {
					ip[i]->display(std::cout);
					std::cout << std::endl;
				}
				*/
				file.close();
			}
			else {
				std::cout << "Failed to open" << fName << "for reading!" << std::endl;
				std::cout << "Would you like to create a new data file?" << std::endl;
				std::cout << "1- Yes!" << std::endl;
				std::cout << "0- Exit" << std::endl;
				choice = ut.getint(0, 1, "> ", nullptr);
				//if User selects 1, create a new file with the fName name
				if (choice == 1) {
					std::ofstream newFile(fName); 
				}

			}
	}

	int AidMan::list(const char* sub_desc = nullptr) {
		int row;
		int foundIndex = -1;
		//version 5.3 update
		if (sub_desc == nullptr) {
			//Header
			if (iPnum == 0) {
				std::cout << "The list is emtpy!" << std::endl;
			}
			else {
				std::cout << " ROW |  SKU  | Description                         | Have | Need |  Price  | Expiry" << std::endl;
				std::cout << "-----+-------+-------------------------------------+------+------+---------+-----------" << std::endl;
				//Iterate through the records
				for (int i = 0; i < iPnum; i++) {
					ip[i]->linear(true);
					std::cout << std::setfill(' ') << std::setw(4) << i + 1 << " | ";
					ip[i]->display(std::cout);
					std::cout << std::endl;
				}
				std::cout << "-----+-------+-------------------------------------+------+------+---------+-----------" << std::endl;
				std::cin.ignore(5000, '\n');
				//Ask for detailed info, or return back to main menu
				std::cout << "Enter row number to display details or <ENTER> to continue:" << std::endl;
				row = ut.getintv2(1, iPnum, "> ");
				if (row == -666)
				{
					//Return to the menu
					std::cout << std::endl;
				}
				else
				{

					//Display detailed info of the selected item
					//Array of object pointers starts from 0, while rows start from 1
					ip[row - 1]->linear(false);
					ip[row - 1]->display(std::cout);
					std::cout << std::endl;
					std::cout << std::endl;
				}
			}
		}
		else {
			//print only the items containing the sub_desc in their description.
			std::cout << " ROW |  SKU  | Description                         | Have | Need |  Price  | Expiry" << std::endl;
			std::cout << "-----+-------+-------------------------------------+------+------+---------+-----------" << std::endl;
			for (int i = 0; i < iPnum; i++) {
				ip[i]->linear(true);
				if (*ip[i] == sub_desc) {
					std::cout << std::setfill(' ') << std::setw(4) << i + 1 << " | ";
					ip[i]->display(std::cout);
					std::cout << std::endl;
				}
			}
			std::cout << "-----+-------+-------------------------------------+------+------+---------+-----------" << std::endl;
			//Ask for a foolproof SKU input, compare it to existing SKUs
			int removesku = ut.getint(10000, 99999, "Enter SKU: ");
			foundIndex = search(removesku);
			
		}
		return foundIndex;
	}
	int AidMan::search(int sku) const {
		//Look through array of pointers to find matching sku
		int returnIndex = -1;
		for (int i = 0; i < iPnum && returnIndex == -1; i++) {
			//Dereference the pointer before using overloaded operator else error!
			if (ip[i]->getSKU() == sku) {
				returnIndex = i;
			}
		}
		//return the first match
		return returnIndex;
	}

	void AidMan::addItem() {
		int addSelect;
		//For improved modularity, this has been separated from the run() function
		//If there is room in the iProducts array of objects, create a new object
		if (iPnum < sdds_max_num_items) {
			std::cout << "1- Perishable" << std::endl;
			std::cout << "2- Non-Perishable" << std::endl;
			std::cout << "-----------------" << std::endl;
			std::cout << "0- Exit" << std::endl;
			addSelect = ut.getint(0, 2, "> ");
			if (addSelect == 1) {
				//create a new Perishable object
				ip[iPnum] = new Perishable;
				ip[iPnum]->readSku(std::cin); //read a valid Item SKU
				if (search(ip[iPnum]->getSKU()) != -1) {
					std::cout << "Sku: " << ip[iPnum]->getSKU() << " is already in the system, try updating quantity instead." << std::endl;
					std::cout << std::endl;
					delete ip[iPnum];
				}
				else {
					//read the rest
					ip[iPnum]->read(std::cin);
					//Check if state failed or object creation was successful
					if (ip[iPnum]) {
						++iPnum; //increment if successful
						/*Debug
						std::cout << iPnum << std::endl;
						ip[iPnum]->display(std::cout);
						*/
					}
					else {
						delete ip[iPnum];
						
					}
				}
			}
			else if (addSelect == 2) {
				//create a new Item object
				ip[iPnum] = new Item;			
				ip[iPnum]->readSku(std::cin); //read a valid Item SKU
				//If the sku of the new object matches any other existing sku
				if (search(ip[iPnum]->getSKU()) != -1) {
					std::cout << "Sku: " << ip[iPnum]->getSKU() << " is already in the system, try updating quantity instead." << std::endl;
					delete ip[iPnum];
				}
				else {
					//read the rest
					ip[iPnum]->read(std::cin);
					//Check if state failed or object creation was successful
					if (ip[iPnum]) {
						//Successful creation, no need to delete or decrement counter
						//ip[iPnum]->display(std::cout); //Debugging
						iPnum++;
					}
					else {
						delete ip[iPnum];
					}
				}
			}
			else {
				std::cout << "Aborted" << std::endl;
			}

		}
		else {
			std::cout << "Database full!" << std::endl;
		}
		std::cout << std::endl;
	}
	void AidMan::remove(int index) {
		if (index == -1) {
			std::string removal;
			std::cout << "Item description: ";
			std::cin >> removal;
			//search for item matching substring, returns matching index of SKU
			int foundIndex = list(removal.c_str());
			if (foundIndex != -1) {
				//If a SKU match is found, display object in descriptive format & ask confirmation 
				std::cout << "Following item will be removed: " << std::endl;
				ip[foundIndex]->linear(false);
				ip[foundIndex]->display(std::cout);
				std::cout << std::endl;
				std::cout << "Are you sure?" << std::endl;
				std::cout << "1- Yes!" << std::endl;
				std::cout << "0- Exit" << std::endl;
				int confirm = ut.getint(0, 1, "> ");
				//If user confirms deletion, remove the specified index
				if (confirm == 1) remove(foundIndex);
				else std::cout << "Aborted!\n" << std::endl;
			}
			else std::cout << "SKU not found!\n" << std::endl;		
		}
		else {
			delete ip[index];
			std::cout << "Item removed!" << std::endl;
			std::cout << std::endl;
			//Shift objects to the left to fill in the space
			for (int i = index; i < iPnum - 1; ++i) {
				ip[i] = ip[i + 1]; 
			}
			//Update the counter to reflect one less object in the array
			--iPnum;
		}

	}
	void AidMan::qtyUpdate() {
		std::string updateQty;
		std::cout << "Item description: ";
		std::cin >> updateQty;
		//search for item matching substr, returns matching index of SKU
		int foundIndex = list(updateQty.c_str());
		if (foundIndex != -1) {
			std::cout << "1- Add" << std::endl;
			std::cout << "2- Reduce" << std::endl;
			std::cout << "0- Exit" << std::endl;
			int selectOpt = ut.getint(0, 3, "> ");
			if (selectOpt == 1) {
				//Check if still need to add any items
				if (ip[foundIndex]->qtyNeeded() > ip[foundIndex]->qty()) {
					//Add to quantity of object (m_qty)
					std::cout << "Quantity to add: ";
					//Cant exceed the quantity needed (m_qtyNeeded)
					int addQty = ut.getint(1, ip[foundIndex]->qtyNeeded() - ip[foundIndex]->qty(), "");
					ip[foundIndex]->operator+=(addQty);
					std::cout << addQty << " items added!\n" << std::endl;
				}
				else std::cout << "Quantity Needed already fulfilled!\n" << std::endl;
			}
			else if (selectOpt == 2) {
				//Check if object has any quantity to reduce
				if (ip[foundIndex]->qty() > 0) {

					//Reduce the quantity of the selected object
					std::cout << "Quantity to reduce: ";
					//Can't exceed the quantity currently on hand (m_qty)
					int reduceQty = ut.getint(1, ip[foundIndex]->qty(), "");
					ip[foundIndex]->operator-=(reduceQty);
					std::cout << reduceQty << " items removed!\n" << std::endl;
				}
				else std::cout << "Quaintity on hand is zero!\n" << std::endl;
			}
			else std::cout << "Aborted!\n" << std::endl;
			
		}
		else std::cout << "SKU not found!\n" << std::endl;
	}

	void AidMan::sort() {
		//Sorts the array of iProduct object pointers by the difference between
		//quantity needed & quantity on hand, in descending order
		if (iPnum > 1) {
			/*If there is more than one object in the array, we can rearrange them
			Using nested loops with a temporary iProduct obj
			as a bridge for swapping the object pointers if they're out of order*/
			for (int i = 0; i < iPnum - 1; ++i) {
				for (int j = 0; j < iPnum - i - 1; ++j) {
					if (!sortHelper(ip[j], ip[j + 1])) {
						iProduct* temp = ip[j];
						ip[j] = ip[j + 1];
						ip[j + 1] = temp;
					}
				}
			}
		}
		std::cout << "Sort completed!\n" << std::endl;
	}

	bool AidMan::sortHelper(iProduct* current, iProduct* next) {
		//Compares the differences in quantity needed for 2 different objects
		int a = current->qtyNeeded() - current->qty();
		int b = next->qtyNeeded() - next->qty();
		return a > b;
	}

	void AidMan::ship(const char* file) {
		std::ofstream ofstr(file, std::ios::trunc); //open the file
		Date today;
		today.formatted(true);
		ofstr << "Shipping Order, Date: "; //YYYY/MM/DD
		ofstr << today << std::endl;
		ofstr << " ROW |  SKU  | Description                         | Have | Need |  Price  | Expiry" << std::endl;
		ofstr << "-----+-------+-------------------------------------+------+------+---------+-----------" << std::endl;
		/*In a loop go through all the iProduct Pointers elements if the quantity needed
		and quantity on hand of the product is a match print it in the linear format
		into the file & remove it from the iProduct Pointers array.*/
		int rowCount = 1;
		for (int i = 0; i < iPnum; i++) {
			if (ip[i]->qty() == ip[i]->qtyNeeded()) {
				ip[i]->linear(true);
				ofstr << std::setfill(' ') << std::setw(4) << rowCount << " | ";
				ip[i]->display(ofstr);
				ofstr << std::endl;
				rowCount++;
				//Remove the item from the objects array
				delete ip[i];
				//Shift the remaining items over
				for (int j = i; j < iPnum - 1; ++j) {
					ip[j] = ip[j + 1];
				}
				//update array object counter
				iPnum--;
			}
		}
		ofstr << "-----+-------+-------------------------------------+------+------+---------+-----------" << std::endl;
		std::cout << "Shipping Order for " << rowCount - 1 << " times saved!\n" << std::endl;
		ofstr.close();
	}
}
