#ifndef ASS4_H
#define ASS4_H

#include <iostream>
#include <fstream>
#include<string>

using namespace std;

#define BLOCK_NUM 300

typedef struct
{
	string type;
	int begin_order = 0;
	int end_order = 0;
	int nums = 0;
} UTF8_Blocks_t;

int read_file(ifstream* file, UTF8_Blocks_t*);
int HexToDec(string Hex_string);
long  get_unicode(ifstream* infile);
#endif //
