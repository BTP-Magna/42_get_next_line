/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamahag <BTP_Magna@proton.me>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 22:00:08 by thamahag          #+#    #+#             */
/*   Updated: 2025/07/10 22:46:07 by thamahag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

/**
 * Allocate extend str to copy over old stash and append buffer
 * update nl_offset when found right now it not optimize and check newline
 * every buffer loop so either while loop to stop when found new line or
 * reach end of buffer and then check if current index of buffer is new line
 * if so update once then copy the less of the character in buffer
 */
/**
 * @brief Append buffer to existing stash and track newline position
 *
 * @details
 * - Allocates new buffer to hold previous stash + new buffer
 * - Copies both into it while updating newline offset if '\n' is found
 * - Frees old stash and updates stash size
 *
 * @param node Pointer to fd node (contains stash and size tracking)
 * @param buffer Newly read content from fd
 * @param buff_size Number of bytes read into buffer
 * @return char* New stash string, or NULL on allocation failure
 *
 * @note Updates node->nl_offset when newline is found
 * @note Caller must reassign stash pointer (or wrap this internally)
 */
char	*ft_gnl_append_stash(t_fd_list *node, char *buffer, ssize_t buff_size)
{
	char	*new_stash;
	ssize_t total_len;
	ssize_t	i_buff;
	ssize_t	i_stash;

	total_len = node->size + buff_size;
	new_stash = malloc(sizeof(char) * (total_len + 1));
	if (!new_stash)
		return (NULL); // need to clean all link list node and buffer;
	i_stash = -1;
	while(++i_stash < node->size)
		new_stash[i_stash] = node->stash[i_stash];
	i_buff = 0;
	while (i_stash < total_len)
	{
		if (buffer[i_buff] == '\n' && node->nl_offset == -1)
			node->nl_offset = i_stash;
		new_stash[i_stash++] = buffer[i_buff++];
	}
	new_stash[i_stash] = '\0';
	node->size += buff_size;
	free(node->stash);
	return (new_stash);
}

/**
 * @brief
 *
 * If node statsh is empty or doesn't have new line that mean it EOF return
 * node stash and delete the node
 *
 * else split extract the line up to and include newline and update excess to
 * stash
 *
 * @param node
 * @return char*
 */
char	*ft_gnl_extract_line(t_fd_list **head, t_fd_list *node)
{
	char	*extract_str;
	char	*excess_str;

	if (node->nl_offset == -1)
	{
		if (!node->stash || *(node->stash) == '\0')
			extract_str = NULL;
		else
			extract_str = ft_strdup(node->stash, node->size);
		ft_remove_fd_node(head, node->fd);
		return (extract_str);
	}
	extract_str = ft_strslice(node->stash, node->stash + node->nl_offset + 1);
	if (!extract_str)
		return (ft_clear_all_and_return(head, NULL, NULL));
	excess_str = ft_extract_n_update(node);
	if (!excess_str)
		return (ft_clear_all_and_return(head, extract_str, NULL));
	node->stash = excess_str;
	return (extract_str);
}

/**
 * @brief Get the next line bonus object
 *
 * Check if input fd and buffersize is valid
 *
 * @param fd
 * @return char*
 */
char	*get_next_line(int fd)
{
	static t_fd_list	*stash_head;
	t_fd_list			*stash_node;
	char				*buffer;
	ssize_t				byte_read;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) == -1)
		return (NULL);
	stash_node = ft_get_fd_node(&stash_head, fd);
	if (!stash_node)
		return (ft_clear_all_and_return(&stash_head, NULL, NULL));
	buffer = malloc(BUFFER_SIZE + 1);
	while (stash_node->nl_offset == -1)
	{
		byte_read = read(fd, buffer, BUFFER_SIZE);
		if (byte_read == -1)
			return (ft_clear_all_and_return(&stash_head, buffer, NULL));
		if (byte_read == 0)
			break ;
		*(buffer + byte_read) = '\0';
		stash_node->stash = ft_gnl_append_stash(stash_node, buffer, byte_read);
		if (!stash_node->stash)
			return (ft_clear_all_and_return(&stash_head, buffer, NULL));
	}
	free(buffer);
	// print_fd_list(stash_head); // For debug
	return (ft_gnl_extract_line(&stash_head, stash_node));
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
// 	char	*next_line;

// 	int fd, fd1, fd2, fd3;
// 	fd = open("../41_with_nl", O_RDONLY);
// 	if (fd == -1)
// 		return (1);
// 	next_line = get_next_line(1000);
// 	next_line = get_next_line(fd);
// 	ft_putstr(next_line);
// 	free(next_line);
// 	fd1 = open("../42_with_nl", O_RDONLY);
// 	if (fd1 == -1)
// 		return (1);
// 	next_line = get_next_line(1001);
// 	next_line = get_next_line(fd1);
// 	ft_putstr(next_line);
// 	free(next_line);
// 	fd2 = open("../43_with_nl", O_RDONLY);
// 	if (fd1 == -1)
// 		return (1);
// 	next_line = get_next_line(1002);
// 	next_line = get_next_line(fd2);
// 	ft_putstr(next_line);
// 	free(next_line);

// 	next_line = get_next_line(1003);
// 	next_line = get_next_line(fd);
// 	ft_putstr(next_line);
// 	free(next_line);
// 	next_line = get_next_line(1004);
// 	next_line = get_next_line(fd1);
// 	ft_putstr(next_line);
// 	free(next_line);
// 	next_line = get_next_line(1005);
// 	next_line = get_next_line(fd2);
// 	ft_putstr(next_line);
// 	free(next_line);

// 	next_line = get_next_line(fd);
// 	next_line = get_next_line(fd1);
// 	next_line = get_next_line(fd2);

// 	fd3 = open("../nl", O_RDONLY);
// 	if (fd == -1)
// 		return (1);
// 	next_line = get_next_line(1006);
// 	next_line = get_next_line(fd3);
// 	ft_putstr(next_line);
// 	free(next_line);
// 	next_line = get_next_line(1007);
// 	next_line = get_next_line(fd3);

// 	close(fd);
// 	close(fd1);
// 	close(fd2);
// 	close(fd3);
// 	return (0);
// }