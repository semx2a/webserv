#include "../inc/display.hpp"
#include <cstring>

void	printLine(size_t size, std::string c) {
	
	std::cout << "  ";
	for (size_t x = 0; x < size; x++) {
		std::cout << c;
	}
	std::cout << std::endl;
	
}

void	log(int client_fd, std::string str)
{
	char				date[100];
	std::stringstream	sstr;
	std::string			date_str;
	std::string			header_str;
	time_t				now;
	tm					*brk;

	time(&now);
	brk = localtime(&now);
	strftime(date, 99, "%B %d %Y %T", brk);
	date_str.append(date);

	sstr << client_fd;
	header_str.append("Client fd: ");
	header_str.append(sstr.str());
	
	size_t	total_len 	= 60;
	size_t	len_between	= total_len - (header_str.length() + date_str.length() + 3);
	size_t	len 		= total_len - (str.length() + 2);
	
	printLine(total_len , "_");

	std::cout << " |" << ORANGE << date_str << std::string(len_between, ' ') << CYAN << header_str << NO_COLOR << "|" << std::endl;
	std::cout << " |" << PURPLE << str << NO_COLOR << std::string(len, ' ') << "|" << std::endl;
	
	printLine(total_len, "‾");
	std::cout << std::ends;
}


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

void	display_buffer (std::string buffer) {

	std::string const& str ("Received: ");
	std::cout << str << buffer << std::endl;
}

void	display_buffer (std::vector <char>& buffer) {

	std::string const& str ("Received: ");
	std::cout << str << &buffer[0] << std::endl;
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

//	std::string const& str (" Waiting for connections...");
//	std::cout << "\n" << GREEN << custom_width (68, ':', str) << str << NO_COLOR << std::endl;
}


