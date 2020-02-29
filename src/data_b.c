/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_b.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 18:17:17 by mbrunel           #+#    #+#             */
/*   Updated: 2020/02/29 06:44:48 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <psm.h>

t_lst *create_lst(void)
{
	t_lst *new;

	if (!(new = malloc(sizeof(t_lst))))
		return (NULL);
	new->line = NULL;
	new->next = NULL;
	return (new);
}

uint8_t *gen_iv()
{
	uint8_t *rt;
	int i;

	if (!(rt = malloc(sizeof(uint8_t) * 8)))
		return (NULL);
	srandom(time(NULL));
	for (i = 0; i < 16; i++)
		rt[i] = random();
	return (rt);
}

void *set_up_db(void *arg)
{
	void *tmp;
	char *name;
	char *line;
	t_world *w = (t_world*)arg;

	AES_init_ctx(&w->ctx_aes, w->log->pass);
	name = ft_strjoin(getenv("HOME"), "/.safe/");
	if (call(w->db.env, "/bin/mkdir", "-p", "-m", "00700", name, NULL))
		return (NULL);
	tmp = name; name = ft_strjoin(name, w->log->login);
	free(tmp);
	if (call(w->db.env, "/bin/mkdir", "-p", "-m", "00700", name, NULL))
		return (NULL);
	tmp = name;
	name = ft_strjoin(name, "/passwd");
	free(tmp);
	int fd = open(name, O_RDWR|O_CREAT|O_APPEND, 00700);
	if (ft_get_next_line(fd, &line))
	{
		w->db.iv = line;
		AES_ECB_decrypt(&w->ctx_aes, (uint8_t*)w->db.iv);
	}
	else
	{
		free(line);
		line = ft_strdup(w->db.iv = gen_iv());
		AES_ECB_encrypt(&w->ctx_aes, line);
		ft_dprintf(fd, "%s\n", (char*)line);
	}
	w->stage++;
	return (arg);
}
