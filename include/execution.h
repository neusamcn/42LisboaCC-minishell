/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 15:46:35 by megi              #+#    #+#             */
/*   Updated: 2026/04/16 18:47:10 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
# include <fcntl.h>
# include <sys/types.h>
# include <signal.h>
# include <stdbool.h>

# define p(...) printf(__VA_ARGS__)
# define CD "cd"
# define ECHO "echo"
# define EXIT "exit"
# define PWD "pwd"
# define ENV "env"
# define EXPORT "export"
# define UNSET "unset"
# define TRUE 0
# define FALSE 1
# define HD "minishell: warning: here-document delimited by end-of-file (wanted '"

typedef enum e_stdio
{
	READ,
	WRITE
}   t_stdio;

typedef enum e_fd
{
	FD_CLOSED = -1,
	FD_OPEN
}	t_fd;

typedef enum e_types_of_redirections
{
	NONE, // 0
	IN,  // < redir input to a cmd, taking input from a file
	OUT, // > redir output to a file, and overwrites the file if it already exists
	APPEND, // >> redir output top a file, append the output to the end of the file
	HEREDOC, // <<
}	t_redir_type;

typedef enum e_builts
{
	BUILTINS,
	EXTRENAL
}	t_builtins_check;

typedef enum e_mode
{
	INTERACTIVE, // prompt
	BLT_EXECUTING, // no fork, no rl
	CHILD, // fork + execve
	MNDWAIT
}	e_mode_for_sig;

typedef struct s_redirections
{
	t_redir_type			type;
	char					*filename;
    char    				*delimiter;
	int						fd[2];
	struct s_redirections	*next;
}   t_redirects;


typedef struct s_cmd_line
{
    char            	**cmds;
	t_builtins_check	isbuiltin;
    t_redirects     	redir;
	int					pipefd[2];
	int					prevfd;
	int 				is_there_more_pipes;
    struct s_cmd_line   *next;
}   t_cmd_line;

/* typedef struct s_execution
{
	t_builtin_cmd		builtins[12];
	char				*cmd_name;
	int					*pid;
}	t_execution; */

// [ls] → [grep src] → [wc -l] → NULL 

// PATH.C //
char 	*relative_path(t_cmd_line *cmd_line, char **envp);
char 	*paths_helper(t_cmd_line *cmd_line, char *path_var);
char 	*absolute_path(t_cmd_line *cmd_line);
int 	free_path(char **paths);

// EXECUTION.C // 
/* int		exec(t_cmd_line *cmd_line, char **envp);
void	child_exec(char *cmd, t_cmd_line *cmd_line, char **envp);
void	parent_exec(int status, pid_t pid); */
int		mommy_n_father(t_cmd_line *s, char **envp);
void 	exec_loop(t_cmd_line *cmds, char **envp);
int 	lonely_blt(t_cmd_line *s, char **envp);
bool 	if_redir(t_cmd_line *s);
void 	do_redri(t_cmd_line *s);
char    *abs_or_rel_p(t_cmd_line *c, char **envp);
int		status_check(int status);
int		are_you_builtin(t_cmd_line *cmd_line);
void child_ex(char *cmd, t_cmd_line *kid, char **envp);

// SIGNALS.C //
void	sigint_glob(int sig);
int		get_signal_stat(void);
void	set_signal_stat(int value);
void	set_signals_interactive_parent(void);
void 	sig_mode(int md);
void	sigint_prompt_handler(int signal);

// PIPES.C //
void 	heredoc(t_redirects *redir);
void 	append(t_redirects *redir);
void	close_fds(void);
void 	pipe_handler(t_redirects *redir);

# endif
	
	