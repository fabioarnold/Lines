#include <glad/glad.h>
#include <stdint.h>

struct Color {
    uint8_t r, g, b, a;
};

struct Vertex {
    float pos[2];
    Color color;
};

const int MAX_VERTICES = 1000; // increase as needed

/// Simple renderer for drawing 2D primitives
class Renderer {
public:
    // Initialize renderer, OpenGL needs to be ready before calling this
    void init();
    void destroy();

    /// All drawing should be wrapped by begin and end, end draws everything to the screen
    void begin(float width, float height);
    void end();

    /// Set current color for drawing
    void setColor(const Color& color) {_color = color;};
    /// Clear screen using current color
    void clear();
    /// Draw a polygon fan
    void drawPolygon(float *coords, unsigned num_coords);
    /// Draw a line from start to end with a certain width
    void drawLine(float sx, float sy, float ex, float ey, float w);

private:
    void addVertex(float x, float y, const Color &color);
    Vertex _vertices[MAX_VERTICES];
    unsigned _num_vertices;
    GLuint _vbo;
    GLuint _program;
    GLuint _resolution_loc;
    Color _color;
};