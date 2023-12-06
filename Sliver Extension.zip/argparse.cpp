#include "pch.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <cstdint>

#include "argparse.h"

char** parse_args(char* input_string, size_t* arg_count) {

    // the arg string array to return
    char** args = (char**)malloc(MAX_TOKENS * sizeof(char*));

    // count is used to count the args parsed
    size_t count = 0;
    // null chars is used to mark where the arg delimiters are found
    uint32_t* null_chars = (uint32_t*)calloc(MAX_TOKENS, sizeof(uint32_t*));

    // used to detect a quoted arg
    bool quoted_arg = false;

    // enumerate over the string and find arg delimiters
    for (uint32_t i = 0; i < MAX_STRING_LENGTH; ++i)
    {
        // look for spaces or null terminator
        if (input_string[i] == ' ' || input_string[i] == '\0')
        {
            // ignore spaces if in a quoted arg
            if (quoted_arg && input_string[i - 1] != '"')
                continue;
            else
                quoted_arg = false;

            // start of a quoted arg
            if (input_string[i + 1] == '"')
            {
                quoted_arg = true;
            }

            // register where we found the delimiter
            null_chars[count] = i;

            // this is the end of the arg string
            if (input_string[i] == '\0')
                break;

            count++;
        }
    }

    // apply the null byte and assign the args to the array
    uint32_t start_index = 0;
    for (uint32_t i = 0; i < count; ++i)
    {
        input_string[null_chars[i]] = '\0';
        args[i] = input_string + start_index;
        start_index = null_chars[i] + 1;
    }

    // final token
    args[count] = input_string + start_index;

    // there is something a little buggy here...
    // if the final argument is passed using '"an argument"' then garbage is on the end of the buffer
    // this is a result of working around: https://github.com/BishopFox/sliver/issues/1493
    char* final_str = args[count];
    if (*final_str == '"')
    {
        for (uint32_t i = 1; i < MAX_STRING_LENGTH; ++i)
        {
            char c = *final_str + i;
            if (c == '"')
            {
                uint32_t offset = i + 1;
                *(final_str + offset) = '\0';
                break;
            }
        }
    }
    // end - this will do until Sliver v1.6 arrives
    
    // make sure we set the arg count
    *arg_count = count + 1;

    // free the null chars
    free(null_chars);

    // return the args
    return args;
}
