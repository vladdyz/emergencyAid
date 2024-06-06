/***********************************************************************
							Main Entry Point
***********************************************************************/
#include <iostream>
#include <fstream>
#include "AidMan.h"
#include "Utils.h"
using namespace sdds;
using namespace std;

void copyfile(const char* to, const char* from);
void displayFile(const char* file);

int main() {
   copyfile("placeholder.dat", "data.dat");
   ut.testMode();
   AidMan().run();
   displayFile("placeholder.dat"); //displays the contents of the created placeholder.dat file after user quits the program
   cout << endl << "------------------------------------------------" << endl;
   displayFile("shippingOrder.txt"); //displays the contents of the shippingOrder.txt file (if one is created by the program) upon termination
   return 0;
}

void displayFile(const char* file) {
   ifstream in(file);
   char ch;
   cout << "File: " << file << endl;
   while (in.get(ch)) cout << ch;
}

void copyfile(const char* to, const char* from) {
   std::ifstream in(from);
   std::ofstream out(to);
   char ch;
   while (in.get(ch)) out.put(ch);
}
