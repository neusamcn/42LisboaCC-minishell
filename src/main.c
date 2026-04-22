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

int	main(int ac, char **av, char **envp)
{
	char	    *prompt;
	t_cmd_line	*cmd_line;
//	int		status;

	(void)ac;
	(void)av;
    if (!envp || !envp[0])
        write(1, "envp is NULL!\n", 14);
	//status = 0;
	while (1)
	{
		prompt = readline("minishell$ ");
		if (!prompt)
			break ;
		if (*prompt)
			add_history(prompt);
		cmd_line = fake_parse(prompt);
		if (!cmd_line)
		{
			free(prompt);
			continue ;
		}
        exec_loop(cmd_line, envp);
		free(prompt);
	}
	return 0;
}


