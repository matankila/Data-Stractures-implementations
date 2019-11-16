#ifndef OL61_RECURSION
#define OL61_RECURSION

#include <stddef.h> /* size_t */

size_t Strlen(const char *str);

int StrCmp(const char *str1, const char *str2);

int StrnCmp(const char *str1, const char *str2, size_t n);

char *Strcpy(char *dest, const char *src);

char *Strcat(char *dest, const char *src);

char *Strstr(const char *haystack, const char *needle, size_t length);

#endif /* OL61_RECURSION */