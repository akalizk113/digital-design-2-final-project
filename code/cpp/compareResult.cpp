#include <bits/stdc++.h>

using namespace std;

#define N 32
#define Q 20
#define PI 3.141592653589793
#define VERILOG_OUTPUT_FOLDER_PATH "E:/TL/2021-1/TKHTS-2/project/Final/code/text/output_verilog/"
#define CPP_OUTPUT_FOLDER_PATH "E:/TL/2021-1/TKHTS-2/project/Final/code/text/output_cpp/"
#define COMPARE_RESULT_FOLDER_PATH "E:/TL/2021-1/TKHTS-2/project/Final/code/text/compare_result/"

float decode(string a)
{
   int size = a.size();
   int bits_size = N;
   int fraction_size = Q;
   float result = 0;
   char sign_bit = a[0];
   if (size != bits_size)
   {
      cout << "ERROR: Invalid input";
      return (float)0;
   }
   string interger = a.substr(1, bits_size - fraction_size - 1);
   string fraction = a.substr(bits_size - fraction_size);
   for (int i = 0; i < interger.size(); i++)
      if (interger[i] == '1')
         result += pow(2, interger.size() - i - 1);

   for (int i = -1; i >= 0 - fraction.size(); i--)
   {
      if (fraction[-i - 1] == '1')
         result += pow(2, i);
   }
   if (a[0] == '0')
      return result;
   else if (a[0] == '1')
      return result - pow(2, (bits_size - fraction_size - 1));
   else
   {

      cout << "ERROR: Invalid input" << endl;
      return (float)0;
   }
}

string **getVerilogOutput(string filePath)
{
   string **matrix = 0;
   matrix = new string *[8];

   ifstream file(filePath.c_str());
   if (!file.is_open())
   {
      cout << "Could not open file - " << filePath << endl;
      return matrix;
   }
   for (int i = 0; i < 8; i++)
   {
      matrix[i] = new string[8];
      for (int j = 0; j < 8; j++)
      {
         getline(file, matrix[i][j]);
      }
   }
   file.close();
   return matrix;
}

float **getCppOutput(string filePath)
{
   string a;
   float **matrix = 0;
   matrix = new float *[8];

   ifstream file(filePath.c_str());
   if (!file.is_open())
   {
      cout << "Could not open file - " << filePath << endl;
      return matrix;
   }
   for (int i = 0; i < 8; i++)
   {
      matrix[i] = new float[8];
      for (int j = 0; j < 8; j++)
      {
         getline(file, a);
         matrix[i][j] = stod(a);
      }
   }
   file.close();
   return matrix;
}

float **decodeVerilogOuput(string **m)
{
   float **matrix = 0;
   matrix = new float *[8];
   for (int i = 0; i < 8; i++)
   {
      matrix[i] = new float[8];
      for (int j = 0; j < 8; j++)
      {
         matrix[i][j] = decode(m[i][j]);
      }
   }
   return matrix;
}

void compareResult(float **cppResult, float **verilogResult, int n)
{

   string filename = COMPARE_RESULT_FOLDER_PATH + string("compare_results") + string(".txt");
   ofstream file(filename, fstream::app);
   file << endl
        << "================ Error rate of output " << n << " ================" << endl;
   float x, s = 0;
   for (int i = 0; i < 8; i++)
   {
      for (int j = 0; j < 8; j++)
      {
         x = abs(cppResult[i][j] - verilogResult[i][j]) / abs(cppResult[i][j]) * 100;
         s = s + x;
         file << x << "% ";
      }
      file << endl;
   }
   file << endl
        << "Avarage error rate: " << s / 64 << "%" << endl;
   file.close();
}

main()
{
   string cppFilePath, verilogFilePath;
   float **cpp, **verilog;
   string **a;
   int n;
   cout << "Number of output files: ";
   cin >> n;
   for (int i = 1; i <= n; i++)
   {
      verilogFilePath = VERILOG_OUTPUT_FOLDER_PATH + string("output") + to_string(i) + string(".txt");
      a = getVerilogOutput(verilogFilePath);
      verilog = decodeVerilogOuput(a);
      cppFilePath = CPP_OUTPUT_FOLDER_PATH + string("output") + to_string(i) + string(".txt");
      cpp = getCppOutput(cppFilePath);
      compareResult(cpp, verilog, i);
   }
}
