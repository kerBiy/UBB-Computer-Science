//
// Created by h on 3/10/24.
//

#include "Utility.h"

#include <ctype.h>
#include <stdio.h>

void clear(void) {
    while (getchar() != '\n');
}

void modify_string(char *str) {
    while (*str != '\0') {
        *str = tolower(*str);
        str++;
    }
}