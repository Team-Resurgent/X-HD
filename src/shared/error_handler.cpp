#include "error_handler.h"
#include "debug.h"

extern "C"
void _Error_Handler(char *file, int line) {
    debug_log("Error in file %s at line %d\n", file, line);
    while (1) {}
}
