#ifndef BUFFER_HPP
# define BUFFER_HPP

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "macros.hpp"
#include "Request.hpp"
#include "ServerContext.hpp"
#include "HttpStatus.hpp"

typedef unsigned long long t_ull;

class Buffer {

	public:
		Buffer();
		Buffer(Buffer const &rhs);
		Buffer &operator=(Buffer const& rhs);
		~Buffer();

		std::vector<char> const&	raw(void) const;

		bool						hasBody(void) const;
		bool						isEnded(void) const;

		std::string const&			boundary(void) const;
		std::string const&			finalBoundary(void) const;
		size_t						contentLength(void) const;
		bool						transferEncoding(void) const;

		size_t						maxBodySize(void) const;
		size_t						headerSize(void) const;
		t_ull						remainingContentLength(void) const;

		void						setMaxBodySize(size_t maxBodySize);
		void						setRemainingContentLength(t_ull remainingContentLength);

		void						add(std::vector<char> raw);


		void	checkEnd(void);
		void	searchForHeaders(void);
		void	processBodyEndCheck(void);
		void	clear(void);
		void	resetFlagsAndValues(void);

	private:

		std::vector<char>	_raw;

		bool				_hasBody;
		bool				_isEnded;

		std::string			_boundary;
		std::string 		_finalBoundary;
		t_ull				_contentLength;
		bool				_transferEncoding;

		size_t				_maxBodySize;
		size_t				_headerSize;
		t_ull				_remainingContentLength;
		
		bool	_findContentLength(void);
		bool	_findTransferEncoding(void);
		bool	_findBoundary(void);
		void	_checkEndBoundary(void);
		void	_checkEndContentLength(void);
		void	_checkEndTransferEncoding(void);
};

#endif