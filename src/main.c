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

#include <signal.h>

int catch_exceptions = 0;

void hook(int sig)
{
	if (sig == SIGQUIT || sig == SIGINT || sig == SIGTERM)
		catch_exceptions = 2;
	else if (sig == SIGSEGV || sig == SIGABRT || sig == SIGPIPE || sig == SIGFPE)
		catch_exceptions = 1;
}

void exceptions_handler(void)
{
	/*signal(SIGINT, hook);
	signal(SIGQUIT, hook);
	signal(SIGTERM, hook);
	signal(SIGSEGV, hook);
	signal(SIGABRT, hook);
	signal(SIGPIPE, hook);
	signal(SIGFPE, hook);*/
}

void load_img(t_world *w)
{
	if (OSX == 1)
	{
	char *imgs;
	char home[1024];
	strcpy(home, search_env("HOME", w->env));
	if ((imgs = strjoin(home, "/Applications/Safe.app/Contents/icon/logo.psd")))
	{
		w->img->logo = nkc_load_image_file(w->nkc, imgs);
		free(imgs);
	}
	if ((imgs = strjoin(home, "/Applications/Safe.app/Contents/icon/view_hide_icon.png")))
	{
		w->img->hide = nkc_load_image_file(w->nkc, imgs);
		free(imgs);
	}
	if ((imgs = strjoin(home, "/Applications/Safe.app/Contents/icon/view_show_icon.png")))
	{
		w->img->show = nkc_load_image_file(w->nkc, imgs);
		free(imgs);
	}
	if ((imgs = strjoin(home, "/Applications/Safe.app/Contents/icon/cpy.png")))
	{
		w->img->cpy = nkc_load_image_file(w->nkc, imgs);
		free(imgs);
	}
	if ((imgs = strjoin(home, "/Applications/Safe.app/Contents/icon/reroll.png")))
	{
		w->img->reroll = nkc_load_image_file(w->nkc, imgs);
		free(imgs);
	}
	if ((imgs = strjoin(home, "/Applications/Safe.app/Contents/icon/pinguin.png")))
	{
		w->img->pinguin = nkc_load_image_file(w->nkc, imgs);
		free(imgs);
	}
	}
	if (LINUX == 1)
	{
	w->img->logo = nkc_load_image_file(w->nkc, "/usr/share/icons/Safe/logo.psd");
	w->img->hide = nkc_load_image_file(w->nkc, "/usr/share/icons/Safe/view_hide_icon.png");
	w->img->show = nkc_load_image_file(w->nkc, "/usr/share/icons/Safe/view_show_icon.png");
	w->img->cpy = nkc_load_image_file(w->nkc, "/usr/share/icons/Safe/cpy.png");
	w->img->reroll = nkc_load_image_file(w->nkc, "/usr/share/icons/Safe/reroll.png");
	w->img->pinguin = nkc_load_image_file(w->nkc, "/usr/sare/icons/Safe/pinguin.png");
	}
}
void mainLoop(void* loopArg)
{
	t_world				*w = (t_world*)loopArg;
	struct nk_context	*ctx = nkc_get_ctx(w->nkc);
	union nkc_event		e = nkc_poll_events(w->nkc);
	void				(*aff[NB])(t_world *w) = {aff_error, aff_login, aff_dashboard, aff_one, aff_new};

	if((e.type == NKC_EWINDOW) && (e.window.param == NKC_EQUIT))
		nkc_stop_main_loop(w->nkc);
	SDL_GetWindowSize(w->nkc->window, &(w->nkc->win_width), &(w->nkc->win_height));
	if (!nk_begin(ctx, "Show", nk_rect(0, 0, w->nkc->win_width, w->nkc->win_height),0) || catch_exceptions == 1)
		w->stage = ERROR;
	set_style(w->nkc->ctx, THEME_RED);
	if (w->nkc->keepRunning == ALWAYS_A_THIRD_OPTION || catch_exceptions == 2)
	{
		w->nkc->keepRunning = nk_false;
		if (w->stage > LOGIN)
			save_chng(w);
	}
	else
		(*aff[w->stage])(w);
	nk_end(ctx);
	nkc_render(w->nkc, nk_rgb(50,50,50));
}

int main(int ac, char *av[], char *env[])
{
	struct nkc		nkcx;
	struct AES_ctx	aes;
	t_img			 img;
	t_world			w;
	t_log			log;
	t_popup			popup = {0, 0};

	if (ac != 1 || av[1])
		exit(0);
	w = (t_world){&img, &log, &nkcx, LOGIN, NULL, &aes, env, NULL, NULL, NULL, NULL, &popup, (void*)av};
	memset(w.log->login, 0, 256);
	memset(w.log->check, 0, 256);
	w.log->lens[0] = 0;w.log->lens[1] = 0;w.log->lens[2] = 0;
	exceptions_handler();
	if(nkc_init( w.nkc, "", 0.4,0.4, NKC_WIN_MAXIMIZED))
	{
		load_img(&w);
		nkc_set_main_loop(w.nkc, mainLoop, (void*)&w);
	}
	nkc_shutdown( w.nkc);
	return (0);
}
