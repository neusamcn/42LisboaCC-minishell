User: what is tokenization? explain considering I'm new to programming and share good resources for me to continue studying

GitHub Copilot: **Tokenization** is the process of breaking a long piece of text into smaller parts called **tokens**.  
If you’re new to programming, think of tokens as “words” or “symbols” the computer can understand.  

Example (shell input):  
````text
ls -la /home
````
Tokens might be:  
- `ls`  
- `-la`  
- home  

In a shell, tokenization is the first step before the program understands and runs the command.

---

## 📚 Beginner-Friendly Resources
- **What is a tokenizer? (simple intro)**  
  https://www.freecodecamp.org/news/lexical-analysis-explained/  

- **Shell parsing basics**  
  https://www.gnu.org/software/bash/manual/bash.html#Shell-Operation  

- **Compiler/lexer basics (easy)**  
  https://craftinginterpreters.com/scanning.html  

If you want, I can point you to more C-focused lexer/tokenizer tutorials.

User: what is tokenization? explain considering I'm new to programming and share good resources for me to continue studying

GitHub Copilot: **Tokenization** is the process of breaking a long piece of text into smaller parts called **tokens**.  
If you’re new to programming, think of tokens as “words” or “symbols” the computer can understand.  

Example (shell input):  
````text
ls -la /home
````
Tokens might be:  
- `ls`  
- `-la`  
- home  

In a shell, tokenization is the first step before the program understands and runs the command.

---

## 📚 Beginner-Friendly Resources
- **What is a tokenizer? (simple intro)**  
  https://www.freecodecamp.org/news/lexical-analysis-explained/  

- **Shell parsing basics**  
  https://www.gnu.org/software/bash/manual/bash.html#Shell-Operation  

- **Compiler/lexer basics (easy)**  
  https://craftinginterpreters.com/scanning.html  

If you want, I can point you to more C-focused lexer/tokenizer tutorials.

User: please do

GitHub Copilot: Here are **C-focused** lexer/tokenizer resources (beginner-friendly):

- **Tiny lexer in C (tutorial)**  
  https://www.youtube.com/watch?v=H5Xf4G2QGvw

- **Write a simple lexer in C (article)**  
  https://norasandler.com/2017/11/29/Write-a-compiler.html (see “Lexing” section)

- **Crafting Interpreters — Scanning** (language-agnostic but very clear)  
  https://craftinginterpreters.com/scanning.html

- **Flex (lex) manual** *(optional tooling, good for understanding)*  
  https://westes.github.io/flex/manual/


---
---

**What is a process?**

When you run a program, the OS creates a **process** — a running instance of that program. Every process has a PID (Process ID) so the OS can track it.

---

**What is a parent and child process?**

When a process calls `fork()`, it creates a copy of itself. The original is the **parent**, the copy is the **child**. The parent is responsible for "acknowledging" that its child has finished.

---

**What is a zombie process?**

When a child process finishes, it does not disappear immediately. It stays in the process table in a dead state, waiting for its parent to **read its exit status** (i.e., acknowledge it exited and how). Until the parent does that, the child is a **zombie** — it is dead but not yet fully cleaned up.

Think of it like this: imagine you send someone a letter and wait for a reply confirming they received it. Until they reply, the letter is just... hanging there.

---

**How does it happen?**

```
Parent process
    │
    ├── fork() → Child process starts
    │               │
    │               └── Child finishes (exits)
    │                       ↓
    │               [zombie: waiting for parent to acknowledge]
    │
    └── Parent never calls wait() → zombie lingers
```

The system call that "acknowledges" the child is `wait()` or `waitpid()`. If the parent never calls it, the child stays a zombie until the parent itself exits.

---

**Is it dangerous?**

A single zombie is harmless — it uses almost no memory. But if a program keeps spawning children without ever acknowledging them, you accumulate thousands of zombies, and the process table fills up. Eventually, the OS cannot create new processes at all.

---

Consider this example from `man fork`:

```c
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	main(void)
{
	pid_t	pid;

	if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
		perror("signal");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	switch (pid)
	{
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
		case 0:
			puts("Child exiting.");
			exit(EXIT_SUCCESS);
		default:
			printf("Child is PID %jd\n", (intmax_t) pid);
			puts("Parent exiting.");
			exit(EXIT_SUCCESS);
	}
}
```

**How does `SIG_IGN` on `SIGCHLD` help?**

`SIGCHLD` is the signal the OS sends to the parent when a child exits. By setting it to `SIG_IGN` (ignore), you are telling the OS:

> "I don't care about the exit status of my children — clean them up automatically."

That way, when the child exits, the OS skips the zombie stage entirely and cleans up immediately without the parent needing to call `wait()`.


Here is a step-by-step breakdown of main:

**1. Ignore SIGCHLD**
```c
signal(SIGCHLD, SIG_IGN);
```
Tells the OS: when a child process exits, do not notify the parent. This prevents the child from becoming a zombie process when it exits before the parent does.

**2. Fork a child process**
```c
pid = fork();
```
`fork()` creates an exact copy of the current process. It returns:
- `-1` → failed
- `0` → you are the child
- a positive number → you are the parent, and the number is the child's PID

**3. Switch on the return value**

- **`case -1`**: fork failed — print an error and exit.

- **`case 0`**: this code runs in the **child process**. It prints `"Child exiting."` and exits cleanly.

- **`default`**: this code runs in the **parent process**. It prints the child's PID using `%jd` and `intmax_t` (a portable way to print `pid_t`), then also exits.

**In summary, the execution flow after `fork()` is:**

```
fork()
 ├── Child  → prints "Child exiting."     → exits
 └── Parent → prints "Child is PID 12345" → prints "Parent exiting." → exits
```

Because `SIGCHLD` is ignored, if the child exits before the parent processes it, the OS cleans up the child immediately without leaving a zombie.

---
---

**Code(quoi); Errno and Error Management in C**
Mia Combeau > C > November 1, 2022
https://www.codequoi.com/en/errno-and-error-management-in-c/

---
---

https://www.man7.org/linux/man-pages/man0/limits.h.0p.html#:~:text=Acceptable%20Value%3A%20%7B_XOPEN_NAME_MAX%7D-,%7BPATH_MAX%7D,-Maximum%20number%20of

Great question.

PATH_MAX is a constant (a fixed number) that means:
the maximum length allowed for a filesystem path on your system.

A path is something like:
/home/neusa/projects/minishell/src/parsing/init.c

So when you write:

char cwd_str[PATH_MAX];

you are creating a character array big enough to store a full path safely in most normal cases.

Why it helps:
1. It prevents you from guessing buffer size manually.
2. Functions like getcwd need a buffer and a size.
3. Using PATH_MAX reduces risk of overflow and crashes from too-small arrays.

In your code, this is the safe pattern:
1. Create buffer with PATH_MAX
2. Fill it with getcwd_protec(cwd_str, PATH_MAX)
3. Use cwd_str as your current directory string

One important beginner note:
PATH_MAX is system-dependent and not always a perfect universal limit across every environment, but for projects like minishell (especially at 42) it is the common and correct practical choice.

If you want, I can also explain what happens in memory when you do char cwd_str[PATH_MAX], using a simple visual analogy.

---
---

**Tokenization** in a shell is the **lexing step**: turning the raw command line into meaningful pieces called **tokens**.

A shell tokenizer usually recognizes:
- **words**: `ls`, `grep`, `file.txt`
- **operators**: `|`, `>`, `>>`, `<`, `<<`, `&&`, `||`
- **special handling for quotes/escapes** so spaces inside quotes stay part of the same token

Example:

```sh
echo "hello world" | grep hello
```

Possible tokens:
- `echo`
- `"hello world"`
- `|`
- `grep`
- `hello`

In a shell, tokenization happens **before parsing and execution**. After tokenization, the shell typically:
1. tokenizes
2. parses into a syntax structure
3. performs expansions
4. executes commands

## Good resources

### Official / authoritative
- **POSIX Shell Command Language**  
  https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html
- **GNU Bash Reference Manual**
  - Shell Grammar
  - Quoting
  - Word Splitting
  - Shell Expansions  
  https://www.gnu.org/software/bash/manual/

### Very useful for learning
- **The Linux Command Line** by William Shotts  
  https://linuxcommand.org/tlcl.php
- **BashGuide**  
  https://mywiki.wooledge.org/BashGuide
- **Bash Pitfalls**  
  https://mywiki.wooledge.org/BashPitfalls

### For minishell / parser work
Search for:
- **lexical analysis in shells**
- **shell grammar**
- **AST for shell commands**
- **quote handling and word splitting**

---
---

## Walkthrough: tokenization in a minishell

Tokenization is the step where the shell reads the raw input line and splits it into **tokens**: words, operators, and special syntax pieces.

Example input:

```sh
echo "hello world" | grep hello > out.txt
```

A tokenizer might produce:

- `echo` → word
- `"hello world"` → word with quotes preserved or marked
- `|` → pipe operator
- `grep` → word
- `hello` → word
- `>` → redirection operator
- `out.txt` → word

---

## What the tokenizer must handle

### 1. Spaces separate tokens
Normally, spaces split words:

```sh
ls -l /tmp
```

Tokens:
- `ls`
- `-l`
- tmp

### 2. Quotes protect spaces and special chars
Inside quotes, spaces are part of the same token:

```sh
echo "a b c"
```

Tokens:
- `echo`
- `a b c`

Single quotes and double quotes behave differently later in expansion, but during tokenization they both group text.

### 3. Operators are separate tokens
Shell operators must be recognized as their own tokens:

- `|`
- `<`
- `>`
- `>>`
- `<<`
- `&&`
- `||`

Example:

```sh
cat < infile >> outfile
```

Tokens:
- `cat`
- `<`
- `infile`
- `>>`
- `outfile`

### 4. Escapes matter
A backslash can prevent special meaning:

```sh
echo hello\ world
```

Tokens:
- `echo`
- `hello world`

### 5. Empty input and syntax errors
Tokenizer should also detect invalid cases:

- unclosed quote: `echo "hello`
- invalid operator placement: `| ls`
- consecutive operators if not allowed by your grammar

---

## Typical minishell flow

For a project like `minishell`, the pipeline is usually:

1. **Read line**
2. **Tokenize**
3. **Parse tokens into commands / AST**
4. **Expand variables**
5. **Execute**

Tokenization should **not** execute anything. It only classifies text.

---

## Example walkthrough

Input:

```sh
export PATH="$PATH:/my/bin" | grep bin
```

A simple tokenizer may scan character by character:

- `export` → word token
- `PATH="$PATH:/my/bin"` → word token, with quoted section inside
- `|` → pipe token
- `grep` → word token
- `bin` → word token

Then the parser later decides:

- first command: `export ...`
- pipe to second command: `grep bin`

---

## Common token categories for minishell

You may want enums like:

- `WORD`
- `PIPE`
- `REDIR_IN`
- `REDIR_OUT`
- `APPEND`
- `HEREDOC`

This makes parsing easier.

---

## Important implementation idea

A tokenizer often tracks:

- current index in the input
- current quote state:
  - none
  - single quote
  - double quote
- current token start/end
- whether an operator starts here

This is usually easier if you scan one character at a time.

---

## In your `minishell`

For your project, the tokenizer should probably:

- ignore spaces outside quotes
- keep quoted strings as one token
- separate shell operators
- report syntax errors early
- preserve enough info for later expansion and parsing

---

## Good next step

A useful exercise is to test these inputs:

```sh
ls -l
echo "hello world"
cat < infile | grep a > out
echo 'a | b'
echo "$HOME"
```

For each one, write down the expected tokens before coding.

If useful, I can also provide:
- a **step-by-step tokenizer algorithm**
- a **token struct design for minishell**
- or a **sample `tokenize()` pseudocode**.

---
---


