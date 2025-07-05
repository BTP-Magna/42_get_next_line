/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamahag <BTP_Magna@proton.me>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 19:46:47 by thamahag          #+#    #+#             */
/*   Updated: 2025/07/06 06:14:27 by thamahag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_substr(const char *from, char *to)
{
	char	*new_str;
	char	*n_ptr;

	if (!from || !to || from > to)
		return (NULL);
	new_str = malloc(to - from + 1);
	if (!new_str)
		return (NULL);
	n_ptr = new_str;
	while (from < to)
		*n_ptr++ = *from++;
	*n_ptr = '\0';
	return (new_str);
}

char	*ft_strdup(const char *str)
{
	char	*n_str;
	char	*ptr;

	if (!str)
		return (NULL);
	n_str = malloc(ft_strlen(str) + 1);
	if (!n_str)
		return (NULL);
	ptr = n_str;
	while (*str)
		*ptr++ = *str++;
	*ptr = '\0';
	return (n_str);
}

char	*ft_strchr(char *str, unsigned char c)
{
	char	*ptr;

	if (!str)
		return (NULL);
	ptr = str;
	while (*ptr)
	{
		if (*ptr == c)
			return (ptr);
		ptr++;
	}
	return (NULL);
}

size_t	ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	if (!str)
		return (len);
	while (*str++)
		len++;
	return (len);
}

char	*ft_free_n_return(char *free_ptr, char *ret)
{
	free(free_ptr);
	free_ptr = NULL;
	return (ret);
}
