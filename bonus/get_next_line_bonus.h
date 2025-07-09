/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamahag <BTP_Magna@proton.me>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 21:58:16 by thamahag          #+#    #+#             */
/*   Updated: 2025/07/10 04:31:07 by thamahag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <stdlib.h>
# include <unistd.h>

// Define BUFFER_SIZE if not provided via compiler flag (-D BUFFER_SIZE=42)
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

typedef struct s_fd_list
{
	int					fd;
	char				*stash;
	ssize_t				size;
	ssize_t nl_offset; // or char *nl_ptr;
	struct s_fd_list	*next;
}						t_fd_list;

char					*get_next_line(int fd);
char					*ft_gnl_append_stash(t_fd_list *node, char *buffer,
							ssize_t buff_size);
char					*ft_gnl_extract(char **st_buff, char *nl_ptr);

t_fd_list				*ft_get_fd_node(t_fd_list **head, int fd);
void					ft_remove_fd_node(t_fd_list **head, int fd);
char					*ft_extract_n_update(t_fd_list *node);
char					*ft_strdup(const char *str, size_t size);
char					*ft_strslice(const char *start, const char *end);
char	*ft_clear_all_and_return(t_fd_list **head, char *to_free, char *ret);
#endif
