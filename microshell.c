#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

void	print_error(char *err, char *file)
{
	while (*err)
		write(2, err++, 1);
	if (file)
	{
		write(2, " ", 1);
		while (*file)
			write(2, file++, 1);
	}
	write(2, "\n", 1);
}

int	mini_cd(char **argv, int nargs)
{
	if (nargs != 2)
		return (print_error("error: cd: bad arguments", NULL), 1);
	if (chdir(argv[1]) != 0)
		return (print_error("error: cd: cannot change directory to", argv[1]), 1);
	return (0);
}

void	set_pipe(int has_pipe, int *fds, int stdfd)
{
	if (has_pipe && (dup2(fds[stdfd], stdfd) == -1 || close(fds[0]) == -1 || close(fds[1]) == -1))
		print_error("error: fatal\n", NULL), exit(1);
}

int	exec(char **argv, int i, int has_pipe, char **envp)
{
	int		fds[2];
	int		pid;

	if (strcmp(argv[0], "cd") == 0)
		return (mini_cd(argv, i));
	if ((has_pipe && pipe(fds) == -1) || (pid = fork()) == -1)
		print_error("error: fatal", NULL), exit(1);
	if (pid == 0)
	{
		argv[i] = NULL;
		set_pipe(has_pipe, fds, 1);
		if (execve(argv[0], argv, envp) == -1)
			print_error("error: cannot execute", argv[0]), exit(1);
	}
	waitpid(pid, 0, 0);
	set_pipe(has_pipe, fds, 0);
	return (0);
}

// This function is optional to exam, this part is just for the Github repository
void	print_usage()
{
	print_error("Usage: ./microshell [command1 args1 ; command2 args2 | command3 args3 ...]", NULL);
	print_error("Commands are separated by ';'.", NULL);
	print_error("Pipes '|' can be used to connect commands.", NULL);
	print_error("Built-in command: cd [directory]", NULL);
}

int	main(int argc, char **argv, char **envp)
{
	int	i = 0;
	(void)argc; // Needed to compile to exam

	if (argc < 2)                  // Optional to exam, this part is just for the Github repository
		return (print_usage(), 0); // Optional to exam, this part is just for the Github repository
    argv++;
	while (argv[i])
	{
		while (argv[i] && strcmp(argv[i], ";") && strcmp(argv[i], "|"))
			i++;
		if (i > 0)
			exec(argv, i, (argv[i] && strcmp(argv[i], "|") == 0), envp);
		if (!argv[i])
		    break ;
		argv += i + 1;
		i = 0;
	}
	return (0);
}