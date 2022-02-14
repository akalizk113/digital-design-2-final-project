#include <bits/stdc++.h>
using namespace std;

#define INPUT_FOLDER_PATH "E:/TL/2021-1/TKHTS-2/project/Final/code/text/input/"
#define OUTPUT_FOLDER_PATH "E:/TL/2021-1/TKHTS-2/project/Final/code/text/output_cpp/"
#define BITS_OF_FRACTION 20
#define BITS_OF_INTEGER 12
string **get_encode_input(string filePath)
{
   string **matrix = 0;
   matrix = new string *[8];

   ifstream file(filePath.c_str());
   if (!file.is_open())
   {
      cout << "Could not open file - " << filePath << endl;
      return matrix;
   }
   cout << "2D DCT with input matrix: " << endl;
   for (int i = 0; i < 8; i++)
   {
      matrix[i] = new string[8];
      for (int j = 0; j < 8; j++)
      {
         getline(file, matrix[i][j]);
         cout << matrix[i][j] << " ";
      }
      cout << endl;
   }
   file.close();
   return matrix;
}

int decode(string n)
{
   int num = stoi(n);
   int dec_value = 0;

   // Initializing base value to 1, i.e 2^0
   int base = 1;

   int temp = num;
   while (temp)
   {
      int last_digit = temp % 10;
      temp = temp / 10;

      dec_value += last_digit * base;

      base = base * 2;
   }

   return dec_value;
}

float **decode_input(string **input)
{
   float **matrix = 0;
   matrix = new float *[8];
   for (int i = 0; i < 8; i++)
   {
      matrix[i] = new float[8];
      for (int j = 0; j < 8; j++)
         matrix[i][j] = decode(input[i][j]);
   }
   return matrix;
}

string encode(float num)
{
   int h_int = BITS_OF_INTEGER, k_prec = BITS_OF_FRACTION;
   string binary = "";

   // Fetch the integral part of decimal number
   int Integral = num;

   // Fetch the fractional part decimal number
   float fractional = abs(num - Integral);

   // Conversion of integral part to
   // binary equivalent
   if (Integral < 0)
      Integral = Integral + pow(2, h_int - 1);

   while (Integral)
   {
      int rem = Integral % 2;

      // Append 0 in binary
      binary.push_back(rem + '0');

      Integral /= 2;
   }
   int size = binary.size();
   if (binary.size() > h_int - 1)
      return "Can not convert this number to a binary representation";
   else
      for (int i = 0; i < h_int - 1 - size; i++)
         binary.push_back('0');
   if (num < 0)
      binary.push_back('1');
   else
      binary.push_back('0');
   // Reverse string to get original binary
   // equivalent
   reverse(binary.begin(), binary.end());

   // Append point before conversion of
   // fractional part
   // binary.push_back('.');

   // Conversion of fractional part to
   // binary equivalent
   while (k_prec--)
   {
      // Find next bit in fraction
      fractional *= 2;
      int fract_bit = fractional;

      if (fract_bit == 1)
      {
         fractional -= fract_bit;
         binary.push_back(1 + '0');
      }
      else
         binary.push_back(0 + '0');
   }

   return binary;
}

string **encode_output(float **matrix)
{
   string **encoded_matrix = 0;
   encoded_matrix = new string *[8];
   for (int i = 0; i < 8; i++)
   {
      encoded_matrix[i] = new string[8];
      for (int j = 0; j < 8; j++)
      {
         encoded_matrix[i][j] = encode(matrix[i][j]);
      }
   }
   return encoded_matrix;
}

struct cordicAngle
{
   float cos, sin;
};

float degrees(float radians)
{
   return radians * (180.0 / 3.141592653589793238463);
}

float find_An(int n)
{
   float An = sqrt(2);
   for (int i = 1; i < n; i++)
      An = An * sqrt(1 + pow(2, -2 * i));
   return An;
}

cordicAngle cordic_iteration(float angle, int n = 40)
{
   float x = 1.0 / find_An(n),
         y = 0.0,
         newx,
         newy,
         di,
         z = angle;

   for (int i = 0; i <= n; i++)
   {
      if (z <= 0)
         di = -1;
      else
         di = 1;
      newx = x - (y * di * pow(2, -i));
      newy = y + (x * di * pow(2, -i));
      x = newx;
      y = newy;
      z = z - (di * degrees(atan(pow(2, -i))));
      // cout << z << endl;
   }
   // cout << "cos: " << x << endl;
   // cout << "sin: " << y << endl;
   return {x, y};
}

float **_1D_DCT(float **matrix)
{
   cordicAngle anglePI_4 = cordic_iteration(45);
   cordicAngle angle3PI_8 = cordic_iteration(67.5);
   cordicAngle angle3PI_16 = cordic_iteration(33.75);
   cordicAngle angle7PI_16 = cordic_iteration(78.75);

   float **dct_1 = 0;
   dct_1 = new float *[8];
   for (int i = 0; i < 8; i++)
   {
      dct_1[i] = new float[8];
      for (int j = 0; j < 8; j++)
      {
         switch (j)
         {
         case 0:
            dct_1[i][0] = 0.5 * ((matrix[i][0] + matrix[i][7] + matrix[i][3] + matrix[i][4]) * anglePI_4.sin + (matrix[i][1] + matrix[i][6] + matrix[i][2] + matrix[i][5]) * anglePI_4.cos);
            break;
         case 4:
            dct_1[i][4] = 0.5 * ((matrix[i][0] + matrix[i][7] + matrix[i][3] + matrix[i][4]) * anglePI_4.cos - (matrix[i][1] + matrix[i][6] + matrix[i][2] + matrix[i][5]) * anglePI_4.sin);
            break;
         case 2:
            dct_1[i][2] = 0.5 * ((matrix[i][0] + matrix[i][7] - matrix[i][3] - matrix[i][4]) * angle3PI_8.sin + (matrix[i][1] + matrix[i][6] - matrix[i][2] - matrix[i][5]) * angle3PI_8.cos);
            break;
         case 6:
            dct_1[i][6] = 0.5 * ((matrix[i][0] + matrix[i][7] - matrix[i][3] - matrix[i][4]) * angle3PI_8.cos - (matrix[i][1] + matrix[i][6] - matrix[i][2] - matrix[i][5]) * angle3PI_8.sin);
            break;

         case 1:
            dct_1[i][1] = 0.5 * ((matrix[i][0] - matrix[i][7]) * angle7PI_16.sin + (matrix[i][3] - matrix[i][4]) * angle7PI_16.cos + (matrix[i][1] - matrix[i][6]) * angle3PI_16.cos + (matrix[i][2] - matrix[i][5]) * angle3PI_16.sin);
            break;
         case 7:
            dct_1[i][7] = 0.5 * ((matrix[i][0] - matrix[i][7]) * angle7PI_16.cos - (matrix[i][3] - matrix[i][4]) * angle7PI_16.sin + (matrix[i][1] - matrix[i][6]) * angle3PI_16.sin * -1 + (matrix[i][2] - matrix[i][5]) * angle3PI_16.cos);
            break;
         case 3:
            dct_1[i][3] = 0.5 * ((matrix[i][0] - matrix[i][7]) * angle3PI_16.cos - (matrix[i][3] - matrix[i][4]) * angle3PI_16.sin - (matrix[i][1] - matrix[i][6]) * angle7PI_16.cos - (matrix[i][2] - matrix[i][5]) * angle7PI_16.sin);
            break;
         case 5:
            dct_1[i][5] = 0.5 * ((matrix[i][0] - matrix[i][7]) * angle3PI_16.sin + (matrix[i][3] - matrix[i][4]) * angle3PI_16.cos - (matrix[i][1] - matrix[i][6]) * angle7PI_16.sin + (matrix[i][2] - matrix[i][5]) * angle7PI_16.cos);
            break;
         default:
            break;
         }
      }
   }
   // cout << dct_1[0][5] << endl;
   return dct_1;
}

float **transpose(float **matrix)
{
   float **transpose = 0;
   transpose = new float *[8];
   for (int i = 0; i < 8; i++)
   {
      transpose[i] = new float[8];
      for (int j = 0; j < 8; j++)
         transpose[i][j] = matrix[j][i];
   }
   return transpose;
}

float **_2D_DCT(float **matrix)
{
   float **stage1 = _1D_DCT(matrix);
   // cout << "================ 2D DCT ================" << endl;
   for (int i = 0; i < 8; i++)
   {

      for (int j = 0; j < 8; j++)
      {
         cout << stage1[i][j] << " ";
      }
      cout << endl;
   }
   float **stage2 = transpose(stage1);
   float **stage3 = _1D_DCT(stage2);
   float **stage4 = transpose(stage3);
   return stage4;
}

void export_encode_output_file(string **matrix, int n)
{
   string filename = OUTPUT_FOLDER_PATH + string("output") + to_string(n) + string("_encoded.txt");
   ofstream file(filename);
   for (int i = 0; i < 8; i++)
      for (int j = 0; j < 8; j++)
         file << matrix[i][j] << endl;
   file.close();
}

void export_output_file(float **matrix, int n)
{
   string filename = OUTPUT_FOLDER_PATH + string("output") + to_string(n) + string(".txt");
   ofstream file(filename);
   for (int i = 0; i < 8; i++)
      for (int j = 0; j < 8; j++)
         file << matrix[i][j] << endl;
   file.close();
}

main()
{
   int f;
   string input_filename, output_filename;
   cout << "Number of files: " << endl;
   cin >> f;

   for (int i = 1; i <= f; i++)
   {
      input_filename = INPUT_FOLDER_PATH + string("input") + to_string(i) + string("_encoded.txt");
      string **encodeMatrix = get_encode_input(input_filename);
      float **decoded_input = decode_input(encodeMatrix);
      float **output = _2D_DCT(decoded_input);
      string **encoded_output = encode_output(output);
      export_output_file(output, i);
      export_encode_output_file(encoded_output, i);
   }
}