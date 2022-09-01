#include <string>
#include <vector>

class HeaderGen{
private:
	std::vector<std::string> Info;

	// std::string _StatusLine;
	// std::string _StatusCode;
	// std::string _ServerVersion;
	// std::string _ContentType;
	// std::string _ContentLen;
	// std::string _Date;
	// std::string _Connection;
	// std::string _Body;

	std::string _response;
public:
	void setInfo(int infoType, std::string info);
	/*
	*	infoType = DEFINE 
	*		ex : STATUS = "HTTP/1.1 " ou SERV = "Server: "
	*	info = value du type
	*		ex: "400 Bad Request" ou "Apache/2.2.14 (Win32)"
	*/

	void processResponse(){
		/*
		* append dans _reponse avec "\n" apres info
		* quand Info[x] est le body faire "\n avant et aptre"
		*/

	}

	char *getStr(){ return _response.c_str(); }
};

/*EXEMPLE
HTTP/1.1 404 Not Found
Date: Sun, 18 Oct 2012 10:36:20 GMT
Server: Apache/2.2.14 (Win32)
Content-Length: 230
Connection: Closed
Content-Type: text/html; charset=iso-8859-1
*/