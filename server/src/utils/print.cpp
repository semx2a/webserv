#include "print.hpp"

std::string	str_of(size_t size, const std::string & c) {
	
	std::string	buffer;

	buffer.reserve(size);
	for (size_t x = 0; x < size; ++x) {
		buffer.append(c);
	}
	return buffer;
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
	
	std::cout << "  " << str_of(total_len , "_") << std::endl;

	std::cout << " |" << ORANGE << date_str << str_of(len_between, " ") << CYAN << header_str << NO_COLOR << "|" << std::endl;
	std::cout << " |" << PURPLE << str << NO_COLOR << str_of(len, " ") << "|" << std::endl;
	
	std::cout << "  " << str_of(total_len, "‾") << std::endl;
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

std::string	print_str (std::string buffer) {

	std::stringstream sstr;

	std::string str ("Received: ");
	sstr << str << buffer;
	return sstr.str();
}

std::string		print_vector_of_char (std::vector <char>& buffer) {

	std::stringstream sstr;

	std::string	 str ("Received: ");
	sstr << str << &buffer[0];
	return sstr.str();
}

std::string		print_client_added () {

	std::stringstream sstr;

	std::string	 str ("New client added");
	sstr << PURPLE << str << NO_COLOR;
	return sstr.str();
}

std::string		print_end_connexion () {

	std::stringstream sstr;

	std::string	 str ("End of connection with a client");
	sstr << PURPLE << str << NO_COLOR;
	return sstr.str();
}

std::string		print_wait () {

	std::stringstream sstr;

	std::string	 str (" Waiting for connections...");
	sstr << "\n" << GREEN << custom_width (68, ':', str) << str << NO_COLOR;
	return sstr.str();
}

