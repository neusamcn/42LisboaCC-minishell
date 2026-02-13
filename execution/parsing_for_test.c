/* ************************************************************************** */
/*  minishell_parser_onefile.c                                                */
/*  Minimal parser for testing execution (pipes + redirs + quotes + $expand). */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* ============================ Data structures ============================ */

typedef enum e_toktype {
	T_WORD,
	T_PIPE,
	T_REDIR_IN,      /* <  */
	T_REDIR_OUT,     /* >  */
	T_REDIR_APPEND,  /* >> */
	T_HEREDOC        /* << */
}	t_toktype;

typedef struct s_token {
	t_toktype        type;
	char            *text;   /* only for WORD; for ops it's optional */
	struct s_token  *next;
}	t_token;

typedef struct s_redir {
	t_toktype        type;   /* one of redir types */
	char            *target; /* filename or heredoc delimiter */
	struct s_redir  *next;
}	t_redir;

typedef struct s_cmd {
	char           **argv;       /* NULL-terminated */
	int              argc;
	int              argv_cap;
	t_redir         *redirs;
	struct s_cmd    *next;
}	t_cmd;

typedef struct s_pipeline {
	t_cmd *cmds; /* linked list of commands */
}	t_pipeline;

/* ============================ Small helpers ============================ */

static void	*xfree(void *p) { free(p); return NULL; }

static int	is_space(int c) { return (c == ' ' || c == '\t' || c == '\n'); }

static char	*xstrndup(const char *s, size_t n)
{
	char *r = (char *)malloc(n + 1);
	if (!r) return NULL;
	memcpy(r, s, n);
	r[n] = '\0';
	return r;
}

static char	*xstrdup(const char *s)
{
	if (!s) return NULL;
	return xstrndup(s, strlen(s));
}

static char	*xstrjoin2(const char *a, const char *b)
{
	size_t la = a ? strlen(a) : 0;
	size_t lb = b ? strlen(b) : 0;
	char *r = (char *)malloc(la + lb + 1);
	if (!r) return NULL;
	if (a) memcpy(r, a, la);
	if (b) memcpy(r + la, b, lb);
	r[la + lb] = '\0';
	return r;
}

static int	append_char(char **s, char c)
{
	size_t len = (*s) ? strlen(*s) : 0;
	char *r = (char *)malloc(len + 2);
	if (!r) return 0;
	if (*s) memcpy(r, *s, len);
	r[len] = c;
	r[len + 1] = '\0';
	free(*s);
	*s = r;
	return 1;
}

static int	append_cstr(char **dst, const char *src)
{
	char *joined = xstrjoin2(*dst ? *dst : "", src ? src : "");
	if (!joined) return 0;
	free(*dst);
	*dst = joined;
	return 1;
}

/* ============================ Token list helpers ============================ */

static t_token	*tok_new(t_toktype type, const char *text)
{
	t_token *t = (t_token *)malloc(sizeof(*t));
	if (!t) return NULL;
	t->type = type;
	t->text = text ? xstrdup(text) : NULL;
	t->next = NULL;
	return t;
}

static void	tok_add_back(t_token **head, t_token *node)
{
	t_token *cur;
	if (!node) return;
	if (!*head) { *head = node; return; }
	cur = *head;
	while (cur->next) cur = cur->next;
	cur->next = node;
}

static void	free_tokens(t_token *t)
{
	while (t)
	{
		t_token *n = t->next;
		free(t->text);
		free(t);
		t = n;
	}
}

/* ============================ Cmd / pipeline helpers ============================ */

static t_cmd	*cmd_new(void)
{
	t_cmd *c = (t_cmd *)malloc(sizeof(*c));
	if (!c) return NULL;
	c->argv = NULL;
	c->argc = 0;
	c->argv_cap = 0;
	c->redirs = NULL;
	c->next = NULL;
	return c;
}

static int	cmd_argv_push(t_cmd *c, const char *word)
{
	char **newv;
	int newcap;
	if (!c) return 0;
	if (c->argc + 1 >= c->argv_cap)
	{
		newcap = (c->argv_cap == 0) ? 8 : (c->argv_cap * 2);
		newv = (char **)malloc(sizeof(char *) * newcap);
		if (!newv) return 0;
		for (int i = 0; i < c->argc; i++)
			newv[i] = c->argv[i];
		free(c->argv);
		c->argv = newv;
		c->argv_cap = newcap;
	}
	c->argv[c->argc] = xstrdup(word);
	if (!c->argv[c->argc]) return 0;
	c->argc++;
	c->argv[c->argc] = NULL;
	return 1;
}

static int	cmd_add_redir(t_cmd *c, t_toktype rtype, const char *target)
{
	t_redir *r = (t_redir *)malloc(sizeof(*r));
	t_redir *cur;
	if (!r) return 0;
	r->type = rtype;
	r->target = xstrdup(target);
	r->next = NULL;
	if (!r->target) { free(r); return 0; }
	if (!c->redirs) { c->redirs = r; return 1; }
	cur = c->redirs;
	while (cur->next) cur = cur->next;
	cur->next = r;
	return 1;
}

static void	free_pipeline(t_pipeline *p)
{
	t_cmd *c;
	if (!p) return;
	c = p->cmds;
	while (c)
	{
		t_cmd *cn = c->next;
		if (c->argv)
		{
			for (int i = 0; c->argv[i]; i++)
				free(c->argv[i]);
			free(c->argv);
		}
		while (c->redirs)
		{
			t_redir *rn = c->redirs->next;
			free(c->redirs->target);
			free(c->redirs);
			c->redirs = rn;
		}
		free(c);
		c = cn;
	}
	free(p);
}

/* ============================ Quote check ============================ */

static int	quotes_closed(const char *s)
{
	char q = 0;
	for (size_t i = 0; s && s[i]; i++)
	{
		if (!q && (s[i] == '\'' || s[i] == '"'))
			q = s[i];
		else if (q && s[i] == q)
			q = 0;
	}
	return (q == 0);
}

/* ============================ Tokenizer ============================ */
/* Tokenizer keeps quoted content inside WORD tokens, including the quote chars.
   Expansion will later remove quotes and expand $ rules. */

static int	is_opchar(char c) { return (c == '|' || c == '<' || c == '>'); }

static t_token	*tokenize(const char *line, char **err)
{
	size_t i = 0;
	t_token *head = NULL;

	*err = NULL;
	if (!line) return NULL;

	while (line[i])
	{
		if (is_space((unsigned char)line[i])) { i++; continue; }

		/* Operators */
		if (line[i] == '|')
		{
			tok_add_back(&head, tok_new(T_PIPE, NULL));
			i++;
			continue;
		}
		if (line[i] == '<')
		{
			if (line[i + 1] == '<') { tok_add_back(&head, tok_new(T_HEREDOC, NULL)); i += 2; }
			else { tok_add_back(&head, tok_new(T_REDIR_IN, NULL)); i += 1; }
			continue;
		}
		if (line[i] == '>')
		{
			if (line[i + 1] == '>') { tok_add_back(&head, tok_new(T_REDIR_APPEND, NULL)); i += 2; }
			else { tok_add_back(&head, tok_new(T_REDIR_OUT, NULL)); i += 1; }
			continue;
		}

		/* WORD: read until space or operator, but honor quotes */
		{
			size_t start = i;
			char q = 0;
			while (line[i])
			{
				if (!q && (line[i] == '\'' || line[i] == '"'))
					q = line[i++];
				else if (q && line[i] == q)
					q = 0, i++;
				else if (!q && (is_space((unsigned char)line[i]) || is_opchar(line[i])))
					break;
				else
					i++;
			}
			{
				char *w = xstrndup(line + start, i - start);
				t_token *t;
				if (!w) { *err = xstrdup("malloc failed"); free_tokens(head); return NULL; }
				t = tok_new(T_WORD, w);
				free(w);
				if (!t) { *err = xstrdup("malloc failed"); free_tokens(head); return NULL; }
				tok_add_back(&head, t);
			}
		}
	}
	return head;
}

/* ============================ Syntax checks ============================ */

static int	is_redir(t_toktype t)
{
	return (t == T_REDIR_IN || t == T_REDIR_OUT || t == T_REDIR_APPEND || t == T_HEREDOC);
}

static int	syntax_check(t_token *t, char **err)
{
	*err = NULL;
	if (!t) return 1;

	/* cannot start with pipe */
	if (t->type == T_PIPE) { *err = xstrdup("syntax error near unexpected token `|`"); return 0; }

	while (t)
	{
		if (t->type == T_PIPE)
		{
			if (!t->next || t->next->type == T_PIPE)
			{
				*err = xstrdup("syntax error near unexpected token `|`");
				return 0;
			}
		}
		if (is_redir(t->type))
		{
			if (!t->next || t->next->type != T_WORD)
			{
				*err = xstrdup("syntax error near unexpected token `newline`");
				return 0;
			}
		}
		t = t->next;
	}
	return 1;
}

/* ============================ Expansion ============================ */

static const char	*env_get(char **envp, const char *name, size_t nlen)
{
	if (!envp || !name) return NULL;
	for (int i = 0; envp[i]; i++)
	{
		const char *e = envp[i];
		const char *eq = strchr(e, '=');
		if (!eq) continue;
		if ((size_t)(eq - e) == nlen && strncmp(e, name, nlen) == 0)
			return eq + 1;
	}
	return NULL;
}

static char	*itoa_simple(int x)
{
	char buf[32];
	snprintf(buf, sizeof(buf), "%d", x);
	return xstrdup(buf);
}

/* Takes a raw WORD token (may include quotes), returns expanded word with quotes removed. */
static char	*expand_word(const char *raw, char **envp, int last_status)
{
	size_t i = 0;
	char q = 0; /* 0 / '\'' / '"' */
	char *out = xstrdup("");
	if (!out) return NULL;

	while (raw && raw[i])
	{
		if (!q && (raw[i] == '\'' || raw[i] == '"'))
		{
			q = raw[i++];
			continue;
		}
		else if (q && raw[i] == q)
		{
			q = 0;
			i++;
			continue;
		}

		if (raw[i] == '$' && q != '\'')
		{
			/* expand $?, $VAR */
			if (raw[i + 1] == '?')
			{
				char *num = itoa_simple(last_status);
				if (!num || !append_cstr(&out, num)) { free(num); return xfree(out); }
				free(num);
				i += 2;
				continue;
			}
			else if (isalpha((unsigned char)raw[i + 1]) || raw[i + 1] == '_')
			{
				size_t j = i + 1;
				while (isalnum((unsigned char)raw[j]) || raw[j] == '_') j++;
				{
					const char *val = env_get(envp, raw + i + 1, j - (i + 1));
					if (val && !append_cstr(&out, val)) return xfree(out);
				}
				i = j;
				continue;
			}
			/* otherwise: treat '$' as literal */
		}

		if (!append_char(&out, raw[i]))
			return xfree(out);
		i++;
	}
	return out;
}

/* ============================ Build pipeline ============================ */

static t_pipeline	*build_pipeline(t_token *t, char **envp, int last_status, char **err)
{
	t_pipeline *p = (t_pipeline *)malloc(sizeof(*p));
	t_cmd *cur = NULL;
	t_cmd *last = NULL;

	*err = NULL;
	if (!p) { *err = xstrdup("malloc failed"); return NULL; }
	p->cmds = NULL;

	cur = cmd_new();
	if (!cur) { free(p); *err = xstrdup("malloc failed"); return NULL; }
	p->cmds = cur;
	last = cur;

	while (t)
	{
		if (t->type == T_PIPE)
		{
			cur = cmd_new();
			if (!cur) { *err = xstrdup("malloc failed"); free_pipeline(p); return NULL; }
			last->next = cur;
			last = cur;
			t = t->next;
			continue;
		}
		if (is_redir(t->type))
		{
			t_toktype rt = t->type;
			t = t->next; /* must be WORD due to syntax_check */
			{
				char *expanded_target = expand_word(t->text, envp, last_status);
				if (!expanded_target) { *err = xstrdup("malloc failed"); free_pipeline(p); return NULL; }
				if (!cmd_add_redir(cur, rt, expanded_target))
				{
					free(expanded_target);
					*err = xstrdup("malloc failed");
					free_pipeline(p);
					return NULL;
				}
				free(expanded_target);
			}
			t = t->next;
			continue;
		}
		if (t->type == T_WORD)
		{
			char *expanded = expand_word(t->text, envp, last_status);
			if (!expanded) { *err = xstrdup("malloc failed"); free_pipeline(p); return NULL; }
			/* Keep empty args as empty string (bash keeps "" as arg). */
			if (!cmd_argv_push(cur, expanded))
			{
				free(expanded);
				*err = xstrdup("malloc failed");
				free_pipeline(p);
				return NULL;
			}
			free(expanded);
		}
		t = t->next;
	}
	return p;
}

/* ============================ Public API ============================ */
/* Returns pipeline or NULL. If NULL, err contains message (malloc’d) or NULL. */
t_pipeline	*parse_line(const char *line, char **envp, int last_status, char **err)
{
	t_token *toks = NULL;
	t_pipeline *p = NULL;

	*err = NULL;
	if (!line) return NULL;

	if (!quotes_closed(line))
	{
		*err = xstrdup("syntax error: unclosed quotes");
		return NULL;
	}

	toks = tokenize(line, err);
	if (!toks)
		return NULL;

	if (!syntax_check(toks, err))
	{
		free_tokens(toks);
		return NULL;
	}

	p = build_pipeline(toks, envp, last_status, err);
	free_tokens(toks);
	return p;
}

/* ============================ Debug print (optional) ============================ */
static const char *tokname(t_toktype t)
{
	if (t == T_REDIR_IN) return "<";
	if (t == T_REDIR_OUT) return ">";
	if (t == T_REDIR_APPEND) return ">>";
	if (t == T_HEREDOC) return "<<";
	if (t == T_PIPE) return "|";
	return "WORD";
}

void	debug_print_pipeline(t_pipeline *p)
{
	int cmd_i = 0;
	for (t_cmd *c = p ? p->cmds : NULL; c; c = c->next)
	{
		printf("CMD %d:\n", cmd_i++);
		printf("  argv:");
		if (!c->argv || !c->argv[0]) printf(" (empty)");
		for (int i = 0; c->argv && c->argv[i]; i++)
			printf(" [%s]", c->argv[i]);
		printf("\n");
		printf("  redirs:");
		if (!c->redirs) printf(" (none)");
		for (t_redir *r = c->redirs; r; r = r->next)
			printf(" (%s %s)", tokname(r->type), r->target);
		printf("\n");
	}
}

int main(int ac, char **av, char **envp)
{
	(void)ac; (void)av;
	char *err = NULL;
	int last = 0;

	const char *tests[] = {
		"echo $USER | grep mi > out.txt",
		"cat < in.txt | wc -l",
		"echo '$USER' \"$USER\" $?",
		"ls | | wc",
		"echo hi >",
		NULL
	};

	for (int i = 0; tests[i]; i++)
	{
		printf("INPUT: %s\n", tests[i]);
		t_pipeline *p = parse_line(tests[i], envp, last, &err);
		if (!p)
		{
			printf("ERR: %s\n\n", err ? err : "(null)");
			free(err);
			err = NULL;
			continue;
		}
		debug_print_pipeline(p);
		printf("\n");
		free_pipeline(p);
	}
	return 0;
}




///////////////
