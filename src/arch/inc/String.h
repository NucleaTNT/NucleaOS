#pragma once
#include "Typedefs.h"

void StrCat(char* dest, const char* src);
void StrCopy(char* dest, const char* src);
size_t StrLength(const char* string);

const char* ToHexString(uint8_t value, bool hasPrefix = true);
const char* ToHexString(uint16_t value, bool hasPrefix = true);
const char* ToHexString(uint32_t value, bool hasPrefix = true);
const char* ToHexString(uint64_t value, bool hasPrefix = true);

const char* ToString(uint64_t value);
const char* ToString(int64_t value);
const char* ToString(double value, uint8_t decimalPlaces = 2);