/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamahag <BTP_Magna@proton.me>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 22:00:08 by thamahag          #+#    #+#             */
/*   Updated: 2025/07/09 06:15:11 by thamahag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include "stdio.h"

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
	t_fd_list	*previous_node;
	t_fd_list	*present_node;

	previous_node = NULL;
	present_node = *head;
	while (present_node)
	{
		if (present_node->fd == fd)
		{
			if (!previous_node)
				*head = (*head)->next;
			else
				previous_node->next = present_node->next;
			free(present_node->stash);
			free(present_node);
			return ;
		}
		previous_node = present_node;
		present_node = present_node->next;
	}
}

// For debuging initial link list testing
void	print_fd_list(t_fd_list *head)
{
	while (head)
	{
		printf("fd: %d -> ", head->fd);
		head = head->next;
	}
	printf("NULL\n");
}

/**
 * @brief Get the next line bonus object
 *
 * Check if input fd and buffersize is valid
 *
 * @param fd
 * @return char*
 */
char	*get_next_line_bonus(int fd)
{
	static t_fd_list	*stash_head;
	t_fd_list			*stash_node;
	char				*buffer;
	ssize_t				byte_read;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash_node = ft_get_fd_node(&stash_head, fd);
	if (!stash_node)
		return (NULL); // Clean all old list either inside or here decide later
	// buffer = malloc(BUFFER_SIZE + 1);
	// while (stash_node->nl_offset == -1)
	// {
	// 	byte_read = read(fd, buffer, BUFFER_SIZE);
	// 	if (byte_read == -1)
	// 		return (NULL); // Clean buffer and all link list node;
	// 	if (byte_read == 0)
	// 		break ;
	// 	*(buffer + byte_read) = '\0';
	// 	stash_node->stash = NULL;
	// 	if (!stash_node->stash)
	// 		return (NULL); // Clean buffer and all link list node;
	// }
	print_fd_list(stash_head); // For debug
	if (fd == 5)
		ft_remove_fd_node(&stash_head, 4);
	printf("After\n");
	print_fd_list(stash_head);
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
	char	*next_line;

	int fd, fd1, fd2;
	fd = open("../test.txt", O_RDONLY);
	if (fd == -1)
		return (1);
	fd1 = open("../todo", O_RDONLY);
	if (fd1 == -1)
		return (1);
	fd2 = open("../LICENSE", O_RDONLY);
	if (fd1 == -1)
		return (1);
	// next_line = malloc(BUFFER_SIZE);
	// *next_line = '\0';
	// ft_putstr(next_line);
	next_line = get_next_line_bonus(fd);
	next_line = get_next_line_bonus(fd1);
	next_line = get_next_line_bonus(fd);
	next_line = get_next_line_bonus(fd2);
	next_line = get_next_line_bonus(fd1);
	next_line = get_next_line_bonus(fd2);
	while (next_line)
	{
		ft_putstr(next_line);
		free(next_line);
		next_line = get_next_line_bonus(fd);
	}
	close(fd);
	return (0);
}

/**
 * if link list point to null do nothing return
 * Need two pointer one walk ahead first and another one walk behind
 * iterate through the link list if current node equal fd
 * previous node next will point to present node next
 * if it was fd match first node move static link list head address to next one
 *
 * So I set the head to next address if it match fd
 * but still use present pointer in inner loop to free it properly in the loop
 * to limit the line usage other while it would exceed 25 line
 *
 * or I can move if ((*head)->fd == fd)
		*head = (*head)->next;
   to be inside the loop after free present node I can check if *head point
   to present and if so move the head forward and clean it but this way it
   will loop I suppose it would only check once when fd match huh
 */
// This one work as well but will leave two dangling pointer if head == fd
// But it will instantly move assign to head on next call anyway though
void	ft_remove_fd_node_ver0(t_fd_list **head, int fd)
{
	t_fd_list	*previous_node;
	t_fd_list	*present_node;

	if (!*head)
		return ;
	present_node = *head;
	previous_node = *head;
	if ((*head)->fd == fd)
		*head = (*head)->next;
	while (present_node)
	{
		if (present_node->fd == fd)
		{
			previous_node->next = present_node->next;
			if (present_node->stash)
				free(present_node->stash);
			free(present_node);
			return ;
		}
		previous_node = present_node;
		present_node = present_node->next;
	}
}

t_fd_list	*ft_get_fd_node_ver0(t_fd_list **head, int fd)
{
	t_fd_list *node;
	t_fd_list *new_node;

	if (*head)
	{
		node = *head;
		while (node->fd != fd && node->next)
			node = node->next;
		if (node->fd == fd)
			return (node);
	}
	new_node = malloc(sizeof(t_fd_list));
	if (!new_node)
		// need way to clean the head for all it link list node
		return (NULL);      // or maybe clean it outside
	new_node->stash = NULL; // Struct init without explicit assign value will
	new_node->size = 0;     // be assign as NULL or 0 so these two can be remove
	new_node->fd = fd;
	new_node->nl_offset = -1;
	new_node->next = *head;
	*head = new_node;
	return (new_node);
}