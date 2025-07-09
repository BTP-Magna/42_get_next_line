/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamahag <BTP_Magna@proton.me>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 22:00:08 by thamahag          #+#    #+#             */
/*   Updated: 2025/07/10 05:05:57 by thamahag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/**
 * @brief Join previous static buffer with new read, and track newline address
 *
 * @details
 *
 * Merge a buffer onto the end of static buffer, allocate new string to hold
 * result. then frees old static buffer
 *
 * If newline character is found during copy from buffer, update new_line_ptr
 * flag to it location on newly form string.
 *
 * Reason to get newline address here is to prevent re-read of the buffer
 *
 * @param st_buff Hold leftover from previous get_next_line call (can be NULL)
 * @param buffer Newly read content from fd
 * @param b_size Size of newly read buffer
 * @param nl_ptr store '\n' position if found
 * @return char* newly allocated string containing static_buffer + buffer or
 * NULL if any error occour
 *
 * @note This function frees `st_buff` internally. Caller must not reuse it.
 * @note `*nl_ptr` will point to the location of `\n` in new string if found.
 */
char	*ft_gnl_join(char *st_buff, char *buffer, ssize_t b_size, char **nl_ptr)
{
	char	*new_str;
	char	*stb_ptr;
	char	*b_ptr;
	char	*n_ptr;

	new_str = malloc(ft_strlen(st_buff) + b_size + 1);
	if (!new_str)
		return (NULL);
	n_ptr = new_str;
	stb_ptr = st_buff;
	if (stb_ptr)
		while (*stb_ptr)
			*n_ptr++ = *stb_ptr++;
	b_ptr = buffer;
	while (b_size--)
	{
		if (*b_ptr == '\n' && !(*nl_ptr))
			*nl_ptr = n_ptr;
		*n_ptr++ = *b_ptr++;
	}
	*n_ptr = '\0';
	free(st_buff);
	return (new_str);
}

/**
 * @brief Extract one line from static buffer (include '\n' if exist), and store
 * excess in newly allocated string in buffer
 *
 * @details
 * If buffer is NULL or empty, return NULL
 * If no newline was found, return entire buffer and clear it
 * Otherwise:
 * Allocate new string for the line (up to '\n')
 * Allocate new string for the excess (after '\n')
 * Update static buffer to point to excess
 *
 * @param st_buff
 * @param nl_ptr
 * @return char* Allocated string containing the next line, or NULL on error
 *
 */
char	*ft_gnl_extract(char **st_buff, char *nl_ptr)
{
	char	*line;
	char	*excess;

	if (!(*st_buff) || **st_buff == '\0')
		return (ft_free_n_return(st_buff, NULL, NULL));
	if (!nl_ptr)
	{
		line = ft_strdup(*st_buff);
		return (ft_free_n_return(st_buff, NULL, line));
	}
	line = ft_strslice(*st_buff, nl_ptr + 1);
	if (!line)
		return (ft_free_n_return(st_buff, NULL, NULL));
	excess = ft_strdup(nl_ptr + 1);
	if (!excess)
		return (ft_free_n_return(st_buff, line, NULL));
	free(*st_buff);
	*st_buff = excess;
	return (line);
}

/**
 * @brief Get the next line from fd
 * @details Read and get one line (include '\n') from file descriptor.
 * Maintain any leftover content across call with static variable.
 * Function stop reading after reaching newline or EOF.
 * Memory is dynamically allocated
 *
 * newline pointer will always set to NULL at beggining if there aren't
 * newline in static_buffer
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
	static char	*st_buff;
	char		*nl_ptr;
	char		*buffer;
	ssize_t		byte;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	nl_ptr = ft_strchr(st_buff, '\n');
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (ft_free_n_return(&st_buff, NULL, NULL));
	while (!nl_ptr)
	{
		byte = read(fd, buffer, BUFFER_SIZE);
		if (byte == -1)
			return (ft_free_n_return(&st_buff, buffer, NULL));
		if (byte == 0)
			break ;
		*(buffer + byte) = '\0';
		st_buff = ft_gnl_join(st_buff, buffer, byte, &nl_ptr);
		if (!st_buff)
			return (ft_free_n_return(&st_buff, buffer, NULL));
	}
	free(buffer);
	return (ft_gnl_extract(&st_buff, nl_ptr));
}

// #include <fcntl.h>
// #include <stdio.h>

// void	ft_putstr(char *str)
// {
// 	while (*str)
// 		write(1, str++, 1);
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
