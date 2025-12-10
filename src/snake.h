#ifndef SNAKE_H
#define SNAKE_H
#include <stdlib.h>
#include <raylib.h>
#include "../textures/logo.h"
#include "../textures/pick_a.h"
#include "../textures/pick_b.h"
#include "../textures/pick_c.h"
#include "../textures/pick_d.h"
#include "../textures/menu_p.h"
#include "../textures/menu_o.h"
#define MAP_VOID 0
#define MAP_FOOD 1
#define MAP_WALL 2
#define SNK_HEAD 3
#define SNK_BODY 4
#define SNK_TAIL 5
#define MAX_X 30
#define MAX_Y 20
#define PIXEL_SIZE 32
#define MAX_WIDTH 960
#define MAX_HEIGHT 640
#define HALF_WIDTH 480
#define HALF_HEIGHT 320
#define BLUR (Color){245,245,245,200}

static Texture2D pick_a;
static Texture2D pick_b;
static Texture2D pick_c;
static Texture2D pick_d;
static Texture2D menu_p;
static Texture2D menu_o;

typedef struct snake {
    int x;
    int y;
    int dir;
    struct snake *next;
} snake;

typedef struct {
    int tab[MAX_X][MAX_Y];
    snake *snk;
    int map;
    int score;
    int pause;
    int frame;
    int speed;
    int over;
} game;

static void init_map(game *board, int map_id);

static void init_game(game *board, int map_id);

static void turn_snake(game *board, int new_dir);

static void spawn_food(game *board);

static void game_step(game *board);

static Color get_color(int x);

static void draw_game(game *board);

void init_textures();

void unload_textures();

void main_menu(game *board, int *window);

void game_loop(game *board, int *window);
#endif //SNAKE_H
