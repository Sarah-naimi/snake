#include "snake.h"

void init_map(game *board, int map_id) {
    for (int i = 0; i < MAX_X; i++) {
        for (int j = 0; j < MAX_Y; j++) {
            board->tab[i][j] = MAP_VOID;
        }
    }
    if (map_id == 0) {
        return;
    }
    if (map_id == 1) {
        for (int i = 0; i < MAX_X; i++) {
            board->tab[i][0] = MAP_WALL;
            board->tab[i][MAX_Y - 1] = MAP_WALL;
        }
        return;
    }
    if (map_id == 2) {
        for (int i = 0; i < MAX_X; i++) {
            board->tab[i][0] = MAP_WALL;
            board->tab[i][MAX_Y - 1] = MAP_WALL;
        }
        for (int j = 0; j < MAX_Y; j++) {
            board->tab[0][j] = MAP_WALL;
            board->tab[MAX_X - 1][j] = MAP_WALL;
        }
        return;
    }
    if (map_id == 3) {
        for (int i = 0; i < MAX_X; i++) {
            if (i < 10 || i >= 20) {
                board->tab[i][0] = MAP_WALL;
                board->tab[i][MAX_Y - 1] = MAP_WALL;
            } else {
                board->tab[i][7] = MAP_WALL;
                board->tab[i][12] = MAP_WALL;
            }
        }
        for (int j = 0; j < MAX_Y; j++) {
            if (j < 5 || j >= 15) {
                board->tab[0][j] = MAP_WALL;
                board->tab[MAX_X - 1][j] = MAP_WALL;
            }
        }
        return;
    }
    board->over = 1;
}

void init_game(game *board, int map_id) {
    //init 2D map
    init_map(board, map_id);
    //free old snake (if exists)
    while (board->snk) {
        snake *tmp = board->snk->next;
        free(board->snk);
        board->snk = tmp;
    }
    //init snake
    board->snk = (snake *) malloc(sizeof(snake));
    board->snk->x = 3;
    board->snk->y = MAX_Y / 2;
    board->snk->dir = 0;
    board->snk->next = (snake *) malloc(sizeof(snake));
    board->snk->next->x = 2;
    board->snk->next->y = MAX_Y / 2;
    board->snk->next->dir = 0;
    board->snk->next->next = (snake *) malloc(sizeof(snake));
    board->snk->next->next->x = 1;
    board->snk->next->next->y = MAX_Y / 2;
    board->snk->next->next->dir = 0;
    board->snk->next->next->next = NULL;
    board->tab[3][MAX_Y / 2] = SNK_HEAD;
    board->tab[2][MAX_Y / 2] = SNK_BODY;
    board->tab[1][MAX_Y / 2] = SNK_TAIL;
    //init food
    board->tab[MAX_X / 2][MAX_Y / 2] = MAP_FOOD;
    //reset state
    board->map = map_id;
    board->score = 0;
    board->pause = 0;
    board->frame = 0;
    board->speed = 6;
    board->over = 0;
}

void turn_snake(game *board, int new_dir) {
    new_dir %= 4;
    if (new_dir < 0) {
        new_dir += 4;
    }
    if (new_dir % 2 == board->snk->next->dir % 2) {
        return;
    }
    board->snk->dir = new_dir;
}

void spawn_food(game *board) {
    int free = 0;
    for (int i = 0; i < MAX_X; i++) {
        for (int j = 0; j < MAX_Y; j++) {
            if (board->tab[i][j] == MAP_VOID) {
                free++;
            }
        }
    }
    int x = rand() % free;
    int count = 0;
    for (int i = 0; i < MAX_X; i++) {
        for (int j = 0; j < MAX_Y; j++) {
            if (board->tab[i][j] == MAP_VOID) {
                if (x == count) {
                    board->tab[i][j] = MAP_FOOD;
                    return;
                }
                count++;
            }
        }
    }
}

void game_step(game *board) {
    //check target cell
    int x = board->snk->x;
    int y = board->snk->y;
    int d = board->snk->dir;
    x = d == 0 ? (x + 1) % MAX_X : d == 2 ? (x - 1) % MAX_X : x;
    y = d == 1 ? (y - 1) % MAX_Y : d == 3 ? (y + 1) % MAX_Y : y;
    if (x < 0) {
        x += MAX_X;
    }
    if (y < 0) {
        y += MAX_Y;
    }
    int trg = board->tab[x][y];
    if (trg > MAP_FOOD) {
        board->over = 1;
        return;
    }
    //move snake head
    board->tab[x][y] = SNK_HEAD;
    board->tab[board->snk->x][board->snk->y] = SNK_BODY;
    snake *snk0 = malloc(sizeof(snake));
    snk0->x = x;
    snk0->y = y;
    snk0->dir = board->snk->dir;
    snk0->next = board->snk;
    board->snk = snk0;
    //if target was food keep tail
    if (trg == MAP_FOOD) {
        board->score++;
        if (board->score % 10 == 0 && board->speed > 3) {
            board->speed--;
        }
        spawn_food(board);
        return;
    }
    //if target was void move tail
    snake *run = board->snk;
    while (run->next && run->next->next) {
        run = run->next;
    }
    board->tab[run->x][run->y] = SNK_TAIL;
    board->tab[run->next->x][run->next->y] = MAP_VOID;
    free(run->next);
    run->next = NULL;
}

Color get_color(int x) {
    if (x == 0) {
        return LIGHTGRAY;
    }
    if (x == 1) {
        return RED;
    }
    if (x == 2) {
        return GRAY;
    }
    if (x == 3) {
        return GREEN;
    }
    if (x == 4) {
        return GOLD;
    }
    if (x == 5) {
        return GOLD;
    }
    return WHITE;
}

void draw_game(game *board) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    for (int i = 0; i < MAX_X; i++) {
        for (int j = 0; j < MAX_Y; j++) {
            DrawRectangle(i * PIXEL_SIZE, j * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE, get_color(board->tab[i][j]));
        }
    }
    for (int i = 0; i < MAX_X; i++) {
        DrawLine(i * PIXEL_SIZE, 0, i * PIXEL_SIZE, MAX_HEIGHT, BLACK);
    }
    for (int j = 0; j < MAX_Y; j++) {
        DrawLine(0, j * PIXEL_SIZE, MAX_WIDTH, j * PIXEL_SIZE, BLACK);
    }
    if (board->pause) {
        DrawRectangle(0, 0, MAX_WIDTH, MAX_HEIGHT, BLUR);
        DrawTexture(menu_p, 0, 245,RAYWHITE);
    }
    if (board->over) {
        DrawRectangle(0, 0, MAX_WIDTH, MAX_HEIGHT, (Color){196, 40, 40, 140});
        DrawTexture(menu_o, 0, 245,RAYWHITE);
    }
    const char *tmp = TextFormat("Score: %d", board->score);
    DrawRectangle(428, 4, 104, 24, BLUR);
    DrawText(tmp, 480 - MeasureText(tmp, 10), 7, 20, BLACK);
    EndDrawing();
}

void init_textures() {
    Image img1 = {PICK_A_DATA,PICK_A_WIDTH,PICK_A_HEIGHT, 1, PICK_A_FORMAT};
    pick_a = LoadTextureFromImage(img1);
    Image img2 = {PICK_B_DATA,PICK_B_WIDTH,PICK_B_HEIGHT, 1, PICK_B_FORMAT};
    pick_b = LoadTextureFromImage(img2);
    Image img3 = {PICK_C_DATA,PICK_C_WIDTH,PICK_C_HEIGHT, 1, PICK_C_FORMAT};
    pick_c = LoadTextureFromImage(img3);
    Image img4 = {PICK_D_DATA,PICK_D_WIDTH,PICK_D_HEIGHT, 1, PICK_D_FORMAT};
    pick_d = LoadTextureFromImage(img4);
    Image img5 = {MENU_P_DATA, MENU_P_WIDTH, MENU_P_HEIGHT, 1, MENU_P_FORMAT};
    menu_p = LoadTextureFromImage(img5);
    Image img6 = {MENU_O_DATA, MENU_O_WIDTH, MENU_O_HEIGHT, 1, MENU_O_FORMAT};
    menu_o = LoadTextureFromImage(img6);
}

void unload_textures() {
    UnloadTexture(pick_a);
    UnloadTexture(pick_b);
    UnloadTexture(pick_c);
    UnloadTexture(pick_d);
    UnloadTexture(menu_p);
    UnloadTexture(menu_o);
}

void main_menu(game *board, int *window) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawTexture(pick_a, 0, 0,RAYWHITE);
    DrawTexture(pick_b, HALF_WIDTH, 0,RAYWHITE);
    DrawTexture(pick_c, 0, HALF_HEIGHT,RAYWHITE);
    DrawTexture(pick_d,HALF_WIDTH,HALF_HEIGHT,RAYWHITE);
    Vector2 pos = GetMousePosition();
    if (pos.x < HALF_WIDTH) {
        DrawRectangle(HALF_WIDTH, 0, HALF_WIDTH, MAX_HEIGHT, BLUR);
        if (pos.y < HALF_HEIGHT) {
            DrawRectangle(0, HALF_HEIGHT, HALF_WIDTH, HALF_HEIGHT, BLUR);
        } else {
            DrawRectangle(0, 0, HALF_WIDTH, HALF_HEIGHT, BLUR);
        }
    } else {
        DrawRectangle(0, 0, HALF_WIDTH, MAX_HEIGHT, BLUR);
        if (pos.y < HALF_HEIGHT) {
            DrawRectangle(HALF_WIDTH, HALF_HEIGHT, HALF_WIDTH, HALF_HEIGHT, BLUR);
        } else {
            DrawRectangle(HALF_WIDTH, 0, HALF_WIDTH, HALF_HEIGHT, BLUR);
        }
    }
    EndDrawing();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        pos = GetMousePosition();
        if (pos.y < HALF_HEIGHT) {
            if (pos.x < HALF_WIDTH) {
                init_game(board, 0);
            } else {
                init_game(board, 1);
            }
        } else {
            if (pos.x < HALF_WIDTH) {
                init_game(board, 2);
            } else {
                init_game(board, 3);
            }
        }
        *window = 1;
    }
}

void game_loop(game *board, int *window) {
    draw_game(board);
    if (board->over) {
        if (IsKeyPressed(KEY_R)) {
            init_game(board, board->map);
        } else if (IsKeyPressed(KEY_ENTER)) {
            *window = 0;
        }
        return;
    }
    if (IsKeyPressed(KEY_BACKSPACE)) {
        *window = 0;
        return;
    }
    if (board->pause) {
        if (IsKeyPressed(KEY_P)) {
            board->pause = 0;
        }
        return;
    }
    if (IsKeyPressed(KEY_P)) {
        board->pause = 1;
        return;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        turn_snake(board, 0);
    } else if (IsKeyDown(KEY_UP)) {
        turn_snake(board, 1);
    } else if (IsKeyDown(KEY_LEFT)) {
        turn_snake(board, 2);
    } else if (IsKeyDown(KEY_DOWN)) {
        turn_snake(board, 3);
    }
    if (board->frame % board->speed == 0) {
        game_step(board);
    }
    board->frame++;
}
