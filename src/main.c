#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

void sdl_error(const char* msg)
{
	fprintf(stderr, "%s! SDL Error: %s\n", msg, SDL_GetError());
}

void sdl_img_error(const char* msg)
{
	fprintf(stderr, "%s! SDL_image Error: %s\n", msg, IMG_GetError());
}

struct sdlgame {
	SDL_Window* window;
	SDL_Renderer* context;
};

struct sdlgame* sdlgame_init(struct sdlgame* game, const char* title,
		const int screen_width, const int screen_height)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		sdl_error("SDL could not initialize");
		return NULL;
	}

	game->window = SDL_CreateWindow(title,
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			screen_width,
			screen_height,
			SDL_WINDOW_SHOWN);

	if (!game->window) {
		sdl_error("Window could not be created");
		return NULL;
	}

	game->context = SDL_CreateRenderer(game->window, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!game->context) {
		sdl_error("Renderer could not be created");
		return NULL;
	}

	int img_flags = IMG_INIT_PNG;
	if (IMG_Init(img_flags) != img_flags) {
		sdl_img_error("SDL_image could not initialize");
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

	IMG_Quit();
	SDL_Quit();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

enum Screen_Dims { SCREEN_DIMS_WIDTH = 640, SCREEN_DIMS_HEIGHT = 480 };

int main(int argc, char* argv[])
{
	struct sdlgame game;

	if (!sdlgame_init(&game, "My Blue Bees",
				SCREEN_DIMS_WIDTH, SCREEN_DIMS_HEIGHT)) {
		fprintf(stderr, "%s failed to initialize\n", argv[0]);
		return EXIT_FAILURE;
	}

	if (argc == 2) {
		// Placeholder to silence warning for unused argc
		printf("Load test map %s\n", argv[1]);
	}

	SDL_Event e;

	for (bool running = true; running; ) {

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				running = false;
				break;
			}
		}

		SDL_SetRenderDrawColor(game.context, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(game.context);

		// Draw more..

		SDL_RenderPresent(game.context);
	}

	sdlgame_cleanup(&game);

	return EXIT_SUCCESS;
}

