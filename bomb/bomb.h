#ifndef BOMB_H_
#define BOMB_H_

typedef struct bomb {
    // make vtable_ a pointer so they can be shared between instances
    // use _ to mark private members
    const struct bomb_vtable_ *vtable_;
    int t;
} Bomb;

struct bomb_vtable_
{
    const char *(*color)(void);
};

// wrapper function
static inline const char *bomb_color(const Bomb *bomb)
{
    return bomb->vtable_->color();
}

// make the vtables arrays so they can be used as pointers
extern const struct bomb_vtable_ NORMAL[];

#endif
