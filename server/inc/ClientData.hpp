#ifndef CLIENT_DATA_HPP
# define CLIENT_DATA_HPP

# define DB_CRLF "\r\n\r\n"

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

typedef unsigned long long t_ull;

class ClientData {

	public:
		ClientData();
		ClientData(ClientData const &rhs);
		ClientData &operator=(ClientData const& rhs);
		~ClientData();

		bool						hasBody() const;
		bool						hasContentLength() const;
		size_t						contentLength() const;
		bool						isTransferEncoding() const;
		std::vector<char> const&	getRequest() const;
		bool						isRequestEnded() const;

		void						addToBuffer(std::vector<char> raw);
		void						setContentLength(size_t contentLength);


	private:
		std::vector<char>	_request;
		std::string			_requestStr;
		bool				_hasBody;
		bool				_hasContentLength;
		t_ull				_contentLength;
		bool				_isTransferEncoding;
		bool				_isEnded;
		
		void				_searchContentLength();
		void				_searchTransferEncoding();
		void				_checkEndContentLength();
		void				_checkEndTransferEncoding();
};

#endif
