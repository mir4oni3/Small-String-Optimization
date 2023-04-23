#include "SSOString.h"
#pragma warning (disable : 4996)

bool SSOString::isSmallString() const {
	return !(_smallString._length & 1);

}

SSOString operator+(const SSOString& lhs, const SSOString& rhs) {
	SSOString result(lhs);
	result += rhs;
	return result;
}

SSOString& SSOString::operator+=(const SSOString& other) {
	size_t thisLength = length();
	size_t otherLength = other.length();

	//result is bigstring
	if (thisLength + otherLength > sizeof(size_t) + sizeof(char*)) {
		char* newData = new char[thisLength + otherLength + 1];
		for (int i = 0; i < thisLength; i++) {
			newData[i] = (isSmallString()) ? (_smallString._data[i]) : (_bigString._data[i]);
		}
		for (int i = 0; i < otherLength; i++) {
			newData[thisLength + i] = (other.isSmallString()) ? (other._smallString._data[i]) : (other._bigString._data[i]);
		}
		newData[thisLength + otherLength] = '\0';
		if (!isSmallString()) {
			delete[] _bigString._data;
		}
		_bigString._data = newData;
		_bigString._length = ((thisLength + otherLength) << 1) + 1;
	}
	//result is smallstring (both this and other are smallstring)
	else {
		char newData[sizeof(size_t) + sizeof(char*)];
		for (int i = 0; i < thisLength; i++) {
			newData[i] = _smallString._data[i];
		}
		for (int i = 0; i < otherLength; i++) {
			newData[thisLength + i] = other._smallString._data[i];
		}
		copyToSmallString(newData, thisLength + otherLength);
		_smallString._length = (thisLength + otherLength) << 1;
	}
	return *this;
}

//not using strcpy because _smallString._data is not null terminated
void SSOString::copyToSmallString(const char* str, const size_t length) {
	if (length > sizeof(size_t) + sizeof(char*)) {
		throw std::exception("is not SmallString");
	}
	for (int i = 0; i < length; i++) {
		_smallString._data[i] = str[i];
	}
	_smallString._length = length << 1;
}

SSOString::SSOString(const char* data) {
	if (!data) {
		throw std::exception("nullptr exception");
	}

	int curlen = strlen(data);
	if (curlen > sizeof(size_t) + sizeof(char*)) {
		_bigString._length = (curlen << 1) + 1;
		_bigString._data = new char[curlen + 1];
		strcpy(_bigString._data, data);
	}
	else {
		_smallString._length = curlen << 1;
		copyToSmallString(data,curlen);
	}
}

SSOString::SSOString(const SSOString& other) {
	copyFrom(other);
}

SSOString& SSOString::operator=(const SSOString& other) {
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}


void SSOString::free() {
	if (!isSmallString()) {
		delete[] _bigString._data;
		_bigString._data = nullptr;
	}
}

SSOString::~SSOString() {
	free();
}

size_t SSOString::length() const {
	if (isSmallString()) {
		return _smallString._length >> 1;
	}
	else {
		return _bigString._length >> 1;
	}
}

void SSOString::copyFrom(const SSOString& other) {
	if (!other.isSmallString()) {
		_bigString._length = other._bigString._length;
		_bigString._data = new char[_bigString._length + 1];
		strcpy(_bigString._data, other._bigString._data);
	}
	else {
		_smallString._length = other._smallString._length;
		copyToSmallString(other._smallString._data, other.length());
	}
}

char& SSOString::operator[](size_t index) { //non-constant access
	if (isSmallString()) {
		index = (index >= _smallString._length) ? 0 : index;
		return _smallString._data[index];
	}
	else {
		index = (index >= _bigString._length) ? 0 : index;
		return _bigString._data[index];
	}
}

char SSOString::operator[](size_t index) const { //constant access
	if (isSmallString()) {
		index = (index >= _smallString._length) ? 0 : index;
		return _smallString._data[index];
	}
	else {
		index = (index >= _bigString._length) ? 0 : index;
		return _bigString._data[index];
	}
}

SSOString SSOString::substr(size_t beginIndex, size_t endIndex) const {
	if (beginIndex > endIndex || beginIndex >= length() || endIndex >= length()) {
		throw std::exception("invalid index");
	}

	//result is smallstring (avoid heap allocation)
	if (isSmallString() || endIndex - beginIndex + 1 <= sizeof(size_t) + sizeof(char*)) {
		char result[sizeof(size_t) + sizeof(char*) + 1];
		for (size_t i = beginIndex; i <= endIndex; i++) {
			result[i - beginIndex] = (isSmallString()) ? (_smallString._data[i]) : (_bigString._data[i]);
		}
		result[endIndex - beginIndex + 1] = '\0';
		return SSOString(result);
	}

	//result is bigstring
	char* result = new char[endIndex - beginIndex + 2];
	for (size_t i = beginIndex; i <= endIndex; i++) {
		result[i - beginIndex] = _bigString._data[i];
	}
	result[endIndex - beginIndex + 1] = '\0';

	SSOString substr(result);
	delete[] result;
	return substr;
}

const char* SSOString::c_str() const {
	char* result = new char[length() + 1];
	for (int i = 0; i < length(); i++) {
		result[i] = (isSmallString()) ? _smallString._data[i] : _bigString._data[i];
	}
	result[length()] = '\0';
	return result;
}

std::ostream& operator<<(std::ostream& os, const SSOString& str)
{
	return os << str.c_str();
}

std::istream& operator>>(std::istream& is, SSOString& str)
{
	char buff[1024];
	is >> buff; // is.getLine(buff, 1024);
	size_t bufflen = strlen(buff);

	if (!str.isSmallString()) {
		delete[] str._bigString._data;
		str._bigString._length = (bufflen << 1) + 1;
		str._bigString._data = new char[str.length() + 1];
		strcpy(str._bigString._data, buff);
	}
	else {
		str._smallString._length = bufflen << 1;
		str.copyToSmallString(buff, bufflen);
	}
	return is;
}


bool operator<(const SSOString& lhs, const SSOString& rhs) {
	const char* lhsValue = lhs.c_str();
	const char* rhsValue = rhs.c_str();
	bool result = strcmp(lhs.c_str(), rhs.c_str()) < 0;
	delete[] lhsValue;
	delete[] rhsValue;
	return result;
}

bool operator==(const SSOString& lhs, const SSOString& rhs) {
	const char* lhsValue = lhs.c_str();
	const char* rhsValue = rhs.c_str();
	bool result = strcmp(lhs.c_str(), rhs.c_str()) == 0;
	delete[] lhsValue;
	delete[] rhsValue;
	return result;
}

bool operator<=(const SSOString& lhs, const SSOString& rhs) {
	return lhs < rhs || lhs == rhs;
}

bool operator>=(const SSOString& lhs, const SSOString& rhs) {
	return !(lhs < rhs);
}

bool operator>(const SSOString& lhs, const SSOString& rhs) {
	return !(lhs < rhs) && lhs != rhs;
}

bool operator!=(const SSOString& lhs, const SSOString& rhs) {
	return !(lhs == rhs);
}