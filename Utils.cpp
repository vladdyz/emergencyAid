/*

Vladislav Zolotukhin
Revision History
-----------------------------------------------------------
Date        Reason
2023/11/15  Preliminary release
2023/11/29  Final update
-----------------------------------------------------------

*/

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <ctime>
#include <cstring>
#include <iomanip>
#include "Utils.h"
using namespace std;
namespace sdds {
   Utils ut;
   void Utils::testMode(bool testmode) {
      m_testMode = testmode;
   }
   void Utils::getSystemDate(int* year, int* mon, int* day) {
      if (m_testMode) {
         if(day) *day = sdds_testDay;
         if(mon) *mon = sdds_testMon;
         if(year) *year = sdds_testYear;
      }
      else {
         time_t t = std::time(NULL);
         tm lt = *localtime(&t);
         if (day) *day = lt.tm_mday;
         if (mon) *mon = lt.tm_mon + 1;
         if (year) *year = lt.tm_year + 1900;
      }
   }
   int Utils::daysOfMon(int month, int year)const {
      int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, -1 };
      int mon = (month >= 1 && month <= 12 ? month : 13)-1;
      return days[mon] + int((mon == 1) * ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
   }

   void Utils::alocpy(char*& destination, const char* source) {
       /*Safely allocates memory in the destination argument and copies the source into it.
        deletes the destination dynamic array and sets it to null if the source is not null
        Allocates memory in the destination to the size of the source and copies the source into it.*/
       if (destination != nullptr) {
           delete[] destination;
           destination = nullptr;
       }
       destination = new char[strlen(source) + 1];
       strcpy(destination, source);
   }

   int Utils::getint(const char* prompt/* = nullptr*/) {
       //if the prompt is not null, display it and require the user to enter a valid integer
       int validInt = 0;
       bool enterInt = false;
       if (prompt != nullptr)
       {
           cout << prompt;
           while (!enterInt) {
               if (cin >> validInt) {
                   enterInt = true;
               }
               else {
                   cout << "Invalid Integer, retry: ";
                   cin.clear();
                   cin.ignore(5000, '\n');
               }
           }
       }
       return validInt;
   }
   int Utils::getint(int min, int max, const char* prompt /*= nullptr*/, const char* errMes/* = nullptr*/) {
       //same as above but requires the integer to be within a range, also calls the above function

       int validInt = getint(prompt); //call the above function to get an integer & display prompt
       while (validInt < min || validInt > max) {
           //int validInt = getint(prompt); //call the above function to get an integer & display prompt
           if (errMes != nullptr) //if a custom error message string was passed into the function, display it
           {
               cout << errMes << ", retry: ";
           }
           else { //otherwise print a generic message
               cout << "Value out of range [" << min << "<=val<=" << max << "]: ";
           }
           validInt = getint("");
       }
       return validInt;
   }
   //addition created for v0.5
   double Utils::getdouble(const char* prompt) {
       //if the prompt is not null, display it and require the user to enter a valid integer
       double validDouble = 0;
       bool enterDbl = false;
       if (prompt != nullptr)
       {
           cout << prompt;
           while (!enterDbl) {
               if (cin >> validDouble) {
                   enterDbl = true;
               }
               else {
                   cout << "Invalid number, retry: ";
                   cin.clear();
                   cin.ignore(5000, '\n');
               }
           }
       }
       return validDouble;
   }
   double Utils::getdouble(double min, double max, const char* prompt, const char* errMes) {
       double validDouble = getdouble(prompt);
       while (validDouble < min || validDouble > max) {
           if (errMes != nullptr) {
               cout << errMes << ", retry: ";
           }
           else {
               cout << "Value out of range [" << fixed << setprecision(2) << min << "<=val<=" << max << "]: ";
           }
           validDouble = getdouble("");
       }
       return validDouble;
   }
   //additions created for final version, accept newline as a valid input
   int Utils::getintv2(const char* prompt) {
       int validInt;
       bool enterInt = false;

       if (prompt != nullptr) {
           std::cout << prompt;
       }
       if (std::cin.peek() == '\n') {
           // Return a special value if the user enters a newline
           std::cin.ignore(5000, '\n');
           return -666; // Or any other special value to indicate a newline
       }
           while (!enterInt) {
               if (std::cin >> validInt) {
                   enterInt = true;
               }
               else {
                   if (std::cin.peek() == '\n') {
                       // Return a special value if the user ends (aka inputs a newline \n) 
                       std::cin.clear();
                       return -666;
                   }
                   std::cout << "Invalid input. Please enter a valid integer: ";
                   std::cin.clear();
                   std::cin.ignore(5000, '\n');
               }
           }
       
       return validInt;
   }
   int Utils::getintv2(int min, int max, const char* prompt, const char* errMes) {
       int validInt = getintv2(prompt);
 
       while (validInt < min || validInt > max) {
           if (validInt == -666) {
               // Special value (-1) indicates a newline
               return -666; // Return the special value to indicate a newline
           }

           if (errMes != nullptr) {
               std::cout << errMes << ", retry: ";
           }
           else {
               std::cout << "Value out of range [" << min << "<=val<=" << max << "]: ";
           }

           validInt = getintv2("");
       }
       return validInt;
   }

}