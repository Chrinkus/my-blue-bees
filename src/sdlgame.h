#ifndef SXC_SDL_GAME_H
#define SXC_SDL_GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Error message helpers
void sdl_error(const char* msg);
void sdl_img_error(const char* msg);
void sdl_ttf_error(const char* msg);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// sdlgame structs and functions
struct sdlgame_data {
	const char* title;
	int screen_width;
	int screen_height;
	Uint32 init_flags;
	Uint32 ctx_flags;
};

struct sdlgame {
	SDL_Window* window;
	SDL_Renderer* context;
};

struct sdlgame* sdlgame_init(struct sdlgame* game, struct sdlgame_data* data);

void sdlgame_cleanup(struct sdlgame* game);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// texture struct and functions
struct texture {
	SDL_Texture* tex;
	int w;
	int h;
};

SDL_Texture* texture_load(struct texture* tex,
		const char* path,
		SDL_Renderer* ctx,
		const SDL_Color* key);

SDL_Texture* texture_load_text(struct texture* tex,
		TTF_Font* font,
		SDL_Renderer* ctx,
		const char* text,
		SDL_Color color);

/**
 * Free-ing textures
 *
 * This function will free the SDL_Texture held by this struct.
 *
 * @param tex a pointer to a texture struct wrapper
 */
void texture_free(struct texture* tex);

/**
 * Rendering textures
 *
 * This function will render a texture with its top-left corner at the provided
 * location. If clip is NULL the whole texture will be rendered.
 *
 * @param tex	a pointer to a texture struct wrapper
 * @param ctx	a pointer to the window rendering context
 * @param pt	where to render the texture (top-left)
 * @param clip	a section of the texture to render or NULL
 */
void texture_render(struct texture* tex,
		SDL_Renderer* ctx,
		const SDL_Point pt,
		const SDL_Rect* clip);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
#endif // SXC_SDL_GAME_H

