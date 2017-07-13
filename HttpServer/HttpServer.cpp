#include "HttpServer.h"

//--------------------------< Parse request to get fileName >----------------------------

std::string ClientHandler::parseRequestName(std::string& temp)
{
	std::string fileName, nameSize;
	size_t pos = temp.find("GET /", 0);
	if (pos < temp.size())
	{
		fileName = temp.substr(pos + 5);
		pos = fileName.find_first_of(" ", 0);
		fileName = fileName.substr(0, pos);
		fileName = fileName;
	}
	return fileName;
}
//--------------------------< convert an interger size to a string >-----------------------

std::string ClientHandler::sizeToString(size_t size)
{
	std::ostringstream out;
	out << size;
	return out.str();
}
//--------------------------------< readFile >-------------------------------------------

std::string ClientHandler::getFileString(std::string fileName)
{
	std::string line;
	std::ostringstream ss;
	std::ifstream myReadFile(fileName);
	if (myReadFile.is_open()) {
		while (std::getline(myReadFile, line))
		{
			ss << line << std::endl;
		}
		myReadFile.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}
	return ss.str();
}

//--------------------------< parses resource >----------------------------

std::string ClientHandler::parseResource(std::string val) {
	std::string res = val.substr(val.find_first_of('>') + 1);
	size_t pos = res.find_first_of('<');
	res = res.substr(0, pos);
	return res;
}

//<-----------------------------------does processing of recieve child thread---------------------------------->

void ClientHandler::operator()() {
	std::string nameSize;
	std::string fileName = "";
	std::string postString = "";
	std::string postVal = "";
	std::string request;

	Socket httpSock = Socket(_httpSocket);
	for (int i = 0; ; ++i) {
		if (httpSock.bytesLeft() < 1)
			break;
		std::string temp = httpSock.ReadLine().append("\n");
		if (i == 0) {
			fileName = parseRequestName(temp);
			nameSize = sizeToString(fileName.size());
		}
		postString.append(temp);
		if (temp.find(':') == std::string::npos && temp.find("HTTP") == std::string::npos && temp.length() > 2) {
			postVal += temp;
		}
		request.append(temp);
	}
	if (request.size() > 0) {
		std::cout << "\n\n  Received a request:";
		std::cout << "\n---------------------------------------------------------------\n";
		std::cout << request.c_str();
		std::cout << "\n---------------------------------------------------------------\n";
		std::cout << "  Message details:";
		std::cout << "\n---------------------------------------------------------------\n";
		if (postString.find("GET") != std::string::npos) {
			std::cout << "\n  fileName: " << fileName;
			std::cout << "\n  size: " << fileName.size() << std::endl;
		}
		else {
			std::cout << "\n  Message: " << postVal;
			std::cout << "  size: " << postVal.size() - 1 << std::endl;
		}
		std::string testString = "";
		std::string fname = fileName;
		if (fname.find_first_of('.') != std::string::npos) {
			testString = getFileString("../repo/" + fname);
		}
		else {
			testString = fname + postVal;
		}

		std::string response = "HTTP/1.0 200 OK\n";
		response += "Content-Type : text/html\n";
		response += "Access-Control-Allow-Origin : * \n";
		response += "Access-Control-Allow-Methods : GET, PUT, POST, DELETE \n";
		response += std::string("Content-Length : ") + sizeToString(testString.size()) + "\n\n";
		response += testString;

		std::cout << "\n\n  response message:";
		std::cout << "\n---------------------------------------------------------------\n";
		std::cout << response;
		httpSock.WriteLine(response);	
		std::cout << "\n---------------------------------------------------------------\n";
		std::cout << "\nListenThread thread terminating\n";
	}
	closesocket(httpSock);
	
}
//--------------------------< Web Server destructor >----------------------------

WebServer::~WebServer() { delete _pListener; }

//--------------------------< Web Server Processing >----------------------------

void WebServer::run()
{
	std::thread ListenThread(
		[&]()
	{
		while (httpSocket != INVALID_SOCKET) {
			if (stop_.load())
				break;

			if (httpSocket == INVALID_SOCKET) {
				continue;
			}
			httpSocket = _pListener->waitForConnect();
			std::cout << "\n                                                          ";
			ClientHandler process(httpSocket);
			process();
		}
	});
	ListenThread.detach();
}

//----------------------< handle launch event >------------------------------	
void WebServer::launch(std::string valu)
{
	std::cout << "\n  2) Launching User Interface: " << valu;
	std::cout << "\n";
	std::string htm = valu;
	std::string browser("start \"\" \"" + htm + "\"");
	std::system(browser.c_str());
}
//--------------------------< MAIN >----------------------------

int main()
{
	std::cout << "\n  Starting Web Server at Port : 8085";
	std::cout << "\n---------------------------------------------------------------\n";
	WebServer server(8085);
	server.run();
	std::cout << "\n  1) Starting Listener";
	server.launch("../GUI/cp-gui.htm");
	std::cin.get();
}
