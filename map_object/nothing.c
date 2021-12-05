#include "map_object.h"

static const char *nothing_color(void) {
    return "\x1b[39m";
}

static const char nothing_object(void) {
    return '-';
}

const struct map_vtable_ nothing[] = { { nothing_color, nothing_object } };
