#include <unistd.h>
#include <netdb.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <arpa/inet.h>

enum class Color { red, green, blue, white, black};

class HttpConnection
{
	int clientSocket;
	static void assert_get(char* buffer) noexcept(false)
	{
		if(buffer[0] != 'G') throw 0;
		if(buffer[1] != 'E') throw 0;
		if(buffer[2] != 'T') throw 0;
		if(buffer[3] != ' ') throw 0;
		if(buffer[4] != '/') throw 0;
	}
	static bool cmp_str(const std::string& s, char* buffer)
	{
		for(int i=0; i<s.length(); i++)
		{
			if(buffer[i] != s[i]) return false;
		}
		return true;
	}
public:
	HttpConnection(int s):clientSocket(s)
	{
	
	}
	~HttpConnection()
	{
		close(clientSocket);
	}
	//1023
	int http_read_cstr(char* buffer, int size)
	{
		auto rd = read(clientSocket, buffer, size-1);
		buffer[rd] = '\0';
		return rd;
	}
	Color http_read_color()
	{
		static const std::string red("red.bmp");
		static const std::string green("green.bmp");
		static const std::string blue("blue.bmp");
		static const std::string white("white.bmp");
		char buffer[1024];
		auto rd = read(clientSocket, buffer, 1023);
		buffer[rd] = '\0';
		assert_get(buffer);
		
		if(cmp_str(red, buffer+5) )return Color::red;
		if(cmp_str(green, buffer+5) )return Color::green;
		if(cmp_str(blue, buffer+5) )return Color::blue;
		if(cmp_str(white, buffer+5) )return Color::white;
		
		return Color::black;
	}
	//10917
	int http_write(char* buffer, int size)
	{
		return send(clientSocket, buffer, size, 0);
	}
};

class HttpSocket
{
	int serverSocket;
	bool listen_error;
public:
	HttpSocket():serverSocket(socket(AF_INET, SOCK_STREAM, 0) ), listen_error(false)
	{
		sockaddr_in serverAddress;
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_port = htons(8080);
		//serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);//inet_addr("127.0.0.1");
		serverAddress.sin_addr.s_addr = inet_addr("192.168.100.9");
		
		bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress));
		int listening = listen(serverSocket, 10);
		listen_error = listening >= 0;
	}
	~HttpSocket()
	{
		close(serverSocket);
	}
	
	bool is_open()
	{
		return listen_error;
	}
	
	HttpConnection acept()
	{
		return HttpConnection(accept(serverSocket, NULL, NULL));
	}
};
