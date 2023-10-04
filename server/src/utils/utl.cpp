#include "utl.hpp"

utl::utl() {}

utl::utl(utl const& rhs) {
	*this = rhs;
}

utl::~utl() {}

utl& utl::operator=(utl const& rhs) {
	if (this != &rhs) {
		// copy
	}
	return *this;
}

std::string					utl::trim(std::string const& str) {

    const std::string ws = " \n\r\t\f\v";
    
    size_t start = str.find_first_not_of(ws);
    if (start == std::string::npos)
        return ""; // no content except whitespace

    size_t end = str.find_last_not_of(ws);
    
    return str.substr(start, end-start+1);
}

int							utl::find_last_occurrence(const std::vector<char> & haystack, const std::string& needle) {
    // Convert the vector of characters to a string
    std::string haystack_str(haystack.begin(), haystack.end());

    // Use rfind to find the last occurrence of the needle
    size_t pos = haystack_str.rfind(needle);

    // If the needle is not found, return -1
    if(pos == std::string::npos) {
        return -1;
    }

    // Otherwise, return the position of the last character of the needle
    return pos + needle.size() - 1;
}

std::vector<std::string>	utl::tokenize(const std::string str, char sep) {
	
	std::vector<std::string>	token_vector;
	std::string					token;
	std::istringstream 			str_stream(str);

	while (getline(str_stream, token, sep)) {

		if (token.length() > 0)
			token_vector.push_back(token);
	}
	
	return token_vector;
}

std::string 				utl::trimAndReplaceWs(std::string& input) {

	std::stringstream	ss(input);
	std::string			word;

	input.clear();
	while (ss >> word) {
		if (not input.empty()) {
			input += " ";
		}
		input += word;
	}
	
	return input;
}

std::string					utl::str_of(size_t size, const std::string & c) {
	
	std::string	buffer;

	buffer.reserve(size);
	for (size_t x = 0; x < size; ++x) {
		buffer.append(c);
	}
	return buffer;
}

void						utl::log(int client_fd, std::string str) {

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

	std::cout << " | " << ORANGE << date_str << str_of(len_between, " ") << " " << CYAN << header_str << RESET << " |" << std::endl;
	std::cout << " | " << PURPLE << str << RESET << str_of(len, " ") << " |" << std::endl;
	
	std::cout << "  " << str_of(total_len, "‾") << std::endl;
}

std::string					utl::custom_width (int width, char c, std::string const&  content) {

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

std::string					utl::print_str (std::string buffer) {

	std::string str("Received: ");
	std::stringstream sstr(str);

	sstr << buffer;
	return sstr.str();
}

std::string					utl::print_vector_of_char (std::vector <char>& buffer) {

	std::string	 str("Received: ");
	std::stringstream sstr(str);

	sstr << &buffer[0];
	return sstr.str();
}

std::string					utl::print_client_added () {

	std::string	 str("New client added");
	std::stringstream sstr;

	sstr << PURPLE << str << RESET;
	return sstr.str();
}

std::string					utl::print_end_connexion () {

	std::string	 str("End of connection with a client");
	std::stringstream sstr;

	sstr << PURPLE << str << RESET;
	return sstr.str();
}

std::string					utl::print_wait () {

	std::string	 str (" Waiting for connections...");
	std::stringstream sstr;

	sstr << std::endl << GREEN << custom_width (68, ':', str) << str << RESET;
	return sstr.str();
}
