#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <time.h>

#define SCREEN_WIDTH    1920
#define SCREEN_HEIGHT   1080
#define SCREEN_BPP      32
#define SURF_TYPE       SDL_HWSURFACE
#define FRAMERATE       24

#define TILE_SIZE       32
#define TILES_WIDE      100
#define TILES_HIGH      100

#define SIDEBAR_WIDTH   200

#define ROOM_ATTEMPTS   200

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

static void complete_redraw(void);
static void handle_keypress(SDL_KeyboardEvent *key);
static void init(void);
static SDL_Surface* load_image(char *filename);
static void map_add_rect(SDL_Rect *r);
static SDL_Rect map_get_corridor(SDL_Rect *start);
static SDL_Rect map_get_room(SDL_Rect *start);
static int map_rect_fits(SDL_Rect *r);
static void map_make(void);
static void run(void);
static void sidebar_blit(void);
static void sidebar_build(void);
static void tileview_blit(void);
static void tileview_build(void);

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
map_add_rect(SDL_Rect *r) {
    int x, y;

    /* /Make the floor tiles */
    for (y = 0; y < r->h; ++y) {
        for (x = 0; x < r->w; ++x) {
            tiles[x + r->x][y + r->y].base = FLOOR;
        }
    }
}

static SDL_Rect
map_get_corridor(SDL_Rect *start) {
    SDL_Rect r;
    Direction d;

    d = rand() % LAST_DIRECTION;

    /* We assume straight and east for now */

    /* if (d == NORTH) { */
        r.h = rand() % 10 + 2;
        r.y = start->y - r.h;
        r.x = start->x + rand() % start->w;
        r.w = 1;
    /* } */
    /* else { */
    /*     r.x = start->x + start->w; */
    /*     r.y = start->y + rand() % start->h; */
    /*     r.w = rand() % 10 + 2; */
    /*     r.h = 1; */
    /* } */

    return r;
}

static SDL_Rect
map_get_room(SDL_Rect *start) {
    SDL_Rect r;
    Direction d;

    d = rand() % LAST_DIRECTION;

    /* We assume directly to the east for now */
    r.x = start->x + start->w;
    r.w = rand() % 5 + 4;
    r.h = rand() % 5 + 4;
    r.y = start->y - rand() % r.h;

    return r;
}

static int
map_rect_fits(SDL_Rect *r) {
    int x, y;
    int neighbour_count = 0;

    /* Make sure we're within the map's limits */
    if (r->x + r->w >= TILES_WIDE - 1)
        return 0;
    if (r->y + r->h >= TILES_HIGH - 1)
        return 0;
    if (r->x <= 0 || r->y <= 0)
        return 0;

    /* Make sure we're not covering any already-claimed floor */
    for (y = -1; y <= r->h; ++y) {
        for (x = -1; x <= r->w; ++x) {
            if (tiles[x + r->x][y + r->y].base == FLOOR) {
                if (++neighbour_count == 3 )
                    return 0;
            }
        }
    }

    return 1;
}

/* This really needs to be re-arranged to work recursively */
static void
map_make(void) {
    int x, y;
    int attempts;
    SDL_Rect r;
    SDL_Rect c;

    /* Set them all to nothing initially */
    for (y = 0; y < TILES_HIGH; ++y) {
        for (x = 0; x < TILES_WIDE; ++x) {
            tiles[x][y].base = NOTHING;
        }
    }

    /* Get an initial first room */
    attempts = 0;
    do {
        /* r.x = rand() % TILES_WIDE; */
        /* r.y = rand() % TILES_HIGH; */
        r.x = 20; /* Temporary */
        r.y = 20; /* Temporary */
        r.w = rand() % 5 + 4;
        r.h = rand() % 5 + 4;
        ++attempts;
    } while (! map_rect_fits(&r) && attempts < ROOM_ATTEMPTS);
    map_add_rect(&r);

    if (attempts == ROOM_ATTEMPTS) {
        fputs("Error trying to place first room!", stderr);
        exit(1);
    }

    /* Add corridors and rooms in sequence */
    while(attempts < ROOM_ATTEMPTS) {
        /* Add a corridor based upon last room */
        do {
            puts("Ding!");
            c = map_get_corridor(&r);
            ++attempts;
        } while ( !map_rect_fits(&c) && attempts < ROOM_ATTEMPTS );

        /* Add a room based upon the last corridor */
        do {
            puts("Dong!");
            r = map_get_room(&c);
            ++attempts;
        } while ( !map_rect_fits(&r) && attempts < ROOM_ATTEMPTS );

        if (attempts >= ROOM_ATTEMPTS)
            break;

        map_add_rect(&c);
        map_add_rect(&r);
    }
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
