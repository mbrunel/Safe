/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_b.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 18:17:17 by mbrunel           #+#    #+#             */
/*   Updated: 2020/03/01 06:49:42 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <psm.h>

int recup_size(int fd)
{
	char c[13];
	int i = -1;

	while (read(fd, c + ++i, 1))
		if (!ft_isdigit(c[i]))
			break ;
	c[i] = '\0';
	return (ft_atoi(c));
}

void assign(t_world *w, char *buf, int size)
{
	int i = -1;
	t_lst *node;
	w->l = create_lst();
	node = w->l;
	printf("%d\n", size);
	while (i < size -1)
	{
		while (buf[++i])
			node->acc[i] = buf[i];
		node->acc[i] = '\0';
		while (buf[++i])
			node->name[i] = buf[i];
		node->name[i] = '\0';
		while (buf[++i])
			node->email[i] = buf[i];
		node->email[i] = '\0';
		while (buf[++i])
			node->pass[i] = buf[i];
		node->pass[i] = '\0';
		node->next = create_lst();
		printf("%s, %s, %s\n", node->acc, node->name, node->pass);
		node = node->next;
	}
}

void *set_up_db(void *arg)
{
	void *tmp;
	char *name;
	t_world *w = (t_world*)arg;
	uint8_t iv[16];
	char	*buf;
	int		size;

	ft_strcpy(w->log->pass, "012345789123456");
	AES_init_ctx(w->ctx_aes, (uint8_t*)w->log->pass);
	name = ft_strjoin(getenv("HOME"), "/.safe/");
	if (call(w->env, "/bin/mkdir", "-p", "-m", "00700", name, NULL))
		return (NULL);
	tmp = name;
	name = ft_strjoin(name, w->log->login);
	w->home = ft_strdup(name);
	free(tmp);
	if (call(w->env, "/bin/mkdir", "-p", "-m", "00700", name, NULL))
		return (NULL);
	tmp = name;
	name = ft_strjoin(name, "/passwd");
	free(tmp);
	int fd = open(name, O_RDWR|O_CREAT|O_APPEND, 00700);
	if (read(fd, iv, 16) != 16)
	{
		gen_iv(iv);
		AES_ECB_encrypt(w->ctx_aes, iv);
		write(fd, (char*)iv, 16);
		write(fd, "0\0", 2);
		w->l = NULL;
		w->stage++;
		return (arg);
	}
	AES_ECB_decrypt(w->ctx_aes, (uint8_t*)iv);
	AES_ctx_set_iv(w->ctx_aes, iv);
	size = recup_size(fd);
	buf = calloc(size, sizeof(char));
	read(fd, buf, size);
	AES_CBC_decrypt_buffer(w->ctx_aes, (uint8_t*)buf, size);
	assign(w, buf, size);
	w->stage++;
	return (arg);
}

void save_chng(t_world *w)
{
	char *name = ft_strjoin(w->home, "/swap");
	int fd = open(name, O_RDWR|O_CREAT|O_APPEND, 00700);
}
