/*

Vladislav Zolotukhin
Revision History
-----------------------------------------------------------
Date      Reason
2023/11/15  Preliminary release
2023/12/02  Final update
-----------------------------------------------------------

*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include "Menu.h"
#include "Utils.h"

namespace sdds {

	Menu::Menu() {
		menuContent = nullptr;
		numOpt = 0;
	}

	Menu::Menu(const char* content) {
		if (content != nullptr && content[0] != '\0') {
			menuContent = new char[strlen(content) + 1];
			strcpy(menuContent, content);
		}
		else menuContent = nullptr;
		numOpt = 0;
		//check the amount of tabs in the menuContent array, increment numOpt for each
		//do this by iterating through the char array and matching /t
		//only do this if there's actually a non-null reference
		if (menuContent != nullptr) {
			for (int i = 0; menuContent[i] != '\0'; i++) {
				if (menuContent[i] == '\t')
					numOpt++;
			}
		}
		//if the menu has more than 15 options, it should be rendered invalid
		if (numOpt > 15)
		{
			delete[] menuContent;
			menuContent = nullptr;
			numOpt = 0;
		}

	}

	Menu::~Menu() {
		if (menuContent != nullptr) {
			delete[] menuContent;
			menuContent = nullptr;
		}
	}

	unsigned int Menu::run() const {
		//print the menu in a 'menu' format without modifying it
		//We have to deallocate this after its done printing or else there will be a memory leak
		//Check if menu exists first 
		if (this->menuContent != nullptr && menuContent[0] != '\0') { //&& menuContent[0] != '\n') {
			char* printMenu = new char[strlen(menuContent) + 1];
			strcpy(printMenu, menuContent);
			//iterate through the array again, same way we counted the tab characters
			for (int i = 0; printMenu[i] != '\0'; i++) {
				if (printMenu[i] == '\t')
					printMenu[i] = '\n';
			}
			//we need to preface each line with "#-"
			int menuLength = strlen(printMenu);
			int line = 1; //I tried to use the increment (i) variable here but it didnt work because of the nested loop
			for (int i = 0; i < menuLength; ++i)
			{
				if (printMenu[i] != '\n') {
					//at the start of the cout and for each corresponding new line, print "#-"
					std::cout << line << "- ";
					while (printMenu[i] != '\n' && printMenu[i] != '\0') {
						std::cout << printMenu[i];
						i++; //keep printing each letter until reaching a newline or the end
					}
					std::cout << std::endl;
					++line;
				}
			}
			std::cout << "---------------------------------" << std::endl;
			std::cout << "0- Exit" << std::endl;
			//call getInt from Utils to prompt for a valid integer output
			delete[] printMenu;
			unsigned int exitCode = ut.getint(0, numOpt + 1, "> ", nullptr);
			return exitCode;
		}
		else std::cout << "Invalid Menu!" << std::endl;
		return 0;

	}
	char* Menu::Content() const {
		return menuContent;
	}
}