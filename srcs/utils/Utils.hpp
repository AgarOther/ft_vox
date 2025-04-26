/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 21:49:29 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/26 21:48:17 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP
# include <string>
# include <GL/glew.h>
# include <imgui/imgui.h>
#include "../renderer/Camera.hpp"

extern int WIDTH;
extern int HEIGHT;

class Block;

class Utils
{
	public:
		static std::string appendInt(std::string str, int n);
		static std::string getShaderAsString(const char *name);
		static int ft_error(int error_id, const std::string &error);
		static void setupGlfw();
		static GLuint compileShader(const char *vertSrc, const char *fragSrc);
		static void unbindAll();
		static void showImGui(const ImGuiIO &io, Camera &camera);
		static void renderImGui();
		static void shutdownImGui();
		static ImGuiIO &getImGuiIO(GLFWwindow *window);
		static void dispatchCommand(Camera &camera, const char *buffer);
		static void toggleFullscreen(GLFWwindow *window, Camera &camera);
		static int getBlockIndex(int x, int y, int z);
		static int getBlockIndex(const Block &block);
};

void	debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);

#endif
