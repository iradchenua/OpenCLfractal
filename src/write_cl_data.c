#include "fractal.h"

void		create_cl_data(t_cldata *cl)
{
	cl->out = cl_mem_create(cl, MCL_WRO, sizeof(int) * WIDTH *\
		HEIGHT);
	cl->fractal_params = cl_mem_create(cl, MCL_RDO, sizeof(t_fractal_params));
}

void			write_cl_data(t_cldata *cl,	t_fractal_params *fractal_params)
{
	cl->err |= clSetKernelArg(cl->kernel, 0, sizeof(cl_mem), &cl->out);

	cl_mem_write(cl, cl->fractal_params, sizeof(t_fractal_params), fractal_params);

	cl->err |= clSetKernelArg(cl->kernel, 1, sizeof(cl_mem), &cl->fractal_params);

	/*cl->err |= clSetKernelArg(cl->kernel, 1, sizeof(double), fractal_params->xShift);
	cl->err |= clSetKernelArg(cl->kernel, 2, sizeof(double), fractal_params->yShift);
	cl->err |= clSetKernelArg(cl->kernel, 3, sizeof(double), fractal_params->zoom);
	cl->err |= clSetKernelArg(cl->kernel, 4, sizeof(int), fractal_params->maxIt);*/
	if (cl->err != MCL_OK) {
		ocl_error(9);
	}
}


void		execute_and_read(t_cldata *cl, size_t res_blen, void *res)
{
	size_t dim_size[2];

	dim_size[0] = WIDTH;
	dim_size[1] = HEIGHT;
	if (clEnqueueNDRangeKernel(cl->commands, cl->kernel, 2, NULL, dim_size,
		NULL, 0, NULL, NULL))
		ocl_error(10);
	if (cl->err != MCL_OK)
		ocl_error(5);
	clFinish(cl->commands);
	cl->err = clEnqueueReadBuffer(cl->commands, cl->out, CL_TRUE, 0,\
			res_blen, res, 0, NULL, NULL);
	if (cl->err != MCL_OK)
		ocl_error(11);
}

