/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 22:00:19 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/19 15:07:26 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

/* Our libs */
# include "minishell.h"

// TODO: review std libs that should be general =>> minishell.h
/* Standard libs */
// TODO: Review why I (might) need this & if PATH_MAX must be replaced (Norme?):
# include <linux/limits.h>

/* Structs */
// TODO: create separate header?
// TODO: use here or in minishell.h?
// typedef struct s_shelly
// {
// 	char	**envp;
// 	int		*open_fd;
// 	void	*malloc_ptrs;
// }	t_shelly;

typedef struct s_syntax_err
{
	char	*err_str;
	int		i;
}	t_syntax_err;

// TODO: use here or in execution.h?
typedef enum e_token_type
{
	WORD,
	REDIR,
	CTRL_OP
	// ASSIGN, // VAR=value before command
	// TK_EOF, TODO: include?
}	t_token_type;

/*
PIPE // |
OR, // ||, bonus
AND, // &&, bonus
LFT_PAREN, // (, bonus
RGT_PAREN, // ), bonus
*/
typedef enum e_ctrlop_type
{
	PIPE
}	t_ctrlop_type;

// TODO: keep here or in execution.h ?
/*
IN, // < redir input to a cmd, taking input from a file
OUT, // > redir output to a file, and overwrites the file if it already exists
APPEND, // >> redir output top a file, append the output to the end of the file
HEREDOC, // <<
*/
// typedef enum e_redir_type
// {
// 	NONE,
// 	IN,
// 	OUT,
// 	APPEND,
// 	HEREDOC
// }	t_redir_type;

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
	int				index;
	char			*value; // For WORD, filename, delimiter, etc.
	int				quoted; // 1 if token had quotes TODO: optional but useful?
	struct s_token	*previous; // TODO: review if necessary?
	struct s_token	*next;
}	t_token;

// TODO: use here or in execution.h ? Milena check struct_notes.md pls
// one redirection node. This represents one redirection attached to a command (like < infile, >> out, << EOF).
// typedef struct s_redir
// {
// 	t_redir_type	type; // IN (<), OUT (>), APPEND (>>), HEREDOC (<<)
// 	char			*filename; // Target file for <, >, >>. Usually NULL for heredoc.
// 	int				io; // fd being redirected (destination/source side), e.g.: default: 0 for input redirs, 1 for output redirs
// 	int				fd; // Runtime opened FD (result of open() or heredoc pipe read-end). Best practice: initialize to -1, set when opened, close during cleanup after dup2() / on error paths
// 	char			*delimiter; // Used only for heredoc (<<), e.g. EOF in cat << EOF. Usually NULL for non-heredoc.
// 	struct s_redir	*next; // Linked list of redirections in lexical order.
// }	t_redir;

// TODO: use here or in execution.h ? Milena check struct_notes.md pls
// one command in a pipeline. This represents a single simple command, with args + its redirections.

typedef struct s_cmd
{
	char			**argv; // Command + arguments array for execution: argv[0] command name (ls, echo, etc.); argv[n] args; NULL-terminated for execve()
	t_redirects		*redirs; // Head of that command’s redirection list (t_redir nodes).
	int				in_fd; // Final input FD chosen for this command at execution time. Typical defaults: STDIN_FILENO unless changed by pipe/redirection
	int				out_fd; // Final output FD chosen for this command at execution time. Typical defaults: STDOUT_FILENO unless changed by pipe/redirection
	struct s_cmd	*next; // Next command in pipeline (|). So a pipeline is a linked list of t_cmd.
}	t_cmd;

// TODO: use here or in execution.h ?
// typedef struct s_pipe
// {
// 	char			**cmds;
// 	t_redir			redir;
// 	struct s_pipe	*next;
// }	t_pipe;


// TODO: review functions that should be general =>> minishell.h & =/= files
/* Wrapper functions - maybe Protected std functions? */
char			*getcwd_protec(char *buf, size_t size, t_shelly *shelly);
void			*malloc_protec(size_t size, t_shelly *shelly);
void			*ft_calloc_protec(size_t nmemb, size_t size);

/* Parsing functions */
t_shelly		*init(char **envp);
char			*put_prompt(t_shelly *shelly, char *prompt);
char			*put_extra_prompt(t_shelly *shelly, char *prev_input);
t_shelly		*set_shellyenvp(char **envp);
char			*find_var_shellyenvp(t_shelly *shelly, char *envp_var_key);
// void	read_eval_print_loop(char **envp); // currently static

/* Main tokenizing functions */
t_token			*tokenize_input(char *input_str);
// char			*validate_complete_input(char *input_str, t_shelly *shelly); // currently static
// char			quote_check(char *input_str); // currently static
char			*syntax_check(char *input_str);

/* Tokenizing utils */
t_token			*new_tkn(t_token_type type, char *value);
void			append_tkn(t_token **head, t_token *new_node);
int				scan_word_end(char *s, int i);
int				op_len(char *s);

bool			char_is_op(char c);
t_syntax_err	syntax_err_redir(char *input_str, int i);
t_syntax_err	syntax_err_pipe(char *input_str, int i);

//delete later
t_cmd_line	*parse_tokens(t_token *tokens);

#endif