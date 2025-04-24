# Compilation
CC					=	g++
CFLAGS				=	-Wall -Wextra -Werror
GLFLAGS				=	-lglfw -lGL -lm -ldl -lGLEW -lglut -lpthread

# Names
NAME				=	ft_vox

# Sources & Includes
SRCS				=	srcs/main.cpp \
						srcs/debug.cpp \
						srcs/utils/command_utils.cpp \
						srcs/utils/gl_utils.cpp \
						srcs/utils/imgui_utils.cpp \
						srcs/utils/misc_utils.cpp \
						srcs/utils/shader_utils.cpp \
						srcs/renderer/Shader.cpp \
						srcs/renderer/Crosshair.cpp \
						srcs/renderer/VBO.cpp \
						srcs/renderer/VAO.cpp \
						srcs/renderer/EBO.cpp \
						srcs/renderer/Texture.cpp \
						srcs/renderer/TextureType.cpp \
						srcs/renderer/Camera.cpp \
						srcs/minecraft/BlockType.cpp \
						srcs/minecraft/Block.cpp \
						srcs/minecraft/Location.cpp \
						srcs/minecraft/Chunk.cpp
OBJ_FOLDER			=	objs
INCLUDES			=	-I includes \
						-I libs \
						-I $(GLAD_PATH)/include \
						-I $(GLM_PATH) \
						-I $(STB_PATH) \
						-I $(IMGUI_PATH)

GLFW_PATH			=	libs/GLFW
GLEW_PATH			=	libs/GL
GLM_PATH			=	libs/glm
STB_PATH			=	libs/stb
IMGUI_PATH			=	libs/imgui
IMGUI_SRCS			=	libs/imgui/imgui_demo.cpp \
						libs/imgui/imgui_draw.cpp \
						libs/imgui/imgui_tables.cpp \
						libs/imgui/imgui_impl_glfw.cpp \
						libs/imgui/imgui_impl_opengl3.cpp \
						libs/imgui/imgui_widgets.cpp \
						libs/imgui/imgui.cpp

GLFW				=	$(GLFW_PATH)/libglfw3.a
GLEW				=	$(GLAD_PATH)/libGLEW.a
STB					=	$(STB_PATH)/stb_image.cpp
IMGUI				=	$(IMGUI_SRCS:.cpp=.o)

# Objects
OBJS				=	$(patsubst srcs/%.cpp, $(OBJ_FOLDER)/%.o, $(SRCS)) \
						$(patsubst %.c, $(OBJ_FOLDER)/%.o, $(LIBS)) \
						$(patsubst %.cpp, $(OBJ_FOLDER)/%.o, $(LIBS))

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

all : glfw glm $(NAME)

$(NAME): $(IMGUI) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(STB) $(GLFW) $(GLAD) $(IMGUI) -o $(NAME) $(INCLUDES) $(GLFLAGS)
	$(EXE_DONE)

$(OBJ_FOLDER)/%.o: srcs/%.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

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
	rm -rf GLFW 2>/dev/null; \
	echo "\033[31;1;4mglfw Not Found\033[0m"; \
	echo "\033[31;1mDownloading it from github\033[0m"; \
	git clone https://github.com/glfw/glfw.git GLFW_DOWNLOADED; \
	echo "\033[31;1mCompiling it\033[0m"; \
	cmake -S GLFW_DOWNLOADED -B GLFW_DOWNLOADED/build; \
	cmake --build GLFW_DOWNLOADED/build; \
	mkdir -p GLFW; \
	cp GLFW_DOWNLOADED/include/GLFW/glfw3.h GLFW/.; \
	cp GLFW_DOWNLOADED/build/src/libglfw3.a GLFW/.; \
	rm -rf GLFW_DOWNLOADED

glfw:
	@mkdir -p libs
	@if ! ls ./libs/GLFW 2>/dev/null | grep -q "glfw3.h" ; then \
		make --no-print-directory download_glfw; \
	fi
	@if ! ls ./libs/GLFW 2>/dev/null | grep -q "libglfw3.a" ; then \
		make --no-print-directory download_glfw; \
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
	@if ! ls ./libs/glm 2>/dev/null | grep -q "glm.hpp" ; then \
		make --no-print-directory download_glm; \
	fi

$(IMGUI): %.o: %.cpp
	@gcc $(INCLUDES) $< -c -o $@
	@echo "\033[32;1mCompiled " $@ "\033[0m"

.PHONY: all clean fclean re debug
