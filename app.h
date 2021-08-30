#include <vector>
using namespace std;

#include "renderer.h"

struct Line {
    float sx, sy;
    float ex, ey;
    float width;
    Color color;
};

Color randomForegroundColor();
Color randomBackgroundColor();

class App {
public:
    void init();
    void destroy();

    void draw(Renderer &renderer);

private:
    vector<Line> _lines;
};