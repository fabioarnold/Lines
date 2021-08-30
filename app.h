#include <vector>
using namespace std;

#include "renderer.h"

struct Line {
    float sx, sy;
    float ex, ey;
    float width;
    uint8_t r, g, b, a;
};

class App {
public:
    void draw(Renderer &renderer);

private:
    vector<Line> _lines;
};