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

#include <psm.h>

void init_log(t_log *log)
{
	ft_memset(log->login, 0, 256);
	ft_memset(log->masqued, '*', 256);
}

void copy_osx(char *buf, char *env[])
{
	/*int in;
	int out;
	int pip[2];

	save_std(&in, &out);
	pipe(pip);
	set_std(pip[0], pip[1]);							IT will work one day
	call(env, "/bin/echo", buf, NULL);
	call(env, "/usr/bin/pbcopy", NULL);
	set_std(0, 1);*/
	char bad[256];
	ft_sprintf(bad, "/bin/echo %s | /usr/bin/pbcopy", buf);
	system(bad);
}

void aff_loading(t_world *w)
{
	struct nk_image img;

	img = nkc_load_image_file(w->nkc, "icon/logo.png");
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 3, 1);
	nk_layout_row_static(w->nkc->ctx, w->nkc->win_width / 7,w->nkc->win_width / 7, 6);
	nk_spacing(w->nkc->ctx, 3);
	nk_image(w->nkc->ctx, img);
}

void load(t_world *w, void *fct)
{
	static int boo = 0;
	pthread_t thread;

	if (!boo)
	{
		boo = 1;
		pthread_create(&thread, NULL, fct, w);
	}
	aff_loading(w);
}

void aff_login(t_world *w)
{
	int old_len = w->log->lens[1];
	int i = 0;
	for (i = 0; i < w->log->lens[1]; ++i) w->log->masqued[i] = '*';

	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 4, 1);
	nk_layout_row_begin(w->nkc->ctx, NK_DYNAMIC, w->nkc->win_height / 12, 3);
	nk_layout_row_push(w->nkc->ctx, 0.25f);
	nk_label(w->nkc->ctx, "login : ", NK_TEXT_RIGHT);
	nk_layout_row_push(w->nkc->ctx, 0.5f);
	nk_edit_string(w->nkc->ctx, NK_EDIT_SIMPLE, w->log->login, w->log->lens, 128, nk_filter_default);
	nk_layout_row_end(w->nkc->ctx);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 10, 1);
	nk_layout_row_begin(w->nkc->ctx, NK_DYNAMIC, w->nkc->win_height / 12, 3);
	nk_layout_row_push(w->nkc->ctx, 0.25f);
	nk_label(w->nkc->ctx, "Password : ", NK_TEXT_RIGHT);
	nk_layout_row_push(w->nkc->ctx, 0.5f);
	nk_edit_string(w->nkc->ctx, NK_EDIT_FIELD, w->log->masqued, w->log->lens + 1, 128, nk_filter_default);
	if (old_len < w->log->lens[1])
		ft_memcpy(w->log->pass + old_len, w->log->masqued + old_len, (nk_size)(w->log->lens[1] - old_len));
	nk_layout_row_end(w->nkc->ctx);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 7, 1);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 10, 5);
	nk_spacing(w->nkc->ctx, 2);
	set_style(w->nkc->ctx, THEME_GREEN);
	if (nk_button_label(w->nkc->ctx, "DONE"))
		w->stage++;
}

void aff_dashboard(t_world *w)
{
	t_lst *node;
	int i = 1;
	node = w->l;
	w->one = NULL;
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 18, 1);
	nk_layout_row_begin(w->nkc->ctx, NK_DYNAMIC, w->nkc->win_height / 8, 8);
    nk_layout_row_push(w->nkc->ctx, 0.04f);
	nk_spacing(w->nkc->ctx, 1);
	nk_layout_row_push(w->nkc->ctx, 0.20f);
	set_style(w->nkc->ctx, THEME_GREEN);
	if (nk_button_label(w->nkc->ctx, "NEW"))
	{
		w->l = create_lst();
		w->l->next = node;
		w->one = w->l;
		w->backup = create_lst();
		w->stage = NEW;
	}
	set_style(w->nkc->ctx, THEME_DARK);
	while (node)
	{
		if (i % 4 == 0)
		{
			nk_layout_row_end(w->nkc->ctx);
			nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 18, 1);
			nk_layout_row_begin(w->nkc->ctx, NK_DYNAMIC, w->nkc->win_height / 8, 8);
		}
		nk_layout_row_push(w->nkc->ctx, 0.04f);
		nk_spacing(w->nkc->ctx, 1);
		nk_layout_row_push(w->nkc->ctx, 0.20f);
		if (nk_button_label(w->nkc->ctx, node->acc))
		{
			w->one = node;
			w->backup = create_lst();
			ft_strcpy(w->backup->acc, w->one->acc);
			ft_strcpy(w->backup->name, w->one->name);
			ft_strcpy(w->backup->email, w->one->email);
			w->stage = ONE;
		}
		node = node->next;
		i++;
	}
	set_style(w->nkc->ctx, THEME_RED);
}

void aff_new(t_world *w)
{
	int l_acc, l_name, l_email;
	static char *items[] = {"a-z A-Z", "a-z A-Z 0-9", "a-z A-Z 0-9 !-~", NULL};
	static int selected = 0;
	static int l_pass = 16;
	int i;

	l_acc = ft_strlen(w->backup->acc);
	l_name = ft_strlen(w->backup->name);
	l_email = ft_strlen(w->backup->email);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 7, 1);
	nk_layout_row_begin(w->nkc->ctx, NK_DYNAMIC, w->nkc->win_height / 15, 3);
	nk_layout_row_push(w->nkc->ctx, 0.25f);
	nk_label(w->nkc->ctx, "LABEL : ", NK_TEXT_RIGHT);
	nk_layout_row_push(w->nkc->ctx, 0.5f);
	nk_edit_string(w->nkc->ctx, NK_EDIT_SIMPLE, w->backup->acc, &l_acc, 128, nk_filter_default);
	w->backup->acc[l_acc] = '\0';
	nk_layout_row_end(w->nkc->ctx);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 16, 1);
	nk_layout_row_begin(w->nkc->ctx, NK_DYNAMIC, w->nkc->win_height / 15, 3);
	nk_layout_row_push(w->nkc->ctx, 0.25f);
	nk_label(w->nkc->ctx, "LOGIN : ", NK_TEXT_RIGHT);
	nk_layout_row_push(w->nkc->ctx, 0.5f);
	nk_edit_string(w->nkc->ctx, NK_EDIT_SIMPLE, w->backup->name, &l_name, 128, nk_filter_default);
	nk_layout_row_end(w->nkc->ctx);
	w->backup->name[l_name] = '\0';
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 16, 1);
	nk_layout_row_begin(w->nkc->ctx, NK_DYNAMIC, w->nkc->win_height / 15, 3);
	nk_layout_row_push(w->nkc->ctx, 0.25f);
	nk_label(w->nkc->ctx, "MAIL : ", NK_TEXT_RIGHT);
	nk_layout_row_push(w->nkc->ctx, 0.5f);
	nk_edit_string(w->nkc->ctx, NK_EDIT_SIMPLE, w->backup->email, &l_email, 128, nk_filter_default);
	w->backup->email[l_email] = '\0';
	nk_layout_row_end(w->nkc->ctx);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 16, 1);
	nk_layout_row_begin(w->nkc->ctx, NK_DYNAMIC, w->nkc->win_height / 15, 3);
	nk_layout_row_push(w->nkc->ctx, 0.25f);
	nk_label(w->nkc->ctx, "PASSWORD : ", NK_TEXT_RIGHT);
	nk_layout_row_push(w->nkc->ctx, 0.24f);
	set_style(w->nkc->ctx, THEME_DARK);
	if (nk_combo_begin_label(w->nkc->ctx, items[selected], nk_vec2(nk_widget_width(w->nkc->ctx), 200))) {
        nk_layout_row_dynamic(w->nkc->ctx, 24, 1);
        for (i = 0; i < 3; ++i)
            if (nk_combo_item_label(w->nkc->ctx, items[i], NK_TEXT_LEFT))
                selected = i;
        nk_combo_end(w->nkc->ctx);
    }
	nk_layout_row_push(w->nkc->ctx, 0.01f);
	nk_spacing(w->nkc->ctx, 1);
	nk_layout_row_push(w->nkc->ctx, 0.20f);
	l_pass = nk_slide_int(w->nkc->ctx, 1, l_pass, 32, 1);
	nk_layout_row_push(w->nkc->ctx, 0.002f);
	nk_spacing(w->nkc->ctx, 1);
	nk_layout_row_push(w->nkc->ctx, 0.01f);
	nk_label(w->nkc->ctx, ft_itoa(l_pass), NK_TEXT_RIGHT);
	nk_layout_row_end(w->nkc->ctx);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 7, 1);
	nk_layout_row_begin(w->nkc->ctx, NK_DYNAMIC, w->nkc->win_height / 9, 4);
	nk_layout_row_push(w->nkc->ctx, 0.25);
	nk_spacing(w->nkc->ctx, 1);
	nk_layout_row_push(w->nkc->ctx, 0.20);
	set_style(w->nkc->ctx, THEME_GREEN);
	if (nk_button_label(w->nkc->ctx, "SAVE"))
	{
		ft_strcpy(w->one->acc, w->backup->acc);
		ft_strcpy(w->one->name, w->backup->name);
		ft_strcpy(w->one->email, w->backup->email);
		w->one->pass = gen_pass(selected, l_pass);
		destroy_lst(w->backup);
		w->stage = DASHBOARD;
	}
	nk_layout_row_push(w->nkc->ctx, 0.10);
	nk_spacing(w->nkc->ctx, 1);
	nk_layout_row_push(w->nkc->ctx, 0.20);
	set_style(w->nkc->ctx, THEME_RED);
	if (nk_button_label(w->nkc->ctx, "CANCEL"))
	{
		destroy_lst(w->backup);
		w->one = w->l->next;
		destroy_lst(w->l);
		w->l = w->one;
		w->stage = DASHBOARD;
	}
	nk_layout_row_end(w->nkc->ctx);
}

void aff_one(t_world *w)
{
	int l_acc, l_name, l_email;

	l_acc = ft_strlen(w->backup->acc);
	l_name = ft_strlen(w->backup->name);
	l_email = ft_strlen(w->backup->email);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 7, 1);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 15, 3);
	nk_label(w->nkc->ctx, "LABEL : ", NK_TEXT_RIGHT);
	nk_edit_string(w->nkc->ctx, NK_EDIT_SIMPLE, w->backup->acc, &l_acc, 128, nk_filter_default);
	w->backup->acc[l_acc] = '\0';
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 16, 1);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 15, 3);
	nk_label(w->nkc->ctx, "LOGIN : ", NK_TEXT_RIGHT);
	nk_edit_string(w->nkc->ctx, NK_EDIT_SIMPLE, w->backup->name, &l_name, 128, nk_filter_default);
	w->backup->name[l_name] = '\0';
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 16, 1);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 15, 3);
	nk_label(w->nkc->ctx, "MAIL : ", NK_TEXT_RIGHT);
	nk_edit_string(w->nkc->ctx, NK_EDIT_SIMPLE, w->backup->email, &l_email, 128, nk_filter_default);
	w->backup->email[l_email] = '\0';
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 16, 1);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 15, 3);
	nk_label(w->nkc->ctx, "PASSWORD : ", NK_TEXT_RIGHT);
	set_style(w->nkc->ctx, THEME_DARK);
	if (nk_button_label(w->nkc->ctx, "COPY"))
		copy_osx(w->one->pass, w->env);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 7, 1);
	nk_layout_row_begin(w->nkc->ctx, NK_DYNAMIC, w->nkc->win_height / 9, 6);
	nk_layout_row_push(w->nkc->ctx, 0.10);
	nk_spacing(w->nkc->ctx, 1);
	nk_layout_row_push(w->nkc->ctx, 0.20);
	set_style(w->nkc->ctx, THEME_GREEN);
	if (nk_button_label(w->nkc->ctx, "SAVE"))
	{
		ft_strcpy(w->one->acc, w->backup->acc);
		ft_strcpy(w->one->name, w->backup->name);
		ft_strcpy(w->one->email, w->backup->email);
		destroy_lst(w->backup);
		w->stage = DASHBOARD;
	}
	nk_layout_row_push(w->nkc->ctx, 0.10);
	nk_spacing(w->nkc->ctx, 1);
	nk_layout_row_push(w->nkc->ctx, 0.20);
	set_style(w->nkc->ctx, THEME_YELLOW);
	if (nk_button_label(w->nkc->ctx, "CANCEL"))
	{
		destroy_lst(w->backup);
		w->stage = DASHBOARD;
	}
	nk_layout_row_push(w->nkc->ctx, 0.10);
	nk_spacing(w->nkc->ctx, 1);
	nk_layout_row_push(w->nkc->ctx, 0.20);
	set_style(w->nkc->ctx, THEME_RED);
	if (nk_button_label(w->nkc->ctx, "DELETE"))
	{
		destroy_lst(w->backup);
		w->one = w->l->next;
		destroy_lst(w->l);
		w->l = w->one;
		w->stage = DASHBOARD;
	}
	nk_layout_row_end(w->nkc->ctx);
}

void mainLoop(void* loopArg)
{
	t_world *w = (t_world*)loopArg;
	struct nk_context *ctx = nkc_get_ctx(w->nkc);
	union nkc_event e = nkc_poll_events(w->nkc);

	if((e.type == NKC_EWINDOW) && (e.window.param == NKC_EQUIT))
		nkc_stop_main_loop(w->nkc);
	SDL_GetWindowSize(w->nkc->window, &(w->nkc->win_width), &(w->nkc->win_height));
	if (!nk_begin(ctx, "Show", nk_rect(0, 0, w->nkc->win_width, w->nkc->win_height),0))
		w->stage = ERROR;
	set_style(w->nkc->ctx, THEME_RED);
	switch (w->stage)
	{
		case LOGIN:
		aff_login(w);
		break ;

		case UNLOCK:
		load(w, &set_up_db);
		break ;

		case DASHBOARD:
		aff_dashboard(w);
		break ;

		case ONE:
		aff_one(w);
		break ;

		case NEW:
		aff_new(w);
		break ;

		default:
		break ;
	}
	nk_end(ctx);
	nkc_render(w->nkc, nk_rgb(40,40,40));
}

int main(int ac, char *av[], char *env[])
{
	struct nkc		nkcx;
	struct AES_ctx	ctx;
	t_world			w;
	t_log			log;

	if (ac != 1 || av[1])
		exit(0);
	w.nkc = &nkcx;
	w.ctx_aes = &ctx;
	w.log = &log;
	init_log(w.log);
	w.stage = LOGIN;
	w.env = env;

	if(nkc_init( w.nkc, "", 0.4,0.4, NKC_WIN_MAXIMIZED))
		nkc_set_main_loop(w.nkc, mainLoop, (void*)&w);
	save_chng(&w);
	nkc_shutdown( w.nkc);
	exit(0);
}
