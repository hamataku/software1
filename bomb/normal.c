#include "bomb.h"

static const char *color(void)
{
    return "\x1b[31m";
}

const struct bomb_vtable_ NORMAL[] = { { color } };
