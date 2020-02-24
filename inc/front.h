/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   front.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 18:34:00 by mbrunel           #+#    #+#             */
/*   Updated: 2020/02/24 19:50:59 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRONT_H
# define FRONT_H

# include "nuklear_cross.h"

enum stage {ERROR, LOGIN, UNLOCK, DASHBOARD};

typedef struct			s_log
{
	char				login[256];
	char				pass[256];
	char				masqued[256];
	int					lens[10];
}						t_log;

void					set_style(struct nk_context *ctx, enum theme theme);

#endif
