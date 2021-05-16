#include "String.h"

char toHexStringBuffer[128];
template <typename T>
const char* ConvertHexString(T value, uint8_t size, bool hasPrefix) {
	T* valPtr = &value;
	uint8_t* ptr;
	uint8_t tmp;
	uint8_t start;

	if (hasPrefix) start = 2;
	else start = 0;

	for (uint8_t i = 0; i < size; i++) {
		ptr = ((uint8_t*)valPtr + i);

		tmp = ((*ptr &0xf0) >> 4);
		toHexStringBuffer[(start + size) - ((i * 2) + 1)] = tmp + (tmp > 9 ? 55 : '0');
		
		tmp = ((*ptr &0x0f));
		toHexStringBuffer[(start + size) - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
	}

	if (hasPrefix) {
		toHexStringBuffer[0] = '0';
		toHexStringBuffer[1] = 'x';
	}

	toHexStringBuffer[(start + size) + 1] = 0;
	return toHexStringBuffer;
}

const char* ToHexString(uint8_t value, bool hasPrefix) {
	return ConvertHexString(value, 1, hasPrefix);
}

const char* ToHexString(uint16_t value, bool hasPrefix) {
	return ConvertHexString(value, 3, hasPrefix);
}

const char* ToHexString(uint32_t value, bool hasPrefix) {
	return ConvertHexString(value, 7, hasPrefix);
}

const char* ToHexString(uint64_t value, bool hasPrefix) { 
	return ConvertHexString(value, 15, hasPrefix);
}

char uintToStringBuffer[128];
const char* ToString(uint64_t value) {
	uint8_t size;
	uint64_t sizeTest = value;

	while ((sizeTest / 10) > 0) {
		sizeTest /= 10;
		size++;
	}

	uint8_t i = 0;
	while ((value / 10) > 0) {
		uint8_t remainder = value % 10;
		value /= 10;

		uintToStringBuffer[size - i] = remainder + '0';
		i++;
	}

	uint8_t remainder = value % 10;
	uintToStringBuffer[size - i] = remainder + '0';
	uintToStringBuffer[size + 1] = 0;

	return uintToStringBuffer;
}

char intToStringBuffer[128];
const char* ToString(int64_t value) {
	uint8_t signOffset = 0;

	if (value < 0) {
		signOffset = 1;
		value *= -1;
		intToStringBuffer[0] = '-';
	}

	uint8_t size;
	uint64_t sizeTest = value;

	while ((sizeTest / 10) > 0) {
		sizeTest /= 10;
		size++;
	}

	uint8_t i = 0;
	while ((value / 10) > 0) {
		uint8_t remainder = value % 10;
		value /= 10;

		intToStringBuffer[(signOffset + size) - i] = remainder + '0';
		i++;
	}

	uint8_t remainder = value % 10;
	intToStringBuffer[(signOffset + size) - i] = remainder + '0';
	intToStringBuffer[(signOffset + size) + 1] = 0;

	return intToStringBuffer;
}


char doubleToStringBuffer[128];
const char* ToString(double value, uint8_t decimalPlaces) {
	if (decimalPlaces > 20) decimalPlaces = 20;

	char* intPtr = (char*)ToString((int64_t)value);
	char* doublePtr = doubleToStringBuffer;

	if (value < 0) value *= -1;

	while (*intPtr != 0) {
		*doublePtr = *intPtr;
		intPtr++;
		doublePtr++;
	}

	*doublePtr = '.';
	doublePtr++;

	double newValue = value - (int)value;
	for (uint8_t i = 0; i < decimalPlaces; i++) {
		newValue *= 10;
		*doublePtr = (int)newValue + '0';
		newValue -= (int)newValue;
		doublePtr++;
	}
	*doublePtr = 0;

	return doubleToStringBuffer;
}