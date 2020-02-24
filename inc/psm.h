/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   psm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 01:28:25 by mbrunel           #+#    #+#             */
/*   Updated: 2020/02/24 21:09:55 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PSM_H
# define PSM_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <libft.h>
# include <errno.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <fcntl.h>
# include <front.h>
# include <pthread.h>
# include <aes.h>

# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define PURPLE "\033[35m"
# define CYAN "\033[36m"
# define NONE "\033[0m"
# define PREFIX 5
#define LEN_MAX 99999

typedef struct			s_db
{
	uint8_t				iv;
	void				*env;
}						t_db;

typedef struct			s_lst
{
	char				*line;
	int					len;
	struct s_lst		*next;
}						t_lst;

typedef struct			s_world
{
	t_log				*log;
	struct nkc			*nkc;
	int					stage;
	t_db				db;
	t_lst				*l;
	struct AES_ctx		ctx_aes;
}						t_world;

void	*set_up_db(void *arg);
int		call(char *env[], char *exec , ...);
void	save_std(int *in, int *out);
void	set_std(int in, int out);

#endif
