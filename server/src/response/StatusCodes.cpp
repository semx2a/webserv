#include "StatusCodes.hpp"

StatusCodes::StatusCodes() : AFile() {}

StatusCodes::StatusCodes(std::string const& filename) : AFile(filename) {}

StatusCodes::~StatusCodes() {}

std::string const& StatusCodes::getStatusMessage(std::string const& code) const { return this->getLexicon().find(code)->second; }