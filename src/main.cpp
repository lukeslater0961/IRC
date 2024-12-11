#include "../includes/Utils.hpp"

int	ErrorMngment(std::string msg)
{
	std::cout << msg << std::endl;
	return (1);
}

int main(int argc, char **argv)
{
	if (argc != 3)
		return (ErrorMngment(INVALID_ARGS));	
	else
		std::cout << argv[1] <<std::endl;
	return(0);
}
