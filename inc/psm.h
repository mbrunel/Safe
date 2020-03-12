/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   psm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 01:28:25 by mbrunel           #+#    #+#             */
/*   Updated: 2020/03/12 16:11:16 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PSM_H
# define PSM_H

# include "utils.h"

# include "nuklear_cross.h"

# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define PURPLE "\033[35m"
# define CYAN "\033[36m"
# define NONE "\033[0m"
# define PREFIX 5
# define LEN_MAX 99999

# ifndef HOME
#  define HOME "/"
# endif

enum stage 				{ERROR, LOGIN, DASHBOARD, ONE, NEW, NB};

typedef struct			s_log
{
	char				login[256];
	unsigned char		pass[33];
	unsigned char		tmp[33];
	char				check[256];
	char				second[256];
	char				masqued[256];
	int					lens[10];
}						t_log;

typedef struct			s_img
{
	struct nk_image		show;
	struct nk_image		hide;
	struct nk_image		cpy;
	struct nk_image		reroll;
	struct nk_image		pinguin;
	struct nk_image		logo;
}						t_img;

typedef struct			s_popup
{
	int					wrong_pass;
	int					new_user;
}						t_popup;

typedef struct			s_keybinds
{
	int					*enter;
	int					*tab;
	int					*esc;
	int					*right;
	int					*left;
	int					*bot;
	int					*top;
}						t_keybinds;

typedef struct			s_world
{
	t_img				*img;
	t_log				*log;
	struct nkc			*nkc;
	int					stage;
	t_lst				*l;
	struct AES_ctx		*ctx_aes;
	void				*env;
	t_lst				*one;
	t_lst				*backup;
	t_lst				*prev;
	char				*home;
	t_popup				*popup;
	void				*av;
}						t_world;

void					set_style(struct nk_context *ctx, enum theme theme);

void					*set_up_db(void *arg);
void					*save_chng(t_world *w);

void					aff_error(t_world *w);
void					aff_login(t_world *w);
void					aff_one(t_world *w);
void					aff_new(t_world *w);
void					aff_dashboard(t_world *w);
void					aff_loading(t_world *w);

#endif
