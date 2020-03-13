/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   front_end.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/02 06:37:09 by mbrunel           #+#    #+#             */
/*   Updated: 2020/03/13 12:22:52 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "psm.h"

#include <unistd.h>

void	aff_new(t_world *w)
{
	int l_acc, l_name, l_email;
	static char *items[] = {"0-9", "a-z A-Z 0-9", "a-z A-Z 0-9 !-~", NULL};
	static int l_pass = 16;
	char *len_p;
	int i;
	static int	boo;
	static int selected;
	int force = 0;
	t_keybinds key = {&(w->nkc->ctx->input.keyboard.keys[NK_KEY_ENTER].pity), &(w->nkc->ctx->input.keyboard.keys[NK_KEY_TAB].pity),
		&(w->nkc->ctx->input.keyboard.keys[NK_KEY_ESC].pity), &(w->nkc->ctx->input.keyboard.keys[NK_KEY_RIGHT].pity), &(w->nkc->ctx->input.keyboard.keys[NK_KEY_LEFT].pity),
			&(w->nkc->ctx->input.keyboard.keys[NK_KEY_UP].pity), &(w->nkc->ctx->input.keyboard.keys[NK_KEY_DOWN].pity)};

	if (w->nkc->ctx->input.mouse.buttons[NK_BUTTON_LEFT].down)
		boo = -1;
	if (*key.tab == 1)
	{
		*key.tab = 0;
		boo = (boo + 1) % 3;
	}
	if (*key.top == 1)
	{
		*key.top = 0;
		boo = boo + 1 > 2 ? boo : boo + 1;
	}
	if (*key.bot == 1)
	{
		*key.bot = 0;
		boo = boo - 1 >= 0 ? boo - 1: boo;
	}
	l_acc = strlen(w->backup->acc);
	l_name = strlen(w->backup->name);
	l_email = strlen(w->backup->email);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 7, 1);
	nk_layout_row_begin(w->nkc->ctx, NK_DYNAMIC, w->nkc->win_height / 15, 3);
	nk_layout_row_push(w->nkc->ctx, 0.25f);
	nk_label(w->nkc->ctx, "LABEL : ", NK_TEXT_RIGHT);
	nk_layout_row_push(w->nkc->ctx, 0.5f);
	if (!boo)
		nk_edit_focus(w->nkc->ctx, NK_EDIT_SIMPLE|NK_EDIT_GOTO_END_ON_ACTIVATE);
	else if (boo != -1)
		nk_edit_unfocus(w->nkc->ctx);
	nk_edit_string(w->nkc->ctx, NK_EDIT_SIMPLE|NK_EDIT_GOTO_END_ON_ACTIVATE, w->backup->acc, &l_acc, 128, nk_filter_default);
	w->backup->acc[l_acc] = '\0';
	nk_layout_row_end(w->nkc->ctx);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 16, 1);
	nk_layout_row_begin(w->nkc->ctx, NK_DYNAMIC, w->nkc->win_height / 15, 3);
	nk_layout_row_push(w->nkc->ctx, 0.25f);
	nk_label(w->nkc->ctx, "LOGIN : ", NK_TEXT_RIGHT);
	nk_layout_row_push(w->nkc->ctx, 0.5f);
	if (boo == 1)
		nk_edit_focus(w->nkc->ctx, NK_EDIT_SIMPLE|NK_EDIT_GOTO_END_ON_ACTIVATE);
	else if (boo != -1)
		nk_edit_unfocus(w->nkc->ctx);
	nk_edit_string(w->nkc->ctx, NK_EDIT_SIMPLE|NK_EDIT_GOTO_END_ON_ACTIVATE, w->backup->name, &l_name, 128, nk_filter_default);
	force = w->nkc->ctx->current->edit.name;
	nk_layout_row_end(w->nkc->ctx);
	w->backup->name[l_name] = '\0';
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 16, 1);
	nk_layout_row_begin(w->nkc->ctx, NK_DYNAMIC, w->nkc->win_height / 15, 3);
	nk_layout_row_push(w->nkc->ctx, 0.25f);
	nk_label(w->nkc->ctx, "MAIL : ", NK_TEXT_RIGHT);
	nk_layout_row_push(w->nkc->ctx, 0.5f);
	if (boo == 2)
		nk_edit_focus(w->nkc->ctx, NK_EDIT_SIMPLE|NK_EDIT_GOTO_END_ON_ACTIVATE);
	else if (boo != -1)
		nk_edit_unfocus(w->nkc->ctx);
	nk_edit_string(w->nkc->ctx, NK_EDIT_SIMPLE|NK_EDIT_GOTO_END_ON_ACTIVATE, w->backup->email, &l_email, 128, nk_filter_default);
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
	boo = force ? force : w->nkc->ctx->current->edit.name;
	if (nk_button_label(w->nkc->ctx, "SAVE") || *key.enter == 1)
	{
		if (*key.enter == 1 && !boo)
			boo = 1;
		else if (*key.enter == 1 && boo == 1)
			boo = 2;
		else
		{
			strcpy(w->one->acc, w->backup->acc);
			strcpy(w->one->name, w->backup->name);
			strcpy(w->one->email, w->backup->email);
			if (w->one->pass)
				free(w->one->pass);
			w->one->pass = gen_pass(selected, l_pass);
			destroy_lst(w->backup);
			w->stage = DASHBOARD;
		}
		*key.enter = 0;
	}
	nk_layout_row_push(w->nkc->ctx, 0.10);
	nk_spacing(w->nkc->ctx, 1);
	nk_layout_row_push(w->nkc->ctx, 0.20);
	set_style(w->nkc->ctx, THEME_YELLOW);
	if (nk_button_label(w->nkc->ctx, "CANCEL") || *key.esc == 1)
	{
		*key.esc = 0;
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
	int l_acc, l_name, l_email, l_pass;
	static int see_pass = 0;
	int i;
	int force = 0;
	static int boo;
	t_keybinds key = {&(w->nkc->ctx->input.keyboard.keys[NK_KEY_ENTER].pity), &(w->nkc->ctx->input.keyboard.keys[NK_KEY_TAB].pity),
		&(w->nkc->ctx->input.keyboard.keys[NK_KEY_ESC].pity), &(w->nkc->ctx->input.keyboard.keys[NK_KEY_RIGHT].pity), &(w->nkc->ctx->input.keyboard.keys[NK_KEY_LEFT].pity),
			&(w->nkc->ctx->input.keyboard.keys[NK_KEY_UP].pity), &(w->nkc->ctx->input.keyboard.keys[NK_KEY_DOWN].pity)};

	if (w->nkc->ctx->input.mouse.buttons[NK_BUTTON_LEFT].down)
		boo = -1;
	if (*key.tab == 1)
	{
		*key.tab = 0;
		boo = (boo + 1) % 3;
	}
	if (*key.top == 1)
	{
		*key.top = 0;
		boo = boo + 1 > 2 ? boo : boo + 1;
	}
	if (*key.bot == 1)
	{
		*key.bot = 0;
		boo = boo - 1 >= 0 ? boo - 1: boo;
	}
	l_pass = strlen(w->backup->pass);
	l_acc = strlen(w->backup->acc);
	l_name = strlen(w->backup->name);
	l_email = strlen(w->backup->email);
	for (i = 0; i < l_pass; ++i) w->log->masqued[i] = '*';
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 7, 1);
	nk_layout_row_begin(w->nkc->ctx, NK_DYNAMIC, w->nkc->win_height / 15, 3);
	nk_layout_row_push(w->nkc->ctx, 0.25f);
	nk_label(w->nkc->ctx, "LABEL : ", NK_TEXT_RIGHT);
	nk_layout_row_push(w->nkc->ctx, 0.5f);
	if (!boo)
		nk_edit_focus(w->nkc->ctx, NK_EDIT_SIMPLE|NK_EDIT_GOTO_END_ON_ACTIVATE);
	else if (boo != -1)
		nk_edit_unfocus(w->nkc->ctx);
	nk_edit_string(w->nkc->ctx, NK_EDIT_SIMPLE|NK_EDIT_GOTO_END_ON_ACTIVATE, w->backup->acc, &l_acc, 128, nk_filter_default);
	w->backup->acc[l_acc] = '\0';
	nk_layout_row_end(w->nkc->ctx);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 16, 1);
	nk_layout_row_begin(w->nkc->ctx, NK_DYNAMIC, w->nkc->win_height / 15, 3);
	nk_layout_row_push(w->nkc->ctx, 0.25f);
	nk_label(w->nkc->ctx, "LOGIN : ", NK_TEXT_RIGHT);
	nk_layout_row_push(w->nkc->ctx, 0.5f);
	if (boo == 1)
		nk_edit_focus(w->nkc->ctx, NK_EDIT_SIMPLE|NK_EDIT_GOTO_END_ON_ACTIVATE);
	else if (boo != -1)
		nk_edit_unfocus(w->nkc->ctx);
	nk_edit_string(w->nkc->ctx, NK_EDIT_SIMPLE|NK_EDIT_GOTO_END_ON_ACTIVATE, w->backup->name, &l_name, 128, nk_filter_default);
	nk_layout_row_end(w->nkc->ctx);
	w->backup->name[l_name] = '\0';
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 16, 1);
	nk_layout_row_begin(w->nkc->ctx, NK_DYNAMIC, w->nkc->win_height / 15, 3);
	nk_layout_row_push(w->nkc->ctx, 0.25f);
	force = w->nkc->ctx->current->edit.name;
	nk_label(w->nkc->ctx, "MAIL : ", NK_TEXT_RIGHT);
	nk_layout_row_push(w->nkc->ctx, 0.5f);
	if (boo == 2)
		nk_edit_focus(w->nkc->ctx, NK_EDIT_SIMPLE|NK_EDIT_GOTO_END_ON_ACTIVATE);
	else if (boo != -1)
		nk_edit_unfocus(w->nkc->ctx);
	nk_edit_string(w->nkc->ctx, NK_EDIT_SIMPLE|NK_EDIT_GOTO_END_ON_ACTIVATE, w->backup->email, &l_email, 128, nk_filter_default);
	w->backup->email[l_email] = '\0';
	nk_layout_row_end(w->nkc->ctx);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 16, 1);
	nk_layout_row_begin(w->nkc->ctx, NK_DYNAMIC, w->nkc->win_height / 15, 3);
	nk_layout_row_push(w->nkc->ctx, 0.25f);
	nk_label(w->nkc->ctx, "PASSWORD : ", NK_TEXT_RIGHT);
	set_style(w->nkc->ctx, THEME_DARK);
	nk_layout_row_push(w->nkc->ctx, 0.36);
	if (!see_pass)
		nk_edit_string(w->nkc->ctx, NK_EDIT_READ_ONLY, w->log->masqued, &l_pass, 128, nk_filter_default);
	else
		nk_edit_string(w->nkc->ctx, NK_EDIT_READ_ONLY, w->backup->pass, &l_pass, 128, nk_filter_default);
	w->backup->pass[l_pass] = '\0';
	nk_layout_row_push(w->nkc->ctx, 0.042f);
	if (!see_pass){
		if (nk_button_image(w->nkc->ctx, w->img->show))
			see_pass = 1;
	}
	else
		if (nk_button_image(w->nkc->ctx, w->img->hide))
			see_pass = 0;
	nk_layout_row_push(w->nkc->ctx, 0.042f);
	if (nk_button_image(w->nkc->ctx, w->img->reroll))
		if (!(w->backup->pass = new_pass(w->backup->pass)))
		{
			w->stage = ERROR;
			return ;
		}
	nk_layout_row_push(w->nkc->ctx, 0.042f);
	if (nk_button_image(w->nkc->ctx, w->img->cpy))
		SDL_SetClipboardText(w->backup->pass);
	nk_layout_row_end(w->nkc->ctx);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 7, 1);
	nk_layout_row_begin(w->nkc->ctx, NK_DYNAMIC, w->nkc->win_height / 9, 6);
	nk_layout_row_push(w->nkc->ctx, 0.10);
	nk_spacing(w->nkc->ctx, 1);
	nk_layout_row_push(w->nkc->ctx, 0.20);
	set_style(w->nkc->ctx, THEME_GREEN);
	boo = force ? force : w->nkc->ctx->current->edit.name;
	if (nk_button_label(w->nkc->ctx, "SAVE") || *key.enter == 1)
	{
		*key.enter = 0;
		strcpy(w->one->acc, w->backup->acc);
		strcpy(w->one->name, w->backup->name);
		strcpy(w->one->email, w->backup->email);
		strcpy(w->one->pass, w->backup->pass);
		destroy_lst(w->backup);
		see_pass = 0;
		w->stage = DASHBOARD;
	}
	nk_layout_row_push(w->nkc->ctx, 0.10);
	nk_spacing(w->nkc->ctx, 1);
	nk_layout_row_push(w->nkc->ctx, 0.20);
	set_style(w->nkc->ctx, THEME_YELLOW);
	if (nk_button_label(w->nkc->ctx, "CANCEL") || *key.esc == 1)
	{
		*key.esc = 0;
		destroy_lst(w->backup);
		w->stage = DASHBOARD;
		see_pass = 0;
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
		see_pass = 0;
		w->stage = DASHBOARD;
	}
	nk_layout_row_end(w->nkc->ctx);
}

static void mov_b(t_world *w, int *board, int reset, int max)
{
	static int i;
	static char paf[4] = {0, 0, 0, 0};

	if (reset)
	{
		board[i] = 0;
		memset(paf, 0, 4);
		return ;
	}
	board[i] = 0;
	if (w->nkc->ctx->input.keyboard.keys[NK_KEY_UP].clicked)
	{
		if (!paf[0])
		{
			i = i - 4 >= 0 ? i - 4 : i;
			paf[0] = -2;
		}
		else
			paf[0]++;
	}
	if (w->nkc->ctx->input.keyboard.keys[NK_KEY_LEFT].clicked)
	{
		if (!paf[1])
		{
			i = i -1 >= 0 ? i -1 : i;
			paf[1] = -1;
		}
		else
			paf[1]++;
	}
	if (w->nkc->ctx->input.keyboard.keys[NK_KEY_RIGHT].clicked)
	{
		if (!paf[2])
		{
			i = i +1 <= max ? i +1 : i;
			paf[2] = -1;
		}
		else
			paf[2]++;
	}
	if (w->nkc->ctx->input.keyboard.keys[NK_KEY_DOWN].clicked)
	{
		if (!paf[3])
		{
			i = i + 4 <= max ? i + 4 : i;
			paf[3] = -2;
		}
		else
			paf[3]++;
	}
	board[i] = 1;
	if (w->nkc->ctx->input.keyboard.keys[NK_KEY_ENTER].pity == 1)
	{
		w->nkc->ctx->input.keyboard.keys[NK_KEY_ENTER].pity = 0;
		board[i] = 2;
	}
}

static void illuminate(t_world *w)
{
	w->nkc->ctx->style.button.normal.data.color.r += 20;
	w->nkc->ctx->style.button.normal.data.color.g += 20;
	w->nkc->ctx->style.button.normal.data.color.b += 20;
	w->nkc->ctx->style.button.active.data.color.r += 20;
	w->nkc->ctx->style.button.active.data.color.b += 20;
	w->nkc->ctx->style.button.active.data.color.g += 20;
	w->nkc->ctx->style.button.hover.data.color.r += 20;
	w->nkc->ctx->style.button.hover.data.color.b += 20;
	w->nkc->ctx->style.button.hover.data.color.g += 20;
}

static void back(t_world *w)
{
	w->nkc->ctx->style.button.normal.data.color.r -= 20;
	w->nkc->ctx->style.button.normal.data.color.g -= 20;
	w->nkc->ctx->style.button.normal.data.color.b -= 20;
	w->nkc->ctx->style.button.active.data.color.r -= 20;
	w->nkc->ctx->style.button.active.data.color.b -= 20;
	w->nkc->ctx->style.button.active.data.color.g -= 20;
	w->nkc->ctx->style.button.hover.data.color.r -= 20;
	w->nkc->ctx->style.button.hover.data.color.b -= 20;
	w->nkc->ctx->style.button.hover.data.color.g -= 20;
}

int		nb_widg(t_lst *node)
{
	int i = 0;

	while (node)
	{
		node = node->next;
		i++;
	}
	return (i);
}

void	aff_dashboard(t_world *w)
{
	t_lst			*node;
	t_lst			*prev = NULL;
	static int		board[3000] = {0};

	int i = 1;
	node = w->l;
	w->one = NULL;
	w->prev = NULL;
	mov_b(w, board, 0, nb_widg(node));
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 18, 1);
	nk_layout_row_begin(w->nkc->ctx, NK_DYNAMIC, w->nkc->win_height / 8, 8);
	nk_layout_row_push(w->nkc->ctx, 0.04f);
	nk_spacing(w->nkc->ctx, 1);
	nk_layout_row_push(w->nkc->ctx, 0.20f);
	set_style(w->nkc->ctx, THEME_GREEN);
	if (board[0] == 1)
		illuminate(w);
	if (nk_button_label(w->nkc->ctx, "NEW") || board[0] == 2)
	{
		mov_b(w, board, 1, 0);
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
	if (board[0] == 1)
		back(w);
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
		if (board[i] == 1)
			illuminate(w);
		if (nk_button_label(w->nkc->ctx, node->acc) || board[i] == 2)
		{
			mov_b(w, board, 1, 0);
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
			strcpy(w->backup->pass, w->one->pass);
			w->stage = ONE;
		}
		if (board[i] == 1)
			back(w);
		prev = node;
		node = node->next;
		i++;
	}
	set_style(w->nkc->ctx, THEME_RED);
}

void	aff_login(t_world *w)
{
	int			old_len = w->log->lens[1];
	int			i = 0;
	static int	boo = 0;
	struct nk_rect s = {w->nkc->win_width / 3, w->nkc->win_height / 70, w->nkc->win_width / 3, w->nkc->win_height / 6};
	t_keybinds			key = {&(w->nkc->ctx->input.keyboard.keys[NK_KEY_ENTER].pity), &(w->nkc->ctx->input.keyboard.keys[NK_KEY_TAB].pity),
		&(w->nkc->ctx->input.keyboard.keys[NK_KEY_ESC].pity), &(w->nkc->ctx->input.keyboard.keys[NK_KEY_RIGHT].pity), &(w->nkc->ctx->input.keyboard.keys[NK_KEY_LEFT].pity),
			&(w->nkc->ctx->input.keyboard.keys[NK_KEY_UP].pity), &(w->nkc->ctx->input.keyboard.keys[NK_KEY_DOWN].pity)};

	if (w->popup->new_user == 2)
		for (i = 0; i < w->log->lens[2]; ++i) w->log->masqued[i] = '*';
	else
		for (i = 0; i < w->log->lens[1]; ++i) w->log->masqued[i] = '*';
	if (*key.tab == 1)
	{
		*key.tab = 0;
		boo = !boo ? 1 : 0;
	}
	if (w->popup->new_user == 2)
		old_len = w->log->lens[2];
	if (w->nkc->ctx->input.mouse.buttons[NK_BUTTON_LEFT].down)
		boo = -1;
	if (w->popup->new_user == 1)
	{
		w->popup->wrong_pass = 0;
		if (nk_popup_begin(w->nkc->ctx, NK_POPUP_DYNAMIC, "", NK_WINDOW_BORDER, s))
		{
			nk_layout_row_dynamic(w->nkc->ctx, 20, 1);
			nk_label(w->nkc->ctx, "Do you wish to create a new User?", NK_TEXT_CENTERED);
			nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 30, 3);
			set_style(w->nkc->ctx, THEME_GREEN);
			if (nk_button_label(w->nkc->ctx, "YES") || *key.enter == 1)
			{
				*key.enter = 0;
				w->popup->new_user = 2;
				w->log->lens[2] = 0;
				memset(w->log->second, 0, 256);
				nk_popup_close(w->nkc->ctx);
			}
			set_style(w->nkc->ctx, THEME_YELLOW);
			nk_spacing(w->nkc->ctx, 1);
			if (nk_button_label(w->nkc->ctx, "NO") || *key.esc == 1)
			{
				*key.esc = 0;
				bzero(w->log->login, w->log->lens[0]);
				bzero(w->log->check, w->log->lens[1]);
				w->log->lens[0] = 0;
				w->log->lens[1] = 0;
				boo = 0;
				w->popup->new_user = 0;
				w->popup->wrong_pass = 0;
				nk_popup_close(w->nkc->ctx);
			}
			nk_popup_end(w->nkc->ctx);
		} else w->popup->new_user = 0;
	}
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 35, 1);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 4, 5);
	nk_spacing(w->nkc->ctx, 2);
	set_style(w->nkc->ctx, THEME_MASQUED);
	nk_button_image(w->nkc->ctx, w->img->logo);
	set_style(w->nkc->ctx, THEME_RED);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 35, 1);
	nk_layout_row_begin(w->nkc->ctx, NK_DYNAMIC, w->nkc->win_height / 12, 3);
	nk_layout_row_push(w->nkc->ctx, 0.25f);
	nk_label(w->nkc->ctx, "login : ", NK_TEXT_RIGHT);
	nk_layout_row_push(w->nkc->ctx, 0.5f);
	if (!boo)
		nk_edit_focus(w->nkc->ctx, NK_EDIT_SIMPLE|NK_EDIT_GOTO_END_ON_ACTIVATE);
	else if (boo == 1)
		nk_edit_unfocus(w->nkc->ctx);
	nk_edit_string(w->nkc->ctx, NK_EDIT_SIMPLE|NK_EDIT_GOTO_END_ON_ACTIVATE, w->log->login, w->log->lens, 128, nk_filter_default);
	w->log->login[w->log->lens[0]] = '\0';
	nk_layout_row_end(w->nkc->ctx);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 10, 1);
	nk_layout_row_begin(w->nkc->ctx, NK_DYNAMIC, w->nkc->win_height / 12, 3);
	nk_layout_row_push(w->nkc->ctx, 0.25f);
	if (w->popup->wrong_pass)
		w->nkc->ctx->style.text.color = nk_rgba(190, 50, 70, 255);
	if (w->popup->new_user == 2)
	{
		if (!w->popup->wrong_pass)
		{
			w->nkc->ctx->style.text.color = nk_rgba(80, 165, 80, 255);
			nk_label(w->nkc->ctx, "enter it again : ", NK_TEXT_RIGHT);
		}
		else
			nk_label(w->nkc->ctx, "back at the beginning : ", NK_TEXT_RIGHT);
		w->nkc->ctx->style.text.color = nk_rgba(255, 255, 255, 255);
		nk_layout_row_push(w->nkc->ctx, 0.5f);
		if (boo == 1)
			nk_edit_focus(w->nkc->ctx, NK_EDIT_SIMPLE|NK_EDIT_GOTO_END_ON_ACTIVATE);
		else if (!boo)
			nk_edit_unfocus(w->nkc->ctx);
		nk_edit_string(w->nkc->ctx, NK_EDIT_SIMPLE|NK_EDIT_GOTO_END_ON_ACTIVATE, w->log->masqued, w->log->lens + 2, 32, nk_filter_default);
		if (old_len < w->log->lens[2])
			memcpy(w->log->second + old_len, w->log->masqued + old_len, (nk_size)(w->log->lens[2] - old_len));
		w->log->second[w->log->lens[2]] = '\0';
	}
	else
	{
		nk_label(w->nkc->ctx, "Password : ", NK_TEXT_RIGHT);
		w->nkc->ctx->style.text.color = nk_rgba(255, 255, 255, 255);
		nk_layout_row_push(w->nkc->ctx, 0.5f);
		if (boo == 1)
			nk_edit_focus(w->nkc->ctx, NK_EDIT_SIMPLE|NK_EDIT_GOTO_END_ON_ACTIVATE);
		else if (!boo)
			nk_edit_unfocus(w->nkc->ctx);
		nk_edit_string(w->nkc->ctx, NK_EDIT_SIMPLE|NK_EDIT_GOTO_END_ON_ACTIVATE, w->log->masqued, w->log->lens + 1, 32, nk_filter_default);
		if (old_len < w->log->lens[1])
			memcpy(w->log->check + old_len, w->log->masqued + old_len, (nk_size)(w->log->lens[1] - old_len));
		w->log->check[w->log->lens[1]] = '\0';
	}
	nk_layout_row_end(w->nkc->ctx);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 7, 1);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 10, 5);
	nk_spacing(w->nkc->ctx, 2);
	set_style(w->nkc->ctx, THEME_GREEN);
	boo = w->nkc->ctx->current->edit.name;
	if (nk_button_label(w->nkc->ctx, "DONE") || *key.enter == 1)
	{
		*key.enter = 0;
		if (boo == 1 && (w->popup->new_user != 2 || !strcmp(w->log->check, w->log->second)))
			set_up_db(w);
		else if (boo && !w->popup->wrong_pass)
		{
			w->popup->wrong_pass = 1;
			bzero(w->log->check, w->log->lens[1]);
			bzero(w->log->second, w->log->lens[2]);
			w->log->lens[1] = 0;
			w->log->lens[2] = 0;
		}
		else if (!boo)
			boo = 1;
		else
		{
			strcpy(w->log->check, w->log->second);
			bzero(w->log->second, w->log->lens[2]);
			w->log->lens[2] = 0;
			w->popup->wrong_pass = 0;
		}
	}
	if (w->popup->new_user != 2 && *key.esc == 1)
		nkc_stop_main_loop(w->nkc);
}

void	aff_error(t_world *w)
{
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 6, 1);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 14, 1);
	nk_label(w->nkc->ctx, "oooops... an error occured. Don't panic! click to restart", NK_TEXT_CENTERED);
	nk_layout_row_dynamic(w->nkc->ctx, w->nkc->win_height / 2, 3);
	nk_spacing(w->nkc->ctx, 1);
	if (nk_button_image(w->nkc->ctx, w->img->pinguin))
		execve(((char**)w->av)[0], w->av, w->env);
}
