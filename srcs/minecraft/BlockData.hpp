/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BlockData.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:43:36 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/23 00:17:42 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BLOCK_DATA
# define BLOCK_DATA
# include <GL/glew.h>

constexpr int chunkSize = 16;
constexpr int chunkHeight = 32;
constexpr int chunkVolume = chunkSize * chunkHeight * chunkSize;

// Vertices coordinates
const GLfloat vertices[] =
{  //    Position         /   TexCoord  /
	// Front face
	-0.5f, 0.0f,  0.5f,    0.0f, 0.0f,
	 0.5f, 0.0f,  0.5f,    1.0f, 0.0f,
	 0.5f, 1.0f, 0.5f,    1.0f, 1.0f,
	-0.5f, 1.0f, 0.5f,    0.0f, 1.0f,

	// Back face
	 0.5f, 0.0f, -0.5f,    0.0f, 0.0f,
	-0.5f, 0.0f, -0.5f,    1.0f, 0.0f,
	-0.5f, 1.0f, -0.5f,   1.0f, 1.0f,
	 0.5f, 1.0f, -0.5f,   0.0f, 1.0f,

	// Left face
	-0.5f, 0.0f, -0.5f,    0.0f, 0.0f,
	-0.5f, 0.0f,  0.5f,    1.0f, 0.0f,
	-0.5f, 1.0f, 0.5f,    1.0f, 1.0f,
	-0.5f, 1.0f, -0.5f,   0.0f, 1.0f,

	// Right face
	 0.5f, 0.0f,  0.5f,    0.0f, 0.0f,
	 0.5f, 0.0f, -0.5f,    1.0f, 0.0f,
	 0.5f, 1.0f, -0.5f,   1.0f, 1.0f,
	 0.5f, 1.0f, 0.5f,    0.0f, 1.0f,

	// Top face
	-0.5f, 1.0f,  0.5f,   0.0f, 0.0f,
	 0.5f, 1.0f,  0.5f,   1.0f, 0.0f,
	 0.5f, 1.0f, -0.5f,   1.0f, 1.0f,
	-0.5f, 1.0f, -0.5f,   0.0f, 1.0f,

	// Bottom face
	-0.5f, 0.0f, -0.5f,    0.0f, 0.0f,
	 0.5f, 0.0f, -0.5f,    1.0f, 0.0f,
	 0.5f, 0.0f,  0.5f,    1.0f, 1.0f,
	-0.5f, 0.0f,  0.5f,    0.0f, 1.0f
};




const GLuint blockFaces[]
{
	1, 1, 1, 1, // Front
	2, 2, 2, 2, // Back
	3, 3, 3, 3, // Left
	4, 4, 4, 4, // Right
	5, 5, 5, 5, // Top
	6, 6, 6, 6  // Bottom
};

const GLuint textureId4s[]
{
	2, 2, 2, 2, // Front
	1, 1, 1, 1, // Back
	1, 1, 1, 1, // Left
	1, 1, 1, 1, // Right
	0, 0, 0, 0, // Top
	3, 3, 3, 3  // Bottom
};

const GLuint textureId3s[]
{
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	0, 0, 0, 0,
	2, 2, 2, 2
};

const GLuint textureId2s[]
{
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	0, 0, 0, 0,
	0, 0, 0, 0
};

const GLuint textureId1s[]
{
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0
};

// Indices for vertices order
const GLuint indices[] =
{
	// Front face
	0, 1, 2,
	0, 2, 3,

	// Back face
	4, 5, 6,
	4, 6, 7,

	// Left face
	8, 9,10,
	8,10,11,

	// Right face
	12,13,14,
	12,14,15,

	// Top face
	16,17,18,
	16,18,19,

	// Bottom face
	20,21,22,
	20,22,23
};

#endif
