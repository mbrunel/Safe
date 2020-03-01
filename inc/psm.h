/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   psm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 01:28:25 by mbrunel           #+#    #+#             */
/*   Updated: 2020/03/01 06:43:05 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PSM_H
# define PSM_H

# include <front.h>
# include <utils.h>

# include <aes.h>

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <libft.h>
# include <errno.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <fcntl.h>
# include <pthread.h>

# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define PURPLE "\033[35m"
# define CYAN "\033[36m"
# define NONE "\033[0m"
# define PREFIX 5
# define LEN_MAX 99999

typedef struct			s_world
{
	t_log				*log;
	struct nkc			*nkc;
	int					stage;
	t_lst				*l;
	struct AES_ctx		*ctx_aes;
	void				*env;
	t_lst				*one;
	t_lst				*backup;
	char				*home;
}						t_world;

void	*set_up_db(void *arg);
void	save_chng(t_world *w);
#endif
