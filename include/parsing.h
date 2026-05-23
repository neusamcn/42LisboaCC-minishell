/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 22:00:19 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/22 23:25:08 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

/* Our libs */
# include "minishell.h"

/* Standard libs */
// TODO: Review why I (might) need this & if PATH_MAX must be replaced (Norme?):
//# include <linux/limits.h>

/* Structs */
typedef struct s_syntax_err
{
	char	*err_str;
	int		i;
}	t_syntax_err;

typedef enum e_token_type
{
	WORD,
	REDIR,
	CTRL_OP
}	t_token_type;

typedef enum e_ctrlop_type
{
	PIPE
}	t_ctrlop_type;

typedef enum e_word_type
{
	CMD,
	QMARK1,
	QMARK2
}	t_word_type;

typedef struct s_token
{
	t_token_type	type;
	t_ctrlop_type	ctrlop; // Valid only if type == CTRL_OP
	t_redir_type	redir; // Valid only if type == REDIR
	t_word_type		word; // valid only if type == WORD
	bool			space_b4_word; // true if at least 1 space b4 word
	int				word_xpndd; // 1 if expansion is done, -1 if failed/invalid
	int				index;
	char			*value; // For WORD, filename, delimiter, etc.
	struct s_token	*previous; // TODO: review if necessary?
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	char			**argv; // Cmd + arg arr for exec.
	t_redirects		*redirs; // Head of that command’s redirection list.
	int				in_fd; // in FD chosen for this command at exec time.
	int				out_fd; // outp FD chosen for this command at exec time.
	struct s_cmd	*next; // Next cmd in pl (|). 
}	t_cmd;

// TODO: review functions that should be general =>> minishell.h & =/= files
/* Wrapper functions - maybe Protected std functions? */
char			*getcwd_protec(char *buf, size_t size, t_shelly *shelly);
void			*malloc_protec(size_t size, t_shelly *shelly);
void			*ft_calloc_protec(size_t nmemb, size_t size);

/* Prompt & envp functions */
t_shelly		*init(char **envp);
char			*put_prompt(t_shelly *shelly, char *prompt);
char			*put_extra_prompt(t_shelly *shelly, char *prev_input);
t_shelly		*set_shellyenvp(char **envp);
char			*find_var_shellyenvp(t_shelly *shelly, char *envp_var_key);
// void	read_eval_print_loop(char **envp); // currently static

/* Main tokenizing functions */
t_token			*tokenize_input(char *input_str);
// char			*validate_complete_input(char *input_str, t_shelly *shelly);
// char			quote_check(char *input_str); // currently static
char			*syntax_check(char *input_str);

/* Tokenizing utils */
t_token			*new_tkn(t_token_type type, char *value);
void			append_tkn(t_token **head, t_token *new_node);
int				scan_word_end(char *input_str, int i);
int				op_len(char *input_str);
bool			char_is_op(char c);
t_syntax_err	syntax_err_redir(char *input_str, int i);
t_syntax_err	syntax_err_pipe(char *input_str, int i);
//void 			free_tokens(t_token *tok);

/* Expansion */
void			expand_params(t_token *tokens, t_shelly *shelly);

/* Parser */
t_cmd_line		*parser(t_token *tokens);

#endif
