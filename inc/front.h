/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   front.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 18:34:00 by mbrunel           #+#    #+#             */
/*   Updated: 2020/02/19 20:41:16 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRONT_H
# define FRONT_H

# ifndef NKC_IMPLEMENTATION
#  define NKC_IMPLEMENTATION
# endif

# include "nuklear_cross.h"

enum stage {ERROR, LOGIN, UNLOCK};

typedef struct			s_log
{
	char				login[256];
	char				pass[256];
	char				masqued[256];
	int					lens[10];
}						t_log;

typedef struct			s_front
{
	t_log				*log;
	struct nkc			*nkc;
	int					stage;
}						t_front;

void					set_style(struct nk_context *ctx, enum theme theme);


#endif
