# Compilation
CC					=	c++
CFLAGS				=	-Wall -Wextra -Werror -std=c++17 -g -MP -MMD
LDFLAGS				=	-lglfw -lGL -lm

# Names
NAME				=	player

# Sources & Includes
SRCS				= 	srcs/debug.cpp \
						srcs/glfw.cpp \
						srcs/main.cpp \
						srcs/models.cpp \
						srcs/utils.cpp \
						srcs/imgui_utils.cpp \
						srcs/renderer/Camera.cpp \
						srcs/renderer/Shader.cpp \
						srcs/renderer/Skybox.cpp \
						srcs/minecraft/BlockTypeRegistry.cpp \
						srcs/minecraft/BoundingBox.cpp \
						srcs/minecraft/Chunk.cpp \
						srcs/minecraft/Location.cpp \
						srcs/minecraft/ObjectRegistry.cpp \
						srcs/minecraft/Player.cpp \
						srcs/minecraft/TextureAtlas.cpp \
						srcs/minecraft/World.cpp \
						libs/stb/stb_image.cpp \
						libs/imgui/imgui_demo.cpp \
						libs/imgui/imgui_draw.cpp \
						libs/imgui/imgui_tables.cpp \
						libs/imgui/imgui_impl_glfw.cpp \
						libs/imgui/imgui_impl_opengl3.cpp \
						libs/imgui/imgui_widgets.cpp \
						libs/imgui/imgui.cpp
OBJ_FOLDER			=	objs
INCLUDES			=	-I includes \
						-I srcs/engine \
						-I srcs/renderer \
						-I srcs/minecraft \
						-I libs/imgui \
						-I libs

LIBS				=	libs/GL/libGLEW.a \
						libs/GLFW/libglfw3.a

# Objects
OBJS				=	$(patsubst srcs/%, $(OBJ_FOLDER)/%, $(SRCS:.cpp=.o))
DEPS				=	$(patsubst srcs/%, $(OBJ_FOLDER)/%, $(SRCS:.cpp=.d))

# Custom Makefile Flags
MAKEFLAGS			+=	--no-print-directory --silent

# Custom Colors
PURPLE				=	\033[1;35m
LIGHT_GREEN			=	\033[1;32m
RESET				=	\033[0m

# Custom messages
EXE_DONE			=	@echo "🎉$(PURPLE) $(NAME) compiled! 🎉$(RESET)\n"
ALL_CLEAN			=	@echo "🧹$(LIGHT_GREEN) Project's objects cleaned! 🧹$(RESET)\n"
ALL_FCLEAN			=	@echo "🧹$(LIGHT_GREEN) Project's objects & Executables cleaned! 🧹$(RESET)\n"

# Rules

all : glfw glm glew
	@make -j4 $(NAME)

$(NAME): $(IMGUI) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME) $(LDFLAGS)
	$(EXE_DONE)

$(OBJ_FOLDER)/%.o: srcs/%.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean :
	@rm -rf $(OBJ_FOLDER)
	$(ALL_CLEAN)

fclean :
	@rm -f $(NAME)
	@rm -rf $(OBJ_FOLDER)
	$(ALL_FCLEAN)

re : fclean all

debug : CFLAGS += -DDEBUG
debug :
	$(MAKE) CFLAGS="$(CFLAGS)" -j4 $(NAME)

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

download_glew:
	@cd libs; \
	rm -rf GLEW_DOWNLOADED 2>/dev/null; \
	echo "\033[31;1;4mGLEW Not Found\033[0m"; \
	echo "\033[31;1mDownloading it from github\033[0m"; \
	git clone https://github.com/nigels-com/glew.git GLEW_DOWNLOADED; \
	echo "\033[31;1mCompiling it\033[0m"; \
	cd GLEW_DOWNLOADED; \
	make extensions; \
	make; \
	mkdir -p ../GLEW; \
	cp include/GL/glew.h ../GLEW/.; \
	cp lib/libGLEW.a ../GLEW/.; \
	cd ..; \
	rm -rf GLEW_DOWNLOADED

glew:
	@mkdir -p libs
	@if ! ls ./libs/GL 2>/dev/null | grep -q "glew.h" ; then \
		make --no-print-directory download_glew; \
	fi
	@if ! ls ./libs/GL 2>/dev/null | grep -q "libGLEW.a" ; then \
		make --no-print-directory download_glew; \
	fi

$(IMGUI): %.o: %.cpp
	@gcc $(INCLUDES) $< -c -o $@

.PHONY: all clean fclean re

-include $(DEPS)
