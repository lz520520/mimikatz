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
    // �쳣������
    if (dest == NULL) {
        return NULL;
    }

    unsigned char* d = (unsigned char*)dest;

    // ���ڴ�����Ϊ�ض�ֵ
    while (count--) {
        *d++ = (unsigned char)value;
    }

    return dest;
}

void acc_free(void* ptr) {
    // ���ָ���Ƿ�ΪNULL
    if (ptr != NULL) {
        // ���� kernel32.dll
        HMODULE kernel32Module = LoadLibraryA("kernel32.dll");
        if (kernel32Module != NULL) {
            // ��ȡ VirtualFree �����ĵ�ַ
            LPFuncVirtualFree virtualFreeFunc = (LPFuncVirtualFree)GetProcAddress(kernel32Module, "VirtualFree");

            if (virtualFreeFunc != NULL) {
                virtualFreeFunc(ptr, 0, MEM_RELEASE);
                // ��ѡ����ָ������Ϊ NULL���Ա���Ұָ��
                ptr = NULL;
            }
        }
    }
    return;
}


void* acc_malloc(size_t size) {
    // ���� kernel32.dll
    HMODULE kernel32Module = LoadLibraryA("kernel32.dll");

    if (kernel32Module != NULL) {
        // ��ȡ VirtualAlloc �����ĵ�ַ
        LPFuncVirtualAlloc virtualAllocFunc = (LPFuncVirtualAlloc)GetProcAddress(kernel32Module, "VirtualAlloc");

        if (virtualAllocFunc != NULL) {
            // ʹ�� VirtualAlloc ���������ڴ�
            LPVOID memory = virtualAllocFunc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

            if (memory == NULL) {
                // �������ʧ�ܵ����
                return NULL;
                //printf(stderr, "Failed to allocate memory\n");
            }

            // ж�� kernel32.dll
            FreeLibrary(kernel32Module);

            return memory;
        }
        else {
            // �����ȡ������ַʧ�ܵ����
            //fprintf(stderr, "Failed to get address of VirtualAlloc function\n");
        }

        // ж�� kernel32.dll
        FreeLibrary(kernel32Module);
    }
    else {
        // ������ؿ�ʧ�ܵ����
        //fprintf(stderr, "Failed to load kernel32.dll\n");
    }

    return NULL;
}


void* acc_calloc(size_t num, size_t size) {
    // ʹ�� malloc �����ڴ�
    void* memory = acc_malloc(num * size);

    if (memory != NULL) {
        // ʹ�� memset ���ڴ��ʼ��Ϊ��
        acc_memset(memory, 0, num * size);
    }

    return memory;
}
