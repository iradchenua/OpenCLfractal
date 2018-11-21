
ifdef OPENCL_INC
  CL_CFLAGS = -I$(OPENCL_INC)
endif

ifdef OPENCL_LIB
  CL_LDFLAGS = -L$(OPENCL_LIB)
endif


NAME = fractal

SRCS = 	src/main.c \
		src/sdl_staff.c \
		src/error.c \
		src/draw.c \
		src/cl_staff.c \
		src/write_cl_data.c

OBJ = $(SRCS:%.c=%.o)

CC = gcc

LIBS = -L libft/ -lft -lSDL2 -lOpenCL

INCLUDES = -I libft/includes/ -I includes/

FLAGS = -Wall -Wextra $(INCLUDES) $(CL_CFLAGS) $(CL_LDFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(FLAGS)

all: $(NAME)

$(NAME) : $(OBJ)
	@make -C libft
	@$(CC) $(FLAGS) $(OBJ) $(SDL2_P) $(LIBS) -o $(NAME)

clean:
	@make -C libft/ clean
	@rm -f $(OBJ)

fclean: clean
	@make -C libft/ fclean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

.NOTPARALLEL: all clean fclean re
