#include "StatusCodes.hpp"

StatusCodes::StatusCodes() : AFile("../neoserv/status.codes") {}

StatusCodes::~StatusCodes() {}

std::string const&	StatusCodes::getReasonPhrase(std::string const& statusCode) const {

	std::map<std::string, std::string>::const_iterator it = this->lexicon().find(statusCode);

	if (it == this->lexicon().end())
		return this->lexicon().find("500")->second;
	return it->second;
}