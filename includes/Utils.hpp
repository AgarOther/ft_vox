/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 21:49:29 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/16 21:51:19 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

class Utils
{
	public:
		static std::string	getShaderAsString(const char *name);
		static int ft_error(int error_id, const std::string &error);
};
