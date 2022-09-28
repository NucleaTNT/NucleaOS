#pragma once

#include <stddef.h>
#include <stdint.h>

/*
 * Slight warning: the output of any function returning
 * a `const char*` must be used immediately or copied
 * to another buffer, as currently they all share the
 * same OutputBuffer[128]. Usually this is fine since
 * the pointer is used to print and then discarded
 * however holding the returned pointer in a variable
 * can have undesired results should the buffer be
 * modified.
 *
 * Cry about it.
 */

template <typename T>
const char* to_string(T value);

template <typename T>
const char* to_string(T value, uint8_t precision);

template <typename T>
const char* to_hstring(T value);

/**
 * @brief Calculates the length of the string provided.
 *
 * @param str String to calculate length of.
 * @return Length of the string in characters.
 */
size_t strlen(const char* str);

/**
 * @brief Copies values from src string to dest string.
 *
 * @param dest String to copy values to.
 * @param src String to copy values from.
 * @return Pointer to destination string.
 */
char* strcpy(char* dest, const char* src);

const char* str_padleft(const char* str, size_t count, const char padChar);
const char* str_padright(const char* str, size_t count, const char padChar);
