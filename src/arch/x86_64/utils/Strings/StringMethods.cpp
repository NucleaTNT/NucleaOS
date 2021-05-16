#include "String.h"

void StrCat(char* dest, const char* src) {
	int originLen = StrLength(dest);
	int finalLen = originLen + StrLength(src);

	for (int i = originLen; i < finalLen; i++) dest[i] = src[i - originLen];
	dest[finalLen] = 0;
}

void StrCopy(char* dest, const char* src) {
	int i = 0;
	for (i = 0; src[i] != 0; i++) dest[i] = src[i];
	dest[i] = 0;
}

size_t StrLength(const char* string) {
	size_t length = 0;
	while (string[length] != 0) length++;
	return length;
}