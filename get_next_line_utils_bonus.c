/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamahag <BTP_Magna@proton.me>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:26:54 by thamahag          #+#    #+#             */
/*   Updated: 2025/07/16 00:59:19 by thamahag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

/**
 * @brief Get the fd node object - Search for fd and return it else initialize
 * and insert new node into link list at head no sort
 *
 * @param head
 * @param fd
 * @return t_fd_list*
 *
 * @note normally when initialize struct data it usually initialize implicitly
 * as 0 or NULL but gonna add it explicitly on fd and nl_offset to be -1
 * meaning no nl_line in stash as of yet
 */
t_fd_list	*ft_get_fd_node(t_fd_list **head, int fd)
{
	t_fd_list	*node;

	node = *head;
	while (node)
	{
		if (node->fd == fd)
			return (node);
		node = node->next;
	}
	node = malloc(sizeof(t_fd_list));
	if (!node)
		return (NULL);
	node->stash = NULL;
	node->size = 0;
	node->fd = fd;
	node->nl_offset = -1;
	node->next = *head;
	*head = node;
	return (node);
}

/**
 * @brief Remove a single node matching the given fd.
 *
 * @param head Address of head pointer to fd link list
 * @param fd File descriptor to match.
 *
 * @note Frees the node’s stash and the node itself.
 */
void	ft_remove_fd_node(t_fd_list **head, int fd)
{
	t_fd_list	*previous;
	t_fd_list	*current;

	previous = NULL;
	current = *head;
	while (current)
	{
		if (current->fd == fd)
		{
			if (!previous)
				*head = current->next;
			else
				previous->next = current->next;
			free(current->stash);
			free(current);
			return ;
		}
		previous = current;
		current = current->next;
	}
}

/**
 * @brief Frees all fd lists node and optional memory, returning ret.
 *
 * @param head Address of head pointer to fd link list
 * @param to_free Optional memory (like buffer) to free.
 * @param ret Value to return (usually NULL).
 * @return ret
 *
 * @note Cleans all `stash` strings in each node before freeing them.
 */
char	*ft_clear_all_and_return(t_fd_list **head, char *to_free, char *ret)
{
	t_fd_list	*node;

	if (head)
	{
		while (*head)
		{
			node = *head;
			*head = (*head)->next;
			free(node->stash);
			free(node);
		}
		*head = NULL;
	}
	free(to_free);
	return (ret);
}

/**
 * @brief Duplicate the `size` characters of a string.
 *
 * @param str The source string to duplicate from.
 * @param size The maximum number of characters to copy.
 * @return A newly allocated string, or NULL on allocation failure.
 *
 * @note Does not check if `size` exceeds the actual string length.
 */
char	*ft_strndup(const char *str, size_t size)
{
	char	*new_str;
	char	*ptr;

	if (!str)
		return (NULL);
	new_str = malloc(size + 1);
	if (!new_str)
		return (NULL);
	ptr = new_str;
	while (size--)
		*ptr++ = *str++;
	*ptr = '\0';
	return (new_str);
}

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
 * @note Caller must ensure that `start <= end` and that both belong to the same
 * allocation string. If start == end, returns empty string ("")
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
