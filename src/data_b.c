/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_b.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 18:17:17 by mbrunel           #+#    #+#             */
/*   Updated: 2020/02/24 22:21:49 by mbrunel          ###   ########.fr       */
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

int8_t gen_iv(uint8_t *iv)
{
	uint8_t rt;

	srandom(time(NULL));
	rt = random();
	if (iv)
		*iv = (uint8_t)rt;
	return (rt);
}

void *set_up_db(void *arg)
{
	void *tmp;
	char *name;
	char *line;
	t_world *w = (t_world*)arg;

	name = ft_strjoin(getenv("HOME"), "/.safe/");
	if (call(w->db.env, "/bin/mkdir", "-p", "-m", "00700", name, NULL))
		return (NULL);
	tmp = name;
	name = ft_strjoin(name, w->log->login);
	free(tmp);
	if (call(w->db.env, "/bin/mkdir", "-p", "-m", "00700", name, NULL))
		return (NULL);
	tmp = name;
	name = ft_strjoin(name, "/passwd");
	free(tmp);
	int fd = open(name, O_RDWR|O_CREAT|O_APPEND, 00700);
	if (ft_get_next_line(fd, &line))
		w->db.iv = (uint8_t)atoi(line);
	else
		ft_dprintf(fd, "%d\n", gen_iv(&(w->db.iv)));
	free(line);



	uint8_t iv[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
	uint8_t pass[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
   uint8_t msg[]  = { 'm', 'a', 'n', 'g', 0x81, 0x19, 0xb2, 0x46, 0xce, 0xe9, 0x8e, 0x9b, 0x12, 0xe9, 0x19, 0x7d,'m', 'a', 'n', 'g', 0x81, 0x19, 0xb2, 0x46, 0xce, 0xe9, 0x8e, 0x9b, 0x12, 0xe9, 0x19, 0x7d,
                      0x50, 0x86, 0xcb, 0x9b, 0x50, 0x72, 0x19, 0xee, 0x95, 0xdb, 0x11, 0x3a, 0x91, 0x76, 0x78, 0xb2,
                      0x73, 0xbe, 0xd6, 0xb8, 0xe3, 0xc1, 0x74, 0x3b, 0x71, 0x16, 0xe6, 0x9e, 0x22, 0x22, 0x95, 0x16,
                      0x3f, 0xf1, 0xca, 0xa1, 0x68, 0x1f, 0xac, 0x09, 0x12, 0x0e, 0xca, 0x30, 0x75, 0x86, 'p', 'd' };

	struct AES_ctx ctx;
	AES_init_ctx_iv(&ctx, pass, iv);
	printf("||%s||\n", msg);
	AES_CBC_encrypt_buffer(&ctx, msg, (uint32_t)64 + 16);
//	printf("||%s||\n", msg);
	AES_CBC_decrypt_buffer(&ctx, msg, (uint32_t)64 + 16);
	printf("||%s||\n", msg);


/*
	w->l = create_lst();
	while (ft_get_next_line(fd, &line))
	{
		w->l->len = strlen((char*)msg);//atoi(line);
	//	padd(line);
		printf("%s\n", msg);
		AES_CBC_encrypt_buffer(&ctx, msg, 16);
		printf("%s\n", msg);
		AES_CBC_decrypt_buffer(&ctx, msg, 16);
		printf("%s\n", msg);
		free(line);
	}
	close(fd);*/
	w->stage++;
	return (arg);
}
