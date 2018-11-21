#include "fractal.h"

void	draw(t_SDL_staff *SDL_staff, int height)
{
	SDL_UpdateTexture(SDL_staff->texture, NULL, SDL_staff->data, height * 4);

	SDL_SetRenderDrawColor(SDL_staff->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(SDL_staff->renderer);
	
	SDL_RenderCopy(SDL_staff->renderer, SDL_staff->texture, NULL, NULL);
	
	SDL_RenderPresent(SDL_staff->renderer);
}
