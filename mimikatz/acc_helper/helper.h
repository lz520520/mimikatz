#pragma once
#include <windows.h>


#ifdef __cplusplus
extern "C" {
#endif
	PVOID acc_memcpy(_Inout_ PVOID Destination, _In_ CONST PVOID Source, _In_ SIZE_T Length);
	void* acc_memset(void* dest, int value, size_t count);
	void acc_free(void* ptr);
	void* acc_malloc(size_t size);
	void* acc_calloc(size_t num, size_t size);
#ifdef __cplusplus
}
#endif
