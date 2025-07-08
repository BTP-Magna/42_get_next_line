/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamahag <BTP_Magna@proton.me>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 19:46:47 by thamahag          #+#    #+#             */
/*   Updated: 2025/07/08 16:12:19 by thamahag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/**
 * @brief Duplicates characters from start pointer up to but not including end
 *
 * @details
 * Allocates a new null-terminated string containing the bytes starting from
 * `start` and ending just before `end`. The range is exclusive of `end`.
 *
 * @param start Pointer to beginning of range.
 * @param end Pointer one past the final character to copy.
 * @return Newly allocated string on success, or NULL on error.
 *
 * @note Caller must ensure that `start < end` and that both belong to the same
 * allocation string.
 */
char	*ft_strslice(const char *start, const char *end)
{
	char	*new_str;
	char	*ptr;

	if (!start || !end || start > end)
		return (NULL);
	new_str = malloc(end - start + 1);
	if (!new_str)
		return (NULL);
	ptr = new_str;
	while (start < end)
		*ptr++ = *start++;
	*ptr = '\0';
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

/**
 * @brief Frees one or two pointers and returns a value.
 *
 * @details
 * If `*ptr1` is non-NULL, it is freed and set to NULL.
 * If `ptr2` is non-NULL, it is freed.
 * Returns `ret`, useful for early-exit or error handling.
 *
 * @param ptr1 Pointer to a pointer to be freed and nulled (double free-safe).
 * @param ptr2 Optional second pointer to free (not nulled).
 * @param ret Value to return after freeing.
 * @return The value of `ret`.
 *
 * @note `ptr1` must be valid (not NULL); `*ptr1` may be NULL.
 */
char	*ft_free_n_return(char **ptr1, char *ptr2, char *ret)
{
	if (ptr1 && *ptr1)
	{
		free(*ptr1);
		*ptr1 = NULL;
	}
	if (ptr2)
		free(ptr2);
	return (ret);
}
