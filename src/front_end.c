/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   front_end.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/02 06:37:09 by mbrunel           #+#    #+#             */
/*   Updated: 2020/03/02 13:54:56 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "psm.h"

void	aff_new(t_world *w)
{
	int l_acc, l_name, l_email;
	static char *items[] = {"0-9", "a-z A-Z 0-9", "a-z A-Z 0-9 !-~", NULL};
	static int selected = 0;
	static int l_pass = 16;
	char *len_p;
	int i;

	l_acc = strlen(w->backup->acc);
	l_name = strlen(w->backup->name);
	l_email = strlen(w->backup->email);
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
	nk_layout_row_push(w->nkc->ctx, 0.03f);
	if (!(len_p = itoa(l_pass)))
	{
		w->stage = ERROR;
		return ;
	}
	nk_label(w->nkc->ctx, len_p, NK_TEXT_CENTERED);
	free(len_p);
	nk_layout_row_end(w->nkc->ctx);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 7, 1);
	nk_layout_row_begin(w->nkc->ctx, NK_DYNAMIC, w->nkc->win_height / 9, 4);
	nk_layout_row_push(w->nkc->ctx, 0.25);
	nk_spacing(w->nkc->ctx, 1);
	nk_layout_row_push(w->nkc->ctx, 0.20);
	set_style(w->nkc->ctx, THEME_GREEN);
	if (nk_button_label(w->nkc->ctx, "SAVE"))
	{
		strcpy(w->one->acc, w->backup->acc);
		strcpy(w->one->name, w->backup->name);
		strcpy(w->one->email, w->backup->email);
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

void	aff_one(t_world *w)
{
	int l_acc, l_name, l_email;

	l_acc = strlen(w->backup->acc);
	l_name = strlen(w->backup->name);
	l_email = strlen(w->backup->email);
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
		strcpy(w->one->acc, w->backup->acc);
		strcpy(w->one->name, w->backup->name);
		strcpy(w->one->email, w->backup->email);
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
		if (!w->prev)
		{
			w->one = w->l->next;
			destroy_lst(w->l);
			w->l = w->one;
		}
		else
		{
			w->prev->next = w->one->next;
			destroy_lst(w->one);
		}
		w->stage = DASHBOARD;
	}
	nk_layout_row_end(w->nkc->ctx);
}

void	aff_dashboard(t_world *w)
{
	t_lst *node;
	t_lst *prev = NULL;

	int i = 1;
	node = w->l;
	w->one = NULL;
	w->prev = NULL;
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 18, 1);
	nk_layout_row_begin(w->nkc->ctx, NK_DYNAMIC, w->nkc->win_height / 8, 8);
    nk_layout_row_push(w->nkc->ctx, 0.04f);
	nk_spacing(w->nkc->ctx, 1);
	nk_layout_row_push(w->nkc->ctx, 0.20f);
	set_style(w->nkc->ctx, THEME_GREEN);
	if (nk_button_label(w->nkc->ctx, "NEW"))
	{
		if (!(w->l = create_lst()))
		{
			w->stage = ERROR;
			return ;
		}
		if (!(w->backup = create_lst()))
		{
			destroy_lst(w->l);
			w->stage = ERROR;
			return ;
		}
		w->l->next = node;
		w->one = w->l;
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
			w->prev = prev;
			if (!(w->backup = create_lst()))
			{
				w->stage = ERROR;
				return ;
			}
			strcpy(w->backup->acc, w->one->acc);
			strcpy(w->backup->name, w->one->name);
			strcpy(w->backup->email, w->one->email);
			w->stage = ONE;
		}
		prev = node;
		node = node->next;
		i++;
	}
	set_style(w->nkc->ctx, THEME_RED);
}

void	aff_login(t_world *w)
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
		memcpy(w->log->check + old_len, w->log->masqued + old_len, (nk_size)(w->log->lens[1] - old_len));
	nk_layout_row_end(w->nkc->ctx);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 7, 1);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 10, 5);
	nk_spacing(w->nkc->ctx, 2);
	set_style(w->nkc->ctx, THEME_GREEN);
	if (nk_button_label(w->nkc->ctx, "DONE"))
		set_up_db(w);
}

void	aff_error(t_world *w)
{
	printf("ERROR TARGETTED\n");
	w->nkc->keepRunning = nk_false;
}
