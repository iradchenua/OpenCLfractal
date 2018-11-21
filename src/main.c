#include "fractal.h"


long load_file_str(char *fname, char **str)
{
	long flen;
	FILE *fp;

	if ((fp = fopen(fname, "r")) == NULL)
		error("error in file opening");
	fseek(fp, 0, SEEK_END);
	flen = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	if ((*str = malloc((size_t)flen + 1)) == NULL)
		return (-1);
	bzero((void*)*str, flen+1);
	fread(*str, (size_t)flen, 1, fp);
	fclose (fp);
	return (flen);
}

void			key(int *done, t_fractal_params *fractal_params)
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT)
			*done = 1;
		if( e.type == SDL_MOUSEMOTION ) {

			fractal_params->JuliaCx =  e.motion.x;
			fractal_params->JuliaCy = e.motion.y;
		}
		if (e.type == SDL_KEYDOWN) {
			if (e.key.keysym.sym == SDLK_ESCAPE)
				*done = 1;
			if (e.key.keysym.sym == SDLK_UP)
				fractal_params->upYShift = 1;
			if (e.key.keysym.sym == SDLK_DOWN)
				fractal_params->downYShift = 1;
			if (e.key.keysym.sym == SDLK_RIGHT)
				fractal_params->upXShift = 1;
			if (e.key.keysym.sym == SDLK_LEFT)
				fractal_params->downXShift = 1;
			if (e.key.keysym.sym == SDLK_KP_PLUS)
				fractal_params->upZoom = 1;
			if (e.key.keysym.sym == SDLK_KP_MINUS)
				fractal_params->downZoom = 1;
			if (e.key.keysym.sym == SDLK_RETURN)
				fractal_params->upIt = 1;
			if (e.key.keysym.sym == SDLK_SPACE)
				fractal_params->downIt = 1;
			if (e.key.keysym.sym == SDLK_m)
				fractal_params->mandelbrot = !fractal_params->mandelbrot;
			if (e.key.keysym.sym == SDLK_RSHIFT)
				fractal_params->shift_down = 1;
		}
		if (e.type == SDL_KEYUP) {
			if (e.key.keysym.sym == SDLK_UP)
				fractal_params->upYShift = 0;
			if (e.key.keysym.sym == SDLK_DOWN)
				fractal_params->downYShift = 0;
			if (e.key.keysym.sym == SDLK_RIGHT)
				fractal_params->upXShift = 0;
			if (e.key.keysym.sym == SDLK_LEFT)
				fractal_params->downXShift = 0;
			if (e.key.keysym.sym == SDLK_KP_PLUS)
				fractal_params->upZoom = 0;
			if (e.key.keysym.sym == SDLK_KP_MINUS)
				fractal_params->downZoom = 0;
			if (e.key.keysym.sym == SDLK_RETURN)
				fractal_params->upIt = 0;
			if (e.key.keysym.sym == SDLK_SPACE)
				fractal_params->downIt = 0;
			if (e.key.keysym.sym == SDLK_RSHIFT)
				fractal_params->shift_down = 0;
		}
	}
}


void move(t_fractal_params *fractal_params) {
	if (fractal_params->upYShift)
		fractal_params->yShift += 0.03 / fractal_params->zoom;
	if (fractal_params->downYShift)
		fractal_params->yShift -= 0.03 / fractal_params->zoom;
	if (fractal_params->upXShift)
		fractal_params->xShift += 0.03 / fractal_params->zoom;
	if (fractal_params->downXShift)
		fractal_params->xShift -= 0.03 / fractal_params->zoom;
	if (fractal_params->upZoom)
		fractal_params->zoom *= 1.1;
	if (fractal_params->downZoom) 
		fractal_params->zoom /= 1.1;
	if (fractal_params->upIt)
		fractal_params->maxIt += 10;
	if (fractal_params->shift_down)
		fractal_params->shift = (5 + fractal_params->shift) % 360;
	if (fractal_params->downIt) {
		if (fractal_params->maxIt >= 50)
			fractal_params->maxIt -= 10;
	}
}

int main(void)
{

	t_SDL_staff *SDL_staff;
	SDL_staff = init_sdl("fractal");
	char *cl_source;
	t_cldata		*cl;


	load_file_str("src/fractal.cl", &cl_source);

	cl = ocl_init(cl_source, "main");

	ft_strdel(&cl_source);

	create_cl_data(cl);
	
	int done = 0;

	t_fractal_params *fractal_params = ft_memalloc(sizeof(t_fractal_params));

	fractal_params->maxIt = 50;
	fractal_params->zoom = 1;
	fractal_params->mandelbrot = 1;

	fractal_params->JuliaCx =  -0.7;
	fractal_params->JuliaCy = 0.27015;
	fractal_params->shift = 1;

	while (!done) {
		key(&done, fractal_params);
		move(fractal_params);
		write_cl_data(cl, fractal_params);
		execute_and_read(cl,  SDL_staff->data_len, SDL_staff->data);
		draw(SDL_staff, HEIGHT);
	}
	free_sdl(&SDL_staff);
	ft_memdel((void **)&fractal_params);
	SDL_Quit();
	return (0);
}