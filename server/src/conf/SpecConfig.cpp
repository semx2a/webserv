#include "SpecConfig.hpp"

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

SpecConfig::SpecConfig() : CommonConfig(), _listenIpPort(), _serverNames() {

	this->_listenIpPort["127.0.0.1"] = 80;
	this->_serverNames.push_back("localhost");
}

SpecConfig::SpecConfig(SpecConfig const &rhs) : CommonConfig() {

	*this = rhs;
}

SpecConfig &	SpecConfig::operator=(SpecConfig const & rhs) {

	if (this != &rhs) {
		
		this->_listenIpPort = rhs.getListenIpPort();
		this->_serverNames = rhs.getServerNames();
	}
	return *this;
} 

SpecConfig::~SpecConfig(void) {}


// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

std::map<std::string, int> const &		SpecConfig::getListenIpPort(void) const { return this->_listenIpPort; }

std::vector<std::string> const &		SpecConfig::getServerNames(void) const { return this->_serverNames; }

void	SpecConfig::setListIpPort(std::string const& line) {
	
	std::stringstream	stream(line);
	std::string			listen;
	std::string			ip;
	int					port;

	std::map<std::string, int>::iterator it = this->_listenIpPort.begin();
	if (it->first == "127.0.0.1" && it->second == 80)
		this->_listenIpPort.erase(it);

	stream >> listen;
	if (stream.str().find(':') != std::string::npos)
	{
		std::getline(stream, ip, ':');
		ip = ip.substr(ip.find_first_not_of(" \t"), ip.size());
		if (!Parser::isValidIPv4(ip) && !Parser::isValidIPv6(ip))
			throw Parser::InvalidParam(ip, "listen", 0);
	}
	stream >> port;
	this->_listenIpPort[ip] = port;
}

void	SpecConfig::setServerName(std::string const& line) {

	std::stringstream	stream(line);
	std::string			tmp;
	std::string			serverName;

	std::vector<std::string>::iterator it = this->_serverNames.begin();
	if (*it == "localhost")
		this->_serverNames.erase(it);
	
	stream >> tmp >> serverName;
	this->_serverNames.push_back(serverName);
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: METHODS::




// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::: OPERATOR OVERLOAD::

std::ostream &	operator<<(std::ostream & o, SpecConfig const & sc) {

	o << HIPURPLE << "listenIpPort = " << HIGREEN << print_map(sc.getListenIpPort()) << NO_COLOR << std::endl;
	o << HIPURPLE << "serverNames = " << HIGREEN << print_vector(sc.getServerNames()) << NO_COLOR << std::endl;
	return o;
}