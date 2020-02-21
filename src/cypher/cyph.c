/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cyph.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 21:49:47 by mbrunel           #+#    #+#             */
/*   Updated: 2020/02/21 14:51:23 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cyph.h>

void aes(unsigned char *key, unsigned char *buf)
{

}

static int add_buf(unsigned char **file, unsigned char *buf)
{
	unsigned char *tmp;
	static int len = 16;

	tmp = *file;
	*file = malloc(sizeof(unsigned char) * len);
	ft_memcpy(*file, tmp, len - 16);
	free(tmp);
	ft_memcpy(*file + len - 16, buf, 16);
	len += 16;
	return (len);
}

int cyph(unsigned char *key, char *opath, char *newpath)
{

	unsigned char expanded[EXPANDED_SIZE];
	unsigned char buf[33];
	unsigned char *file = NULL;
	int len;
	int status;
	int in;
	int out;

	if ((in = open(opath, O_RDONLY)) == -1)
		return (-1);
	expand_key(expanded, key);
	while ((status = read(in, buf, 16)) == 16)
	{
		aes(expanded, buf);
		add_buf(&file, buf);
	}
	close(in);
	if (status == -1)
		return (-1);
	ft_memset(buf + status, '\0', 16);
	aes(expanded, buf);
	len = add_buf(&file, buf) - 32 + status;
	if ((out = open(newpath, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU)) == -1)
		return (-1);
	write(out, file, len);
	close(out);
	free(file);
	return (0);
}
