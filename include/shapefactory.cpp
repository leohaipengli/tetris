#include "shapefactory.h"

// create a random shape
template<typename T>
Shape *Create(vec2 pivot_position, vec3 color) {
    return new T(pivot_position, color);
}

// FIXME: return to normal
typedef Shape* (*CreateFn)(vec2, vec3);
CreateFn createArray[] = {
    &Create<TShape>,
    &Create<OShape>,
    &Create<IShape>,
    &Create<IShape>,
    &Create<IShape>,
    &Create<IShape>,
    &Create<IShape>,
    &Create<IShape>,
    &Create<IShape>,
    &Create<IShape>,
    &Create<IShape>,
    &Create<IShape>,
    &Create<ZShape>,
    &Create<LShape>,
    &Create<JShape>,
};

const size_t fncount = sizeof(createArray)/sizeof(*createArray);
Shape *Create(vec2 pivot_position, vec3 color) {
   return createArray[rand() % fncount](pivot_position, color); //forward the call
}