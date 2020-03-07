/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/01 02:08:00 by mbrunel           #+#    #+#             */
/*   Updated: 2020/03/06 20:00:14 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

#include <stdint.h>

enum pass 				{NUM, ALNUM, GRAPH, ASCII};

enum theme				{THEME_BLACK, THEME_WHITE, THEME_RED, THEME_BLUE, THEME_DARK, THEME_GREEN, THEME_YELLOW};

typedef struct			s_lst
{
	char				*acc;
	char				*name;
	char				*email;
	char				*pass;
	struct s_lst		*next;
}						t_lst;

int						call(char *env[], char *exec , ...);
void					save_std(int *in, int *out);
void					set_std(int in, int out);
t_lst					*create_lst(void);
void					destroy_lst(t_lst *lst);
uint8_t 				*gen_iv(uint8_t *rt);
char					*gen_pass(int mode, int len);
void					copy_osx(char *buf, char *env[]);
char					*itoa(int n);
char					*strjoin(char const *s1, char const *s2);
void					sha256_string(unsigned char *string, unsigned char outputBuffer[65]);
char					*new_pass(char *str);

#endif
