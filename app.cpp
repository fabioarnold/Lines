#include "app.h"

void App::draw(Renderer &renderer) {
    float coords[] = {
        0.5f, -0.5f,
        0.0f, 0.5f,
        -0.5f, -0.5f,
    };
    renderer.setColor(0xff, 0, 0, 0xff); // red
    renderer.drawPolygon(coords, 3);
}