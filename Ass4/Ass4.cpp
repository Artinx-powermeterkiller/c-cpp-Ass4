// Ass4.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "Ass4.h"

using namespace std;

int main()
{
	ifstream infile;
	string temp_string;
	UTF8_Blocks_t* UTF8_Blocks=new UTF8_Blocks_t[BLOCK_NUM];
	int max_num = 0, type_orde = 0;
	long long unicode = 0;
	infile.open("Blocks.txt", ios::in);
	//	if (&infile == NULL)
	//	{
	//		cout << "File missing!" << endl;
	//	}
	//	else
		if (read_file(&infile, UTF8_Blocks))
		{
			infile.close();
			infile.open("sample6.txt", ios::in);
			while (!infile.eof())
			{
				unicode=get_unicode(&infile);
				if (unicode == -1) break;
				for (int i = 0; i < BLOCK_NUM; i++)
				{
					if (unicode >= UTF8_Blocks[i].begin_order && unicode <= UTF8_Blocks[i].end_order)
					{
						UTF8_Blocks[i].nums++;
					}
				}
			}
		}

	for (int i = 0; i < BLOCK_NUM; i++)
	{
		if (UTF8_Blocks[i].nums > max_num)
		{
			max_num = UTF8_Blocks[i].nums;
			type_orde = i;
		}
	}
	cout << "type" << UTF8_Blocks[type_orde].type << "num" << UTF8_Blocks[type_orde].nums << endl;
}

int read_file(ifstream* file, UTF8_Blocks_t* UTF8_Blocks_ptr)
{
	string temp_string, temp_sub_string;
	int block_num = 0, begin_order = 0, end_order = 0;

	do
	{
		getline(*file, temp_string);
		if (temp_string.find("#") == 0)
		{
			continue;
		}
		else
		{
			begin_order = temp_string.find("..");
			if (begin_order != -1)
			{
				temp_sub_string = temp_string.substr(0, begin_order);
				(UTF8_Blocks_ptr + block_num)->begin_order = HexToDec(temp_sub_string);

				end_order = temp_string.find(";");
				temp_sub_string = temp_string.substr(begin_order + 2, begin_order);
				(UTF8_Blocks_ptr + block_num)->end_order = HexToDec(temp_sub_string);

				temp_sub_string = temp_string.substr(end_order + 1, temp_string.length() - begin_order * 2 - 2);
				(UTF8_Blocks_ptr + block_num)->type = temp_sub_string;
			}
			else
			{
				continue;
			}
			block_num++;
		}
	} while (!file->eof() && block_num < BLOCK_NUM);

	return 1;
}

int HexToDec(string Hex_string)
{
	int length = Hex_string.length(), re = 0;

	char* temp_char = (char*)Hex_string.c_str();
	for (int i = length - 1; i >= 0; i--)
	{
		if ((int) * (temp_char + i) <= '9' && *(temp_char + i) >= '0')
		{
			re += (int)pow(16, length - i - 1) * ((int) * (temp_char + i) - '0');
		}
		else if ((int) * (temp_char + i) <= 'Z' && *(temp_char + i) >= 'A')
		{
			re += (int)pow(16, length - i - 1) * ((int) * (temp_char + i) - 'A'+ 10);
		}
		else if ((int) * (temp_char + i) <= 'z' && *(temp_char + i) >= 'a')
		{
			re += (int)pow(16, length - i - 1) * ((int) * (temp_char + i) - 'a' + 10);
		}
		else
		{
			return 0;
		}
	}
	return re;
}

long  get_unicode(ifstream* infile)
{
	long long re = 0;
	int UTF_len = 0;
	int temp_char, temp_chars[4] = {0};
	temp_char = infile->get();
	if (temp_char == EOF) return -1;
	if (temp_char >> 7 != 0)
	{
		for (int i = 0; i < 6; i++)
		{
			if ((temp_char & (0x80 >> i)) != 0)
			{
				UTF_len++;
			}
			else
			{
				break;
			}
		}
		temp_chars[0] = temp_char;

		for (int j = 1; j < UTF_len; j++)
		{
			temp_char = infile->get();
			temp_chars[j] = temp_char;
		}
	}
	else if (temp_char >> 7 != 0)
	{
		return temp_char;
	}

	UTF_len--;

	re = (temp_chars[0] & (0xFF >> (UTF_len+1)))<<(6* UTF_len);

	for (int i = 1; i <=UTF_len; i++)
	{
		re += (temp_chars[i] & (0xFF >> 2)) << (6 *( UTF_len-i));
	}
	return re;
}
