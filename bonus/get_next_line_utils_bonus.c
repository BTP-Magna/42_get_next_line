/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamahag <BTP_Magna@proton.me>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:26:54 by thamahag          #+#    #+#             */
/*   Updated: 2025/07/10 22:46:26 by thamahag         ###   ########.fr       */
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
	if (!node)          // need way to clean the head for all it link list node
		return (NULL);  // or maybe clean it outside
	node->stash = NULL; // Struct init without explicit assign value will
	node->size = 0;     // be assign as NULL or 0 so these two can be remove
	node->fd = fd;
	node->nl_offset = -1;
	node->next = *head;
	*head = node;
	return (node);
}

/**
 * @brief Delete and clean link list node that match the fd
 *
 * @param head
 * @param fd
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
				*head = (*head)->next;
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
 * @brief Frees entire fd stash list and an optional pointer, returning ret.
 *
 * @param head Double pointer to stash list head.
 * @param to_free Optional memory (like buffer) to free.
 * @param ret Value to return (usually NULL).
 * @return ret
 *
 * @note Cleans all `stash` strings in each node before freeing them.
 */
char	*ft_clear_all_and_return(t_fd_list **head, char *to_free, char *ret)
{
	t_fd_list	*node;

	while (*head)
	{
		node = *head;
		*head = (*head)->next;
		free(node->stash);
		free(node);
	}
	free(to_free);
	return (ret);
}

char	*ft_extract_n_update(t_fd_list *node)
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

char	*ft_strdup(const char *str, size_t size)
{
	char	*n_str;
	char	*ptr;

	if (!str)
		return (NULL);
	n_str = malloc(size + 1);
	if (!n_str)
		return (NULL);
	ptr = n_str;
	while (*str)
		*ptr++ = *str++;
	*ptr = '\0';
	return (n_str);
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