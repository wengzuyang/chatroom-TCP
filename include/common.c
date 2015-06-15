#include "chatroom.h"

void error_delt(const char *err, int line)
{
    fprintf(stderr, "line %d ", line);
    perror(err);
    exit(EXIT_FAILURE);
}
