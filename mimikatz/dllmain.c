#include "tools.h"
#include "acc_helper/acc.h"

__declspec(dllexport) void install(install_args_t* install_args) {
    acc_install(install_args);
}

char* WideStringToNarrowString(wchar_t* wideStr) {
    // 获取宽字符字符串的长度（包括终止 null 字符）
    size_t wideStrLen = wcslen(wideStr) + 1;

    // 使用 mbstowcs 进行宽字符到多字节字符转换
    // 注意: 这里假设系统使用 UTF-8 编码
    char* narrowStr = (char*)LocalAlloc(LPTR,wideStrLen);  // 分配足够的内存
    size_t converted;
    if (narrowStr) {
        wcstombs_s(&converted, narrowStr, wideStrLen,wideStr, wideStrLen);
    }

    return narrowStr;
}

__declspec(dllexport) void DLLWMain(args_list_t* args_list) {
    outputBufferElements = 0xff;
    outputBufferElementsPosition = 0;
    outputBuffer = (wchar_t*)LocalAlloc(LPTR, outputBufferElements * sizeof(wchar_t));

    datap parser;
    acc_data_parse(&parser, args_list->params, args_list->params_length);
    wchar_t* lpszCmdLine = (wchar_t*)acc_data_extract(&parser,NULL);
    kprintf(L"args: %s\n", lpszCmdLine);

    int argc = 0;
    wchar_t** argv;

    if (lpszCmdLine && lstrlenW(lpszCmdLine))
    {
        if (argv = CommandLineToArgvW(lpszCmdLine, &argc))
        {
            wmain(argc, argv);
            LocalFree(argv);
            char* result = WideStringToNarrowString(outputBuffer);
            acc_print_info(result);
            
            LocalFree(result);
            LocalFree(outputBuffer);

        }
    }
    else wmain(0, NULL);
}

// #if defined(_POWERKATZ)
// __declspec(dllexport) wchar_t * powershell_reflective_tools(LPCWSTR input)
// {
// 	int argc = 0;
// 	wchar_t ** argv;

// 	if(argv = CommandLineToArgvW(input, &argc))
// 	{
// 		outputBufferElements = 0xff;
// 		outputBufferElementsPosition = 0;
// 		if(outputBuffer = (wchar_t *) LocalAlloc(LPTR, outputBufferElements * sizeof(wchar_t)))
// 			wmain(argc, argv);
// 		LocalFree(argv);
// 	}
// 	return outputBuffer;
// }
// #endif

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}