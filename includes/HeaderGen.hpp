#include <string>
#include <vector>

#define NB_INFOS 4

class HeaderGen{
private:
	enum idx_info{
		STATUS,
		TYPE,
		LENGTH,
		CONTENT
	};

	std::vector<std::string> _Info;

	std::string _response;
public:
	HeaderGen() : _response(""){
		_Info.assign(NB_INFOS, "");
	}

	void setStatus(std::string str){
		_Info[STATUS].append("HTTP/1.1 ");
		_Info[STATUS].append(str);
		_Info[STATUS].append("\n");
	}

	void setType(std::string str){
		_Info[TYPE].append("Content-Type: ");
		_Info[TYPE].append(str);
		_Info[TYPE].append("\n");
	}

	void setContentString(std::string str){
		_Info[LENGTH].append("Content-Length: ");
		_Info[LENGTH].append(std::to_string(str.length()));
		_Info[LENGTH].append("\n\n");
		_Info[CONTENT].append(str);
		_Info[CONTENT].append("\n");
	}
	
	void processResponse(){
		for (std::vector<std::string>::iterator it = _Info.begin(); it != _Info.end(); it++){
			if (it->size() > 0)
				_response.append(*it);
		}
	}

	std::string getStr(){ return _response; }
};