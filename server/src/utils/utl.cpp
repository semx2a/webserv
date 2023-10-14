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

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: METHODS::

int					utl::searchVectorChar(std::vector<char> tab, char const* to_find, size_t index) {

	std::vector<char>::iterator it = std::search(tab.begin() + index, tab.end(), to_find, to_find + std::strlen(to_find));
	if (it == tab.end())
		return (-1);
	return (it - tab.begin());
}

int					utl::searchVectorCharUntil(std::vector<char> tab, char const* to_find, size_t index) {

	std::vector<char>::iterator it = std::search(tab.begin(), tab.begin() + index, to_find, to_find + std::strlen(to_find));
	if (it == tab.begin() + index)
		return (-1);
	return (it - tab.begin());
}

std::vector<char>&	utl::replaceVectorChar(std::vector<char>& tab, size_t position, size_t length, std::string str) {

	tab.erase(tab.begin() + position, tab.begin() + position + length);
	tab.insert(tab.begin() + position, str.begin(), str.end());
	return (tab);
}

std::string			utl::trim(std::string const& str) {

    const std::string ws = " \n\r\t\f\v";
    
    size_t start = str.find_first_not_of(ws);
    if (start == std::string::npos)
        return ""; // no content except whitespace

    size_t end = str.find_last_not_of(ws);
    
    return str.substr(start, end-start+1);
}

int					utl::find_first_occurrence(const std::vector<char> & haystack, const std::string& needle) {

	std::string haystack_str(haystack.begin(), haystack.end());

	size_t pos = haystack_str.find(needle);

	if(pos == std::string::npos) {
		return -1;
	}

	return pos; 
}

int					utl::find_last_occurrence(const std::vector<char> & haystack, const std::string& needle) {

    std::string haystack_str(haystack.begin(), haystack.end());

    size_t pos = haystack_str.rfind(needle);

    if(pos == std::string::npos) {
        return -1;
    }

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

std::string					utl::vectorOfCharToStr (std::vector <char> const& buffer) {

	std::string	 str(buffer.begin(), buffer.end());
	return str;
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

	std::string	 str ("~~~Waiting for connexions~~~");
	std::stringstream sstr;

	sstr << std::endl << GREEN << str << RESET << "\n";
	return sstr.str();
}

bool	utl::isDirectory(std::string path) {

	struct stat		fileStat;

	if (stat(path.c_str(), &fileStat) != 0) {
		// error occured 
		return false;
	}

	return S_ISDIR(fileStat.st_mode);
}

bool	utl::createDirectory(std::string path) {

	std::string *createCmd = new std::string("mkdir \"" + path + "\"");
    const char* cmd = createCmd->c_str();
	delete(createCmd);

    int result = std::system(cmd);
	
	if (result != 0) {
        return false;
    }

    return true;

}

std::string utl::fileToStr(std::ifstream& file) {

	std::stringstream	content;
	std::string			line;
	
	while (std::getline(file, line) && !file.eof()) {
		content << line << std::endl;
	}
	file.close();
	return content.str();
}

std::string	utl::printCharArray(char** charArray, int size) {

	std::stringstream	sstream;

    for (int i = 0; i < size; ++i) {
        sstream << charArray[i] << std::endl;
    }
	
	return sstream.str();
}

void	utl::deleteCharArray(char **arr) {

	for (int i = 0; arr[i] != NULL; ++i)
		delete[] arr[i];
	delete arr;
}

bool	utl::createFile(std::string path, std::vector<char> content, std::string filename) {

	filename = path + filename;
	std::ofstream	out(filename.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
	out.clear();

	for (std::vector<char>::iterator it = content.begin(); it != content.end(); it++)
		out.put(*it);

	if (out.fail()) {
		out.close();
		return false;
	}
	out.close();
	return true;
}

std::string	utl::getDate() {

    std::time_t now = std::time(NULL);

    // Convert time to a string in the HTTP date format
    char buf[50];
    struct tm tm;
	setenv("TZ", "Europe/Paris", 1);
    localtime_r(&now, &tm); // Use GMT/UTC time
    std::strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", &tm);

    return std::string(buf);
}