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

	template<typename T>
	void SerializeBaseDataTypes(T* data, const int size, const std::string path)
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

	template<typename T>
	void DeserializeBaseDataTypes(T* data, const int size, const std::string path)
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

	void SerializeString(std::string* data, const int size, const std::string path)
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
				std::size_t dataLenght = (*pArr).length() + 1;
				
				m_fout.write((char*)&dataLenght, sizeof(dataLenght));
				m_fout.write((char*)(*pArr).c_str(), dataLenght);
			}
		}

		m_fout.close();
	}

	void DeserializeString(std::string* data, const int size, const std::string path)
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

	void ClearFile(const std::string path)
	{
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
		serialization.SerializeBaseDataTypes(nums, SIZE, PATH);

		std::string strings[SIZE] = { "sssss", "dsfsdfsdfsd", "fsdfsdfksdriireirioreoireogdfgdfg" };
		serialization.SerializeString(strings, SIZE, PATH);

		float fnums[SIZE] = { 1.2, 3.4, 7.7 };
		serialization.SerializeBaseDataTypes(fnums, SIZE, PATH);
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
		serialization.DeserializeBaseDataTypes(result_nums, SIZE, PATH);
		for (int i = 0; i < SIZE; i++)
		{
			std::cout << result_nums[i] << std::endl;
		}

		std::string result_strings[SIZE] = { STRING_NULL };
		serialization.DeserializeString(result_strings, SIZE, PATH);
		for (int i = 0; i < SIZE; i++)
		{
			std::cout << result_strings[i] << std::endl;
		}

		float result_fnums[SIZE] = { NULL };
		serialization.DeserializeBaseDataTypes(result_fnums, SIZE, PATH);
		for (int i = 0; i < SIZE; i++)
		{
			std::cout << result_fnums[i] << std::endl;
		}
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