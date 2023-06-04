# Small-String-Optimization
Implementation of my own String class that uses Small String Optimization(SSO) for sufficiently small Strings
#How it works
My implementation of SSO is for strings, whose length is <= 15 (excluding \0).

When length is > 15, there is heap allocation, and the fields are
size_t length and char* data (pointer to heap).

When length is <= 15, the string data is stored in the memory of the size_t and the char*
which is accomplished using union of two structures, thus I don't have to heap allocate.
Here my fields are char length and char data[sizeof(sizeof(size_t) + sizeof(char*)) - sizeof(char)].
Since the length of the small string is always <= 15, i used char to keep the length, 
since it is the smallest type. I am now left with (sizeof(size_t) + sizeof(char*) - sizeof(char)) bytes 
for the actual small string. Since I have the length, i don't need to null terminate the 
string, which frees space for one additional symbol (unlike the heap string, which is null terminated).

Both structures are sizeof(size_t) + sizeof(char*) bytes, so union is very useful here.

The way I know if my string is small or big is by bitshifting. The length field is shifted left
by one bit in the constructor. If the original length is > 15, I add one to
the new length. If it is <= 15, i don't add anything(final bit is left 0). This is why 
length & 1 returns true if the string is big, and false if the string is small.
Everytime I want to get the actual length of the string, i bitshift right by 1 bit.
(This is why length() function returns _length >> 1).
