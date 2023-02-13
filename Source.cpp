#include<iostream>
#include<string>
#include "SerializationBin.h"

#define STRING_NULL " "
#define NEW_LINE '\n'

#define READ 1
#define WRITE 1

int main()
{
	std::string_view PATH = "my_data.bin";
	szon::SerializationBin serialization;
	
	const int SIZE = 3;

#if WRITE

	try
	{
		serialization.ClearFile(PATH);

		int nums[SIZE] = { 1, 2, 3 };
		serialization.SerializeArrayOfData(nums, SIZE, PATH);

		std::string strings[SIZE] = { "bebra", "dsfsdfsdfsd", "fsdfsdfksdriireirioreoireogdfgdfg" };
		serialization.SerializeArrayOfData(strings, SIZE, PATH);

		float fnums[SIZE] = { 1.2, 3.4, 7.7 };
		serialization.SerializeArrayOfData(fnums, SIZE, PATH);

		double x = 2.343545;
		serialization.SerializeData(x, PATH);

		std::string str = "hello world";
		serialization.SerializeData(str, PATH);

		char symbol = 's';
		serialization.SerializeData(symbol, PATH);

		char symbols[SIZE] = { 'm', 'g', '\0' };
		serialization.SerializeArrayOfData(symbols, SIZE, PATH);
	}
	catch (const std::ofstream::failure& ex)
	{
		std::cout << ex.what() << NEW_LINE;
		std::cout << "Error code: " << ex.code() << NEW_LINE;
		std::cout << "Error opening the file" << NEW_LINE;
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << NEW_LINE;
	}
	catch (...)
	{
		std::cout << "Something went wrong" << NEW_LINE;
	}
	
#endif
	
#if READ

	try
	{
		int result_nums[SIZE] = { NULL };
		serialization.DeserializeArrayOfData(result_nums, SIZE, PATH);
		for (int i = 0; i < SIZE; i++)
		{
			std::cout << result_nums[i] << NEW_LINE;
		}

		std::string result_strings[SIZE] = { STRING_NULL };
		serialization.DeserializeArrayOfData(result_strings, SIZE, PATH);
		for (int i = 0; i < SIZE; i++)
		{
			std::cout << result_strings[i] << NEW_LINE;
		}

		float result_fnums[SIZE] = { NULL };
		serialization.DeserializeArrayOfData(result_fnums, SIZE, PATH);
		for (int i = 0; i < SIZE; i++)
		{
			std::cout << result_fnums[i] << NEW_LINE;
		}

		double result_x = NULL;
		serialization.DeserializeData(result_x, PATH);
		std::cout << result_x << NEW_LINE;

		std::string result_str = STRING_NULL;
		serialization.DeserializeData(result_str, PATH);
		std::cout << result_str << NEW_LINE;

		char result_symbol = ' ';
		serialization.DeserializeData(result_symbol, PATH);
		std::cout << result_symbol << NEW_LINE;

		char result_symbols[SIZE] = { ' ' };
		serialization.DeserializeArrayOfData(result_symbols, SIZE, PATH);
		for (int i = 0; i < SIZE; i++)
		{
			std::cout << result_symbols[i];
		}
		std::cout << std::endl;

		serialization.ResetFileReadPosition();
	}
	catch (const std::ifstream::failure& ex)
	{
		std::cout << ex.what() << NEW_LINE;
		std::cout << "Error code: " << ex.code() << NEW_LINE;
		std::cout << "Error opening the file" << NEW_LINE;
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << NEW_LINE;
	}
	catch (...)
	{
		std::cout << "Something went wrong" << NEW_LINE;
	}
#endif

	return EXIT_SUCCESS;
}