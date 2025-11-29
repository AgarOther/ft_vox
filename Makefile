# Compilation
CC					=	c++
CFLAGS				=	-Wall -Wextra -Werror -std=c++20 -g -MP -MMD
LDFLAGS				=	-lglfw -lGL -lm

# Names
NAME				=	player

# Sources & Includes
SRCS				= 	srcs/debug.cpp \
						srcs/main.cpp \
						srcs/input/InputManager.cpp \
						srcs/minecraft/BoundingBox.cpp \
						srcs/minecraft/Location.cpp \
						srcs/minecraft/Player.cpp \
						srcs/minecraft/TextureAtlas.cpp \
						srcs/minecraft/voxel/WorldManager.cpp \
						srcs/minecraft/voxel/chunk/Chunk.cpp \
						srcs/minecraft/voxel/chunk/ChunkMonitor.cpp \
						srcs/minecraft/voxel/chunk/ChunkWorker.cpp \
						srcs/minecraft/voxel/world/World.cpp \
						srcs/noise/Noise.cpp \
						srcs/registry/BlockTypeRegistry.cpp \
						srcs/registry/ObjectRegistry.cpp \
						srcs/registry/StructureRegistry.cpp \
						srcs/renderer/Camera.cpp \
						srcs/renderer/Crosshair.cpp \
						srcs/renderer/Frustum.cpp \
						srcs/renderer/Shader.cpp \
						srcs/renderer/Skybox.cpp \
						srcs/scene/Scene.cpp \
						srcs/utils/command_utils.cpp \
						srcs/utils/glfw_utils.cpp \
						srcs/utils/imgui_utils.cpp \
						srcs/utils/utils.cpp \
						srcs/utils/textureatlas_utils.cpp
LIBS_SRC			=	libs/stb/stb_image.cpp \
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
						-I srcs/input \
						-I srcs/minecraft \
						-I srcs/minecraft/voxel \
						-I srcs/minecraft/voxel/chunk \
						-I srcs/minecraft/voxel/world \
						-I srcs/noise \
						-I srcs/registry \
						-I srcs/renderer \
						-I srcs/scene \
						-I libs/imgui \
						-I libs

LIBS				=	libs/GL/libGLEW.a \
						libs/GLFW/libglfw3.a

# Objects
OBJS				=	$(patsubst srcs/%, $(OBJ_FOLDER)/%, $(SRCS:.cpp=.o)) \
						$(patsubst libs/%, $(OBJ_FOLDER)/%, $(LIBS_SRC:.cpp=.o))
DEPS				=	$(patsubst srcs/%, $(OBJ_FOLDER)/%, $(SRCS:.cpp=.d)) \
						$(patsubst libs/%, $(OBJ_FOLDER)/%, $(LIBS_SRC:.cpp=.d))

# Custom Makefile Flags
MAKEFLAGS			+=	--no-print-directory --silent

# Custom Colors
PURPLE				=	\033[1;35m
LIGHT_GREEN			=	\033[1;32m
RESET				=	\033[0m

# Custom messages
EXE_DONE			=	@echo "[PROGRAM] $(PURPLE)$(NAME) compiled!$(RESET)"
ALL_CLEAN			=	@echo "[SRCS] $(LIGHT_GREEN)Project's objects cleaned!$(RESET)"
ALL_FCLEAN			=	@echo "[SRCS] $(LIGHT_GREEN)Project's objects & Executables cleaned!$(RESET)"

# Rules

all : glfw glm glew $(NAME)

$(NAME): $(IMGUI) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME) $(LDFLAGS)
	$(EXE_DONE)

$(OBJ_FOLDER)/%.o: srcs/%.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@echo "[SRCS] $(LIGHT_GREEN)Compiled: $@ $(RESET)"

$(OBJ_FOLDER)/%.o: libs/%.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@echo "[LIBS] $(LIGHT_GREEN)Compiled: $@ $(RESET)"

clean :
	@rm -rf $(OBJ_FOLDER)
	$(ALL_CLEAN)

fclean :
	@rm -f $(NAME)
	@rm -rf $(OBJ_FOLDER)
	$(ALL_FCLEAN)

run: all
	./$(NAME)

re :
	$(MAKE) fclean
	$(MAKE) all

debug : CFLAGS += -DDEBUG
debug :
	$(MAKE) CFLAGS="$(CFLAGS)" -j32 $(NAME)

debugrun : CFLAGS += -DDEBUG
debugrun :
	$(MAKE) CFLAGS="$(CFLAGS)" -j32 $(NAME)
	./$(NAME)

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
	echo "\033[31;1;4mglm Not Found\033[0m"; \
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
