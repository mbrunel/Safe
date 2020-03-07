/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   back_end.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 18:17:17 by mbrunel           #+#    #+#             */
/*   Updated: 2020/03/07 07:02:14 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "psm.h"

#include "aes.h"

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int recup_size(int fd)
{
	char c[13];
	int i = -1;

	while (read(fd, c + ++i, 1))
		if (!isdigit(c[i]))
			break ;
	c[i] = '\0';
	return (atoi(c));
}

int assign(t_world *w, char *buf)
{
	int i = 0;
	t_lst *node;
	t_lst *prev = NULL;

	while (1)
	{
		if (!w->l)
		{
			if (!(w->l = create_lst()))
				return (-1);
			node = w->l;
		}
		else
		{
			if (!(node->next = create_lst()))
				return (-1);
			prev = node;
			node = node->next;
		}
		strcpy(node->acc, buf + i);
		i += strlen(node->acc) + 1;
		strcpy(node->name, buf + i);
		i += strlen(node->name) + 1;
		strcpy(node->email, buf + i);
		i += strlen(node->email) + 1;
		strcpy(node->pass, buf + i);
		i += strlen(node->pass) + 1;;
		if (!node->pass[0])
			break ;
	}
	destroy_lst(node);
	if (!prev)
		w->l = NULL;
	else
		prev->next = NULL;
	free(buf);
	return (0);
}

void *set_up_db(void *arg)
{
	void *tmp;
	char *name;
	t_world *w = (t_world*)arg;
	unsigned char old_h[32];
	unsigned char new_h[32];
	unsigned char *buf;
	int		size;
	int fd;
	uint8_t iv[16];
	unsigned char salt[33];
	unsigned char conc[33];
	int i = -1;

	if (!(name = strjoin(getenv("HOME"), "/.safe/")))
	{
		w->stage = ERROR;
		return (NULL);
	}
	if (call(w->env, "/bin/mkdir", "-p", "-m", "00700", name, NULL))
	{
		w->stage = ERROR;
		return (NULL);
	}
	tmp = name;
	name = strjoin(name, w->log->login);
	w->home = strdup(name);
	free(tmp);
	if (call(w->env, "/bin/mkdir", "-p", "-m", "00700", name, NULL))
	{
		w->stage = ERROR;
		return (NULL);
	}
	tmp = name;
	if (!(name = strjoin(name, "/passwd")))
	{
		w->stage = ERROR;
		return (NULL);
	}
	free(tmp);
	if ((fd = open(name, O_RDWR|O_CREAT|O_APPEND, 00700)) == -1)
	{
		w->stage = ERROR;
		return (NULL);
	}
	if (read(fd, salt, 32) != 32)
	{
		w->l = NULL;
		w->stage++;
		return (arg);
	}
	salt[32] = 0;
	while (w->log->check[++i] && salt[i])
		conc[i] = w->log->check[i] ^ salt[i];
	conc[i] = '\0';
	sha256_string((unsigned char*)conc, w->log->pass);
	AES_init_ctx(w->ctx_aes, (uint8_t*)w->log->pass);
	if (read(fd, old_h, 32) != 32)
	{
		w->stage = ERROR;
		return (NULL);
	}
	w->log->pass[32] = '\0';
	sha256_string(w->log->pass, new_h);
	for (i = 0; i < 32; i++)
		if (old_h[i] != new_h[i])
			exit(0);
	if (read(fd, iv, 16) != 16)
	{
		w->stage = ERROR;
		return (NULL);
	}
	AES_ECB_decrypt(w->ctx_aes, (uint8_t*)iv);
	AES_ctx_set_iv(w->ctx_aes, iv);
	size = recup_size(fd);
	if (!(buf = calloc(size, sizeof(char))))
	{
		w->stage = ERROR;
		return (NULL);
	}
	if (read(fd, buf, size) != size)
	{
		w->stage = ERROR;
		return (NULL);
	}
	AES_CBC_decrypt_buffer(w->ctx_aes, (uint8_t*)buf, size);
	buf[size] = '\0';
	if (assign(w, (char*)buf) == -1)
	{
		w->stage = ERROR;
		return (NULL);
	}
	w->stage = DASHBOARD;
	free(name);
	return (arg);
}

void *save_chng(t_world *w)
{
	char *name = NULL, *new = NULL, *salt = NULL, *s_len = NULL;
	int fd;
	unsigned char buf[4096] = {0};
	t_lst *node = w->l;
	t_lst *tmp;
	int i = 0;
	int padd;
	uint8_t iv[16];
	unsigned char new_h[32];
	unsigned char new_pass[33];
	unsigned char conc[33];
	int j = -1;

	if (!w->home)
		exit(0);
	if (!(name = strjoin(w->home, "/swap")) || !(new = strjoin(w->home, "/passwd")) || !(salt = gen_pass(GRAPH, 32)))
	{
		free(w->home);
		if (name)
			free(name);
		if (new)
			free(new);
		w->stage = ERROR;
		return (NULL);
	}
	free(w->home);
	if ((fd = open(name, O_WRONLY|O_CREAT|O_TRUNC, 00700)) == -1)
	{
		w->stage = ERROR;
		return (NULL);
	}
	write(fd, salt, 32);
	while (w->log->check[++j])
		conc[j] = w->log->check[j] ^ salt[j];
	conc[j] = '\0';
	sha256_string((unsigned char*)conc, new_pass);
	new_pass[32] = '\0';
	free(salt);
	AES_init_ctx(w->ctx_aes, (uint8_t*)new_pass);
	sha256_string(new_pass, new_h);
	write(fd, new_h, 32);
	gen_iv(iv);
	AES_ctx_set_iv(w->ctx_aes, iv);
	AES_ECB_encrypt(w->ctx_aes, iv);
	write(fd, (char*)iv, 16);
	while (node)
	{
		strcpy((char*)buf + i, node->acc);
		i += strlen(node->acc) + 1;
		free(node->acc);
		strcpy((char*)buf + i, node->name);
		i += strlen(node->name) + 1;
		free(node->name);
		strcpy((char*)buf + i, node->email);
		i += strlen(node->email) + 1;
		free(node->email);
		strcpy((char*)buf + i, node->pass);
		i += strlen(node->pass) + 1;
		free(node->pass);
		tmp = node;
		node = node->next;
		free(tmp);
	}
	padd = !(i % 16) ? 0 : 16 - i % 16;
	while (padd--)
		buf[i++] = '\0';
	s_len = itoa(i);
	write(fd, s_len, strlen(s_len));
	write(fd, "\0", 1);
	free(s_len);
	AES_CBC_encrypt_buffer(w->ctx_aes, (uint8_t*)buf, i);
	write(fd, buf, i);
	if (call(w->env, "/bin/mv", name, new, NULL))
	{
		w->stage = ERROR;
		return (NULL);
	}
	free(name);
	free(new);
	return (NULL);
}
