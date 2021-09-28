#include <stdlib.h>
#include <stdio.h>

#include "sdlgame.h"

struct sdlgame_data bees_data = {
	.title		= "My Blue Bees",
	.screen_width	= 640,
	.screen_height	= 480,
	.init_flags	= SDL_INIT_VIDEO, // | SDL_INIT_JOYSTICK,
	.ctx_flags	= SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
};

const char* tilemap = "./images/bees_tiles_1.png";

SDL_Texture* texture_load_scaled(struct texture* tex, const char* path,
		SDL_Renderer* ctx, int factor, const SDL_Color* key)
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

	SDL_Rect dst = { 0, 0, surface->w * factor, surface->h * factor };
	SDL_Surface* scaled = SDL_CreateRGBSurface(0, dst.w, dst.h, 32,
			0, 0, 0, 0);	// 0 colour masks
	if (SDL_BlitScaled(surface, NULL, scaled, &dst) < 0) {
		// Do nothing for now.
		sdl_error("Could not scale texture");
	}

	// If this fails we'd be returning NULL anyways. This way we still
	// clean up the surface.
	tex->tex = SDL_CreateTextureFromSurface(ctx, scaled);
	tex->w = scaled->w;
	tex->h = scaled->h;

	SDL_FreeSurface(surface);
	SDL_FreeSurface(scaled);

	return tex->tex;
}

int main(int argc, char* argv[])
{
	struct sdlgame game;

	if (!sdlgame_init(&game, &bees_data)) {
		fprintf(stderr, "%s failed to initialize\n", argv[0]);
		return EXIT_FAILURE;
	}

	if (argc == 2) {
		// Placeholder to silence warning for unused argc
		printf("Load test map %s\n", argv[1]);
	}

	struct texture tex = { NULL, 0, 0 };

	if (!texture_load_scaled(&tex, tilemap, game.context, 4, NULL)) {
		sdl_error("Failed to render texture");
		return EXIT_FAILURE;
	}

	SDL_Event e;
	SDL_Point origin = { 0, 0 };	// not sure about this..

	for (int running = 1; running; ) {

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				running = 0;
				break;
			}
		}

		SDL_SetRenderDrawColor(game.context, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(game.context);

		// Draw more..
		texture_render(&tex, game.context, origin, NULL);

		SDL_RenderPresent(game.context);
	}

	texture_free(&tex);

	sdlgame_cleanup(&game);

	return EXIT_SUCCESS;
}

