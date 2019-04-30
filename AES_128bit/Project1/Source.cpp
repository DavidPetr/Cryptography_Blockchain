#include <iostream>
#include <string> 
#include <fstream>
#include <vector>
#include <memory>
#define SOURCE_FILE "test.txt"
#define CIPHER_FILE "ciphertest.txt"

void readFile(std::ifstream &file, std::vector<char> &buffer)
{
	if (!file) {
		std::cout << "Unable to open file to read";
		system("PAUSE");
		exit(1); // terminate with error
	}
	for (int i = 0; i < 16; ++i)
	{
		buffer[i] = '0';
	}
	file.read(buffer.data(), 16);
	
}
void writeInFile(std::ofstream &file, std::vector<char> &buffer)
{
	if (!file) {
		std::cout << "Unable to open file";
		exit(1); // terminate with error
	}
	file.write(buffer.data(), 16);
}
void swap(int &a, int &b)
{
	a ^= b;
	b ^= a;
	a ^= b;
}
void Reverse(int* row, int length)
{
	for (int i = 0; i < length / 2; ++i)
	{
		swap(row[i], row[length - 1 - i]);
	}
}
void ReverseLikeMatrix(int* key)
{
	swap(key[1], key[4]);
	swap(key[2], key[8]);
	swap(key[3], key[12]);
	swap(key[6], key[9]);
	swap(key[7], key[13]);
	swap(key[11], key[14]);
}
void ConstantXor(int* key, int rcon)
{
	int a[4] = {};
	a[0] = (rcon >> 6 * 4) & 0xff;
	a[1] = (rcon >> 4 * 4) & 0xff;
	a[2] = (rcon >> 2 * 4) & 0xff;
	a[3] = rcon & 0xff;
	for (int i = 0; i < 4; i++)
	{
		key[i] ^= a[i];
	}
}
int* WordXor(int* word1, int* word2)
{
	int *a = new int[4];
	for (int i = 0; i < 4; i++)
	{
		a[i] = word1[i] ^ word2[i];
	}
	return a;
}
void AddKey(int* text, int** words, int& wordIndex)
{
	int key[16]=
	{
		words[wordIndex - 4][0], words[wordIndex - 3][0], words[wordIndex - 2][0], words[wordIndex - 1][0],
		words[wordIndex - 4][1], words[wordIndex - 3][1], words[wordIndex - 2][1], words[wordIndex - 1][1],
		words[wordIndex - 4][2], words[wordIndex - 3][2], words[wordIndex - 2][2], words[wordIndex - 1][2],
		words[wordIndex - 4][3], words[wordIndex - 3][3], words[wordIndex - 2][3], words[wordIndex - 1][3]
	};
	for (int i = 0; i < 16; ++i)
	{
		text[i] ^= key[i];
	}
	wordIndex += 4;
}
void FirstFourWords(int* key,int** words)
{
	ReverseLikeMatrix(key);
	for (int i = 0; i < 4; ++i)
	{
		words[i] = new int[4];
		for (int j=0;j<4;j++)
		{
			words[i][j] = key[i * 4 + j];
		}
	}
}
void IncrementCounter(int* nonceCounter)
{
	int n = 16;
	for (int i = n - 1; i >= 8; --i)
	{
		if (nonceCounter[i] != 0xff)
		{
			++nonceCounter[i];
			break;
		}
		else
		{
			nonceCounter[i] = 0;
		}
	}
}


void SubBytes(int* row,int length)
{
	const int S[16][16] = {

		{ 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,	0x30, 0x01,	0x67, 0x2b,	0xfe, 0xd7, 0xab, 0x76 },
		{ 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf,	0x9c, 0xa4,	0x72, 0xc0 },
		{ 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15 },
		{ 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75 },
		{ 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84 },
		{ 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf },
		{ 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8 },
		{ 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2 },
		{ 0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73 },
		{ 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb },
		{ 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79 },
		{ 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08 },
		{ 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a },
		{ 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e },
		{ 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf },
		{ 0x8c, 0xa1,	0x89, 0x0d,	0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99,	0x2d, 0x0f,	0xb0, 0x54, 0xbb, 0x16 },
	};
	for (int i = 0; i < length; ++i)
	{
		int X = row[i] / 16;
		int Y = row[i] % 16;
		row[i] = S[X][Y];
	}
}
void ShiftRows(int* row)
{
	int size = 4;
	for (int i = 0; i < 4; ++i)
	{
		Reverse(&row[i * size], size);
		Reverse(&row[i * size], size - i);
		Reverse(&row[(i * size) + (size - i)], i);
	}
}
void MixColumns(int* row)
{
	const int C[4][4] = {
		{ 2, 3, 1, 1 },
		{ 1, 2, 3, 1 },
		{ 1, 1, 2, 3 },
		{ 3, 1, 1, 2 }
	};
	int a[4][4] = {};
	for (int k = 0; k < 4; k++)
	{
		for (int j = 0; j < 4; ++j)
		{
			for (int i = k; i < 16; i += 4)
			{
				if (C[j][i / 4] == 1)
				{
					a[j][k] ^= row[i];
				}
				else if (C[j][i / 4] == 2)
				{
					a[j][k] ^= row[i] << 1;
					a[j][k] &= 0xff;
					if ((row[i] & 0x80) == 0x80)
					{
						a[j][k] ^= 27;
					}
				}
				else if (C[j][i / 4] == 3)
				{
					a[j][k] ^= row[i] << 1;
					a[j][k] &= 0xff;
					if ((row[i] & 0x80) == 0x80)
					{
						a[j][k] ^= 27;
					}
					a[j][k] ^= row[i];
				}
			}
		}
	}
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			row[i * 4 + j] = a[i][j];
		}
	}
}
void AddRoundKey(int** words, int* lastKey, int& wordIndex)
{
	words[wordIndex] = WordXor(words[wordIndex-4],lastKey);
	++wordIndex;
	for (int i = 0; i < 3; ++i)
	{
		words[wordIndex] = WordXor(words[wordIndex - 1], words[wordIndex - 4]);
		++wordIndex;
	}
}

void RotWord(int* key)
{
	Reverse(key, 4);
	Reverse(key, 3);
}
void Auxilary(int* key, int &rcon)
{
	RotWord(key);
	SubBytes(key,4);
	ConstantXor(key, rcon);
	rcon = (rcon << 1) ^ (0x11b & -(rcon >> 7));
}
int convertHexToDec(char hex)
{
	int number = static_cast<int>(hex);
	if (number > 0x39)
	{
		number ^= 0x20;	//if key would be lowercase, converts it to uppercase: a->A
		number -= 0x37;	//converts from hex to dec: A->10
	}
	else
	{
		number -= 0x30;
	}
	return number;
}
int* covertToArray(std::string key)
{
	int *a = new int[16]();
	int n = key.length();
	int m = 16;
	for (int i = n - 1; i > 0; i -= 2)
	{
		int number = convertHexToDec(key[i - 1]);
		number *= 16;
		number += convertHexToDec(key[i]);
		a[--m] = number;
	}
	return a;
}
void convertToChar(int& number)
{
	if (number > 9)
	{
		number += 0x37;
	}
	else
	{
		number += 0x30;
	}
}
std::string convertToString(int* a, int n)
{
	std::string s;
	for (int i = 0; i < n; ++i)
	{
		int number = a[i] / 16;
		convertToChar(number);
		s.push_back(static_cast<char>(number));
		number = a[i] % 16;
		convertToChar(number);
		s.push_back(static_cast<char>(number));
	}
	return s;
}
void GetWords(int** words)
{
	int wordIndex = 4;
	int rcon = 0x1000000;
	for (int i = 0; i < 10; ++i)
	{
		int* key = new int[4]{ words[wordIndex - 1][0],words[wordIndex - 1][1],words[wordIndex - 1][2],words[wordIndex - 1][3] };

		Auxilary(key, rcon);
		AddRoundKey(words, key, wordIndex);

		delete[]key;
	}
}

void AES_128(int* nonceCounter,int** words)
{
	int wordIndex = 4;
	//AddKey K0
	AddKey(nonceCounter, words, wordIndex);

	for (int i = 0; i < 10; i++)
	{
		SubBytes(nonceCounter, 16);
		ShiftRows(nonceCounter);
		MixColumns(nonceCounter);
		AddKey(nonceCounter, words, wordIndex);
	}
}

int main(int argc,char *argv[])
{
	std::ifstream file(SOURCE_FILE);
	std::ofstream cipherFile(CIPHER_FILE);
	std::vector<char> buffer(16);

	int *nonceCounter = covertToArray("c59bcf35c59bcf350000000000000000");
	int *key = covertToArray("0f470caf15d9b77f71e8ad67c959d698");
	int **words;
	words = new int*[44];

	FirstFourWords(key, words);
	GetWords(words);

	while (!file.eof())
	{
		readFile(file, buffer);

		AES_128(nonceCounter,words);

		std::string key = convertToString(nonceCounter, 16);

		for (int i = 0; i < 16; ++i)
		{
			buffer[i] ^= key[i];
		}

		writeInFile(cipherFile, buffer);
		IncrementCounter(nonceCounter);
	}
	file.close();
	cipherFile.close();

	delete[]nonceCounter;
	for (int i = 0; i < 44; ++i)
	{
		delete[]words[i];
	}
	delete[]words;
	std::cout << "Encryption in Counter Mode via Cpp, complete successfully :) :) ;)!!!" << std::endl;
	getchar();
	return 0;
}
//Hello world. I am going to complete AES task