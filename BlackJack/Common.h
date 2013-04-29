
#ifndef COMMON_H_
#define COMMON_H_

#define TRUE (1)
#define FALSE (0)

#if defined(WIN32)
#define safe_sprintf(buffer, size, fmt, ...) sprintf_s(buffer, size, fmt, __VA_ARGS__)
#define safe_strcpy(dstStr, srcStr, size) strcpy_s(dstStr, size, srcStr)
#else
#define safe_sprintf(buffer, size, fmt, ...) snprintf(buffer, size, fmt, __VA_ARGS__)
#define safe_strcpy(dstStr, srcStr, size) strncpy(dstStr, srcStr, size)
#endif

#endif // ! COMMON_H_
