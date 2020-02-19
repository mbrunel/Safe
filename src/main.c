/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 18:31:24 by mbrunel           #+#    #+#             */
/*   Updated: 2020/02/19 18:31:24 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <front.h>
#include <psm.h>

void init_log(t_log *log)
{
	ft_memset(log->login, 0, 256);
	ft_memset(log->masqued, '*', 256);
}

void aff_unlock(t_front *w)
{
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 3, 1);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 3, 1);
	nk_label(w->nkc->ctx, "OWEEEEE", NK_TEXT_CENTERED);
}

void aff_login(t_front *w)
{
	int old_len = w->log->lens[1];
	int i = 0;
	for (i = 0; i < w->log->lens[1]; ++i) w->log->masqued[i] = '*';

	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 4, 1);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 15, 3);
	nk_label(w->nkc->ctx, "login : ", NK_TEXT_RIGHT);
	nk_edit_string(w->nkc->ctx, NK_EDIT_SIMPLE, w->log->login, w->log->lens, 64, nk_filter_default);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 10, 1);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 15, 3);
	nk_label(w->nkc->ctx, "Password:", NK_TEXT_RIGHT);
	nk_edit_string(w->nkc->ctx, NK_EDIT_FIELD, w->log->masqued, w->log->lens + 1, 64, nk_filter_default);
	if (old_len < w->log->lens[1])
		ft_memcpy(w->log->pass + old_len, w->log->masqued + old_len, (nk_size)(w->log->lens[1] - old_len));
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 10, 1);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 15, 5);
	nk_label(w->nkc->ctx, "", NK_TEXT_RIGHT);
	nk_label(w->nkc->ctx, "", NK_TEXT_RIGHT);
	if (nk_button_label(w->nkc->ctx, "DONE"))
		w->stage = UNLOCK;
}

void mainLoop(void* loopArg)
{
	t_front *w = (t_front*)loopArg;
	struct nk_context *ctx = nkc_get_ctx(w->nkc);
	union nkc_event e = nkc_poll_events(w->nkc);

	if((e.type == NKC_EWINDOW) && (e.window.param == NKC_EQUIT))
		nkc_stop_main_loop(w->nkc);
	if (!nk_begin(ctx, "Show", nk_rect(0, 0, w->nkc->win_width, w->nkc->win_height),0))
		w->stage = ERROR;
	set_style(w->nkc->ctx, THEME_RED);
	switch (w->stage)
	{
		case LOGIN:
		aff_login(w);
		break ;

		case UNLOCK:
		aff_unlock(w);
		break ;

		default:
		break ;
	}
	nk_end(ctx);
	nkc_render(w->nkc, nk_rgb(40,40,40) );
}

int main()
{
    struct nkc	nkcx;
	t_front		w;
	t_log		log;

	w.nkc = &nkcx;
	w.log = &log;
	init_log(w.log);
	w.stage = LOGIN;
    if(!nkc_init( w.nkc, "PS manager", 0.4,0.4, NKC_WIN_MAXIMIZED) )
		nkc_shutdown( w.nkc);
	nkc_set_main_loop(w.nkc, mainLoop, (void*)&w);
	nkc_shutdown( w.nkc);
	return (0);
}
