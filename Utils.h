/*

Vladislav Zolotukhin
Revision History
-----------------------------------------------------------
Date        Reason
2023/11/15  Preliminary release
2023/11/29  Final update
-----------------------------------------------------------

*/
#ifndef SDDS_UTILS_H
#define SDDS_UTILS_H
namespace sdds {
   const int sdds_testYear = 2023;
   const int sdds_testMon = 12;
   const int sdds_testDay = 9;
   class Utils {
      bool m_testMode = false;
   public:
      void getSystemDate(int* year = nullptr, int* mon = nullptr, int* day = nullptr);
      int daysOfMon(int mon, int year)const;
      void testMode(bool testmode = true);
      //Recommended methods for Utils
      void alocpy(char*& destination, const char* source);
      int getint(const char* prompt = nullptr);
      int getint(int min, int max, const char* prompt = nullptr, const char* errMes = nullptr);
      double getdouble(const char* prompt = nullptr); //replicated the getint function for doubles
      double getdouble(double min, double max, const char* prompt = nullptr, const char* errMes = nullptr);
      //Final version variations supporting newline input
      int getintv2(const char* prompt = nullptr);
      int getintv2(int min, int max, const char* prompt = nullptr, const char* errMes = nullptr);
   };
   extern Utils ut;
}

#endif // !SDDS_UTILS_H
