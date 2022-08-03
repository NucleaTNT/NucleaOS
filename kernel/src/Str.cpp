#include "Str.hpp"

static char OutputBuffer[128] = {};
static char PadOutputBuffer[128] = {};

void ensureBufferSafety(char** buffer, size_t bufferLen = 128) {
    *buffer[bufferLen - 1] = 0;
}

template <typename T>
const char* to_string(T value) {
    uint8_t negativeOffset = 0;
    if (value < 0) {
        negativeOffset = 1;
        value *= -1;
        OutputBuffer[0] = '-';
    }

    uint8_t size = 0;
    for (T sizeTest = value; (sizeTest / 10) > 0; size++) {
        sizeTest /= 10;
    }

    uint8_t index = 0;
    for (; (value / 10) > 0; index++) {
        uint8_t remainder = value % 10;
        value /= 10;
        OutputBuffer[negativeOffset + size - index] = remainder + '0';
    }
    OutputBuffer[negativeOffset + size - index] = (value % 10) + '0';
    OutputBuffer[negativeOffset + size + 1] = 0;  // Null termination

    ensureBufferSafety((char**)&OutputBuffer);
    return OutputBuffer;
}

template <typename T>
const char* to_string(T value, uint8_t precision) {
    char* intPtr = (char*)to_string((int64_t)value);  // Place int portion into buffer

    precision = (precision > 16) ? 16 : precision;
    value = (value < 0) ? value * -1 : value;

    uint8_t index = 0;
    for (; *intPtr++ != 0; index++)
        ;  // Catch up with int portion in buffer
    OutputBuffer[index++] = '.';

    T decValue = value - (int)value;  // Extract exclusively decimal portion
    for (uint8_t i = 0; i < precision; i++) {
        decValue *= 10;                               // Shift next decimal unit up
        OutputBuffer[index++] = (int)decValue + '0';  // Extract isolated unit and add it
        decValue -= (int)decValue;                    // Remove isolated unit
    }
    OutputBuffer[index] = 0;  // Null termination

    ensureBufferSafety((char**)&OutputBuffer);
    return OutputBuffer;
}

template <typename T>
const char* to_hstring(T value) {
    uint64_t* valPtr = (uint64_t*)&value;
    uint8_t *ptr, tmp, size = (sizeof(T) * 2) - 1;

    for (uint8_t i = 0; i < (size / 2) + 1; i++) {
        ptr = ((uint8_t*)valPtr + i);

        tmp = ((*ptr & 0xf0) >> 4);
        OutputBuffer[(size - ((i * 2) + 1))] = tmp + ((tmp > 9) ? 55 : '0');

        tmp = (*ptr & 0x0f);
        OutputBuffer[(size - (i * 2))] = tmp + ((tmp > 9) ? 55 : '0');
    }
    OutputBuffer[size + 1] = 0;  // Null termination

    ensureBufferSafety((char**)&OutputBuffer);
    return OutputBuffer;
}

char* strcpy(char* dest, const char* src) {
    if (dest == NULL) {
        return NULL;
    }

    for (char* ptr = dest; *src != 0; *(ptr + 1) = 0) {
        *ptr++ = *src++;
    }

    return dest;
}

size_t strlen(const char* str) {
    int i = 0;
    for (; *str++ != 0; i++)
        ;
    return i;
}

const char* str_padleft(const char* str, size_t count, const char padChar) {
    count = (count > 127) ? 127 : count;
    size_t len = strlen(str);

    if ((count <= 0) || count < len) {
        return str;
    }

    for (uint8_t i = 0; i < count - len; i++) {
        PadOutputBuffer[i] = padChar;
    }
    for (uint8_t i = 1; i <= len; i++) {
        PadOutputBuffer[count - i] = str[len - i];
    }
    PadOutputBuffer[count] = 0;

    ensureBufferSafety((char**)&PadOutputBuffer);
    return PadOutputBuffer;
}

const char* str_padright(const char* str, size_t count, const char padChar) {
    count = (count > 127) ? 127 : count;

    size_t len = strlen(str);

    for (uint8_t i = len; i < count; i++) {
        PadOutputBuffer[i] = padChar;
    }
    for (uint8_t i = 0; i < len; i++) {
        PadOutputBuffer[i] = str[i];
    }
    PadOutputBuffer[count] = 0;

    ensureBufferSafety((char**)&PadOutputBuffer);
    return PadOutputBuffer;
}

template const char* to_string(uint8_t value);
template const char* to_string(uint16_t value);
template const char* to_string(uint32_t value);
template const char* to_string(uint64_t value);

template const char* to_string(int8_t value);
template const char* to_string(int16_t value);
template const char* to_string(int32_t value);
template const char* to_string(int64_t value);

template const char* to_string(float value, uint8_t precision);
template const char* to_string(double value, uint8_t precision);

template const char* to_hstring(uint8_t value);
template const char* to_hstring(uint16_t value);
template const char* to_hstring(uint32_t value);
template const char* to_hstring(uint64_t value);
