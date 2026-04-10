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

# define p(...) printf(__VA_ARGS__)
# define CD "cd"
# define ECHO "echo"
# define EXIT "exit"
# define PWD "pwd"
# define ENV "env"
# define EXPORT "export"
# define UNSET "unset"
# define DELIMETER "delimeter"
# define TRUE 0
# define FALSE 1
# define HD "minishell: warning: here-document delimited by end-of-file (wanted '"

typedef enum e_fds
{
	STDIN,
	STDOUT
}   t_fds;

typedef enum e_builts
{
	BUILTINS,
	NON_BUILTINS
}	t_builtins_check;

typedef enum e_types_of_redirections
{
	IN,  // < redir input to a cmd, taking input from a file
	OUT, // > redir output to a file, and overwrites the file if it already exists
	APPEND, // >> redir output top a file, append the output to the end of the file
	HEREDOC, // <<
	FD
}	t_redir_type;

typedef enum e_signal_types
{
	SIG_INT, // ctrl+c
	SIG_QUIT // ctrl \+
}	e_signal_types;

typedef struct s_redirections
{
	t_redir_type			type;
	char					*filename;
    char    				*delimiter;
	struct s_redirections	*next;
}   t_redirects;

typedef struct s_pipe
{
    char            **cmds;
    t_redirects     redir;
    struct s_pipe   *next;
}   t_pipe;

/* typedef struct s_execution
{
	t_builtin_cmd		builtins[12];
	char				*cmd_name;
	int					*pid;
}	t_execution; */

// [ls] → [grep src] → [wc -l] → NULL 

// PATH //
char 	*path(t_pipe *cmd_line, char **envp);
char 	*paths_helper(t_pipe *cmd_line, char *path_var);
char 	*absolute_path(t_pipe *cmd_line);
int 	free_path(char **paths);

// FORKs // 
int		exec(t_pipe *cmd_line, char **envp, int status);
void	child_exec(char *cmd, t_pipe *cmd_line, char **envp);
void	parent_exec(int status, pid_t pid);
int		status_check(int status);
int		are_you_builtin(t_pipe *cmd_line);

// SIGNALs //
void		sigint_glob(int sig);
int			get_signal_stat(void);
void		set_signal_stat(int value);
void		set_signals_interactive_parent(void);
void		set_signals_noninteractive(void);

// PIPEs and REDIRECTIONs//
void 	heredoc(t_redirects *redir);
void 	append(t_redirects *append);
void 	append(t_redirects *redir);
void 	pipe_handler(t_redirects *redir);

//TO DELETE FOR PARSER
void	print_err_msg(char *my_msg);
void	p_log_err(char *cmd, char *msg);
//void	exit_cleanup(int exit_status, t_minishell *minishell);
char	*getcwd_protec(char *buf, size_t size);
int		ft_arrlen(char **arr);
t_pipe 	*fake_parse(char *line);

# endif
	
	