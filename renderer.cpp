#include "renderer.h"

#include <cmath> // for sqrtf
#include <cstddef> // for offsetof macro

const char *vs_src = 
    "#ifdef GL_ES\n"
    "    precision mediump float;\n"
    "#endif\n"
    "uniform vec2 u_resolution;\n"
    "attribute vec2 a_position;\n"
    "attribute vec4 a_color;\n"
    "varying vec4 v_color;\n"
    "void main() {\n"
    "    v_color = a_color;\n"
    "    gl_Position = vec4(vec2(2,-2) * a_position / u_resolution + vec2(-1,1), 0, 1);\n"
    "}\n";

const char *fs_src = 
    "varying vec4 v_color;\n"
    "void main() {\n"
    "    gl_FragColor = v_color;\n"
    "}\n";

void Renderer::init() {
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), nullptr, GL_STREAM_DRAW);

    // init basic shader
    _program = glCreateProgram();
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_src, nullptr);
    glCompileShader(vs);
    glAttachShader(_program, vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_src, nullptr);
    glCompileShader(fs);
    glAttachShader(_program, fs);
    glBindAttribLocation(_program, 0, "a_position");
    glBindAttribLocation(_program, 1, "a_color");
    glLinkProgram(_program);
    // TODO: error handling and optionally print shader compile log
    glUseProgram(_program);
    _resolution_loc = glGetUniformLocation(_program, "u_resolution");
}

void Renderer::destroy() {
    glDeleteBuffers(1, &_vbo);
    glDeleteProgram(_program);
}

void Renderer::begin(float width, float height) {
    _num_vertices = 0;

    glUseProgram(_program);
    glUniform2f(_resolution_loc, width, height);
}

void Renderer::end() {
    if (_num_vertices == 0) return;

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * _num_vertices, _vertices);

    glUseProgram(_program);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glDrawArrays(GL_TRIANGLES, 0, _num_vertices);
}

void Renderer::clear() {
    glClearColor((GLfloat)_color.r/255.0f, (GLfloat)_color.g/255.0f, (GLfloat)_color.b/255.0f, (GLfloat)_color.a/255.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::drawPolygon(float *coords, unsigned num_coords) {
    for (unsigned i = 2; i < num_coords; i++) {
        addVertex(coords[0], coords[1], _color);
        addVertex(coords[2 * (i-1) + 0], coords[2 * (i-1) + 1], _color);
        addVertex(coords[2 * i + 0], coords[2 * i + 1], _color);
    }
}

void Renderer::addVertex(float x, float y, const Color &color) {
    if (_num_vertices + 1 > MAX_VERTICES) return;
    _vertices[_num_vertices].pos[0] = x;
    _vertices[_num_vertices].pos[1] = y;
    _vertices[_num_vertices].color = color;
    _num_vertices++;
}

void Renderer::drawLine(float sx, float sy, float ex, float ey, float w) {
    float dx = ex - sx;
    float dy = ey - sy;
    float d = sqrtf(dx*dx + dy*dy);
    if (d > 0.0001f) {
        // normalize
        dx /= d;
        dy /= d;
    }
    // normal
    float nx = dy;
    float ny = -dx;
    w *= 0.5f;
    dx *= w;
    dy *= w;
    nx *= w;
    ny *= w;

    float coords[2*4];
    coords[0] = sx - dx - nx;
    coords[1] = sy - dy - ny;
    coords[2] = sx - dx + nx;
    coords[3] = sy - dy + ny;
    coords[4] = ex + dx + nx;
    coords[5] = ey + dy + ny;
    coords[6] = ex + dx - nx;
    coords[7] = ey + dy - ny;
    drawPolygon(coords, 4);
}