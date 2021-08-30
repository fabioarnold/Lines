#include "app.h"

void App::draw(Renderer &renderer) {
    float coords[] = {
        200, 200,
        150, 100,
        100, 200,
    };
    renderer.setColor(0xff, 0, 0, 0xff); // red
    renderer.drawPolygon(coords, 3);
}