#include <bits/stdc++.h>
using namespace std;
#define INPUT_FOLDER_PATH "E:/TL/2021-1/TKHTS-2/project/Final/code/text/input/"

int encode(int decimalNumber)
{
   int binaryNumber = 0;
   int p = 0;
   while (decimalNumber > 0)
   {

      binaryNumber += (decimalNumber % 2) * pow(10, p);
      ++p;
      decimalNumber /= 2;
   }
   return binaryNumber;
}

void generate_input(int f)
{
   srand(unsigned(time(NULL)));
   int n = 255, decimalNumber;
   string s1, s2;
   cout << "Generating input file ..." << endl;

   for (int i = 1; i <= f; i++)
   {
      srand(i);

      s1 = INPUT_FOLDER_PATH + string("input") + to_string(i) + string(".txt");
      s2 = INPUT_FOLDER_PATH + string("input") + to_string(i) + string("_encoded.txt");
      ofstream Decimal(s1);
      ofstream Encoded(s2);
      for (int i = 0; i < 64; i++)
      {

         decimalNumber = rand() % (254 - 0 + 1) + 0;

         Decimal << right << setw(3) << setfill('0') << decimalNumber << endl;

         Encoded << right << setw(8) << setfill('0') << encode(decimalNumber) << endl;
      }

      Encoded.close();
      Decimal.close();
   }
   cout << 2 * f << "File input have been generated" << endl;
}

main()
{
   int n;
   cout << " Number of input files : ";
   cin >> n;
   generate_input(n);
}