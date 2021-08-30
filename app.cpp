#include "app.h"

#include <cstdlib> // for rand

void App::init() {
    // generate lines

    // 1. Angled, vertical and horizontal lines of different widths & lengths
    float y = 200;
    for (int i = 0; i < 8; i++) {
        Line line;
        line.sx = 50 + (float)i * 60;
        line.sy = y - 100;
        line.ex = 50 + (float)i * 80;
        line.ey = y;
        if (i == 7) line.ey = line.sy; // horizontal line
        line.color = randomForegroundColor();
        line.width = 2 * (float)(i + 1);
        _lines.push_back(line);
    }

    // 2. Angled, vertical and horizontal lines with a width of a single pixel in screen space
    y = 400;
    for (int i = 0; i < 8; i++) {
        Line line;
        line.sx = 50 + (float)i * 60 + 0.5f;
        line.sy = y - 100;
        line.ex = 50 + (float)i * 80 + 0.5f;
        line.ey = y;
        if (i == 7) { // horizontal line
            line.sy += 0.5f; // move to pixel center
            line.ey = line.sy;
        }
        line.color = randomForegroundColor();
        line.width = 1;
        _lines.push_back(line);
    }
}

Color hsvToRgb(float H, float S, float V);
Color randomForegroundColor() {
    return hsvToRgb(rand() % 360, 80, 90);
}
Color randomBackgroundColor() {
    return hsvToRgb(rand() % 360, 10, 100);
}

void App::destroy() {
    _lines.clear();
}

void App::draw(Renderer &renderer) {
    for (auto& line : _lines) {
        renderer.setColor(line.color);
        renderer.drawLine(line.sx, line.sy, line.ex, line.ey, line.width);
    }
}

// from https://www.codespeedy.com/hsv-to-rgb-in-cpp/
Color hsvToRgb(float H, float S, float V) {
    // if(H>360 || H<0 || S>100 || S<0 || V>100 || V<0){
    //     return; // invalid range
    // }
    float s = S/100;
    float v = V/100;
    float C = s*v;
    float X = C*(1-abs(fmod(H/60.0, 2)-1));
    float m = v-C;
    float r,g,b;
    if(H >= 0 && H < 60){
        r = C,g = X,b = 0;
    }
    else if(H >= 60 && H < 120){
        r = X,g = C,b = 0;
    }
    else if(H >= 120 && H < 180){
        r = 0,g = C,b = X;
    }
    else if(H >= 180 && H < 240){
        r = 0,g = X,b = C;
    }
    else if(H >= 240 && H < 300){
        r = X,g = 0,b = C;
    }
    else{
        r = C,g = 0,b = X;
    }
    int R = (r+m)*255;
    int G = (g+m)*255;
    int B = (b+m)*255;
    return Color{(uint8_t)R,(uint8_t)G,(uint8_t)B,0xFF};
}