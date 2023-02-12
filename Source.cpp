#include<iostream>
#include<string>
#include<string_view>
#include<fstream>
#include<memory>

#define STRING_NULL " "
#define NEW_LINE '\n'

#define READ 1
#define WRITE 1

namespace szon
{
	class SerializationBinary
	{
	public:
		SerializationBinary()
		{
			m_current_position = NULL;
			m_fin.exceptions(std::ifstream::badbit | std::ifstream::failbit);
			m_fout.exceptions(std::ofstream::badbit | std::ofstream::failbit);
		};

		SerializationBinary(const SerializationBinary& other) = default;
		
		virtual ~SerializationBinary() = default;

		/// <summary>
		/// Serialize array of data.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="data"></param>
		/// <param name="size"></param>
		/// <param name="path"></param>
		template<typename T>
		void SerializeArrayOfData(T* data, const int size, std::string_view path)
		{
			m_fout.open(path.data(), std::ios::binary | std::ios::app | std::ios::out);

			if (!m_fout.is_open())
				throw std::exception("error opening the file");

			for (T* pArr = data; pArr < data + size; pArr++)
				m_fout.write(reinterpret_cast<const char*>(pArr), sizeof(T));

			m_fout.close();
		}

		/// <summary>
		/// Deserialize array of data.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="data"></param>
		/// <param name="size"></param>
		/// <param name="path"></param>
		template<typename T>
		void DeserializeArrayOfData(T* data, const int size, std::string_view path)
		{
			m_fin.open(path.data(), std::ios::binary | std::ios::in);
			m_fin.seekg(m_current_position, std::ios_base::beg);

			if (!m_fin.is_open())
				throw std::exception("error opening the file");

			for (T* pArr = data; pArr < data + size; pArr++)
				m_fin.read(reinterpret_cast<char*>(pArr), sizeof(T));

			m_current_position = m_fin.tellg();
			m_fin.close();
		}

		/// <summary>
		/// Serialize array of data.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="data"></param>
		/// <param name="size"></param>
		/// <param name="path"></param>
		template<> // implementation for strings ----->
		void SerializeArrayOfData<std::string>(std::string* data, const int size, std::string_view path)
		{
			m_fout.open(path.data(), std::ios::binary | std::ios::app | std::ios::out);

			if (!m_fout.is_open())
				throw std::exception("error opening the file");

			for (std::string* pArr = data; pArr < data + size; pArr++)
			{
				std::size_t data_lenght = (*pArr).length() + 1;

				m_fout.write(reinterpret_cast<const char*>(&data_lenght), sizeof(data_lenght));
				m_fout.write(reinterpret_cast<const char*>((*pArr).c_str()), data_lenght);
			}

			m_fout.close();
		}

		/// <summary>
		/// Deserialize array of data.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="data"></param>
		/// <param name="size"></param>
		/// <param name="path"></param>
		template<>
		void DeserializeArrayOfData<std::string>(std::string* data, const int size, std::string_view path)
		{
			m_fin.open(path.data(), std::ios::binary | std::ios::in);
			m_fin.seekg(m_current_position, std::ios_base::beg);

			if (!m_fin.is_open())
				throw std::exception("error opening the file");

			for (std::string* pArr = data; pArr < data + size; pArr++)
			{
				std::size_t data_lenght;

				m_fin.read(reinterpret_cast<char*>(&data_lenght), sizeof(data_lenght));
				auto buffer = std::make_unique<char[]>(data_lenght);

				m_fin.read(buffer.get(), data_lenght);
				*pArr = buffer.get();
			}

			m_current_position = m_fin.tellg();
			m_fin.close();
		}

		/// <summary>
		/// Serialize one variable.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="data"></param>
		/// <param name="path"></param>
		template<typename T>
		void SerializeData(const T& data, std::string_view path)
		{
			m_fout.open(path.data(), std::ios::binary | std::ios::app | std::ios::out);

			if (!m_fout.is_open())
				throw std::exception("error opening the file");

			m_fout.write(reinterpret_cast<const char*>(&data), sizeof(T));
			m_fout.close();
		}

		/// <summary>
		/// Deserialize one variable.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="data"></param>
		/// <param name="path"></param>
		template<typename T>
		void DeserializeData(T& data, std::string_view path)
		{
			m_fin.open(path.data(), std::ios::binary | std::ios::in);
			m_fin.seekg(m_current_position, std::ios_base::beg);

			if (!m_fin.is_open())
				throw std::exception("error opening the file");

			m_fin.read(reinterpret_cast<char*>(&data), sizeof(T));
			m_current_position = m_fin.tellg();
			m_fin.close();
		}

		/// <summary>
		/// Serialize one variable.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="data"></param>
		/// <param name="path"></param>
		template<>
		void SerializeData<std::string>(const std::string& data, std::string_view path)
		{
			m_fout.open(path.data(), std::ios::binary | std::ios::app | std::ios::out);

			if (!m_fout.is_open())
				throw std::exception("error opening the file");

			std::size_t data_lenght = data.length() + 1;

			m_fout.write(reinterpret_cast<const char*>(&data_lenght), sizeof(data_lenght));
			m_fout.write(reinterpret_cast<const char*>(data.c_str()), data_lenght);

			m_fout.close();
		}

		/// <summary>
		/// Deserialize one variable.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="data"></param>
		/// <param name="path"></param>
		template<>
		void DeserializeData<std::string>(std::string& data, std::string_view path)
		{
			m_fin.open(path.data(), std::ios::binary | std::ios::in);
			m_fin.seekg(m_current_position, std::ios_base::beg);

			if (!m_fin.is_open())
				throw std::exception("error opening the file");

			std::size_t data_lenght;

			m_fin.read(reinterpret_cast<char*>(&data_lenght), sizeof(data_lenght));
			auto buffer = std::make_unique<char[]>(data_lenght);

			m_fin.read(buffer.get(), data_lenght);
			data = buffer.get();

			m_current_position = m_fin.tellg();
			m_fin.close();
		}

		/// <summary>
		/// Is file empty.
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		bool IsFileEmpty(std::string_view path)
		{
			m_fin.open(path.data(), std::ios::binary | std::ios::in);
			m_fin.seekg(NULL, std::ios_base::beg);

			if (!m_fin.is_open())
				throw std::exception("error opening the file");

			bool result = m_fin.peek() == std::ifstream::traits_type::eof();
			m_fin.close();

			return result;
		}

		/// <summary>
		/// Resets the position from which reading begins.
		/// </summary>
		void ResetFileReadPosition() { m_current_position = NULL; };

		/// <summary>
		/// Clear the file.
		/// </summary>
		void ClearFile(std::string_view path)
		{
			m_current_position = NULL;
			m_fout.open(path.data(), std::ios::out | std::ios::trunc);
			m_fout.close();
		}

	private:
		unsigned long m_current_position;

		std::ofstream m_fout; 
		std::ifstream m_fin;
	};
}

int main()
{
	std::string_view PATH = "my_data.bin";
	szon::SerializationBinary serialization;
	
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