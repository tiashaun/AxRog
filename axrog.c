#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <signal.h>
#include <time.h>

#define MIN(x, y)       ((x) < (y) ? (x) : (y))
#define MAX(x, y)       ((x) > (y) ? (x) : (y))

#define SCREEN_WIDTH    1920
#define SCREEN_HEIGHT   1080
#define SCREEN_BPP      32
#define SURF_TYPE       SDL_SWSURFACE
#define FRAMERATE       24

#define CAMSPEED        500

#define TILE_SIZE       32
#define TILES_WIDE      100
#define TILES_HIGH      100

#define SIDEBAR_WIDTH   0

#define ROOM_ATTEMPTS           200
#define ROOM_BRANCH_ATTEMPTS    200
#define ROOM_SIZE()       (rand() % 4 + 4)
#define CORRIDOR_LENGTH() (rand() % 4 + 2)

typedef enum {
    FLOOR = 0,
    WALL = 1,
    DOOR = 2,
    LAST_TILE = 3,
    NOTHING = 255,
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
    Direction direction;
    int x;
    int y;
} Exit;

typedef struct Room Room;
typedef struct {
    SDL_Rect area;
    Exit e1, e2;
    Room *r1, *r2;
} Corridor;

struct Room {
    SDL_Rect area;
    Corridor *corridors[LAST_DIRECTION];
};

static void camera_clip(void);
static void camera_move(SDLKey key);
static void camera_centre_on(SDL_Rect *r);
static void cleanup(void);
static void complete_redraw(void);
static void do_move(Direction d);
static void graphics_init(void);
static void handle_keypress(SDL_KeyboardEvent *key);
static void init(void);
static SDL_Surface* load_image(char *filename);
static Corridor* map_add_corridor(Room *r, Direction d);
static Room* map_add_room(SDL_Rect r, Corridor *c, Direction entered_from);
SDL_Rect map_find_room(Exit *e);
static void map_make(void);
static void map_show_rect(SDL_Rect *r);
static int map_validate_rect(SDL_Rect *r);
static Direction opp_direction(Direction d);
static void run(void);
static void SIG_term(int signal);
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
static Room *rootroom;
static Room *currroom;

/* Torally BROKEN */
static void
camera_centre_on(SDL_Rect *r) {
    Point r_centre;

    r_centre.x = (float)(r->x + (r->w / 2)) * TILE_SIZE;
    r_centre.y = (float)(r->y + (r->h / 2)) * TILE_SIZE;
    r_centre.x -= tileview.clip.w / 2;
    r_centre.y -= tileview.clip.h / 2;

    tileview.clip.x = r_centre.x;
    tileview.clip.y = r_centre.y;

    camera_clip();
    tileview_blit();
}

static void
camera_clip(void) {
     /* Clipping */
    if (tileview.clip.x < 0)
        tileview.clip.x = 0;
    else if (tileview.clip.x + tileview.clip.w > tileview.floorarea->w)
        tileview.clip.x = tileview.floorarea->w - tileview.clip.w;
    if (tileview.clip.y < 0)
        tileview.clip.y = 0;
    else if (tileview.clip.y + tileview.clip.h > tileview.floorarea->h)
        tileview.clip.y = tileview.floorarea->h - tileview.clip.h;
}

static void
camera_move(SDLKey key) {
    switch(key) {
        case SDLK_w:
            tileview.clip.y -= CAMSPEED;
            break;
        case SDLK_s:
            tileview.clip.y += CAMSPEED;
            break;
        case SDLK_a:
            tileview.clip.x -= CAMSPEED;
            break;
        case SDLK_d:
            tileview.clip.x += CAMSPEED;
            break;
    }
    
    camera_clip();
    tileview_blit();
}

static void
cleanup(void) {

}

static void
complete_redraw(void) {
    tileview_blit();
    sidebar_blit();
}

static void
do_move(Direction d) {
    if (!currroom->corridors[d])
        return;

    map_show_rect(&currroom->corridors[d]->area);

    if(currroom->corridors[d]->r1 != currroom)
        currroom = currroom->corridors[d]->r1;
    else 
        currroom = currroom->corridors[d]->r2;

    map_show_rect(&currroom->area);
    camera_centre_on(&currroom->area);
}

static void
graphics_init(void) {
    SDL_Init(SDL_INIT_EVERYTHING);
    /* SDL_EnableKeyRepeat(300, 50); */
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

    tileimg[FLOOR] = load_image("res/tiles/floor.png");
    tileimg[WALL] = load_image("res/tiles/wall.png");
    tileimg[DOOR] = load_image("res/tiles/door.png");

    sidebar_build();
    tileview_build();

    SDL_ShowCursor(0);
    base_frame_delay = 1000 / FRAMERATE;
}

static void
handle_keypress(SDL_KeyboardEvent *key) {
    switch(key->keysym.sym) {
        case SDLK_ESCAPE:
            RUNNING = 0;
            break;
        case SDLK_w:
        case SDLK_a:
        case SDLK_s:
        case SDLK_d:
            camera_move(key->keysym.sym);
            break;
        case SDLK_UP:
            do_move(NORTH);
            break;
        case SDLK_DOWN:
            do_move(SOUTH);
            break;
        case SDLK_LEFT:
            do_move(WEST);
            break;
        case SDLK_RIGHT:
            do_move(EAST);
            break;
        default:
            break;
    }
}

static void
init(void) {
    srand(time(NULL));

    if (signal(SIGTERM, SIG_term) == SIG_ERR) {
        fputs("An error occurred while setting a signal handler.\n", stderr);
        return EXIT_FAILURE;
    }

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

static Corridor*
map_add_corridor(Room *r, Direction d) {
    SDL_Rect room;
    int i = 0;
    int x, y;
    int len;
    Corridor *c;

    c = malloc(sizeof(Corridor));
    c->r2 = r;
    c->e2.direction = opp_direction(d);
    c->e1.direction = d;

    /* handle corridors in different directions */
    if (d == NORTH) {
        c->area.x = r->area.x + rand() % r->area.w;
        c->area.h = CORRIDOR_LENGTH();
        c->area.y = r->area.y - c->area.h - 1;
        c->area.w = 1;
        c->e1.x = c->area.x;
        c->e1.y = c->area.y;
        c->e2.x = c->area.x;
        c->e2.y = r->area.y - 1;
    }
    else if (d == SOUTH) {
        c->area.x = r->area.x + rand() % r->area.w;
        c->area.h = CORRIDOR_LENGTH();
        c->area.y = r->area.y + r->area.h + 1;
        c->area.w = 1;
        c->e1.x = c->area.x;
        c->e1.y = c->area.y + c->area.h;
        c->e2.x = c->area.x;
        c->e2.y = c->area.y - 1;
    }
    else if (d == EAST) {
        c->area.x = r->area.x + r->area.w + 1;
        c->area.w = CORRIDOR_LENGTH();
        c->area.y = r->area.y + rand() % r->area.h;
        c->area.h = 1;
        c->e1.x = c->area.x + c->area.w;
        c->e1.y = c->area.y;
        c->e2.x = c->area.x - 1;
        c->e2.y = c->area.y;
    }
    else { /* WEST */
        c->area.w = CORRIDOR_LENGTH();
        c->area.x = r->area.x - c->area.w - 1;
        c->area.y = r->area.y + rand() % r->area.h;
        c->area.h = 1;
        c->e1.x = c->area.x;
        c->e1.y = c->area.y;
        c->e2.x = r->area.x - 1;
        c->e2.y = c->area.y;
    }

    /* If our attempted corridor doesn't fit */
    if (!map_validate_rect(&c->area)) {
        free(c);
        return NULL;
    }

    /* See if this corridor leads anywhere */
    do {
        room = map_find_room(&c->e1);
    } while (++i < ROOM_ATTEMPTS && !map_validate_rect(&room));

    if (!map_validate_rect(&room)) {
        free(c);
        return NULL;
    }

    /* Draw the corridor and add the room */
    for (y = c->area.y; y < c->area.y + c->area.h; ++y)
        for (x = c->area.x; x < c->area.x + c->area.w; ++x)
            tiles[x][y].base = FLOOR;
    c->r1 = map_add_room(room, c, opp_direction(c->e1.direction));
    /* draw doors */
    tiles[c->e1.x][c->e1.y].base = DOOR;
    tiles[c->e2.x][c->e2.y].base = DOOR;

    return c;
}

static Room*
map_add_room(SDL_Rect r, Corridor *c, Direction entered_from) {
    int x, y;
    int i, e;
    Direction d;
    Room *rm;
    
    rm = malloc(sizeof(Room));
    if (!rm) {
        fputs("Error allocating space for room!", stderr);
        return NULL;
    }

    rm->area = r;
    rm->corridors[NORTH] = NULL;
    rm->corridors[SOUTH] = NULL;
    rm->corridors[EAST] = NULL;
    rm->corridors[WEST] = NULL;

    if (c && entered_from != LAST_DIRECTION)
        rm->corridors[entered_from] = c;

    /* To be gutted out when we have a proper show_room function */
    for (y = r.y; y < r.y + r.h; ++y)
        for (x = r.x; x < r.x + r.w; ++x)
            tiles[x][y].base = FLOOR;

    for (i = 0; i < ROOM_BRANCH_ATTEMPTS; ++i) {
        d = rand() % LAST_DIRECTION;
        if (rm->corridors[d])
            continue;
        rm->corridors[d] = map_add_corridor(rm, d);
    }

    return rm;
}

SDL_Rect
map_find_room(Exit *e) {
    SDL_Rect r;

    r.w = ROOM_SIZE();
    r.h = ROOM_SIZE();

    if (e->direction == NORTH) {
        r.x = e->x - rand() % r.w;
        r.y = e->y - r.h;
    }
    else if (e->direction == SOUTH) {
        r.x = e->x - rand() % r.w;
        r.y = e->y + 1;
    }
    else if (e->direction == EAST) {
        r.x = e->x + 1;
        r.y = e->y - rand() % r.h;
    }
    else { /* WEST */
        r.x = e->x - r.w;
        r.y = e->y - rand() % r.h;
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
            tiles[x][y].base = WALL;
            tiles[x][y].visible = 0;
        }
    }

    do {
        r.x = rand() % TILES_WIDE;
        r.y = rand() % TILES_HIGH;
        r.w = ROOM_SIZE();
        r.h = ROOM_SIZE();
    } while ( !map_validate_rect(&r) );
    currroom = rootroom = map_add_room(r, NULL, LAST_DIRECTION);

    map_show_rect(&currroom->area);
    camera_centre_on(&currroom->area);
}

static void
map_show_rect(SDL_Rect *r) {
    int x, y;
    SDL_Rect pos;

    for (y = r->y - 1; y <= r->y + r->h; ++y) {
        for (x = r->x - 1; x <= r->x + r->w; ++x) {
            if (tiles[x][y].visible)
                continue;

            tiles[x][y].visible = 1;    
            pos.x = x * TILE_SIZE;
            pos.y = y * TILE_SIZE;
            SDL_BlitSurface(tileimg[tiles[x][y].base], NULL, tileview.floorarea, &pos);
        }
    }

}

static int
map_validate_rect(SDL_Rect *r) {
    int x, y;

    if (r->x < 1 || r->y < 1)
        return 0;
    if (r->x + r->w >= TILES_WIDE)
        return 0;
    if (r->y + r->h >= TILES_HIGH)
        return 0;

    for (y = r->y - 1; y <= r->y + r->h; ++y)
        for (x = r->x - 1; x <= r->x + r->w; ++x)
            if (tiles[x][y].base != WALL)
                return 0;

    return 1;
}

static Direction
opp_direction(Direction d) {
    switch(d) {
        case NORTH:
            return SOUTH;
        case SOUTH:
            return NORTH;
        case EAST:
            return WEST;
    }
    return EAST;
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
    if (end - start < base_frame_delay)
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
SIG_term(int signal) {
    puts("Received SIGTERM. Attempting to exit cleanly.");
    RUNNING = 0;
}

static void
tileview_blit(void) {
    SDL_FillRect(screen, &tileview.offset, 0);
    SDL_BlitSurface(tileview.floorarea, &tileview.clip, screen, &tileview.offset);
}

static void
tileview_build(void) {
    int x, y;
    SDL_Rect pos;

    tileview.floorarea = SDL_CreateRGBSurface(SURF_TYPE, TILE_SIZE * TILES_WIDE,
        TILE_SIZE * TILES_HIGH, SCREEN_BPP, screen->format->Rmask,
        screen->format->Gmask, screen->format->Bmask,
        screen->format->Rmask);
    if (!tileview.floorarea) {
        fputs("Failed to create floorarea surface!", stderr);
        exit(1);
    }

    tileview.clip.w = screen->w - sidebar.drawarea->w;
    tileview.clip.h = screen->h;
    tileview.clip.x = 0;
    tileview.clip.y = 0;
    tileview.offset.x = 0;
    tileview.offset.y = 0;
    tileview.offset.w = tileview.clip.w;
    tileview.offset.h = tileview.clip.h;

    map_make();
}

int main(int args, char argv) {
    init();
    graphics_init();
    /* complete_redraw(); */
    while( RUNNING )
        run();

    return 0;
}
