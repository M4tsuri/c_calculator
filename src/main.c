#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "parseopt.h"

int main(int argc, char **argv) {
    FILE *src = get_src(argc, argv);
    FILE *dest = get_dest(argc, argv);

    return 0;
}