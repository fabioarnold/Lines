const char *APP_TITLE = "Lines";

#include <SDL.h>

#ifdef _WIN32
#include <Windows.h>
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    return main(__argc, __argv);
}
#endif

#include "app.h"

Renderer renderer;
App app;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Could not initialize SDL: %s", SDL_GetError());
        return 1;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES); // fails assertion on Windows/AMD
    int window_x = SDL_WINDOWPOS_UNDEFINED;
    int window_y = SDL_WINDOWPOS_UNDEFINED;
    int window_width = 640;
    int window_height = 480;
    SDL_Window* window = SDL_CreateWindow(APP_TITLE, window_x, window_y, window_width, window_height, SDL_WINDOW_OPENGL);
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

    glFrontFace(GL_CW); // we flipped y in the shader -> need cw winding
    glEnable(GL_CULL_FACE);

    renderer.init();
    app.init();
    Color background_color = randomBackgroundColor();

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

        renderer.begin((float)window_width, (float)window_height);
        renderer.setColor(background_color);
        renderer.clear();
        app.draw(renderer);
        renderer.end();

        SDL_GL_SwapWindow(window);
    }

    app.destroy();
    renderer.destroy();
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}