#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <time.h>

#define MIN(x, y)       ((x) < (y) ? (x) : (y))
#define MAX(x, y)       ((x) > (y) ? (x) : (y))

#define SCREEN_WIDTH    1920
#define SCREEN_HEIGHT   1080
#define SCREEN_BPP      32
#define SURF_TYPE       SDL_HWSURFACE
#define FRAMERATE       24

#define TILE_SIZE       32
#define TILES_WIDE      100
#define TILES_HIGH      100

#define SIDEBAR_WIDTH   0

#define ROOM_ATTEMPTS   500
#define ROOM_BRANCH_ATTEMPTS     500
#define ROOM_SIZE()       (rand() % 7 + 4)
#define CORRIDOR_LENGTH() (rand() % 5 + 2)

typedef enum {
    NOTHING = 0,
    FLOOR = 1,
    WALL = 2,
    LAVA = 3,
    CLOSED_DOOR = 4,
    OPEN_DOOR = 5,
    LAST_TILE = 6
} TileBase;

typedef enum {
    NORTH = 0,
    SOUTH = 1,
    EAST = 2,
    WEST = 3,
    LAST_DIRECTION = 4
} Direction;

typedef struct {
    TileBase base;
    int visible;
    void *objects;
} Tile;

typedef struct {
    SDL_Surface *drawarea;
    SDL_Rect offset;
} SideBar;

typedef struct {
    SDL_Surface *floorarea;
    SDL_Rect clip;
    SDL_Rect offset;
} TileView;

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point point;
    Direction direction;
} Exit;

static void complete_redraw(void);
static void handle_keypress(SDL_KeyboardEvent *key);
static void init(void);
static SDL_Surface* load_image(char *filename);
static void map_add_corridor(SDL_Rect r);
static void map_add_room(SDL_Rect r);
SDL_Rect map_find_room(Exit *e);
static void map_make(void);
static int map_validate_room(SDL_Rect *r);
static void map_wrap_with_walls(void);
static void run(void);
static void sidebar_blit(void);
static void sidebar_build(void);
static void tileview_blit(void);
static void tileview_build(void);
static int tile_next_to_object(int x, int y);

static SDL_Surface *screen;
static TileView tileview;
static TTF_Font *font;
static SDL_Rect sidebar_offest;
static SideBar sidebar;
static Tile tiles[TILES_WIDE][TILES_HIGH];
static SDL_Surface *tileimg[LAST_TILE];
static Uint32 base_frame_delay;
static int RUNNING;
static SDL_Event event;
static map_making_counter;

static void
complete_redraw(void) {
    tileview_blit();
    sidebar_blit();
}

static void
handle_keypress(SDL_KeyboardEvent *key) {
    switch(key->keysym.sym) {
        case SDLK_ESCAPE:
            RUNNING = 0;
            break;
        default:
            break;
    }
}

static void
init(void) {
    srand(time(NULL));

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_EnableKeyRepeat(1000, 100);
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
        SURF_TYPE);
    if (!screen) {
        fputs("Failed to initialise video!", stderr);
        exit(1);
    }

    TTF_Init();
    font = TTF_OpenFont("res/Font.ttf", 10);
    if (!font) {
        fputs("Failed to initialise font!", stderr);
        exit(1);
    }

    tileimg[NOTHING] = load_image("res/tiles/nothing.png");
    tileimg[FLOOR] = load_image("res/tiles/floor.png");
    tileimg[WALL] = load_image("res/tiles/wall.png");
    tileimg[LAVA] = load_image("res/tiles/lava.png");
    tileimg[CLOSED_DOOR] = load_image("res/tiles/closed_door.png");
    tileimg[OPEN_DOOR] = load_image("res/tiles/open_door.png");

    sidebar_build();
    tileview_build();

    SDL_ShowCursor(0);
    base_frame_delay = 1000 / FRAMERATE;
    RUNNING = 1;
}

static SDL_Surface*
load_image(char *filename) {
    SDL_Surface *s1, *s2;

    s1 = IMG_Load(filename);
    if (s1) {
        s2 = SDL_DisplayFormatAlpha(s1); 
        SDL_FreeSurface(s1);
    }

    if (!s2) {
        fprintf(stderr, "Error loading image %s\n", filename);
        exit(1);
    }

    return s2;
}

static void
map_add_corridor(SDL_Rect r) {
    SDL_Rect corridor;
    SDL_Rect room;
    Point doorin;
    Exit e;
    int i;
    int x, y;

    /* handle corridors in different directions */
    e.direction = rand() % LAST_DIRECTION;
    if (e.direction == NORTH) {
        corridor.x = r.x + rand() % r.w;
        corridor.h = CORRIDOR_LENGTH();
        corridor.y = r.y - corridor.h - 1;
        corridor.w = 1;
        e.point.x = corridor.x;
        e.point.y = corridor.y;
        doorin.x = corridor.x;
        doorin.y = r.y - 1;
    }
    else if (e.direction == SOUTH) {
        corridor.x = r.x + rand() % r.w;
        corridor.h = CORRIDOR_LENGTH();
        corridor.y = r.y + r.h + 1;
        corridor.w = 1;
        e.point.x = corridor.x;
        e.point.y = corridor.y + corridor.h;
        doorin.x = corridor.x;
        doorin.y = corridor.y - 1;
    }
    else if (e.direction == EAST) {
        corridor.x = r.x + r.w + 1;
        corridor.w = CORRIDOR_LENGTH();
        corridor.y = r.y + rand() % r.h;
        corridor.h = 1;
        e.point.x = corridor.x + corridor.w;
        e.point.y = corridor.y;
        doorin.x = corridor.x - 1;
        doorin.y = corridor.y;
    }
    else { /* WEST */
        corridor.w = CORRIDOR_LENGTH();
        corridor.x = r.x - corridor.w - 1;
        corridor.y = r.y + rand() % r.h;
        corridor.h = 1;
        e.point.x = corridor.x;
        e.point.y = corridor.y;
        doorin.x = r.x - 1;
        doorin.y = corridor.y;
    }

    /* If our attempted corridor doesn't fit */
    if (!map_validate_room(&corridor))
        return;

    /* See if this corridor leads anywhere */
    do {
        room = map_find_room(&e);
    } while (++map_making_counter < ROOM_ATTEMPTS && !map_validate_room(&room));

    if (!map_validate_room(&room))
        return;

    /* Draw the corridor and add the room */
    for (y = corridor.y; y < corridor.y + corridor.h; ++y)
        for (x = corridor.x; x < corridor.x + corridor.w; ++x)
            tiles[x][y].base = FLOOR;
    map_add_room(room);
    /* draw doors */
    tiles[e.point.x][e.point.y].base = OPEN_DOOR;
    tiles[doorin.x][doorin.y].base = OPEN_DOOR;
}

static void
map_add_room(SDL_Rect r) {
    int x, y;
    SDL_Rect c;
    int i;

    for (y = r.y; y < r.y + r.h; ++y)
        for (x = r.x; x < r.x + r.w; ++x)
            tiles[x][y].base = FLOOR;

    for (i = 0; i < ROOM_BRANCH_ATTEMPTS; ++i) {
        map_add_corridor(r);
    }
}

SDL_Rect
map_find_room(Exit *e) {
    SDL_Rect r;

    r.w = ROOM_SIZE();
    r.h = ROOM_SIZE();

    if (e->direction == NORTH) {
        r.x = e->point.x - rand() % r.w;
        r.y = e->point.y - r.h;
    }
    else if (e->direction == SOUTH) {
        r.x = e->point.x - rand() % r.w;
        r.y = e->point.y + 1;
    }
    else if (e->direction == EAST) {
        r.x = e->point.x + 1;
        r.y = e->point.y - rand() % r.h;
    }
    else { /* WEST */
        r.x = e->point.x - r.w;
        r.y = e->point.y - rand() % r.h;
    }

    return r;
}

static void
map_make(void) {
    int x, y;
    int x1, y1;
    SDL_Rect r;

    /* Set them all to nothing initially */
    for (y = 0; y < TILES_HIGH; ++y) {
        for (x = 0; x < TILES_WIDE; ++x) {
            tiles[x][y].base = NOTHING;
        }
    }

    map_making_counter = 0;

    r.x = 20;
    r.y = 20;
    r.w = rand() % 5 + 2;
    r.h = rand() % 5 + 2;
    map_add_room(r);
}

static int
map_validate_room(SDL_Rect *r) {
    int x, y;

    if (r->x < 1 || r->y < 1)
        return 0;
    if (r->x + r->w >= TILES_WIDE)
        return 0;
    if (r->y + r->h >= TILES_HIGH)
        return 0;

    for (y = r->y - 1; y <= r->y + r->h; ++y)
        for (x = r->x - 1; x <= r->x + r->w; ++x)
            if (tiles[x][y].base != NOTHING)
                return 0;

    return 1;
}

static void
run(void) {
    Uint32 start;
    Uint32 end;

    start = SDL_GetTicks();
    SDL_Flip(screen);

    while(SDL_PollEvent(&event)) {
        switch(event.type){
            case SDL_KEYDOWN:
                handle_keypress(&event.key);
                break;
            case SDL_KEYUP:
                break;
            case SDL_QUIT:
                RUNNING = 0;
                break;
        }
    }

    end = SDL_GetTicks();
    SDL_Delay(base_frame_delay + start - end);
}

static void
sidebar_blit(void) {
    SDL_BlitSurface(sidebar.drawarea, NULL, screen, &sidebar.offset);
}

static void
sidebar_build(void) {
    SDL_Rect r;

    sidebar.drawarea = SDL_CreateRGBSurface( SURF_TYPE, SIDEBAR_WIDTH,
        screen->h, SCREEN_BPP, screen->format->Rmask, screen->format->Gmask,
        screen->format->Bmask, screen->format->Amask);
    if (!sidebar.drawarea) {
        fputs("Unable to allocate space for sidebar surface!", stderr);
        exit(1);
    }

    r.x = 0;
    r.y = 0;
    r.w = sidebar.drawarea->w;
    r.h = screen->h;
    SDL_FillRect(sidebar.drawarea, &r, 661238567);

    sidebar.offset.x = screen->w - sidebar.drawarea->w;
    sidebar.offset.y = 0;
    sidebar.offset.w = sidebar.drawarea->w;
    sidebar.offset.h = screen->h;
}

static void
tileview_blit(void) {
    SDL_BlitSurface(tileview.floorarea, &tileview.clip, screen, &tileview.offset);
}

static void
tileview_build(void) {
    int x, y;
    SDL_Rect pos;

    map_make();

    tileview.floorarea = SDL_CreateRGBSurface(SURF_TYPE, TILE_SIZE * TILES_WIDE,
        TILE_SIZE * TILES_HIGH, SCREEN_BPP, screen->format->Rmask,
        screen->format->Gmask, screen->format->Bmask,
        screen->format->Rmask);
    if (!tileview.floorarea) {
        fputs("Failed to create floorarea surface!", stderr);
        exit(1);
    }

    for (y = 0; y < TILES_HIGH; ++y) {
        for (x = 0; x < TILES_WIDE; ++x) {
            pos.x = x * TILE_SIZE;
            pos.y = y * TILE_SIZE;
            SDL_BlitSurface(tileimg[tiles[x][y].base], NULL, tileview.floorarea, &pos);
        }
    }

    tileview.clip.w = screen->w - sidebar.drawarea->w;
    tileview.clip.h = screen->h;
    tileview.clip.x = 0;
    tileview.clip.y = 0;
    tileview.offset.x = 0;
    tileview.offset.y = 0;
    tileview.offset.w = tileview.clip.w;
    tileview.offset.h = tileview.clip.h;
}

int main(int args, char argv) {
    init();
    complete_redraw();
    while( RUNNING )
        run();

    return 0;
}
