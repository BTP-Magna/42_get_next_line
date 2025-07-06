/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamahag <BTP_Magna@proton.me>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 22:00:08 by thamahag          #+#    #+#             */
/*   Updated: 2025/07/06 17:50:10 by thamahag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_gnl_join(char *excess, char *buffer, char **new_line_ptr)
{
	char	*n_str;
	char	*e_ptr;
	char	*b_ptr;
	char	*ptr;

	n_str = malloc(ft_strlen(excess) + ft_strlen(buffer) + 1);
	if (!n_str)
		ft_free_n_return(excess, buffer, NULL);
	ptr = n_str;
	e_ptr = excess;
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
	return (ft_free_n_return(excess, buffer, n_str));
}

char	*ft_gnl_extract(char **excess, char *new_line_ptr)
{
	char	*n_line;
	char	*e_str;

	if (!(*excess))
		return (NULL);
	if (*(*excess) == '\0')
		return (ft_free_n_return(*excess, NULL, NULL));
	if (new_line_ptr)
		n_line = ft_substr(*excess, new_line_ptr + 1);
	else
		n_line = ft_strdup(*excess);
	if (!n_line)
		return (ft_free_n_return(*excess, NULL, NULL));
	e_str = NULL;
	if (new_line_ptr)
		e_str = ft_strdup(new_line_ptr + 1);
	if (!e_str && new_line_ptr)
	{
		free(n_line);
		return (ft_free_n_return(*excess, NULL, NULL));
	}
	free(*excess);
	*excess = e_str;
	return (n_line);
}

char	*get_next_line(int fd)
{
	static char	*excess;
	char		*new_line_ptr;
	char		*buffer;
	ssize_t		byte;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	new_line_ptr = ft_strchr(excess, '\n');
	while (!new_line_ptr)
	{
		buffer = malloc(BUFFER_SIZE + 1);
		if (!buffer)
			return (ft_free_n_return(excess, NULL, NULL));
		byte = read(fd, buffer, BUFFER_SIZE);
		if (byte == -1)
			return (ft_free_n_return(excess, buffer, NULL));
		if (byte == 0)
			break ;
		*(buffer + byte) = '\0';
		excess = ft_gnl_join(excess, buffer, &new_line_ptr);
		if (!excess)
			return (ft_free_n_return(excess, buffer, NULL));
	}
	return (ft_gnl_extract(&excess, new_line_ptr));
}

// #include <fcntl.h>
// #include <stdio.h>

// void	ft_putstr(char *str)
// {
// 	while (*str)
// 		write(1, str++, 1);
// 	write(1, "\n", 1);
// }

// int	main(void)
// {
// 	int		fd;
// 	char	*next_line;

// 	fd = open("test.txt", O_RDONLY);
// 	if (fd == -1)
// 		return (1);
// 	// next_line = malloc(BUFFER_SIZE);
// 	// *next_line = '\0';
// 	// ft_putstr(next_line);
// 	next_line = get_next_line(fd);
// 	while (next_line)
// 	{
// 		ft_putstr(next_line);
// 		free(next_line);
// 		next_line = get_next_line(fd);
// 	}
// 	close(fd);
// 	return (0);
// }
