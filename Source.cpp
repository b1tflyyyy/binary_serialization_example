#include<iostream>
#include<string>
#include<fstream>

#define STRING_NULL " "

#define READ 1
#define WRITE 1

class Serialization
{
public:
	Serialization() 
	{ 
		m_current_position = NULL;
		m_fin.exceptions(std::ifstream::badbit | std::ifstream::failbit);
		m_fout.exceptions(std::ofstream::badbit | std::ofstream::failbit);
	};
	
	Serialization(const Serialization& other) = default;

	~Serialization() = default;

	/// <summary>
	/// Serialize array of base data types, without string and pointers.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="data"></param>
	/// <param name="size"></param>
	/// <param name="path"></param>
	template<typename T>
	void SerializeArrayOfBaseDataTypes(T* data, const int size, const std::string path)
	{
		m_fout.open(path, std::ios::binary | std::ios::app | std::ios::out);

		if (!m_fout.is_open())
		{
			throw std::exception("error opening the file");
		}
		else
		{
			for (T* pArr = data; pArr < data + size; pArr++)
			{
				m_fout.write((char*)(pArr), sizeof(T));
			}
		}

		m_fout.close();
	}

	/// <summary>
	/// Deserialize array of base data types, without string and pointers.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="data"></param>
	/// <param name="size"></param>
	/// <param name="path"></param>
	template<typename T>
	void DeserializeArrayOfBaseDataTypes(T* data, const int size, const std::string path)
	{
		m_fin.open(path, std::ios::binary | std::ios::in);
		m_fin.seekg(m_current_position, std::ios_base::beg);

		if (!m_fin.is_open())
		{
			throw std::exception("error opening the file");
		}
		else
		{
			for (T* pArr = data; pArr < data + size; pArr++)
			{
				m_fin.read((char*)(pArr), sizeof(T));
			}
		}

		m_current_position = m_fin.tellg();
		m_fin.close();
	}

	/// <summary>
	/// Serialize array of string.
	/// </summary>
	/// <param name="data"></param>
	/// <param name="size"></param>
	/// <param name="path"></param>
	void SerializeArrayOfString(std::string* data, const int size, const std::string path)
	{
		m_fout.open(path, std::ios::binary | std::ios::app | std::ios::out);

		if (!m_fout.is_open())
		{
			throw std::exception("error opening the file");
		}
		else
		{
			for (std::string* pArr = data; pArr < data + size; pArr++)
			{
				std::size_t data_lenght = (*pArr).length() + 1;
				
				m_fout.write((char*)&data_lenght, sizeof(data_lenght));
				m_fout.write((char*)(*pArr).c_str(), data_lenght);
			}
		}

		m_fout.close();
	}

	/// <summary>
	/// Deserialize array of string.
	/// </summary>
	/// <param name="data"></param>
	/// <param name="size"></param>
	/// <param name="path"></param>
	void DeserializeArrayOfString(std::string* data, const int size, const std::string path)
	{
		m_fin.open(path, std::ios::binary | std::ios::in);
		m_fin.seekg(m_current_position, std::ios_base::beg);

		if (!m_fin.is_open())
		{
			throw std::exception("error opening the file");
		}
		else
		{
			for (std::string* pArr = data; pArr < data + size; pArr++)
			{
				std::size_t data_lenght;

				m_fin.read((char*)&data_lenght, sizeof(data_lenght));
				char* buffer = new char[data_lenght];
				m_fin.read(buffer, data_lenght);

				*pArr = buffer;

				delete[] buffer;
			}
		}

		m_current_position = m_fin.tellg();
		m_fin.close();
	}

	/// <summary>
	/// Serialize one variable of the base data type.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="data"></param>
	/// <param name="path"></param>
	template<typename T>
	void SerializeBaseDataType(const T& data, const std::string path)
	{
		m_fout.open(path, std::ios::binary | std::ios::app | std::ios::out);

		if (!m_fout.is_open())
		{
			throw std::exception("error opening the file");
		}
		else
		{
			m_fout.write((char*)&data, sizeof(T));
		}

		m_fout.close();
	}

	/// <summary>
	/// Deserialize one variable of the base data type.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="data"></param>
	/// <param name="path"></param>
	template<typename T>
	void DeserializeBaseDataType(T& data, const std::string path)
	{
		m_fin.open(path, std::ios::binary | std::ios::in);
		m_fin.seekg(m_current_position, std::ios_base::beg);

		if (!m_fin.is_open())
		{
			throw std::exception("error opening the file");
		}
		else
		{
			m_fin.read((char*)&data, sizeof(T));
		}

		m_current_position = m_fin.tellg();
		m_fin.close();
	}

	/// <summary>
	/// Serialize one string.
	/// </summary>
	/// <param name="data"></param>
	/// <param name="path"></param>
	void SerializeString(const std::string& data, const std::string path)
	{
		m_fout.open(path, std::ios::binary | std::ios::app | std::ios::out);

		if (!m_fout.is_open())
		{
			throw std::exception("error opening the file");
		}
		else
		{
			std::size_t data_lenght = data.length() + 1;

			m_fout.write((char*)&data_lenght, sizeof(data_lenght));
			m_fout.write((char*)data.c_str(), data_lenght);
		}

		m_fout.close();
	}

	/// <summary>
	/// Deserialize one string.
	/// </summary>
	/// <param name="data"></param>
	/// <param name="path"></param>
	void DeserializeString(std::string& data, const std::string path)
	{
		m_fin.open(path, std::ios::binary | std::ios::in);
		m_fin.seekg(m_current_position, std::ios_base::beg);

		if (!m_fin.is_open())
		{
			throw std::exception("error opening the file");
		}
		else
		{
			std::size_t data_lenght;

			m_fin.read((char*)&data_lenght, sizeof(data_lenght));
			char* buffer = new char[data_lenght];
			m_fin.read(buffer, data_lenght);

			data = buffer;

			delete[] buffer;
		}

		m_current_position = m_fin.tellg();
		m_fin.close();
	}

	/// <summary>
	/// Is file empty.
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	bool IsFileEmpty(const std::string path)
	{
		m_fin.open(path, std::ios::binary | std::ios::in);
		m_fin.seekg(NULL, std::ios_base::beg);

		if (!m_fin.is_open())
		{
			throw std::exception("error opening the file");
		}
		else
		{
			bool result = m_fin.peek() == std::ifstream::traits_type::eof();
			m_fin.close();

			return result;
		}
	}

	/// <summary>
	/// Resets the position from which reading begins.
	/// </summary>
	void ResetFileReadPosition() { m_current_position = NULL; };

	/// <summary>
	/// Clear the file.
	/// </summary>
	/// <param name="path"></param>
	void ClearFile(const std::string path)
	{
		m_current_position = NULL;
		m_fout.open(path, std::ios::out | std::ios::trunc);
		m_fout.close();
	}

private:
	int m_current_position;

	std::ofstream m_fout; // write
	std::ifstream m_fin; // read
};

int main()
{
	const std::string PATH = "my_data.bin";
	Serialization serialization;
	
	const int SIZE = 3;

#if WRITE

	try
	{
		serialization.ClearFile(PATH);

		int nums[SIZE] = { 1, 2, 3 };
		serialization.SerializeArrayOfBaseDataTypes(nums, SIZE, PATH);

		std::string strings[SIZE] = { "sssss", "dsfsdfsdfsd", "fsdfsdfksdriireirioreoireogdfgdfg" };
		serialization.SerializeArrayOfString(strings, SIZE, PATH);

		float fnums[SIZE] = { 1.2, 3.4, 7.7 };
		serialization.SerializeArrayOfBaseDataTypes(fnums, SIZE, PATH);

		double x = 2.343545;
		serialization.SerializeBaseDataType(x, PATH);

		std::string str = "hello world";
		serialization.SerializeString(str, PATH);
	}
	catch (const std::ofstream::failure& ex)
	{
		std::cout << ex.what() << std::endl;
		std::cout << "Error code: " << ex.code() << std::endl;
		std::cout << "Error opening the file" << std::endl;
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Something went wrong" << std::endl;
	}

#endif

#if READ

	try
	{
		int result_nums[SIZE] = { NULL };
		serialization.DeserializeArrayOfBaseDataTypes(result_nums, SIZE, PATH);
		for (int i = 0; i < SIZE; i++)
		{
			std::cout << result_nums[i] << std::endl;
		}

		std::string result_strings[SIZE] = { STRING_NULL };
		serialization.DeserializeArrayOfString(result_strings, SIZE, PATH);
		for (int i = 0; i < SIZE; i++)
		{
			std::cout << result_strings[i] << std::endl;
		}

		float result_fnums[SIZE] = { NULL };
		serialization.DeserializeArrayOfBaseDataTypes(result_fnums, SIZE, PATH);
		for (int i = 0; i < SIZE; i++)
		{
			std::cout << result_fnums[i] << std::endl;
		}

		double result_x = NULL;
		serialization.DeserializeBaseDataType(result_x, PATH);
		std::cout << result_x << std::endl;

		std::string result_str = STRING_NULL;
		serialization.DeserializeString(result_str, PATH);
		std::cout << result_str << std::endl;

		serialization.ResetFileReadPosition();
	}
	catch (const std::ifstream::failure& ex)
	{
		std::cout << ex.what() << std::endl;
		std::cout << "Error code: " << ex.code() << std::endl;
		std::cout << "Error opening the file" << std::endl;
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Something went wrong" << std::endl;
	}
#endif

	return EXIT_SUCCESS;
}