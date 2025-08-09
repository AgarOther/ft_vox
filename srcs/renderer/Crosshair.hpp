#pragma once

#include "Shader.hpp"

class Crosshair
{
    public:
        Crosshair();
        ~Crosshair();

        void	draw(float aspectRatio) const;
    private:
        GLuint	_vao;
        GLuint	_id;
		Shader	_shader;
};
