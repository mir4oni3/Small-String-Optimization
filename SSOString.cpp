#include "SSOString.h"
#pragma warning (disable : 4996)

bool SSOString::isSmallString() const{
	return !(_length & 1);

}

SSOString operator+(const SSOString& lhs, const SSOString& rhs)
{
	SSOString result(lhs._length + rhs._length + 1);

	result[0] = '\0';
	strcat(result._data, lhs._data);
	strcat(result._data, rhs._data);

	return result;
}

SSOString& SSOString::operator+=(const SSOString& other)
{
	char* result = new char[(_length += other._length) + 1];
	result[0] = '\0';
	strcat(result, _data);
	strcat(result, other._data);

	delete[] _data;
	_data = result;

	return *this;
}

SSOString::SSOString(size_t capacity)
{
	_length = capacity - 1;
	_data = new char[capacity];
}

SSOString::SSOString(const char* data) : SSOString(strlen(data) + 1)
{
	strcpy(_data, data);
}

SSOString::SSOString(const SSOString& other)
{
	copyFrom(other);
}

SSOString& SSOString::operator=(const SSOString& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}


void SSOString::free()
{
	delete[] _data;
	_data = nullptr;
}

SSOString::~SSOString()
{
	free();
}

size_t SSOString::length() const
{
	return _length;
}

void SSOString::copyFrom(const SSOString& other)
{
	_length = other._length;
	_data = new char[_length + 1];
	strcpy(_data, other._data);
}

char& SSOString::operator[](size_t index) //Неконстантен достъп
{
	return _data[index];
}

char SSOString::operator[](size_t index) const //Константен достъп 
{
	return _data[index];
}

SSOString SSOString::substr(size_t begin, size_t howMany) const
{
	if (begin + howMany > _length)
		throw std::length_error("Error, Substr out of range");


	SSOString res(howMany + 1);
	for (int i = 0; i < howMany; i++)
		res._data[i] = _data[begin + i];
	res[howMany] = '\0';
	return res;
}

const char* SSOString::c_str() const
{
	return _data;
}

std::ostream& operator<<(std::ostream& os, const SSOString& str)
{
	return os << str.c_str();
}

std::istream& operator>>(std::istream& is, SSOString& str)
{
	char buff[1024];
	is >> buff; // is.getLine(buff, 1024);

	delete[] str._data;
	str._length = strlen(buff);
	str._data = new char[str._length + 1];
	strcpy(str._data, buff);
	return is;
}


bool operator<(const SSOString& lhs, const SSOString& rhs)
{
	return strcmp(lhs.c_str(), rhs.c_str()) < 0;
}

bool operator<=(const SSOString& lhs, const SSOString& rhs)
{
	return strcmp(lhs.c_str(), rhs.c_str()) <= 0;
}
bool operator>=(const SSOString& lhs, const SSOString& rhs)
{
	return strcmp(lhs.c_str(), rhs.c_str()) >= 0;
}
bool operator>(const SSOString& lhs, const SSOString& rhs)
{
	return strcmp(lhs.c_str(), rhs.c_str()) > 0;
}
bool operator==(const SSOString& lhs, const SSOString& rhs)
{
	return strcmp(lhs.c_str(), rhs.c_str()) == 0;
}
bool operator!=(const SSOString& lhs, const SSOString& rhs)
{
	return strcmp(lhs.c_str(), rhs.c_str()) != 0;
}