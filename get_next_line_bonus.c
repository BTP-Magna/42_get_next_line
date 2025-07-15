/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamahag <BTP_Magna@proton.me>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 22:00:08 by thamahag          #+#    #+#             */
/*   Updated: 2025/07/16 00:41:50 by thamahag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

/**
 * @brief Append buffer to existing stash and track newline offset
 *
 * @details
 * - Allocates (stash + buffer) combined space.
 * - Copies stash and buffer into new string.
 * - Updates `nl_offset` if newline is found.
 * - Frees old stash, updates stash pointer and size.
 *
 * @param node FD-specific stash node.
 * @param buffer Newly read string.
 * @param buff_size Size of the buffer.
 * @return GNL_OK on success, GNL_ALLOC_FAIL on malloc failure.
 */
static int	ft_stash_append(t_fd_list *node, char *buffer, ssize_t buff_size)
{
	char	*new_stash;
	ssize_t	total_len;
	ssize_t	i_buff;
	ssize_t	i_stash;

	total_len = node->size + buff_size;
	new_stash = malloc(sizeof(char) * (total_len + 1));
	if (!new_stash)
		return (GNL_ALLOC_FAIL);
	i_stash = -1;
	while (++i_stash < node->size)
		new_stash[i_stash] = node->stash[i_stash];
	i_buff = 0;
	while (i_stash < total_len)
	{
		if (buffer[i_buff] == '\n' && node->nl_offset == -1)
			node->nl_offset = i_stash;
		new_stash[i_stash++] = buffer[i_buff++];
	}
	new_stash[i_stash] = '\0';
	free(node->stash);
	node->stash = new_stash;
	node->size += buff_size;
	return (GNL_OK);
}

/**
 * @brief Update stash after extracting a line (skipping past newline).
 *
 * @details
 * - Copies the remainder of the stash after the first newline.
 * - Updates size and newline offset.
 * - Frees the old stash and replaces it with the new one.
 *
 * @param node The fd node to update.
 * @return Updated stash string, or NULL on allocation failure or empty.
 */
static char	*ft_extract_n_update(t_fd_list *node)
{
	char	*extract_str;
	ssize_t	i_extract;
	ssize_t	i_stash;
	ssize_t	new_len;

	new_len = node->size - (node->nl_offset + 1);
	extract_str = malloc(sizeof(char) * (new_len + 1));
	if (!extract_str)
		return (NULL);
	i_extract = 0;
	i_stash = node->nl_offset + 1;
	node->nl_offset = -1;
	while (i_extract < new_len)
	{
		if (node->stash[i_stash] == '\n' && node->nl_offset == -1)
			node->nl_offset = i_extract;
		extract_str[i_extract++] = node->stash[i_stash++];
	}
	extract_str[i_extract] = '\0';
	node->size = new_len;
	free(node->stash);
	return (extract_str);
}

/**
 * @brief Extract a line from the node’s stash and update it.
 *
 * @details
 * - If no newline is found, return the entire stash and remove the node.
 * - If newline exists, extract line up to newline (inclusive),
 *   update the stash to hold remaining characters (if any).
 * - On allocation failure, clear the list and return NULL.
 *
 * @param head Pointer to the linked list head (modified if node is removed).
 * @param node Node to extract line from.
 * @return Extracted line (heap allocated), or NULL on failure.
 */
static char	*ft_extract_line(t_fd_list **head, t_fd_list *node)
{
	char	*extract_str;
	char	*excess_str;

	if (node->nl_offset == -1)
	{
		if (!node->stash || *(node->stash) == '\0')
			extract_str = NULL;
		else
			extract_str = ft_strndup(node->stash, node->size);
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
 * @brief Read from a file descriptor until newline or EOF is reached.
 *
 * @details
 * - Allocates a buffer to read data in chunks of BUFFER_SIZE.
 * - Continues reading and appending to the stash until a newline is found
 *   or EOF/read error occurs.
 * - Frees the buffer before returning.
 *
 * @param node Pointer to the fd node whose stash will be appended.
 * @return
 * - GNL_OK (1) on success and newline found,
 * - GNL_ALLOC_FAIL (-2) on malloc failure,
 * - GNL_READ_ERR (-1) on read() failure,
 * - GNL_READ_EOF (0) on EOF.
 *
 * @note If node->nl_offset is already set, the function returns immediately.
 */
static int	ft_read_till_nextline(t_fd_list *node)
{
	char	*buffer;
	ssize_t	byte_read;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (GNL_ALLOC_FAIL);
	while (node->nl_offset == -1)
	{
		byte_read = read(node->fd, buffer, BUFFER_SIZE);
		if (byte_read == GNL_READ_ERR || byte_read == GNL_READ_EOF)
		{
			free(buffer);
			return (byte_read);
		}
		buffer[byte_read] = '\0';
		if (ft_stash_append(node, buffer, byte_read) == GNL_ALLOC_FAIL)
		{
			free(buffer);
			return (GNL_ALLOC_FAIL);
		}
	}
	free(buffer);
	return (GNL_OK);
}

/**
 * @brief Get next line bonus from multiple FD
 * @details
 *
 * 1. Validate FD (read fd check might move after get node)
 * 2. Get fd node or create new one it didn't exist in link list
 * 		if malloc node fail clean link list and return NULL cause memory full
 * 3. Check if current node have new line if not read until EOF or find newline
 * 4. Extract and return one line if there excess save them in node stash
 *
 * @param fd
 * @return char*
 */
char	*get_next_line(int fd)
{
	static t_fd_list	*head;
	t_fd_list			*node;
	int					result;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	node = ft_get_fd_node(&head, fd);
	if (!node)
		return (ft_clear_all_and_return(&head, NULL, NULL));
	if (node->nl_offset != -1)
		return (ft_extract_line(&head, node));
	result = ft_read_till_nextline(node);
	if (result == GNL_READ_ERR)
	{
		ft_remove_fd_node(&head, node->fd);
		return (NULL);
	}
	if (result == GNL_ALLOC_FAIL)
		return (ft_clear_all_and_return(&head, NULL, NULL));
	return (ft_extract_line(&head, node));
}