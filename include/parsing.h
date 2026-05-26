/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 22:00:19 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/26 17:07:14 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

/* Our libs */
# include "minishell.h"

/* Standard libs */
# include <linux/limits.h>

/* Structs */
typedef struct s_exp_state
{
	char	**xpndd_word;
	bool	in_single;
	bool	in_double;
}	t_exp_state;

typedef struct s_cmd
{
	char			**argv; // Cmd + arg arr for exec.
	t_redirects		*redirs; // Head of that command’s redirection list.
	int				in_fd; // in FD chosen for this command at exec time.
	int				out_fd; // outp FD chosen for this command at exec time.
	struct s_cmd	*next; // Next cmd in pl (|). 
}	t_cmd;

/* Prompt & envp functions */
t_shelly		*init(char **envp);
char			*put_prompt(t_shelly *shelly, char *prompt);
char			*put_extra_prompt(t_shelly *shelly, char *prev_input);
t_shelly		*set_shellyenvp(char **envp);
char			*find_var_shellyenvp(t_shelly *shelly, char *envp_var_key);
// void	read_eval_print_loop(char **envp); // currently static

/* Main tokenizing functions */
t_token			*tokenize_input(char *input_str);
char			*syntax_check(char *input_str);

/* Tokenizing utils */
t_token			*new_tkn(t_token_type type, char *value);
void			append_tkn(t_token **head, t_token *new_node);
int				scan_word_end(char *input_str, int i);
int				op_len(char *input_str);
bool			char_is_op(char c);
t_syntax_err	syntax_err_redir(char *input_str, int i);
t_syntax_err	syntax_err_pipe(char *input_str, int i);
void			free_tkn(t_token *tkn);

/* Expansion */
t_token			*expand_params(t_token *tokens, t_shelly *shelly);
int				cpy_norm_str(char *tkn_val, int i, char **xpndd_word);
int				xpnd_var(char *tkn_val, int i, char **xpndd_word,
					t_shelly *shelly);
void			handle_in_double(char *tkn_val, int *i, t_exp_state *st,
					t_shelly *shelly);
void			handle_in_single(char *tkn_val, int *i, t_exp_state *st);
void			handle_unquoted(char *tkn_val, int *i, t_exp_state *st,
					t_shelly *shelly);
char			*word_param_expansion(char *tkn_val, t_shelly *shelly);
t_token			*xpnd_word_tkn(t_token *t, t_token **head, t_shelly *shelly);
t_token			*rm_empty_xpnsn(t_token *tkn);

/* Parser */
t_cmd_line		*parser(t_token *tokens);

/* Utils */
void			readevalprint_input(char *input_str, t_shelly *shelly);
void			noninteractive_readevalprint_input(char *l, t_shelly *shelly);

#endif
