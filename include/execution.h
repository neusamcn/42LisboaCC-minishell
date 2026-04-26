/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 15:46:35 by megi              #+#    #+#             */
/*   Updated: 2026/04/26 16:45:24 by megiazar         ###   ########.fr       */
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
# include <sys/wait.h>


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
	int						xd_fd;
	struct s_redirections	*next;
}   t_redirects;

typedef struct s_cmd_line
{
    char            	**cmds;
	t_redirects     	redir;
	int					pipefd[2];
	int					prevfd;
	struct s_cmd_line   *next;
}   t_cmd_line;

typedef struct s_export
{
    char    *argv;
    int     flag;
    char    **envp;
	char 	**mini_env;
	char 	*new_var;
}   t_export;

// [ls] → [grep src] → [wc -l] → NULL 

// PATH.C //
char 	*relative_path(t_cmd_line *cmd_line, char **envp);
char 	*paths_helper(t_cmd_line *cmd_line, char *path_var);
char 	*absolute_path(t_cmd_line *cmd_line);

// EXECUTION.C // 
void 	exec_loop(t_cmd_line *cmds, char **envp);
int		mommy_n_father(t_cmd_line *s, char **envp);
int 	lonely_blt(t_cmd_line *s, char **envp);
int		are_you_builtin(t_cmd_line *cmd_line);
void 	child_ex(char *cmd, t_cmd_line *kid, char **envp);
int 	single_child_ex(t_cmd_line *kid, char **envp);
int 	mndwait(pid_t last_p, int cmd_nmb);
int		ex_pipeline_ec(t_cmd_line *pipeline, char **envp);

// BUILTINs //
int myecho(t_cmd_line *cmd, char **envp);
int r_bltn(t_cmd_line *cmd_line, char **envp);

// EXECUTION UTILS //
bool 	if_redir(t_cmd_line *s);
int		do_redri(t_redirects *s);
char    *abs_or_rel_p(t_cmd_line *c, char **envp);
char    *abs_or_rel_p(t_cmd_line *c, char **envp);

// REDIRECTIONs //
int 	which_redir_type(t_redirects *redir);
int 	in_redir(t_redirects *redir);
void 	heredoc(t_redirects *redir);
void 	append(t_redirects *redir);

// SIGNALS.C //
void	sigint_glob(int sig);
int		get_signal_stat(void);
void	set_signal_stat(int value);
void 	sig_mode(int md);
void	sigint_prompt_handler(int signal);
void	set_signals_interactive_parent(void);
void	set_sigaction(int signo, void (*handler)(int), int flags);
int		status_check(int status);

// ERRORs //
void	p_log_err(char *msg, char *cmd);
int		mndp_log_err(char *msg, char *cmd);

// FREEs //
void	close_fds(void);
int 	free_path(char **paths);
void 	pipe_cl(t_cmd_line *pipeline);
void	cleanup_xd_fds(t_cmd_line *start);

// BUILTINS //
int myecho(t_cmd_line *echo, char **envp);
int myenv(t_cmd_line *env, char **envp);


int r_bltn(t_cmd_line *cmd_line, char **envp);


# endif
	
	
