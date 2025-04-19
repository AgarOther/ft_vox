/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TextureType.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:35:10 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/19 16:03:15 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURE_TYPE
# define TEXTURE_TYPE

# include "Texture.hpp"

typedef enum e_texture_type
{
	WARPED_NYLIUM,
	NETHERRACK
}	Material;

class TextureType
{
	public:
		static std::vector<Texture*> generateTextures(Material material);
		static const GLuint *getIndices(Material type);
};

#endif