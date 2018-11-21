#include "fractal.h"

void		ocl_error(int err)
{
	if (err == 1)
		error("\033[1;31mError: Failed to create a device group!\e[m");
	else if (err == 2)
		error("\033[1;31mError: Failed to create a compute context!\e[m");
	else if (err == 3)
		error("\033[1;31mError: Failed to create a command commands!\e[m");
	else if (err == 4)
		error("\033[1;31mError: Failed to create compute program!\e[m");
	else if (err == 5)
		error("\033[1;31mError: Failed to build program executable!\e[m");
	else if (err == 6)
		error("\033[1;31mError: Failed to create compute kernel!\e[m");
	else if (err == 7)
		error("\033[1;31mError: Failed to allocate device memory!\e[m");
	else if (err == 8)
		error("\033[1;31mError: Failed to write to source array!\e[m");
	else if (err == 9)
		error("\033[1;31mError: Failed to set kernel arguments!\e[m");
	else if (err == 10)
		error("\033[1;31mError: Failed to execute kernel!\e[m");
	else if (err == 11)
		error("\033[1;31mError: Failed to read output array!\e[m");
	exit(0);
}


t_cldata	*ocl_init(char *progstr, char *kernel)
{
	t_cldata	*r;

	if (!(r = ft_memalloc(sizeof(t_cldata))))
		return (NULL);
	if (clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 1, &r->dev_id, NULL) != MCL_OK)
		ocl_error(1);
	if (!(r->cntxt = clCreateContext(0, 1, &r->dev_id, NULL, NULL, &r->err)))
		ocl_error(2);
	if (!(r->commands = clCreateCommandQueueWithProperties(r->cntxt, r->dev_id, 0, &r->err)))
		ocl_error(3);
	r->src = progstr;
	if (!(r->program = clCreateProgramWithSource(r->cntxt, 1,
		(const char**)&r->src, NULL, &r->err)))
		ocl_error(4);
	if (clBuildProgram(r->program, 0, NULL, NULL, NULL, NULL) != MCL_OK)
		ocl_error(5);
	if (!(r->kernel = clCreateKernel(r->program, kernel, &r->err)) ||
		r->err != MCL_OK)
		ocl_error(6);
	return (r);
}

cl_mem		cl_mem_create(t_cldata *cl, cl_mem_flags rw, size_t size)
{
	cl_mem res;

	res = clCreateBuffer(cl->cntxt, rw, size, NULL, NULL);
	if (!res)
		ocl_error(7);
	return (res);
}

void		cl_mem_write(t_cldata *cl, cl_mem mem, size_t size, void *dat)
{
	if (dat != NULL)
		if (clEnqueueWriteBuffer(cl->commands, mem, CL_TRUE, 0, size,\
			dat, 0, NULL, NULL) != MCL_OK)
			ocl_error(8);
}

void		clear_cl(t_cldata *cl)
{
	clReleaseContext(cl->cntxt);
	clReleaseKernel(cl->kernel);
	free(cl);
}