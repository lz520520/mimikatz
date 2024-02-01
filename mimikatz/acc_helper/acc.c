#include "acc.h"
#include "helper.h"



void(*c_print)(int, int, char*, int);
int g_task_id = 0;

void acc_install(install_args_t* install_args) {

    c_print = (void(*)(int, int, char*, int))install_args->log_ptr;
    g_task_id = install_args->task_id;
}

void acc_set_task_id(int task_id) {
    g_task_id = task_id;
}
int acc_get_task_id() {
    return g_task_id;
}


void acc_data_output(char* data, int size) {
    c_print(g_task_id, LOG_DATA_TYPE, data, size);
}

void acc_data_output_with_taskid(int taskid, char* data, int size) {
    c_print(taskid, LOG_DATA_TYPE, data, size);
}

void acc_print_err(const char* fmt, ...) {
    /* Change to maintain internal buffer, and return after done running. */
    int length = 0;

    va_list args;
    va_start(args, fmt);
    length = vsnprintf(NULL, 0, fmt, args);
    va_end(args);
    char* output = NULL;
    output = acc_malloc(length + 1);
    if (output == NULL) {
        return;
    }
    acc_memset(output, 0, length + 1);
    va_start(args, fmt);
    vsnprintf(output, length + 1, fmt, args);
    if (c_print) {
        c_print(g_task_id, LOG_ERROR_TYPE, output, 0);
    }

    va_end(args);
    acc_free(output);
}

void acc_print_err_with_taskid(int taskid, const char* fmt, ...) {
    /* Change to maintain internal buffer, and return after done running. */
    int length = 0;

    va_list args;
    va_start(args, fmt);
    length = vsnprintf(NULL, 0, fmt, args);
    va_end(args);
    char* output = NULL;
    output = acc_malloc(length + 1);
    if (output == NULL) {
        return;
    }
    acc_memset(output, 0, length + 1);
    va_start(args, fmt);
    vsnprintf(output, length + 1, fmt, args);
    if (c_print) {
        c_print(taskid, LOG_ERROR_TYPE, output, 0);
    }

    va_end(args);
    acc_free(output);
}



void acc_print_info(const char* fmt, ...) {
    /* Change to maintain internal buffer, and return after done running. */
    int length = 0;

    va_list args;
    va_start(args, fmt);
    length = vsnprintf(NULL, 0, fmt, args);
    va_end(args);
    char* output = NULL;
    output = acc_malloc(length + 1);
    if (output == NULL) {
        return;
    }
    acc_memset(output, 0, length + 1);
    va_start(args, fmt);
    vsnprintf(output, length + 1, fmt, args);

    //printf("acc printf: %p\n", cPrint);
    if (c_print) {
        c_print(g_task_id, LOG_SUCCESS_TYPE, output, 0);
    }

    va_end(args);
    acc_free(output);
}


void acc_print_info_with_taskid(int taskid, const char* fmt, ...) {
    /* Change to maintain internal buffer, and return after done running. */
    int length = 0;

    va_list args;
    va_start(args, fmt);
    length = vsnprintf(NULL, 0, fmt, args);
    va_end(args);
    char* output = NULL;
    output = acc_malloc(length + 1);
    if (output == NULL) {
        return;
    }
    acc_memset(output, 0, length + 1);
    va_start(args, fmt);
    vsnprintf(output, length + 1, fmt, args);

    //printf("acc printf: %p\n", cPrint);
    if (c_print) {
        c_print(taskid, LOG_SUCCESS_TYPE, output, 0);
    }

    va_end(args);
    acc_free(output);
}


void acc_printf(const char* fmt, ...) {
    /* Change to maintain internal buffer, and return after done running. */
    int length = 0;

    va_list args;
    va_start(args, fmt);
    length = vsnprintf(NULL, 0, fmt, args);
    va_end(args);
    char* output = NULL;
    output = acc_malloc(length + 1);
    if (output == NULL) {
        return;
    }
    acc_memset(output, 0, length + 1);
    va_start(args, fmt);
    vsnprintf(output, length + 1, fmt, args);

    if (c_print) {
        c_print(g_task_id, LOG_INFO_TYPE, output, 0);
    }

    va_end(args);
    acc_free(output);
}

void acc_printf_with_taskid(int taskid, const char* fmt, ...) {
    /* Change to maintain internal buffer, and return after done running. */
    int length = 0;

    va_list args;
    va_start(args, fmt);
    length = vsnprintf(NULL, 0, fmt, args);
    va_end(args);
    char* output = NULL;
    output = acc_malloc(length + 1);
    if (output == NULL) {
        return;
    }
    acc_memset(output, 0, length + 1);
    va_start(args, fmt);
    vsnprintf(output, length + 1, fmt, args);

    if (c_print) {
        c_print(taskid, LOG_INFO_TYPE, output, 0);
    }

    va_end(args);
    acc_free(output);
}

void acc_data_parse(datap* parser, char* buffer, int size) {
    if (parser == NULL) {
        return;
    }
    parser->original = buffer;
    parser->buffer = buffer;
    parser->length = size - 4;
    parser->size = size - 4;
    parser->buffer += 4;
    return;
}

int acc_data_int(datap* parser) {
    int32_t fourbyteint = 0;
    if (parser->length < 4) {
        return 0;
    }
    acc_memcpy(&fourbyteint, parser->buffer, 4);
    parser->buffer += 4;
    parser->length -= 4;
    return (int)fourbyteint;
}

short acc_data_short(datap* parser) {
    int16_t retvalue = 0;
    if (parser->length < 2) {
        return 0;
    }
    acc_memcpy(&retvalue, parser->buffer, 2);
    parser->buffer += 2;
    parser->length -= 2;
    return (short)retvalue;
}

char* acc_data_extract(datap* parser, int* size) {
    uint32_t length = 0;
    char* outdata = NULL;
    /*Length prefixed binary blob, going to assume uint32_t for this.*/
    if (parser->length < 4) {
        return NULL;
    }
    acc_memcpy(&length, parser->buffer, 4);
    parser->buffer += 4;

    outdata = parser->buffer;
    if (outdata == NULL) {
        return NULL;
    }
    parser->length -= 4;
    parser->length -= length;
    parser->buffer += length;
    if (size != NULL && outdata != NULL) {
        *size = length;
    }
    return outdata;
}
int acc_data_length(datap* parser) {
    return parser->length;
}


char* unhexlify(char* value, int* outlen) {
    unsigned char* retval = NULL;
    char byteval[3] = { 0 };
    unsigned int counter = 0;
    int counter2 = 0;
    char character = 0;
    if (value == NULL) {
        return NULL;
    }
    if (value == NULL || strlen((char*)value) % 2 != 0) {
        goto errcase;
    }

    retval = acc_calloc(strlen((char*)value) + 1, 1);
    if (retval == NULL) {
        goto errcase;
    }

    counter2 = 0;
    for (counter = 0; counter < strlen((char*)value); counter += 2) {
        acc_memcpy(byteval, value + counter, 2);
        character = (char)strtol(byteval, NULL, 16);
        acc_memcpy(retval + counter2, &character, 1);
        counter2++;
    }
    *outlen = counter2;

errcase:
    return retval;
}

uint32_t swap_endianess(uint32_t indata) {
    uint32_t testint = 0xaabbccdd;
    uint32_t outint = indata;
    if (((unsigned char*)&testint)[0] == 0xdd) {
        ((unsigned char*)&outint)[0] = ((unsigned char*)&indata)[3];
        ((unsigned char*)&outint)[1] = ((unsigned char*)&indata)[2];
        ((unsigned char*)&outint)[2] = ((unsigned char*)&indata)[1];
        ((unsigned char*)&outint)[3] = ((unsigned char*)&indata)[0];
    }
    return outint;
}




/* format API */

void acc_format_alloc(formatp* format, int maxsz) {
    if (format == NULL) {
        return;
    }
    format->original = acc_calloc(maxsz, 1);
    format->buffer = format->original;
    format->length = 0;
    format->size = maxsz;
    return;
}

void acc_format_reset(formatp* format) {
    acc_memset(format->original, 0, format->size);
    format->buffer = format->original;
    format->length = format->size;
    return;
}

void acc_format_free(formatp* format) {
    if (format == NULL) {
        return;
    }
    if (format->original) {
        acc_free(format->original);
        format->original = NULL;
    }
    format->buffer = NULL;
    format->length = 0;
    format->size = 0;
    return;
}

void acc_format_append(formatp* format, char* text, int len) {
    acc_memcpy(format->buffer, text, len);
    format->buffer += len;
    format->length += len;
    return;
}

void acc_format_printf(formatp* format, char* fmt, ...) {
    /*Take format string, and sprintf it into here*/
    va_list args;
    int length = 0;

    va_start(args, fmt);
    length = vsnprintf(NULL, 0, fmt, args);
    va_end(args);
    if (format->length + length > format->size) {
        return;
    }

    va_start(args, fmt);
    (void)vsnprintf(format->buffer, length, fmt, args);
    va_end(args);
    format->length += length;
    format->buffer += length;
    return;
}


char* acc_format_tostring(formatp* format, int* size) {
    *size = format->length;
    return format->original;
}

void acc_format_int(formatp* format, int value) {
    uint32_t indata = value;
    uint32_t outdata = 0;
    if (format->length + 4 > format->size) {
        return;
    }
    outdata = swap_endianess(indata);
    acc_memcpy(format->buffer, &outdata, 4);
    format->length += 4;
    format->buffer += 4;
    return;
}


/* Token Functions */

BOOL acc_use_token(HANDLE token) {
    /* Probably needs to handle DuplicateTokenEx too */
    return SetThreadToken(NULL, token);
}

BOOL acc_revert_token(void) {
    return !RevertToSelf();
}
