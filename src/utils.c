/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 18:22:17 by mbrunel           #+#    #+#             */
/*   Updated: 2020/03/13 05:42:59 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "utils.h"

# include <stdarg.h>
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <ctype.h>
# include <time.h>
# include <string.h>
# include <dirent.h>
# include <errno.h>
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

void	destroy_lst(t_lst *lst)
{
	if (!lst)
		return ;
	if (lst->acc)
	{
		bzero(lst->acc, strlen(lst->acc));
		free(lst->acc);
	}
	if (lst->name)
	{
		bzero(lst->name, strlen(lst->name));
		free(lst->name);
	}
	if (lst->email)
	{
		bzero(lst->email, strlen(lst->email));
		free(lst->email);
	}
	if (lst->pass)
	{
		bzero(lst->pass, strlen(lst->pass));
		free(lst->pass);
	}
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

int msleep(long msec)
{
	struct timespec ts;
	int res;

	if (msec < 0)
	{
		errno = EINVAL;
		return -1;
	}

	ts.tv_sec = msec / 1000;
	ts.tv_nsec = (msec % 1000) * 1000000;

	do {
		res = nanosleep(&ts, &ts);
	} while (res && errno == EINTR);

	return res;
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
	char *upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char *lower = "abcdefghijklmnopqrstuvwxyz";
	char *digit = "0123456789";
	char *spec =  "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
	int nb_digits = 0;
	int nb_lower = 0;
	int nb_upper = 0;
	int nb_spec = 0;
	int type;
	char *new;
	int i = 0;

	srandom(time(NULL));
	if (!(new = malloc(sizeof(uint8_t) * (len + 1))))
		return (NULL);
	if (mode == NUM)
		nb_digits = len;
	else if (mode == ALNUM)
	{
		nb_lower = len / 3;
		nb_upper = nb_lower;
		nb_digits = len - nb_upper - nb_lower;
	}
	else if (mode == GRAPH)
	{
		nb_spec = len / 4;
		nb_lower = nb_spec;
		nb_upper = nb_spec;
		nb_digits = len - (3 * nb_spec);
	}
	else
	{
		while (++i < len)
			new[i] = random() % 0x7f;
		return (new);
	}
	while (i < len)
	{
		type = random() % 4;
		if (type == 0 && nb_digits-- > 0)
			new[i++] = digit[random() % 10];
		if (type == 1 && nb_lower-- > 0)
			new[i++] = lower[random() % 26];
		if (type == 2 && nb_upper-- > 0)
			new[i++] = upper[random() % 26];
		if (type == 3 && nb_spec-- > 0)
			new[i++] = spec[random() % 32];
	}
	new[i] = '\0';
	return (new);
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
		if (!mode && !isdigit(str[i]))
			mode = 1;
		if (mode != 2 && !isalnum(str[i]))
			mode = 2;
	}
	free(str);
	return (gen_pass(mode, i));
}

int check_new_user(char *usr_path, char *dir_path)
{
	DIR *dir;
	struct dirent *node;
	int len_dir = strlen(dir_path);

	dir = opendir(dir_path);
	while ((node = readdir(dir)))
	{
		if (!strcmp(node->d_name, usr_path + len_dir))
		{
			closedir(dir);
			return (0);
		}
	}
	return (1);
}

char	*search_env(char *env_tmp, char *env[])
{
	int	i;

	i = 0;
	while (env[++i])
		if (!strncmp(env[i], env_tmp, strlen(env_tmp)))
			return (env[i] + strlen(env_tmp) + 1);
	return (NULL);
}
