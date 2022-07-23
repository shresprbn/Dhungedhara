#include <SDL2/SDL.h>

struct rgba {
	int r, g, b, a;
};

void drawpixel(SDL_Renderer* renderer, int x, int y,rgba c) {
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
	SDL_RenderDrawPoint(renderer, x, y);
}
void drawline(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, rgba c) {
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void DrawTriangle(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int x3, int y3,rgba c) {
	drawline(renderer, x1, y1, x2, y2,c);
	drawline(renderer, x2, y2, x3, y3,c);
	drawline(renderer, x1, y1, x3, y3,c);
}