#include "map_object.h"

static const char *bomb_color(void) {
    return "\x1b[31m";
}

static const char bomb_object(void) {
    return 'B';
}

const struct map_vtable_ BOMB[] = { { bomb_color, bomb_object } };
