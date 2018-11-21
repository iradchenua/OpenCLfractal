#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#define WIDTH 800
#define HEIGHT 800

typedef struct  s_fractal_params
{
	int 		mandelbort;
	double 		xShift;
	double 		yShift;
	double 		zoom;
	int 		maxIt;

	int 	shift;
	int 	shift_down;

	int 	JuliaCx;
	int 	JuliaCy;

	int 		upYShift;
	int 		downYShift;

	int 		upXShift;
	int 		downXShift;

	int 		upZoom;
	int 		downZoom;

	int 		upIt;
	int 		downIt;
}				t_fractal_params;

typedef struct  s_color_hsv
{
	int h;
	float s;
	float v;
}				t_color_hsv;

t_color_hsv getHsv(int h, float s, float v, int shift) {
	t_color_hsv hsv;

	hsv.h = h % shift;
	hsv.s = s;
	hsv.v = v;
	return (hsv);
}

int hsvToRgb(t_color_hsv hsv) {
	float C = hsv.v * hsv.s;
	float X = C * (1 - abs((hsv.h / 60) % 2 - 1));
	float m = hsv.v - C;

	int r;
	int g;
	int b;

	if (hsv.h >= 0 && hsv.h < 60) {
		r = C;
		g = X;
		b = 0;
	}
	else if (hsv.h >= 60 && hsv.h < 120) {
		r = X;
		g = C;
		b = 0;
	}
	else if (hsv.h >= 120 && hsv.h < 180) {
		r = 0;
		g = C;
		b = X;
	}

	else if (hsv.h >= 180 && hsv.h < 240) {
		r = 0;
		g = X;
		b = C;
	}

	else if (hsv.h >= 240 && hsv.h < 300) {
		r = X;
		g = 0;
		b = C;
	}

	else {
		r = C;
		g = 0;
		b = X;
	}

	return ((int)((r + m) * 0xff) << 16 | ((int)(g + m) * 0xff) << 8 | (int)(b + m)*0xff);
}

int		get_color(int it, int maxIt, int shift, double2 point)
{
	//return (hsvToRgb(getHsv(360 - ((float)it / maxIt) * 360, 1, 1 * (it < maxIt), shift)));
	return (it + 1 - log(log2(dot(point, point))));
}

void toMandeblrotCords(double2 *C, double x, double y) {

	C->x = (((x / WIDTH) * 3.5) - 2.5);
	C->y = ((2 * y / HEIGHT) - 1);

}

void getC(double2 *C, double x, double y, double zoom) {
	toMandeblrotCords(C, x, y);
	C->x /= zoom;
	C->y /= zoom;
}

void setMandelbrot(double2 *point, double2 *C, int x, int y, __global t_fractal_params *fractal_params) {
	point->x = 0;
	point->y = 0;

	getC(C, x , y, fractal_params->zoom);
	C->x += fractal_params->xShift;
	C->y -= fractal_params->yShift;
}

void setJulia(double2 *point, double2 *C, int x, int y, __global t_fractal_params *fractal_params) {
	getC(point, x, y, fractal_params->zoom);
	
	point->x += fractal_params->xShift;
	point->y -= fractal_params->yShift;

	toMandeblrotCords(C, fractal_params->JuliaCx, fractal_params->JuliaCy);
	
}

__kernel void   main(__global int	*output, __global t_fractal_params *fractal_params)
{
	int x = get_global_id(0);
	int y = get_global_id(1);

	double2 point;
	double2 C;

	if (fractal_params->mandelbort)
		setMandelbrot(&point, &C, x, y, fractal_params);
	else
		setJulia(&point, &C, x, y, fractal_params);
	int it = 0;
	double tempX = 0;

	for ( ; it < fractal_params->maxIt && (dot(point, point) < 4); it++) {
		tempX = point.x * point.x - point.y * point.y + C.x;
		point.y = 2 * point.x * point.y + C.y;
		point.x = tempX;
	}
	output[y * WIDTH + x] = get_color(it, fractal_params->maxIt, fractal_params->shift, point);
}

