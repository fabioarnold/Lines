const char *APP_TITLE = "Lines";

#include <stdio.h>
#include <SDL.h>
#include <glad/glad.h> // OpenGLES 2.0

#ifdef _WIN32
#include <Windows.h>
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    return main(__argc, __argv);
}
#endif

struct Vertex {
    float pos[2];
    uint8_t r, g, b, a;
};

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

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Could not initialize SDL: %s", SDL_GetError());
        return 1;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES); // fails assertion on win64 AMD
    int window_x = SDL_WINDOWPOS_UNDEFINED;
    int window_y = SDL_WINDOWPOS_UNDEFINED;
    SDL_Window* window = SDL_CreateWindow(APP_TITLE, window_x, window_y, 640, 480, SDL_WINDOW_OPENGL);
    if (window == NULL) {
        SDL_Log("Could not create window: %s", SDL_GetError());
        return 1;
    }
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context == NULL) {
        SDL_Log("Could not create OpenGL context: %s", SDL_GetError());
        return 1;
    }
    gladLoadGLES2Loader(SDL_GL_GetProcAddress);

    Vertex data[3];
    memset(data, 0, sizeof(data));
    data[0].pos[0] = 0.5;
    data[0].pos[1] = -0.5;
    data[0].a = 1;
    data[1].pos[0] = 0;
    data[1].pos[1] = 0.5;
    data[1].a = 1;
    data[2].pos[0] = -0.5;
    data[2].pos[1] = -0.5;
    data[2].a = 1;

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    GLuint program = glCreateProgram();
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_src, NULL);
    glCompileShader(vs);
    glAttachShader(program, vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_src, NULL);
    glCompileShader(fs);
    glAttachShader(program, fs);
    glBindAttribLocation(program, 0, "a_position");
    glBindAttribLocation(program, 1, "a_color");
    glLinkProgram(program);
    glUseProgram(program);

    bool quit = false;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) quit = true;
                    break;
            }
        }

        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, r));
        glDrawArrays(GL_TRIANGLES, 0, 3);
        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}