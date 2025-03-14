/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:12:39 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/14 10:03:14 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// apply Shunting Yard Algorithm on the tokens: words, operators, parenthesis
// resulting in and ouput queue(Reverse Polish Notation or postfix)
t_chain	*convert_infix(t_chain *infix, t_chain *post, t_chain *ops)
{
	while (infix)
	{
		if (infix->lvl == VIP)
			move_item(&infix, &post, 0);
		else if ((!ops || infix->lvl == NAN) && infix->type != R_PAREN)
			move_item(&infix, &ops, 1);
		else if (ops && ops->lvl && infix->type != R_PAREN)
		{
			while (ops && ops->lvl && ops->lvl >= infix->lvl)
				move_item(&ops, &post, 0);
			move_item(&infix, &ops, 1);
		}
		else if (infix->type == R_PAREN)
		{
			while (ops && ops->type != L_PAREN)
				move_item(&ops, &post, 0);
			move_item(&infix, &post, 0);
			delete_one(&ops, 1);
		}
		else if (ops->type == L_PAREN)
			move_item(&infix, &ops, 1);
	}
	while (ops)
		move_item(&ops, &post, 0);
	return (post);
}
