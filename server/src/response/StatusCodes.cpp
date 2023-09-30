#include "StatusCodes.hpp"

StatusCodes::StatusCodes() : AFile() {}

StatusCodes::StatusCodes(std::string const& filename) : AFile(filename) {}

StatusCodes::~StatusCodes() {}

std::string const& StatusCodes::statusMessage(std::string const& code) const { return this->lexicon().find(code)->second; }