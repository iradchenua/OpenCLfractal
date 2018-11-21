#ifndef FRACTAL_H
#define FRACTAL_H

#include <stdio.h>
#include <stdlib.h>

#include "libft.h"
#include "SDL2/SDL.h"
#include <CL/cl.h>

#define WIDTH 800
#define HEIGHT 800
#define MCL_GPU CL_DEVICE_TYPE_GPU
#define MCL_OK CL_SUCCESS
#define MCL_RDO CL_MEM_READ_ONLY
#define MCL_WRO CL_MEM_WRITE_ONLY

typedef struct			s_cldata
{
	cl_device_id		dev_id;
	cl_context			cntxt;
	cl_command_queue	commands;
	cl_program			program;
	cl_kernel			kernel;
	char				*src;

	int					err;

	cl_mem				out;
	cl_mem 				fractal_params;
}						t_cldata;

typedef struct  s_fractal_params
{
	int 		mandelbrot;

	double 		xShift;
	double 		yShift;
	double 		zoom;
	int 		maxIt;

	int 		shift;
	int 		shift_down;
	
	int			JuliaCx;
	int			JuliaCy;

	int 		upYShift;
	int 		downYShift;

	int 		upXShift;
	int 		downXShift;

	int 		upZoom;
	int 		downZoom;

	int 		upIt;
	int 		downIt;
}				t_fractal_params;

typedef struct s_SDL_staff
{
	SDL_Window 		*window;
	SDL_Texture		*texture;
	SDL_Renderer 	*renderer;
	int  			*data;
	size_t 			data_len;
}				t_SDL_staff;

void 			error(char *message);

t_SDL_staff 	*init_sdl(char *window_name);
void			free_sdl(t_SDL_staff **SDL_staff);

void			draw(t_SDL_staff *SDL_staff, int height);

cl_mem			cl_mem_create(t_cldata *cl, cl_mem_flags rw, size_t size);
void			cl_mem_write(t_cldata *cl, cl_mem mem, size_t size, void *dat);
void			clear_cl(t_cldata *cl);
void			ocl_error(int err);
t_cldata		*ocl_init(char *progstr, char *kernel);

void			create_cl_data(t_cldata *cl);
void			write_cl_data(t_cldata *cl, t_fractal_params *fractal_params);
void			execute_and_read(t_cldata *cl, size_t res_blen, void *res);

#endif