#include "Parser.hpp"

template <typename Context>
void	Parser::parseServerContext(std::stringstream& stream, Context& context) {

	typename Parser::DirectiveMap<ServerContext>::type directiveMap = initializeServerDirectives();

	std::string		line;
	while (std::getline(stream, line)) {

		this->_linesRead++;
		line = utl::trimAndReplaceWs(line);
		#ifdef DEBUG_PARSER
			std::cout << "[server scope] line " << _linesRead << ": " << line << std::endl;
		#endif
		if (isCommentOrEmptyLine(line)) {
			continue;
		}
		else if (isEndOfScope(line)) {
			break;
		}
		if (line.find("location") != std::string::npos) {

			LocationContext	newLocationCtxt;
			std::string path = line.substr(line.find_first_of(" ") + 1, line.find_first_of("{") - line.find_first_of(" ") - 1);
			path = utl::trimAndReplaceWs(path);
			newLocationCtxt.setName(path);
			parseLocationContext(stream, newLocationCtxt);
			context.addLocation(path, newLocationCtxt);
		} 
		else if (this->isDirective(line)) {

			line = line.substr(0, line.find_first_of(";"));

			std::string directive = line.substr(0, line.find_first_of(" "));
			typename Parser::DirectiveMap<Context>::type::iterator it = directiveMap.find(directive);
			if (it != directiveMap.end()) {
				(this->*(it->second))(line, context);
			} 
			else {
				this->buildAndThrowParamError(line);
			}
		}
		else {
			buildAndThrowParamError(line);
		}
	}
	context.setDefaults();
}

template <typename Context>
void	Parser::parseLocationContext(std::stringstream& stream, Context& context) {

	typename Parser::DirectiveMap<LocationContext>::type directiveMap = initializeLocationDirectives();

	std::string		line;
	while (std::getline(stream, line)) {

		this->_linesRead++;
		line = utl::trimAndReplaceWs(line);
		#ifdef DEBUG_PARSER
		std::cout << "[Location scope] line " << _linesRead << ": " << line << std::endl;
		#endif
		if (isCommentOrEmptyLine(line)) {
			continue;
		}
		else if (isEndOfScope(line)) {
			break;
		}
		else if (this->isDirective(line)) {

			line = line.substr(0, line.find_first_of(";"));

			std::string directive = line.substr(0, line.find_first_of(" "));
			typename Parser::DirectiveMap<Context>::type::iterator it = directiveMap.find(directive);
			if (it != directiveMap.end()) {
				(this->*(it->second))(line, context);
			} 
			else {
				this->buildAndThrowParamError(line);
			}
		}
		else {
			buildAndThrowParamError(line);
		}
	}
	if (not context.root().empty() and not context.alias().empty())
		throw std::runtime_error("Alias and root cannot be set at the same time");
}

template <typename Context>
void	Parser::parseAutoindex(std::string const &line, Context& context) {

	std::stringstream	stream(line);
	std::string			directive;
	std::string			onOff;

	stream >> directive >> onOff;
	if (onOff != "on" && onOff != "off")
		buildAndThrowParamError(line);

	context.setAutoindex(onOff);
}

template <typename Context>
void	Parser::parseMaxBodySize(std::string const& line, Context& context) {

	std::stringstream	stream(line);
	std::string 		directive;
	std::string			sizeStr;

	if (!(stream >> directive >> sizeStr) || !stream.eof()) {
		buildAndThrowParamError(line);
	}

	size_t mPos = sizeStr.find_first_of("mM");
	if (sizeStr.find_first_not_of("0123456789") != mPos || sizeStr.find_first_not_of(" ", mPos + 1) != std::string::npos) {
		buildAndThrowParamError(line);
	}

	size_t size = std::atoll(sizeStr.substr(0, mPos).c_str());
	size *= 1e6;
	context.setMaxBodySize(size);
}

template <typename Context>
void	Parser::parseAlias(std::string const& line, Context& context) {
	
	std::stringstream	stream(line);
	std::string			directive;
	std::string			alias;

	stream >> directive >> alias;
	if (alias.find(';') != std::string::npos)
		alias = alias.substr(0, alias.find_first_of(";"));
	context.setAlias(alias);
}

template <typename Context>
void	Parser::parseRoot(std::string const& line, Context& context) {

	std::stringstream stream(line);
	std::string directive;
	std::string root;

	if (!(stream >> directive >> root) || !stream.eof()) {
		buildAndThrowParamError(line);
	}

	context.setRoot(root);
}

template <typename Context>
void	Parser::parseUploadFolder(std::string const& line, Context& context) {

	std::stringstream stream(line);
	std::string directive;
	std::string uploadFolder;

	if (!(stream >> directive >> uploadFolder) || !stream.eof()) {
		buildAndThrowParamError(line);
	}

	context.setUploadFolder(uploadFolder);
}

template <typename Context>
void	Parser::parseListen(std::string const& line, Context& context) {

	std::stringstream	stream(line);
	std::string			listen;
	std::string			ip;
	int					port(80);

	stream >> listen;
	if (stream.str().find(':') != std::string::npos)
	{
		std::getline(stream, ip, ':');
		ip = ip.substr(ip.find_first_not_of(" "), ip.size());
		this->isValidIPv4(ip);
	}
	else
		ip = "0.0.0.0";
	stream >> port;
	context.setListen(ip, port);
}

template <typename Context>
void	Parser::parseErrorPage(std::string const& line, Context& context) {

	std::stringstream	stream(line);
	std::string			directive;
	std::string			errorCode;
	std::string			errorPage;

	stream >> directive >> errorCode >> errorPage;
	if (errorCode.find_first_not_of("0123456789") != std::string::npos) {
		this->buildAndThrowParamError(line);
	}
	context.addErrorPage(std::atoi(errorCode.c_str()), errorPage);
}

template <typename Context>
void	Parser::parseIndex(std::string const &line, Context& context) {

    std::stringstream            stream(line);
    std::string                  index;
    std::vector<std::string>     indexVec;

    stream >> index;
    while (stream >> index) {

        if (index.find(';') != std::string::npos)
            index = index.substr(0, index.find_first_of(";"));
        indexVec.push_back(index);
    }
    if (indexVec.empty())
        indexVec.push_back("index.html");
    context.setIndex(indexVec);
}

template <typename Context>
void	Parser::parseAuthorizedMethods(const std::string& line, Context& context) {

	std::stringstream			stream(line);
	std::vector<std::string>	authorizedMethods;
	std::string					directive;
	std::string					method;

	stream >> directive;
	while (stream >> method) {
		authorizedMethods.push_back(method);
	}
	context.setAuthorizedMethods(authorizedMethods);
}

template <typename Context>
void Parser::parseServerName(std::string const& line, Context& context) {

	std::stringstream			stream(line);
	std::string					directive;
	std::string					serverName;
	std::vector<std::string> 	serverNames;

	stream >> directive;
	while (stream >> serverName) {
		if (serverName.find(';') != std::string::npos)
			serverName = serverName.substr(0, serverName.find_first_of(";"));
		serverNames.push_back(serverName);
	}
	context.setServerNames(serverNames);
}
