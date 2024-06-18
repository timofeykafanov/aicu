/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_handle_p_x_percent.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:28:23 by tkafanov          #+#    #+#             */
/*   Updated: 2024/06/16 18:17:06 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	put_hex(unsigned long num, char chr, int *count)
{
	if (num >= 16)
	{
		put_hex(num / 16, chr, count);
		put_hex(num % 16, chr, count);
	}
	else
	{
		if (num <= 9)
			printf_ft_putchar(num + '0', count);
		else
		{
			if (chr == 'X')
				printf_ft_putchar(num - 10 + 'A', count);
			else
				printf_ft_putchar(num - 10 + 'a', count);
		}
	}
}

void	handle_p(va_list type, int *count, char chr)
{
	unsigned long	num;

	num = va_arg(type, unsigned long);
	if (!num)
		printf_ft_putstr("(nil)", count);
	else
	{
		printf_ft_putstr("0x", count);
		put_hex(num, chr, count);
	}
}

void	handle_x(va_list type, int *count, char chr)
{
	int long	num;

	num = va_arg(type, unsigned int);
	put_hex(num, chr, count);
}

void	handle_percent(int *count)
{
	printf_ft_putchar('%', count);
}
