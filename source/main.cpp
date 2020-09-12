#include "../include/main.h"
#include <iostream>


int main(int argc, char * argv[])
{
	UserInterface *user = new UserInterface(&argc, &argv);
	user->run_window();
}
