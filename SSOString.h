#pragma once
#include <iostream>

//Initial code from Angeld55 github

class SSOString {

	union {
		struct {
			size_t _length = 0;
			char* _data = nullptr;
		} _bigString;
		struct {
			char _length = 0;
			char _data[sizeof(_bigString) - sizeof(char)]{}; // not null terminated (one symbol additional size)
		} _smallString;
	};

	void copyFrom(const SSOString& data);
	void free();

	void copyToSmallString(const char* str, const size_t length);
	bool isSmallString() const;
public:

	SSOString(const char* data);
	SSOString(const SSOString& other);
	SSOString& operator=(const SSOString& other);
	~SSOString();

	size_t length() const;
	SSOString& operator+=(const SSOString& other);

	SSOString substr(size_t beginIndex, size_t endIndex) const;

	char& operator[](size_t index);
	char operator[](size_t index) const;

	const char* c_str() const;

	friend SSOString operator+(const SSOString& lhs, const SSOString& rhs);
	friend std::istream& operator>>(std::istream&, SSOString& str);
};

SSOString operator+(const SSOString& lhs, const SSOString& rhs);
std::ostream& operator<<(std::ostream& os, const SSOString& str);
std::istream& operator>>(std::istream& os, SSOString& str);

bool operator< (const SSOString& lhs, const SSOString& rhs);
bool operator<=(const SSOString& lhs, const SSOString& rhs);
bool operator>=(const SSOString& lhs, const SSOString& rhs);
bool operator> (const SSOString& lhs, const SSOString& rhs);
bool operator==(const SSOString& lhs, const SSOString& rhs);
bool operator!=(const SSOString& lhs, const SSOString& rhs);