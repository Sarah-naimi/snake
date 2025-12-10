#include "src/snake.h"
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

game board;
int window;

int main() {
    InitWindow(MAX_WIDTH, MAX_HEIGHT, "S~S~SNAKE");
    SetTargetFPS(60);
    init_textures();
    Image logo = {LOGO_DATA,LOGO_WIDTH,LOGO_HEIGHT, 1,LOGO_FORMAT};
    SetWindowIcon(logo);
    while (!WindowShouldClose()) {
        switch (window) {
            case 0:
                main_menu(&board, &window);
                break;
            case 1:
                game_loop(&board, &window);
                break;
        }
    }
    unload_textures();
    CloseWindow();
    return 0;
}
