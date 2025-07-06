/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamahag <BTP_Magna@proton.me>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 19:46:47 by thamahag          #+#    #+#             */
/*   Updated: 2025/07/07 03:02:36 by thamahag         ###   ########.fr       */
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

/**
 * @brief Search for the first occurrence of character in string.
 * @details
 *
 * Scan the string for the first occurrence of character and return it position
 * if exist else return NULL include if searching for '\0'
 *
 * @param str pointer to string to search
 * @param c character to find
 * @return char* position of c if it not exist return NULL
 */

char	*ft_strchr(const char *str, int c)
{
	unsigned char	character;

	if (!str)
		return (NULL);
	character = (unsigned char)c;
	while (*str)
	{
		if (*str == character)
			return ((char *)str);
		str++;
	}
	if (*str == character)
		return ((char *)str);
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

char	*ft_free_n_return(char **free1, char *free2, char *ret)
{
	if (*free1)
	{
		free(*free1);
		*free1 = NULL;
	}
	if (free2)
		free(free2);
	return (ret);
}
