#ifndef BUFFER_HPP
# define BUFFER_HPP

# define DEBUG_BUFFER

# define DB_CRLF "\r\n\r\n"

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

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

		std::vector<char> const&	raw() const;
		std::string const&			str() const;
		size_t						maxBodySize() const;
		bool						hasBody() const;
		bool						hasContentLength() const;
		size_t						contentLength() const;
		t_ull						remainingContentLength() const;
		size_t						headerSize() const;
		bool						isTransferEncoding() const;
		bool						isRequestEnded() const;

		void						setRaw(std::vector<char> request);
		void						setStr(std::string requestStr);
		void						setMaxBodySize(size_t maxBodySize);
		void						setHasBody(bool hasBody);
		void						setHasContentLength(bool hasContentLength);
		void						setRemainingContentLength(t_ull remainingContentLength);
		void						setHeaderSize(size_t headerSize);
		void						setIsTransferEncoding(bool isTransferEncoding);
		void						setIsEnded(bool isEnded);

		void						add(std::vector<char> raw);
		void						checkEnd();
		void						setContentLength(size_t contentLength);


	private:
		std::vector<char>		_raw;
		std::string				_str;
		size_t					_maxBodySize;
		bool					_hasBody;
		bool					_hasContentLength;
		t_ull					_contentLength;
		t_ull					_remainingContentLength;
		size_t					_headerSize;
		bool					_isTransferEncoding;
		bool					_isEnded;
		
		void				_searchContentLength();
		void				_searchTransferEncoding();
		void				_checkEndContentLength();
		void				_checkEndTransferEncoding();
};

#endif
