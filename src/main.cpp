#include "../includes/Utils.hpp"
#include "../includes/Server.hpp"

int main(int argc, char **argv)
{
	if (argc != 3)
		return (ErrorMngment(INVALID_ARGS));	
	else
		SetupServer(argv);
	return(0);
}
