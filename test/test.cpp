#include "../include/shape.h"

using namespace std;

void testShape() {
    Shape *s = Create(vec2(0, 0), vec3(1, 1, 1));
    s->print();
    s->move('u');
    s->print();
    s->move('d');
    s->print();
    s->move('l');
    s->print();
    s->move('r');
    s->print();
    s->rotate();
    s->print();
    s->rotate();
    s->print();
    s->rotate();
    s->print();
    s->rotate();
    s->print();
    delete s;
}
int main() {
    srand(time(NULL));
    testShape();
}