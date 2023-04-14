#include "pipex.h"

int	main(int ac, char **ag, char **envp)
{
	int	infile;
	int	outfile;

	if (ac != 5)
		return (write(1,"Invalid number of arguments.", 29));
	if (check_empty(ag[2]) || check_empty(ag[3]))
		return (1);
	infile = open(ag[1], O_RDONLY);
	outfile = open(ag[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (infile < 0)
		return (perror("infile error"), ag[1]);
	if (outfile < 0)
		return (perror("outfile error"), ag[4]);
	pipex(infile, outfile, ag, envp);
	if (close(infile) < 0 || close(outfile) < 0)
		return (perror("files error"), 0);
	return (0);
}

