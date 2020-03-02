/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 18:31:24 by mbrunel           #+#    #+#             */
/*   Updated: 2020/02/29 06:22:50 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef NKC_IMPLEMENTATION
#  define NKC_IMPLEMENTATION
# endif

#include "psm.h"

#include "aes.h"

void mainLoop(void* loopArg)
{
	t_world				*w = (t_world*)loopArg;
	struct nk_context	*ctx = nkc_get_ctx(w->nkc);
	union nkc_event		e = nkc_poll_events(w->nkc);
	void				(*aff[NB])(t_world *w) = {aff_error, aff_login, aff_dashboard, aff_one, aff_new};

	if((e.type == NKC_EWINDOW) && (e.window.param == NKC_EQUIT))
		nkc_stop_main_loop(w->nkc);
	SDL_GetWindowSize(w->nkc->window, &(w->nkc->win_width), &(w->nkc->win_height));
	if (!nk_begin(ctx, "Show", nk_rect(0, 0, w->nkc->win_width, w->nkc->win_height),0))
		w->stage = ERROR;
	set_style(w->nkc->ctx, THEME_RED);
	if (w->nkc->keepRunning == ALWAYS_A_THIRD_OPTION)
	{
		w->nkc->keepRunning = nk_false;
		save_chng(w);
	}
	else
		(*aff[w->stage])(w);
	nk_end(ctx);
	nkc_render(w->nkc, nk_rgb(40,40,40));
}

int main(int ac, char *av[], char *env[])
{
	struct nkc		nkcx;
	struct AES_ctx	aes;
	t_world			w;
	t_log			log;

	if (ac != 1 || av[1])
		exit(0);
	w = (t_world){&log, &nkcx, LOGIN, NULL, &aes, env, NULL, NULL, NULL, NULL};
	memset(w.log->login, 0, 256);
	memset(w.log->check, 0, 256);
	if(nkc_init( w.nkc, "", 0.4,0.4, NKC_WIN_MAXIMIZED))
		nkc_set_main_loop(w.nkc, mainLoop, (void*)&w);
	nkc_shutdown( w.nkc);
	exit(0);
}
