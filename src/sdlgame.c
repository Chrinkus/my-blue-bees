#include <stdio.h>

#include "sdlgame.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Error message helpers
void sdl_error(const char* msg)
{
	fprintf(stderr, "%s! SDL Error: %s\n", msg, SDL_GetError());
}

void sdl_img_error(const char* msg)
{
	fprintf(stderr, "%s! SDL_image Error: %s\n", msg, IMG_GetError());
}

void sdl_ttf_error(const char* msg)
{
	fprintf(stderr, "%s! SDL_ttf Error: %s\n", msg, TTF_GetError());
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// sdlgame struct functions
struct sdlgame* sdlgame_init(struct sdlgame* game, struct sdlgame_data* data)
{
	if (SDL_Init(data->init_flags) < 0) {
		sdl_error("SDL could not initialize");
		return NULL;
	}

	game->window = SDL_CreateWindow(data->title,
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			data->screen_width,
			data->screen_height,
			SDL_WINDOW_SHOWN);

	if (!game->window) {
		sdl_error("Window could not be created");
		return NULL;
	}

	game->context = SDL_CreateRenderer(game->window, -1, data->ctx_flags);

	if (!game->context) {
		sdl_error("Renderer could not be created");
		return NULL;
	}

	// Needed to process PNG files
	int img_flags = IMG_INIT_PNG;
	if (IMG_Init(img_flags) != img_flags) {
		sdl_img_error("SDL_image could not initialize");
		return NULL;
	}

	// Needed to render fonts
	if (TTF_Init() == -1) {
		sdl_ttf_error("SDL_ttf could not initialize");
		return NULL;
	}

	return game;
}

void sdlgame_cleanup(struct sdlgame* game)
{
	SDL_DestroyRenderer(game->context);
	SDL_DestroyWindow(game->window);
	game->context = NULL;
	game->window = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// texture struct functions
SDL_Texture* texture_load(struct texture* tex, const char* path,
		SDL_Renderer* ctx, const SDL_Color* key)
{
	SDL_Surface* surface = IMG_Load(path);
	if (!surface) {
		sdl_img_error("Could not load image");
		return NULL;
	}

	if (key) {
		SDL_SetColorKey(surface, SDL_TRUE,
				SDL_MapRGB(surface->format,
					key->r, key->g, key->b));
	}
	// If this fails we'd be returning NULL anyways. This way we still
	// clean up the surface.
	tex->tex = SDL_CreateTextureFromSurface(ctx, surface);
	tex->w = surface->w;
	tex->h = surface->h;

	SDL_FreeSurface(surface);

	return tex->tex;
}

SDL_Texture* texture_load_text(struct texture* tex, TTF_Font* font,
		SDL_Renderer* ctx, const char* text, SDL_Color color)
{
	SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
	if (!surface) {
		sdl_ttf_error("Unable to render surface from font");
		return NULL;
	}
	// If this fails we'd be returning NULL anyways. This way we still
	// clean up the surface.
	tex->tex = SDL_CreateTextureFromSurface(ctx, surface);
	tex->w = surface->w;
	tex->h = surface->h;

	SDL_FreeSurface(surface);

	return tex->tex;
}

void texture_free(struct texture* tex)
{
	if (tex->tex) {
		SDL_DestroyTexture(tex->tex);
		tex->tex = NULL;
		tex->w = 0;
		tex->h = 0;
	}
}

void texture_render(struct texture* tex, SDL_Renderer* ctx, const SDL_Point pt,
		const SDL_Rect* clip)
{
	SDL_Rect render_quad = { pt.x, pt.y, tex->w, tex->h };

	if (clip) {
		render_quad.w = clip->w;
		render_quad.h = clip->h;
	}

	SDL_RenderCopy(ctx, tex->tex, clip, &render_quad);
}

