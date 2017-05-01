#include <iostream>
#include <iconv.h>

void conv_utf16(char* input, char* output)
{
    char *ptr_in = input;
    char *ptr_out = output;
    size_t mybufsz = (size_t)1024;
    iconv_t ic;

    ic = iconv_open("UTF8", "UTF16");
    iconv(ic, &ptr_in, &mybufsz, &ptr_out, &mybufsz);
    iconv_close(ic);
}

