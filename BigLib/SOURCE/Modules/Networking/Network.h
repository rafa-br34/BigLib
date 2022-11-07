#pragma once
//#include <WinSock2.h>
//#include <Webhost.h>

#include <vector>
#include <string>

//#pragma comment(lib, "Ws2_32.lib")


#define NETWORK_DEFAULT_CCV __fastcall


namespace Network {
	enum Protocol {

	};


	typedef void(NETWORK_DEFAULT_CCV* MessageHandler)();

	class Connection {
	public:
		std::string ConnectedHost = "";
		


	};

	class Header {
	public:
		std::string Name = "";
		std::string Value = "";

		Header(const std::string& HeaderName, const std::string& HeaderValue) {
			this->Name = HeaderName;
			this->Value = HeaderValue;
		}
	};


	class RequestInfo {
	public:
		std::string Host = "";
	};

	std::string BasicRequest(const std::string& Host, const std::string& Payload);
}
