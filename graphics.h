#ifndef GRAPHICS

#define GRAPHICS

#include <stdlib.h>
#include <SDL3/SDL.h>

#define clear     SDL_RenderClear
#define point     SDL_RenderPoint
#define line      SDL_RenderLine
#define rectangle SDL_RenderRect
#define update    SDL_RenderPresent
#define color     SDL_SetRenderDrawColor
#define render_t  SDL_Renderer

#define PI 3.14159265F
#define swap(t, x, y) { t z = x; x = y; y = z; }

enum {LINE, FILL};

void ellipse(render_t *renderer, int m, float a, float b, float h, float k);
void circle(render_t *renderer, int m, float x, float y, float r);
void polygon(render_t *renderer, int m, float *v, int n);
void mesh(render_t *r, float *v, int *f, int nv, int nf, float ch, float wh);

#endif
