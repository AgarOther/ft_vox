# Compilation
CC					=	g++
CFLAGS				=	-Wall -Wextra -Werror --std=c++11 -g
GLFLAGS				=	-lglfw -lGL -lm -ldl -lGLEW -lglut

# Names
NAME				=	ft_vox

# Sources & Includes
SRCS				=	main.cpp \
						debug.cpp \
						utils/misc_utils.cpp \
						utils/gl_utils.cpp \
						utils/shader_utils.cpp \
						renderer/Shader.cpp \
						renderer/VBO.cpp \
						renderer/VAO.cpp \
						renderer/EBO.cpp \
						renderer/Texture.cpp \
						renderer/TextureType.cpp \
						renderer/Camera.cpp \
						minecraft/Block.cpp
OBJ_FOLDER			=	objs
INCLUDES 			=	-I includes/ -I libs/

GLFW_PATH			=	libs/glfw
GLAD_PATH			=	libs/glad
GLM_PATH			=	libs/glm
STB_PATH			=	libs/stb

GLFW				=	$(GLFW_PATH)/libglfw3.a
GLAD_SRC			=	$(GLAD_PATH)/glad.c
GLAD				=	$(GLAD_SRC:.c=.o)
STB					=	$(STB_PATH)/stb_image.cpp

# Objects
OBJS				=	$(patsubst %.c, $(OBJ_FOLDER)/%.o, $(addprefix srcs/, $(SRCS))) \
						$(patsubst %.c, $(OBJ_FOLDER)/%.o, $(LIBS))

# Custom Makefile Flags
MAKEFLAGS			+=	--no-print-directory --silent

# Custom Colors
PURPLE				=	\033[1;35m
YELLOW				=	\033[33;1m
LIGHT_GREEN			=	\033[1;32m
RESET				=	\033[0m

# Custom messages
EXE_DONE			=	@echo "🎉$(PURPLE) $(NAME) compiled! 🎉$(RESET)\n"
DEBUG_DONE			=	@echo "🎉$(YELLOW) This is the DEBUG MODE version! 🎉$(RESET)\n"
ALL_CLEAN			=	@echo "🧹$(LIGHT_GREEN) Project's objects cleaned! 🧹$(RESET)\n"
ALL_FCLEAN			=	@echo "🧹$(LIGHT_GREEN) Project's objects & Executables cleaned! 🧹$(RESET)\n"

# Rules

all : glfw glad glm check_relink

check_relink:
	@if [ ! -d $(OBJ_FOLDER) ]; then \
		mkdir $(OBJ_FOLDER); \
	fi
	@if [ -f $(NAME) ]; then \
		echo '✅$(LIGHT_GREEN) Nothing to be done for "all"! ✅$(RESET)\n'; \
	else \
		$(MAKE) $(NAME); \
	fi

$(NAME): $(GLAD)
	@$(CC) $(CFLAGS) $(OBJS) $(GLAD) $(STB) $(GLFW) -o $(NAME) $(INCLUDES) $(GLFLAGS)
	$(EXE_DONE)

$(OBJ_FOLDER)/%.o: %.cpp
	@$(CC) $(CFLAGS) -c $< -o $@

clean :
	@rm -rf $(OBJ_FOLDER)
	$(ALL_CLEAN)

fclean :
	@rm -f $(NAME)_debug
	@rm -f $(NAME)
	@rm -rf $(OBJ_FOLDER)
	$(ALL_FCLEAN)

re : fclean all

debug : fclean
	$(MAKE) NAME="$(NAME)_debug" CFLAGS="$(CFLAGS) -DELEO_DEBUG"
	$(DEBUG_DONE)

# Libraries
download_glfw:
	@cd libs; \
	rm -rf glfw 2>/dev/null; \
	echo "\033[31;1;4mglfw Not Found\033[0m"; \
	echo "\033[31;1mDownloading it from github\033[0m"; \
	git clone https://github.com/glfw/glfw.git GLFW; \
	echo "\033[31;1mCompiling it\033[0m"; \
	cmake -S GLFW -B GLFW/build; \
	cmake --build GLFW/build; \
	mkdir -p glfw; \
	cp GLFW/include/GLFW/glfw3.h glfw/.; \
	cp GLFW/build/src/libglfw3.a glfw/.; \
	rm -rf GLFW

glfw:
	@mkdir -p libs
	@if ls ./libs/glfw 2>/dev/null | grep -q "glfw3.h" ; then \
		echo "\033[32;1;4mglfw Header Found\033[0m"; \
	else \
		make --no-print-directory download_glfw; \
	fi
	@if ls ./libs/glfw 2>/dev/null | grep -q "libglfw3.a" ; then \
		echo "\033[32;1;4mglfw Library Found\033[0m"; \
	else \
		make --no-print-directory download_glfw; \
	fi

download_glad:
	@cd libs; \
	echo "\033[31;1;4mglad Not Found\033[0m"; \
	echo "\033[31;1mDownloading it from github\033[0m"; \
	git clone https://github.com/Dav1dde/glad.git glad; \
	echo "\033[31;1mCompiling it\033[0m"; \
	python3 -m glad --out-path=glad/build --generator=c; \
	mkdir -p glad2; \
	cp glad/build/include/glad/glad.h glad2/.; \
	cp glad/build/src/glad.c glad2/.; \
	rm -rf glad; \
	mv glad2 glad

glad:
	@mkdir -p libs
	@if ls ./libs/glad 2>/dev/null | grep -q "glad.h" ; then \
		echo "\033[32;1;4mglad Header Found\033[0m"; \
	else \
		make --no-print-directory download_glad; \
	fi
	@if ls ./libs/glad 2>/dev/null | grep -q "glad.c" ; then \
		echo "\033[32;1;4mglad File Found\033[0m"; \
	else \
		make --no-print-directory download_glad; \
	fi


download_glm:
	@cd libs; \
	echo "\033[31;1;4mglad Not Found\033[0m"; \
	echo "\033[31;1mDownloading it from github\033[0m"; \
	git clone https://github.com/g-truc/glm.git GLM; \
	echo "\033[31;1mGetting the Headers\033[0m"; \
	mkdir -p glm; \
	mv GLM/glm .; \
	rm -rf GLM

glm:
	@mkdir -p libs
	@if ls ./libs/glm 2>/dev/null | grep -q "glm.hpp" ; then \
		echo "\033[32;1;4mglm Header Found\033[0m"; \
	else \
		make --no-print-directory download_glm; \
	fi

$(GLAD): %.o: %.c
	@gcc $(INCLUDES) $< -c -o $@
	@echo "\033[32;1mCompiled " $@ "\033[0m"

.PHONY: all clean fclean re f debug
