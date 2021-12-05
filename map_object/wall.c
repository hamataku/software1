#include "map_object.h"

static const char *wall_color(void) {
    return "\x1b[33m";
}

static const char wall_object(void) {
    return '#';
}

const struct map_vtable_ WALL[] = { { wall_color, wall_object } };
