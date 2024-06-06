/* 

Vladislav Zolotukhin
Revision History
-----------------------------------------------------------
Date      Reason
2023/11/15  Preliminary release
2023/11/20  Version 0.5 update
2023/11/27  Final update
-----------------------------------------------------------

*/
#define _CRT_SECURE_NO_WARNINGS
#include <iomanip>
#include <cstring>
#include "Status.h"
#include "Utils.h"

namespace sdds {

	Status::Status() {
		this->name = nullptr;
		this->code = 0;

	}

	Status::Status(char* name) {
		code = 0;
		if (this->name != nullptr)
		{
			delete[] this->name;
			this->name = nullptr;
		}
		this->name = new char[strlen(name) + 1];
		strcpy(this->name, name);

	}

	Status::~Status() {
		if (this->name != nullptr)
		{
			delete[] this->name;
			this->name = nullptr;
		}
	}

	Status::Status(const Status& src) {
		code = src.code;
		name = nullptr;
		if (src.name != nullptr) {
			name = new char[strlen(src.name) + 1];
			strcpy(name, src.name);
		}

	}

	Status& Status::operator=(const Status& src) {
		if (this != &src)
		{
			delete[] this->name;
			Status(src);
		}
		return *this;
	}

	int Status::getCode() const {
		return code;
	}
	const char* Status::getName() const {
		if (name != nullptr)
			return name;
		else return "NULL";
	}

	Status& Status::operator=(int code) {
		this->code = code;
		return *this;
	}

	Status& Status::operator=(const char* name) {
		if (name != nullptr && name[0] != '\0')
		{
			delete[] this->name;
			this->name = new char[strlen(name) + 1];
			strcpy(this->name, name);
		}
		return *this;
	}


	Status::operator int() const {
		return code;
	}
	
	Status::operator const char* () const {
		return name;
	}

	Status::operator bool() const {
		return (name == nullptr);
	}

	Status& Status::clear() {
		if (this->name != nullptr)
		{
			delete[] this->name;
			this->name = nullptr;
		}
		this->code = 0;
		return *this;
	}

	std::ostream& operator<<(std::ostream& ostr, const Status& status) {
		//no need to print if there isn't any error message, so use the bool operator overload
		//to check if name != nullptr
		if (status.name != nullptr) {
			if (status.getCode() != 0) {
				ostr << "ERR#" << status.getCode() << ": ";
			}
			ostr << status.getName();
		}

		return ostr;
	}
}