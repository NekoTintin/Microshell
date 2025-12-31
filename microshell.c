#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

void	print_error(char *str)
{
	while (*str)
		write(2, str++, 1);
}

int	mini_cd(char **argv, int nargs)
{
	if (nargs != 2)
		return (print_error("error: cd: bad arguments\n"), 1);
	if (chdir(argv[1]) != 0)
		return (print_error("error: cd: cannot change directory to "),
			print_error(argv[1]), print_error("\n"), 1);
	return (0);
}

void	set_pipe(int has_pipe, int *fd, int stdfd)
{
	if (has_pipe && (dup2(fd[stdfd], stdfd) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
		print_error("error: fatal\n"), print_error("\n"), exit(1);
}

int	exec(char **argv, int i, int has_pipe, char **envp)
{
	int		fd[2];
	int		pid;

    if (strcmp(argv[0], ";") == 0)
		return (0);
	if (!has_pipe && strcmp(argv[0], "cd") == 0)
		return (mini_cd(argv, i));
	if ((has_pipe && pipe(fd) == -1) || (pid = fork()) == -1)
		print_error("error: fatal"), exit(1);
	if (pid == 0)
	{
		argv[i] = NULL;
		set_pipe(has_pipe, fd, 1);
		if (strcmp(argv[0], "cd") == 0)
			exit (mini_cd(argv, i));
		if (execve(argv[0], argv, envp) == -1)
			print_error("error: cannot execute "), print_error(argv[0]), print_error("\n"), exit(1);
	}
	waitpid(pid, 0, 0);
	set_pipe(has_pipe, fd, 0);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	int	i = 0;
	(void)argc;

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