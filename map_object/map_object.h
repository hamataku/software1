#ifndef MAP_H_
#define MAP_H_

typedef struct map_object {
    // make vtable_ a pointer so they can be shared between instances
    // use _ to mark private members
    const struct map_vtable_ *vtable_;
} MapObject;

struct map_vtable_ {
    const char *(*color)(void);
    const char (*object)(void);
};


// wrapper function
static inline const char *color(const MapObject *map) {
    return map->vtable_->color();
}
static inline const char object(const MapObject *map) {
    return map->vtable_->object();
}

// make the vtables arrays so they can be used as pointers
extern const struct map_vtable_ NOTHING[], BOMB[], WALL[];

#endif
