/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 21:49:29 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/22 03:10:53 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP
# include <string>
# include <GL/glew.h>
# include <imgui/imgui.h>
# include "../srcs/renderer/Camera.hpp"
# define HEIGHT 1080
# define WIDTH 1920

class Utils
{
	public:
		static std::string appendInt(std::string str, int n);
		static std::string getShaderAsString(const char *name);
		static int ft_error(int error_id, const std::string &error);
		static void setupGlfw();
		static GLuint compileShader(const char *vertSrc, const char *fragSrc);
		static void unbindAll();
		static void showImGui(ImGuiIO &io, Camera &camera);
		static void renderImGui();
		static void shutdownImGui();
		static ImGuiIO &getImGuiIO(GLFWwindow *window);
		static void dispatchCommand(Camera &camera, char *buffer);
		static void toggleFullscreen(GLFWwindow *window, Camera &camera);
		static int getBlockIndex(int x, int y, int z);
};

void	debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);

#endif
