/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   front.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 18:34:00 by mbrunel           #+#    #+#             */
/*   Updated: 2020/03/01 06:50:45 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRONT_H
# define FRONT_H

# include "nuklear_cross.h"

enum theme {THEME_BLACK, THEME_WHITE, THEME_RED, THEME_BLUE, THEME_DARK, THEME_GREEN, THEME_YELLOW};

enum stage {ERROR, LOGIN, UNLOCK, DASHBOARD, NEW, ONE};

enum pass {ALPHA, ALNUM, GRAPH};

typedef struct			s_log
{
	char				login[256];
	char				pass[256];
	char				masqued[256];
	int					lens[10];
}						t_log;

void					set_style(struct nk_context *ctx, enum theme theme);

#endif
