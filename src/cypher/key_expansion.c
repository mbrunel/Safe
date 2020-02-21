/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 23:15:55 by mbrunel           #+#    #+#             */
/*   Updated: 2020/02/21 00:25:16 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cyph.h>

void rotate(unsigned char *buf)
{
	unsigned char c;
	int i;

	c = buf[0];
	for (i = 0; i < 3; i++)
		buf[i] = buf[i+1];
	buf[3] = c;
}

void generate(unsigned char *buf, int rci)
{
	int i;

	rotate(buf);
	for (i = 0; i < 4; i++)
		buf[i] = sbox[buf[i]];
	buf[0] = buf[0] ^ Rcon[rci];
}

void expand_key(unsigned char *expanded, unsigned char *key)
{
	int current;
	int rci = 1;
	int i = -1;
	unsigned char buf[4];

	ft_memset(buf, '\0', 4);
	while (++i < KEY_SIZE)
		expanded[i] = key[i];
	current = i;
	while (current < EXPANDED_SIZE)
	{
		for(i = 0; i < 4; i++)
			buf[i] = expanded[current - 4 + i];
		if (!(current % KEY_SIZE))
			generate(buf, rci++);
		for (i = 0; i < 4; i++) {
			expanded[current] = expanded[current - KEY_SIZE] ^ buf[i];
			current++;
		}
	}
}
