/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thamahag <BTP_Magna@proton.me>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 21:58:16 by thamahag          #+#    #+#             */
/*   Updated: 2025/07/07 01:56:17 by thamahag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

typedef struct s_fd_list
{
	int					fd;
	char				*excess;
	struct s_fd_list	*next;
}						t_fd_list;

char					*get_next_line(int fd);
char					*ft_gnl_join(char *static_buffer, char *buffer,
							char **new_line_ptr);
char					*get_next_line(int fd);

char					*ft_strchr(const char *str, int c);
char					*ft_strdup(const char *str);
char					*ft_substr(const char *from, char *to);
size_t					ft_strlen(const char *str);
char					*ft_free_n_return(char *free1, char *free2, char *ret);

#endif
