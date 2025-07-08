/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamahag <BTP_Magna@proton.me>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 21:58:16 by thamahag          #+#    #+#             */
/*   Updated: 2025/07/08 18:48:05 by thamahag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <stdlib.h>
# include <unistd.h>

// Define BUFFER_SIZE if not provided via compiler flag (-D BUFFER_SIZE=42)
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

typedef struct s_fd_list
{
	int					fd;
	char				*stash;
	ssize_t				size;
	ssize_t				nl_offset; // or char *nl_ptr;
	struct s_fd_list	*next;
}						t_fd_list;

char					*get_next_line(int fd);
char					*ft_gnl_join(char *st_buff, char *buffer,
							ssize_t b_size, char **nl_ptr);
char					*ft_gnl_extract(char **st_buff, char *nl_ptr);

char					*ft_strchr(const char *str, int c);
char					*ft_strdup(const char *str);
char					*ft_strslice(const char *start, const char *end);
size_t					ft_strlen(const char *str);
char					*ft_free_n_return(char **ptr1, char *ptr2, char *ret);

#endif
