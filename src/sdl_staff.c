#include "fractal.h"
#define FAIL_VIDEO "Error: Can't init video"
#define FAIL_WINDOW "Error: Can't create window"

void		free_sdl(t_SDL_staff **SDL_staff)
{
	SDL_DestroyTexture((*SDL_staff)->texture);
	ft_memdel((void **)&(*SDL_staff)->data);
	SDL_DestroyRenderer((*SDL_staff)->renderer);
	SDL_DestroyWindow((*SDL_staff)->window);
	ft_memdel((void **)SDL_staff);
}

t_SDL_staff *init_sdl(char *window_name)
{
	t_SDL_staff 	*SDL_staff;

	SDL_staff = (t_SDL_staff *)ft_memalloc(sizeof(t_SDL_staff));
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		error(FAIL_VIDEO);
	SDL_staff->window = SDL_CreateWindow(
		window_name,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WIDTH,
		HEIGHT,
		0
	);
	SDL_staff->renderer = SDL_CreateRenderer(SDL_staff->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_staff->texture = SDL_CreateTexture(SDL_staff->renderer,
										 SDL_PIXELFORMAT_RGB888,
										 SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);;
	SDL_staff->data_len = sizeof(int) * WIDTH * HEIGHT;
	SDL_staff->data = ft_memalloc(SDL_staff->data_len);
	if (SDL_staff->window == NULL)
		error(FAIL_WINDOW);
	return (SDL_staff);
}