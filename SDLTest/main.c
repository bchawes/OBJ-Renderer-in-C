#include <SDL2/SDL.h>
#include <stdio.h>
#include <tgmath.h>
#define MIN_BRIGHTNESS 0
#define MAX_BRIGHTNESS 255
#define MAX_VERTICES 1000000
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define DEBUG_TARG 2
#define DEBUG_RANGE 0
#define Z_RANGE 1000 // Range of z coordinates to consider for brightness calculation

int x_draw = 320; // Start in the center of the window
int y_draw = 450; // Start in the center of the window
float z_draw = 100; // Start at z = 0
float gamma = 1.1; // Gamma correction value
int brightness;
float moody = 1; // Moodiness factor, 0 means no moodiness, 1 means full moodiness
int scale = 10; // Scale factor for the model
float tick;
int render_distance = 100; // Distance at which objects are rendered
int debug_face_id;
unsigned char update = 1;

struct vector_3 {
    float x, y, z;
    int num;
}typedef vector_3;

struct face {
    int v1, v2, v3;
}typedef face;

struct model {
    vector_3 vertices[MAX_VERTICES];
    face faces[MAX_VERTICES];
    int num_vertices;
    int num_faces;
}typedef model;

struct max_z_coord { //This is my solution for the hidden line problem.
    float y_coord[480]; //For any given x coordinate in an array which holds this struct, y_coord[y] will return the maximum z coordinate for that x,y coordinate.
}typedef max_z_coord; //Thus, you can avoid drawing any pixels that are behind other pixels.


void draw_obj(model *model);
void fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3, float z1, float z2, float z3, SDL_Renderer *ren);
void get_vertices(char *filename, model *model);
void draw_line(int x1, int y1, int x2, int y2, float z1, float z2, SDL_Renderer *ren);
void draw_circle(int x, int y, int radius, SDL_Renderer *ren);

model miku; // Model to hold the miku data
model cow; // Model to hold the cow data
max_z_coord max_x[640]; // For a given x coordinate, say max_x[10].y[10] will return the maximum z coordinate for 10,10

#include <math.h>
void rotateX(vector_3* v, float angle) {
    float rad = angle * M_PI / 180.0;
    float cosA = cos(rad);
    float sinA = sin(rad);
    float y = v->y;
    float z = v->z;
    v->y = y * cosA + z * sinA;
    v->z = -y * sinA + z * cosA;
}
void rotateY(vector_3* v, float angle) {
    float rad = angle * M_PI / 180.0;
    float cosA = cos(rad);
    float sinA = sin(rad);
    float x = v->x;
    float z = v->z;
    v->x = x * cosA + z * sinA;
    v->z = -x * sinA + z * cosA;
}
void rotateZ(vector_3* v, float angle) {
    float rad = angle * M_PI / 180.0;
    float cosA = cos(rad);
    float sinA = sin(rad);
    float x = v->x;
    float y = v->y;
    v->x = x * cosA - y * sinA;
    v->y = x * sinA + y * cosA;
}
void translate_vector(vector_3* v, float tx, float ty, float tz) {
    v->x += tx;
    v->y += ty;
    v->z += tz;
}

void scale_vector(vector_3* v, float scale) {
    v->x *= scale;
    v->y *= scale;
    v->z *= scale;
}


void read_mouse(SDL_Event *e) {
    if (e->type == SDL_MOUSEMOTION && e->motion.state & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        int dx = e->motion.xrel;
        int dy = e->motion.yrel;
        // Move the view based on mouse movement
        x_draw += dx; // Move the view horizontally
        y_draw += dy; // Move the view vertically
        update = 1;
    }
    if (e->type == SDL_MOUSEMOTION && e->motion.state & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
        //rotate around the center of the screen
        int dx = -e->motion.xrel;
        int dy = -e->motion.yrel;
        update = 1;
        // Rotate the view based on mouse movement
        for (int i = 0; i < miku.num_vertices; i++) {
            if (abs(dx)<abs(dy)) {
                rotateX(&miku.vertices[i], dy * 0.1); // Rotate around X axis
            }
            else{
                rotateY(&miku.vertices[i], dx * 0.1); // Rotate around Y axis
            }
        }
    }
    if (e->type == SDL_MOUSEWHEEL) {
        update = 1;
        // Zoom in or out based on mouse wheel movement
        if (e->wheel.y > 0) {
            for (int i = 0; i < miku.num_vertices; i++) {
                scale_vector(&miku.vertices[i], 1.1);
            }
        } else if (e->wheel.y < 0) {
            for (int i = 0; i < miku.num_vertices; i++) {
                scale_vector(&miku.vertices[i], 0.9);
            }
        }
    }
}

void read_keyboard(SDL_Event *e) {
    if (e->type == SDL_KEYDOWN) {
        update = 1;
        switch (e->key.keysym.sym) {
            case SDLK_EQUALS:
                z_draw += 10; // Move up
                break;
            case SDLK_MINUS:
                z_draw -= 10;
                break;
        }
    }
}

int main(void) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    const int width = 640, height = 480;
    SDL_Window *win = SDL_CreateWindow("cow", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (!win) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    //Initialize keyboard and mouse events
    if (!ren) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    // Wait for quit event
    SDL_Event e;
    int quit = 0;

    // Load the model
    //get_vertices("Suzanne.obj", &cow);
    get_vertices("miku.obj", &miku);

    for (int i = 0; i < miku.num_vertices; i++) {
        //rotateY(&miku.vertices[i], 0);
        scale_vector(&miku.vertices[i], 0.5);
        //translate_vector(&miku.vertices[i], 0, 0, 0); // Translate to the center
    }

    while (!quit) {
        for (int i = 0; i < miku.num_vertices; i++) {
            //rotateY(&miku.vertices[i], 1); // Rotate around X axis
        }
        for (int i = 0; i < cow.num_vertices; i++) {
            //rotateY(&cow.vertices[i], 1); // Rotate around X axis
        }
        while (SDL_PollEvent(&e)) {
            read_mouse(&e);
            read_keyboard(&e);
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }
        if (tick >= 180) {
            tick = 0.1;
        }

        if (update){
            for (int i = 0; i < 640; i++) {
                for (int j = 0; j < 480; j++) {
                    max_x[i].y_coord[j] = 0;
                }
            }
            //Green screen
            SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
            SDL_RenderClear(ren);
            draw_obj(&miku); // Draw the miku model
            SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
            SDL_RenderPresent(ren);
            //draw_obj(&cow); // Uncomment to draw the cow model
            update = 0;
            //printf("Frame rendered with %d faces\n", miku.num_faces);
        }
        SDL_Delay(1);
        //clear out max_z
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

void draw_obj(model *model) {
    //reset max z
    SDL_Renderer *ren = SDL_GetRenderer(SDL_GetWindowFromID(1));
    if (!ren) {
        printf("Renderer not found\n");
        return;
    }
    int prev_x = 0, prev_y = 0, prev_z = 0;
    for (int i = 0; i < model->num_faces; i++) {
        int v1 = (int)model->faces[i].v1;
        int v2 = (int)model->faces[i].v2;
        int v3 = (int)model->faces[i].v3;
        debug_face_id = i;
        //Draw vertex points
        //SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
        //SDL_RenderDrawPoint(ren, (int)(model->vertices[v1].x*cos(0) + x_draw), (int)(-model->vertices[v1].y + y_draw));
         fill_triangle(-model->vertices[v1].x*cos(0) + x_draw, -model->vertices[v1].y + y_draw,
                       -model->vertices[v2].x*cos(0) + x_draw, -model->vertices[v2].y + y_draw,
                       -model->vertices[v3].x*cos(0) + x_draw, -model->vertices[v3].y + y_draw,
                         model->vertices[v1].z+z_draw, model->vertices[v2].z+z_draw, model->vertices[v3].z+z_draw, ren);
        }
    }
void get_vertices(char *filename, model *model) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Could not open file: %s\n", filename);
        return;
    }
    int i = 0;
    int j = 0;
    char line[64];
    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] == 'v') { // Vertex line
            if (line[1] == ' '){
                float x, y, z;
                sscanf(line, "v %f %f %f", &x, &y, &z);
                model->vertices[i].x = scale*x;
                model->vertices[i].y = scale*y;
                model->vertices[i].z = scale*z;
                model->vertices[i].num = i;
                i++;
            }
            if (i >= MAX_VERTICES) {
                printf("Vertex array overflow!\n");
                break;
            }
        }
        if (line[0] == 'f') { // Face line
            int v1, v2, v3;

            //if obj file has // inbetween vectors, then we need to parse it differently
            if (strstr(line, "//")) {
                //Parse the face line that goes like f 505//499 323//499 321//499 391//499
                sscanf(line, "f %d//%*d %d//%*d %d//%*d", &v1, &v2, &v3);
            }
            //if obj file has / inbetween vectors, then we need to parse it differently
            else if (strchr(line, '/')) {
                //Parse the face line that goes like f 505/532/499 323/556/499 321/460/499 391/439/499
                sscanf(line, "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &v1, &v2, &v3);
            }
            else {
                sscanf(line, "f %d %d %d", &v1, &v2, &v3);
            }
            model->faces[j].v1 = v1 - 1; // Convert to 0-based index
            model->faces[j].v2 = v2 - 1; // Convert to 0-based index
            model->faces[j].v3 = v3 - 1; // Convert to 0-based index
            j++;
        }
    }
    model->num_faces = j;
    model->num_vertices = i;

    printf("Loaded %d vertices and %d faces from %s\n", i, j, filename);
    fclose(file);
}

void fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3, float z1, float z2, float z3, SDL_Renderer *ren) {
    if (x1 < 0 || x1 >= WINDOW_WIDTH || x2 < 0 || x2 >= WINDOW_WIDTH || x3 < 0 || x3 >= WINDOW_WIDTH ||
        y1 < 0 || y1 >= WINDOW_HEIGHT || y2 < 0 || y2 >= WINDOW_HEIGHT || y3 < 0 || y3 >= WINDOW_HEIGHT) {
        printf("Skipped triangle with out of bounds vertex: (%d, %d), (%d, %d), (%d, %d)\n", x1, y1, x2, y2, x3, y3);
        return; // Skip if any vertex is out of bounds
    }
    // Update max z coordinates for the vertices

    // Sort vertices by y
    if (y1 > y2) { int t=y1; y1=y2; y2=t; t=x1; x1=x2; x2=t; }
    if (y1 > y3) { int t=y1; y1=y3; y3=t; t=x1; x1=x3; x3=t; }
    if (y2 > y3) { int t=y2; y2=y3; y3=t; t=x2; x2=x3; x3=t; }

    float dx13 = (y3 - y1) ? (float)(x3 - x1) / (y3 - y1) : 0;
    float dx12 = (y2 - y1) ? (float)(x2 - x1) / (y2 - y1) : 0;
    float dx23 = (y3 - y2) ? (float)(x3 - x2) / (y3 - y2) : 0;

    float sx13 = x1, sx12 = x1, sx23 = x2;

    for (int y = y1; y <= y3; ++y) {
        if (y < 0 || y >= 480) continue;
        if (y < y2 && y2 != y1) {
            draw_line((int)sx12, y, (int)sx13, y, z1, z3, ren);
            //SDL_RenderDrawLine(ren, (int)sx12, y, (int)sx13, y);

            sx12 += dx12;
            sx13 += dx13;
        } else if (y2 != y3) {
            draw_line((int)sx23, y, (int)sx13, y, z2, z3, ren);
            //SDL_RenderDrawLine(ren, (int)sx23, y, (int)sx13, y);
            sx23 += dx23;
            sx13 += dx13;
        }
    }
}
void draw_line(int x1, int y1, int x2, int y2, float z1, float z2, SDL_Renderer *ren) {
    int dx = abs(x2 - x1); // Difference in x coordinates
    int sx = x1 < x2 ? 1 : -1; // Step in x direction
    int dy = -abs(y2 - y1); // Difference in y coordinates (negative for SDL's coordinate system)
    int sy = y1 < y2 ? 1 : -1; // Step in y direction
    int err = dx + dy; // Error value to determine when to step in x or y direction
    int e2; // Error value for checking both x and y steps
    int n = (abs(x2 - x1) > abs(y2 - y1)) ? abs(x2 - x1) : abs(y2 - y1); // Number of steps to take
    for (int i = 0; i <= n; i++) {
        float z = (z1 + (z2 - z1) * i / n); // Interpolated z value
        int x = x1 + i * sx; // Interpolated x value
        int y = y1 + i * sy; // Interpolated y value
        if (x1 >= 0 && x1 < WINDOW_WIDTH && y1 >= 0 && y1 < WINDOW_HEIGHT) {
            if(-Z_RANGE < z){ // Only draw if the z coordinate is greater than the current max z coordinate for this x,y coordinate
                //max_x[x].y_coord[y] = z; // Update the max z coordinate for this x,y coordinate
                brightness = (int)gamma * (0.5*z + render_distance);
                if (brightness < MIN_BRIGHTNESS) brightness = MIN_BRIGHTNESS;
                if (brightness > MAX_BRIGHTNESS) brightness = MAX_BRIGHTNESS;
                SDL_SetRenderDrawColor(ren, brightness, brightness, brightness, 255);
                SDL_RenderDrawPoint(ren, x1, y1);
            }
            //else {
                //printf("Skipping point at (%d, %d) with z = %f, max z = %f\n", x1, y1, z, max_x[x].y_coord[y]);

            //}
        }
        else {
            continue;
        }
        //if (x1 == x2 && y1 == y2) printf("skipping drawing point at (%d, %d)\n", x1, y1);break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}

void draw_circle(int x, int y, int radius, SDL_Renderer *ren) {
    int x0 = radius - 1;
    int y0 = 0;
    int err = 1 - x0;

    while (x0 >= y0) {
        SDL_RenderDrawPoint(ren, x + x0, y + y0);
        SDL_RenderDrawPoint(ren, x + y0, y + x0);
        SDL_RenderDrawPoint(ren, x - y0, y + x0);
        SDL_RenderDrawPoint(ren, x - x0, y + y0);
        SDL_RenderDrawPoint(ren, x - x0, y - y0);
        SDL_RenderDrawPoint(ren, x - y0, y - x0);
        SDL_RenderDrawPoint(ren, x + y0, y - x0);
        SDL_RenderDrawPoint(ren, x + x0, y - y0);
        if (err <= 0) {
            err += 2 * ++y0 + 1;
        } else {
            err += 2 * (++y0 - --x0);
        }
    }
}

