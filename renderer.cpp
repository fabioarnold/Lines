#include "renderer.h"

#include <cstddef> // for offsetof macro

const char *vs_src = 
    "#ifdef GL_ES\n"
    "    precision mediump float;\n"
    "#endif\n"
    "attribute vec2 a_position;\n"
    "attribute vec4 a_color;\n"
    "varying vec4 v_color;\n"
    "void main() {\n"
    "    v_color = a_color;\n"
    "    gl_Position = vec4(a_position, 0, 1);\n"
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
    // TODO: error handling and shader compile log printing
}

void Renderer::begin() {
    _num_vertices = 0;
}

void Renderer::end() {
    if (_num_vertices == 0) return;
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * _num_vertices, _vertices);
    glUseProgram(_program);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glDrawArrays(GL_TRIANGLES, 0, _num_vertices);
}

void Renderer::drawPolygon(float *coords, unsigned num_coords) {
    for (unsigned i = 2; i < num_coords; i++) {
        drawTriangle(&coords[(i-2)*2]);
    }
}

void Renderer::drawTriangle(float *coords) {
    if (_num_vertices + 3 > MAX_VERTICES) return;

    Vertex *v = &_vertices[_num_vertices];
    for (unsigned i = 0; i < 3; i++) {
        v[i].pos[0] = coords[2 * i + 0];
        v[i].pos[1] = coords[2 * i + 1];
        v[i].color = _color;
    }
    _num_vertices += 3;
}