#pragma once
#include "../Sockets/Sockets.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <atomic>
#include <thread>
#include <stdio.h>

class ClientHandler
{
public:
	ClientHandler(SOCKET httpSocket) : _httpSocket(httpSocket) {};

	//--------------------------< Parse request to get fileName >----------------------------

	std::string parseRequestName(std::string& temp);
	//--------------------------< convert an interger size to a string >-----------------------

	std::string sizeToString(size_t size);
	//--------------------------------< readFile >-------------------------------------------

	std::string getFileString(std::string fileName);
	
	//--------------------------< parses resource >----------------------------

	std::string parseResource(std::string val);

	//<-----------------------------------does processing of recieve child thread---------------------------------->

	void operator()();

	
	
private:
	SOCKET _httpSocket;
	bool complete = false;
};

class WebServer
{
public:
	WebServer(int port) : _port(port), _pListener(new SocketListener(port)) {}
	~WebServer();
	void launch(std::string valu);
	void run();

private:
	SocketListener* _pListener;
	SOCKET httpSocket;
	bool acceptFailed_ = false;
	std::atomic<bool> stop_ = false;
	int _port;
};