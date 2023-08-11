#include "../inc/display.hpp"

std::string custom_width (int width, char c, std::string const&  content) {

	width -= content.length ();
	if (width <= 0)
		return "";

	std::string result;
	result.reserve (width);
	for (int i = 0; i < width; ++i) {
		result.push_back (c);
	}
	return result;
}

void	display_buffer (/* std::vector <char> */std::string buffer) {

	std::string const& str ("Received: ");
	//std::cout << str << &buffer[0] << std::endl;
	std::cout << str << buffer << std::endl;
}

void	display_client_added () {

	std::string const& str ("New client added");
	std::cout << PURPLE << str << NO_COLOR << std::endl;
}

void	display_end_connexion () {

	std::string const& str ("End of connection with a client");
	std::cout << PURPLE << str << NO_COLOR << std::endl;
}

void	display_wait () {

	std::string const& str (" Waiting for connections...");
	std::cout << "\n" << GREEN << custom_width (100, ':', str) << str << NO_COLOR << std::endl;
}
