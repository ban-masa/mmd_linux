#include <iostream>
#include <iconv.h>

void conv_utf16(char* input, char* output, int num)
{
    char *ptr_in = input;
    char *ptr_out = output;
    size_t n_in = (size_t)num;
    size_t n_out = (size_t)1024;
    iconv_t ic;

    ic = iconv_open("UTF8", "UTF16");
    while (0 < n_in) {
      iconv(ic, &ptr_in, &n_in, &ptr_out, &n_out);
    }
    *ptr_out = '\0';
    iconv_close(ic);
}

