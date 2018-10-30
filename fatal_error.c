//Modify by Daniel Charua - A01017419 - 17/10/18


#include "fatal_error.h"

void fatalError(const char * message)
{
    perror(message);
    exit(EXIT_FAILURE);
}
