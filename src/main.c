#include "execution.h"
#include "minishell.h"

#include "../include/minishell.h"

static int ft_arrlen(char **arr)
{
	int i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}

static void add_redir(t_cmd_line *current, t_redir_type type, char *filename, char *delimiter)
{
	t_redirects *new;
	t_redirects *tmp;

	write(2, "add_redir called, type: ", 24);
	if (type == OUT) write(2, "OUT\n", 4);
	else if (type == HEREDOC) write(2, "HEREDOC\n", 8);
	else if (type == IN) write(2, "IN\n", 3);
	else if (type == APPEND) write(2, "APPEND\n", 7);

	if (current->redir.type == NONE)
	{
		current->redir.type = type;
		current->redir.filename = filename;
		current->redir.delimiter = delimiter;
		return ;
	}
	new = malloc(sizeof(t_redirects));
	ft_memset(new, 0, sizeof(t_redirects));
	new->type = type;
	new->filename = filename;
	new->delimiter = delimiter;
	new->next = NULL;
	tmp = &current->redir;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}
t_cmd_line *fake_parse(char *line)
{
	char        **tokens;
	t_cmd_line  *head;
	t_cmd_line  *current;
	int         i;
	int         cmd_i;

	tokens = ft_split(line, ' ');
	if (!tokens)
		return NULL;
	head = malloc(sizeof(t_cmd_line));
	ft_memset(head, 0, sizeof(t_cmd_line));
	head->redir.xd_fd = -1;        // <-- ADD
	head->cmds = malloc(sizeof(char *) * (ft_arrlen(tokens) + 1));
	current = head;
	cmd_i = 0;
	i = 0;
	while (tokens[i])
	{
		if (ft_strcmp(tokens[i], "|") == 0)
		{
			current->cmds[cmd_i] = NULL;
			current->next = malloc(sizeof(t_cmd_line));
			ft_memset(current->next, 0, sizeof(t_cmd_line));
			current->next->redir.xd_fd = -1;   // <-- ADD
			current->next->cmds = malloc(sizeof(char *) * (ft_arrlen(tokens) + 1));
			current = current->next;
			cmd_i = 0;
		}
		else if (ft_strcmp(tokens[i], ">") == 0 && tokens[i + 1])
			add_redir(current, OUT, tokens[++i], NULL);
		else if (ft_strcmp(tokens[i], ">>") == 0 && tokens[i + 1])
			add_redir(current, APPEND, tokens[++i], NULL);
		else if (ft_strcmp(tokens[i], "<") == 0 && tokens[i + 1])
			add_redir(current, IN, tokens[++i], NULL);
		else if (ft_strcmp(tokens[i], "<<") == 0 && tokens[i + 1])
			add_redir(current, HEREDOC, NULL, tokens[++i]);
		else
			current->cmds[cmd_i++] = tokens[i];
		i++;
	}
	current->cmds[cmd_i] = NULL;
	return head;
}

// Neusa, if minishell is invalid -? it crashed
/*
void	exit_cleanup(int exit_status, t_minishell *minishell)
{
	int	i;

	ft_putendl_fd("Exiting minishell...", STDOUT_FILENO);
	clear_history();
	i = 0;
	// segf if mini is NULL
	while (minishell->minienvp[i])
		free(minishell->minienvp[i++]);
	free(minishell->minienvp);
	// TODO: function to close all previously opened fd? // the OS is closing them
	// automatically on exit()
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO); // closing it bad idea here
	// TODO: can i close stderr before sending an exit status? // no
	exit(exit_status);
}*/

void	exit_cleanup(int exit_status, t_minishell *minishell)
{
	int	i;

	ft_putendl_fd("Exiting minishell...", STDOUT_FILENO);
	clear_history();
	i = 0;
	if (minishell)
	{
		while (minishell->minienvp[i])
			free(minishell->minienvp[i++]);
		free(minishell->minienvp);	
	}
	free(minishell);
	exit(exit_status);
}

//changing envp(initializing shell's envp (copied))
int	main(int ac, char **av, char **envp)
{
	char		*prompt;
	t_cmd_line	*cmd_line;
    t_minishell *shelly;

	(void)ac;
	(void)av;
	sig_mode(INTERACTIVE);
	if (!envp)
		write(1, "envp is NULL!\n", 14);
    shelly = set_minienvp(envp);
	while (1)
	{
		prompt = readline("minishell$ ");
		if (!prompt)
		{
			ft_putstr_fd("exit\n", 1);
			exit(get_signal_stat());
		}
		if (*prompt)
			add_history(prompt);
		cmd_line = fake_parse(prompt);
		if (!cmd_line)
		{
			free(prompt);
			continue ;
		}
		exec_loop(cmd_line, shelly);
		free(prompt);
		sig_mode(INTERACTIVE);
	}
	return 0;
}


