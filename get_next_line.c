/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamahag <BTP_Magna@proton.me>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 22:00:08 by thamahag          #+#    #+#             */
/*   Updated: 2025/07/07 02:00:09 by thamahag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_gnl_join(char *static_buffer, char *buffer, char **new_line_ptr)
{
	char	*n_str;
	char	*e_ptr;
	char	*b_ptr;
	char	*ptr;

	n_str = malloc(ft_strlen(static_buffer) + ft_strlen(buffer) + 1);
	if (!n_str)
		return (NULL);
	ptr = n_str;
	e_ptr = static_buffer;
	if (e_ptr)
		while (*e_ptr)
			*ptr++ = *e_ptr++;
	b_ptr = buffer;
	while (*b_ptr && *b_ptr != '\n')
		*ptr++ = *b_ptr++;
	if (*b_ptr == '\n')
		*new_line_ptr = ptr;
	while (*b_ptr)
		*ptr++ = *b_ptr++;
	*ptr = '\0';
	return (ft_free_n_return(static_buffer, NULL, n_str));
}

char	*ft_gnl_extract(char **static_buffer, char *new_line_ptr)
{
	char	*n_line;
	char	*e_str;

	if (!(*static_buffer))
		return (NULL);
	if (*(*static_buffer) == '\0')
		return (ft_free_n_return(*static_buffer, NULL, NULL));
	if (new_line_ptr)
		n_line = ft_substr(*static_buffer, new_line_ptr + 1);
	else
		n_line = ft_strdup(*static_buffer);
	if (!n_line)
		return (ft_free_n_return(*static_buffer, NULL, NULL));
	e_str = NULL;
	if (new_line_ptr)
		e_str = ft_strdup(new_line_ptr + 1);
	if (!e_str && new_line_ptr)
	{
		free(n_line);
		return (ft_free_n_return(*static_buffer, NULL, NULL));
	}
	free(*static_buffer);
	*static_buffer = e_str;
	return (n_line);
}

/**
 * @brief Get the next line from fd
 * @details Read and get one line (include '\n') from file descriptor.
 * Maintain any leftover content across call with static variable.
 * Function stop reading after reaching newline or EOF.
 * Memory is dynamically allocated
 *
 * @param fd file descripotr
 * @return char* One line from fd including '\n'
 * @note
 * - Static variable if not initialize with value will either be 0 or NULL
 * - Only one static buffer is used (Not per fd)
 * - Behavior is undefined if 'fd' change across call (handle in bonus)
 * - BUFFER_SIZE should be defined at compile time or default is '1024'
 */
char	*get_next_line(int fd)
{
	static char	*static_buffer;
	char		*new_line_ptr;
	char		*buffer;
	ssize_t		byte;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	new_line_ptr = ft_strchr(static_buffer, '\n');
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (ft_free_n_return(static_buffer, NULL, NULL));
	while (!new_line_ptr)
	{
		byte = read(fd, buffer, BUFFER_SIZE);
		if (byte == -1)
			return (ft_free_n_return(static_buffer, buffer, NULL));
		if (byte == 0)
			break ;
		*(buffer + byte) = '\0';
		static_buffer = ft_gnl_join(static_buffer, buffer, &new_line_ptr);
		if (!static_buffer)
			return (ft_free_n_return(static_buffer, buffer, NULL));
	}
	free(buffer);
	return (ft_gnl_extract(&static_buffer, new_line_ptr));
}

#include <fcntl.h>
#include <stdio.h>

void	ft_putstr(char *str)
{
	while (*str)
		write(1, str++, 1);
	write(1, "\n", 1);
}

int	main(void)
{
	int		fd;
	char	*next_line;

	fd = open("test.txt", O_RDONLY);
	if (fd == -1)
		return (1);
	// next_line = malloc(BUFFER_SIZE);
	// *next_line = '\0';
	// ft_putstr(next_line);
	next_line = get_next_line(fd);
	while (next_line)
	{
		ft_putstr(next_line);
		free(next_line);
		next_line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
