#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

typedef struct _install_cache {
    LPVOID memory;
    size_t mem_len;
    size_t rand_len;

    LPVOID module_base;
    HMODULE* lib_handles;
    size_t lib_count;
    LPVOID entry_ptr;
    BOOL gc;
} install_cache_t;

typedef struct _install_args {
    char* install_name;
    char* entry_name;
    LPVOID log_ptr;
    int task_id;
    install_cache_t* cache;
} install_args_t;






typedef struct _args_list {
    char* params;
    int params_length;
    int task_id;
} args_list_t; 

typedef struct {
    char* original; /* the original buffer [so we can free it] */
    char* buffer;   /* current pointer into our buffer */
    int    length;   /* remaining length of data */
    int    size;     /* total size of this buffer */
} datap;


typedef struct {
    char* original; /* the original buffer [so we can free it] */
    char* buffer;   /* current pointer into our buffer */
    int    length;   /* remaining length of data */
    int    size;     /* total size of this buffer */
} formatp;



enum LOG_TYPE {
    LOG_INFO_TYPE = 0,
    LOG_SUCCESS_TYPE,
    LOG_ERROR_TYPE,
    LOG_DATA_TYPE = 0xf,
};





#ifdef __cplusplus
extern "C" {
#endif
    void acc_install(install_args_t* install_args);
    void acc_set_task_id(int task_id);
    int acc_get_task_id();

    void acc_data_parse(datap* parser, char* buffer, int size);
    int acc_data_int(datap* parser);
    short acc_data_short(datap* parser);
    char* acc_data_extract(datap* parser, int* size);
    int     acc_data_length(datap* parser);


    // format api
    void    acc_format_alloc(formatp* format, int maxsz);
    void    acc_format_reset(formatp* format);
    void    acc_format_free(formatp* format);
    void    acc_format_append(formatp* format, char* text, int len);
    void    acc_format_printf(formatp* format, char* fmt, ...);

    char* acc_format_tostring(formatp* format, int* size);
    void    acc_format_int(formatp* format, int value);

    /* Token Functions */
    BOOL   acc_use_token(HANDLE token);
    BOOL   acc_revert_token();


    /* Utility Functions */
    uint32_t swap_endianess(uint32_t indata);
    char* unhexlify(char* value, int* outlen);

    void acc_printf(const char* fmt, ...);
    void acc_printf_with_taskid(int taskId, const char* fmt, ...);

    void acc_print_info(const char* fmt, ...);
    void acc_print_info_with_taskid(int taskId, const char* fmt, ...);

    void acc_print_err(const char* fmt, ...);
    void acc_print_err_with_taskid(int taskId, const char* fmt, ...);

    void acc_data_output( char* data, int size);
    void acc_data_output_with_taskid(int taskid, char* data, int size);


#ifdef __cplusplus
}
#endif