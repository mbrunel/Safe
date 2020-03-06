/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 18:22:17 by mbrunel           #+#    #+#             */
/*   Updated: 2020/03/06 06:51:06 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "utils.h"

# include <stdarg.h>
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <ctype.h>
# include <time.h>
# include <strings.h>
# include <openssl/ssl.h>

void	set_std(int in, int out)
{
		dup2(in, STDIN_FILENO);
		close(in);
		dup2(out, STDOUT_FILENO);
		close(out);
}

void	save_std(int *in, int *out)
{
		*in = dup(STDIN_FILENO);
		*out = dup(STDOUT_FILENO);
}

int		call(char *env[], char *exec , ...)
{
	char *av[100];
	va_list ap;
	int i = 0;
	int status;

	va_start(ap, exec);
	av[0] = exec;
	while ((av[++i] = va_arg(ap, char*)))
		;
	av[i] = NULL;
	if (!fork())
		execve(exec, av, env);
	wait(&status);
	va_end(ap);
	return (status);
}

t_lst *create_lst(void)
{
	t_lst *new;
	int boo = 0;

	if (!(new = malloc(sizeof(t_lst))))
		return (NULL);
	*new = (t_lst){NULL, NULL, NULL, NULL, NULL};
	if (!(new->acc = calloc(256, sizeof(char))))
		boo = -1;
	if (!(new->name =  calloc(256, sizeof(char))))
		boo = -1;
	if (!(new->email =  calloc(256, sizeof(char))))
		boo = -1;
	if (!(new->pass =  calloc(256, sizeof(char))))
		boo = -1;
	if (!boo)
		return (new);
	destroy_lst(new);
	return (NULL);
}

void	destroy_lst(t_lst *lst)
{
	if (!lst)
		return ;
	if (lst->acc)
		free(lst->acc);
	if (lst->name)
		free(lst->name);
	if (lst->email)
		free(lst->email);
	if (lst->pass)
		free(lst->pass);
	free(lst);
}

uint8_t  *gen_iv(uint8_t  *rt)
{
	int i;

	srandom(time(NULL));
	for (i = 0; i < 16; i++)
		rt[i] = random();
	return (rt);
}

char	*itoa(int n)
{
	char			*s;
	size_t			i;
	unsigned int	t;

	t = n < 0 ? -n : n;
	i = 1;
	while (t /= 10)
		i++;
	t = n < 0 ? -n : n;
	n = (n < 0);
	if ((s = (char*)malloc((i + n + 1) * sizeof(char))))
	{
		s[i + n] = '\0';
		while (i--)
		{
			s[i + n] = t % 10 + '0';
			t /= 10;
		}
		if (n)
			s[0] = '-';
	}
	return (s);
}

char	*strjoin(char const *s1, char const *s2)
{
	char	*d;
	size_t	n1;
	size_t	n2;

	n1 = strlen(s1);
	n2 = strlen(s2) + 1;
	if ((d = (char*)malloc((n1 + n2) * sizeof(char))))
	{
		memcpy(d, s1, n1);
		memcpy(d + n1, s2, n2);
	}
	return (d);
}

char *gen_pass(int mode, int len)
{
	char *str;
	int i = -1;
	char c;

	if (!(str = malloc(sizeof(uint8_t) * len)))
		return (NULL);
	srandom(time(NULL));
	while (++i < len)
	{
		if (mode == NUM)
			while (!isdigit(c = random()));
		else if (mode == ALNUM)
			while (!isalnum(c = random()));
		else if (mode == GRAPH)
			while (!isgraph(c = random()));
		else
			c = random() % 0x7f;
		str[i] = c;
	}
	str[i] = '\0';
	return (str);
}

void copy_osx(char *buf, char *env[])
{
	int in;
	int out;
	int pip[2];

	save_std(&in, &out);
	pipe(pip);
	set_std(pip[0], pip[1]);
	write(1, buf, strlen(buf));
	close(1);
	call(env, "/usr/bin/pbcopy", NULL);
	set_std(in, out);
}

void sha256_string(unsigned char *string, unsigned char hash[32])
{
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, string, strlen((char*)string));
	SHA256_Final(hash, &sha256);
}

char *new_pass(char *str)
{
	int mode = 0;
	int i = -1;

	while (str[++i])
	{
		if (!mode && !isalpha(str[i]))
			mode = 1;
		if (mode != 2 && isspecial(str[i]))
			mode = 2;
	}
	free(str);
	return (gen_pass(mode, i));
}
