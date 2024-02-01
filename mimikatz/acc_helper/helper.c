#include "helper.h"

typedef BOOL(WINAPI* LPFuncVirtualFree)(LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType);
typedef LPVOID(WINAPI* LPFuncVirtualAlloc)(LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);




PVOID acc_memcpy(_Inout_ PVOID Destination, _In_ CONST PVOID Source, _In_ SIZE_T Length)
{
	PBYTE D = (PBYTE)Destination;
	PBYTE S = (PBYTE)Source;

	while (Length--)
		*D++ = *S++;

	return Destination;
}



void* acc_memset(void* dest, int value, size_t count) {
    // 异常情况检查
    if (dest == NULL) {
        return NULL;
    }

    unsigned char* d = (unsigned char*)dest;

    // 将内存设置为特定值
    while (count--) {
        *d++ = (unsigned char)value;
    }

    return dest;
}

void acc_free(void* ptr) {
    // 检查指针是否为NULL
    if (ptr != NULL) {
        // 加载 kernel32.dll
        HMODULE kernel32Module = LoadLibraryA("kernel32.dll");
        if (kernel32Module != NULL) {
            // 获取 VirtualFree 函数的地址
            LPFuncVirtualFree virtualFreeFunc = (LPFuncVirtualFree)GetProcAddress(kernel32Module, "VirtualFree");

            if (virtualFreeFunc != NULL) {
                virtualFreeFunc(ptr, 0, MEM_RELEASE);
                // 可选：将指针设置为 NULL，以避免野指针
                ptr = NULL;
            }
        }
    }
    return;
}


void* acc_malloc(size_t size) {
    // 加载 kernel32.dll
    HMODULE kernel32Module = LoadLibraryA("kernel32.dll");

    if (kernel32Module != NULL) {
        // 获取 VirtualAlloc 函数的地址
        LPFuncVirtualAlloc virtualAllocFunc = (LPFuncVirtualAlloc)GetProcAddress(kernel32Module, "VirtualAlloc");

        if (virtualAllocFunc != NULL) {
            // 使用 VirtualAlloc 函数分配内存
            LPVOID memory = virtualAllocFunc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

            if (memory == NULL) {
                // 处理分配失败的情况
                return NULL;
                //printf(stderr, "Failed to allocate memory\n");
            }

            // 卸载 kernel32.dll
            FreeLibrary(kernel32Module);

            return memory;
        }
        else {
            // 处理获取函数地址失败的情况
            //fprintf(stderr, "Failed to get address of VirtualAlloc function\n");
        }

        // 卸载 kernel32.dll
        FreeLibrary(kernel32Module);
    }
    else {
        // 处理加载库失败的情况
        //fprintf(stderr, "Failed to load kernel32.dll\n");
    }

    return NULL;
}


void* acc_calloc(size_t num, size_t size) {
    // 使用 malloc 分配内存
    void* memory = acc_malloc(num * size);

    if (memory != NULL) {
        // 使用 memset 将内存初始化为零
        acc_memset(memory, 0, num * size);
    }

    return memory;
}
