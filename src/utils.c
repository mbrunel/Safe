/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 18:22:17 by mbrunel           #+#    #+#             */
/*   Updated: 2020/03/01 06:05:32 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <psm.h>

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

	if (!(new = malloc(sizeof(t_lst))))
		return (NULL);
	new->acc = calloc(256, sizeof(char));
	new->name =  calloc(256, sizeof(char));
	new->email =  calloc(256, sizeof(char));
	new->pass =  calloc(256, sizeof(char));
	new->next = NULL;
	return (new);
}

void	destroy_lst(t_lst *lst)
{
	free(lst->acc);
	free(lst->name);
	free(lst->email);
	free(lst->pass);
	free(lst);
}

uint8_t *gen_iv(uint8_t *rt)
{
	int i;

	if (!(rt = malloc(sizeof(uint8_t) * 8)))
		return (NULL);
	srandom(time(NULL));
	for (i = 0; i < 16; i++)
		rt[i] = random();
	return (rt);
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
		if (mode == ALPHA)
			while (!ft_isalpha(c = random()));
		else if (mode == ALNUM)
			while (!ft_isalnum(c = random()));
		else if (mode == GRAPH)
			while (!ft_isgraph(c = random()));
		str[i] = c;
	}
	str[i] = '\0';
	return (str);
}
