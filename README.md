# Minishell

The "minishell" repository contains my implementation of the minishell project from the 42School curriculum. minishell is a small-scale Unix shell that provides a command-line interface for users to interact with the operating system.

## Description

The minishell project is a significant, complex, and comprehensive project that involves creating a functional Unix shell. It requires implementing various features such as command parsing, executing commands, handling signals, managing environment variables, and supporting advanced functionalities like pipes.

While the project is challenging and extensive, it's important to acknowledge that there are conceptual errors in the implementation of certain functions, especially when it comes to handling pipes. One notable error relates to executing commands in a sequential manner instead of synchronously within the pipe mechanism. However, these mistakes are part of the learning process. They highlight the importance of making mistakes and understanding how things actually work. As the saying goes, "You learn by making mistakes."

Despite the conceptual errors, the minishell project serves as a fundamental learning experience. It provides a deep understanding of how a shell operates, the intricacies of command execution, and the underlying mechanisms of the Unix operating system.

As someone who has a strong affinity for bash and the command-line environment, I find this project particularly interesting. It allows me to explore and appreciate the inner workings of a shell, and it serves as a solid foundation for further exploration and potential improvement in the future. Who knows, maybe one day I'll revisit this project and rewrite it with a better understanding and enhanced functionality.

## Usage

1. Clone repository to your local system:
	```shell
	git clone https://github.com/stenterello/minishell.git

2. Navigate to the repository's directory:
	```shell
	cd minishell

3. Compile the program using the provided Makefile:
	```shell
	make

4. Run the minishell program:
	```shell
	./minishell

5. Use the minishell to execute various commands, navigate the file system, and explore the available functionalities.
