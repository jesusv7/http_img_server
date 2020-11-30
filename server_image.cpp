#include <iostream>
#include <algorithm>
#include <memory>
#include "bmp/bmp.hpp"
#include "socket/socket.hpp"

int main()
{
	std::string http_header("HTTP/1.0 200 Ok\nContent-Type: image/bmp\nContent-Length: 10854\n\n");
	BMP::ImageHeader img_header{};
	auto color_buffer = new char[10917]{};//http + bmp + rgb_matrix
	
	std::copy(http_header.c_str(), http_header.c_str()+http_header.length(),
		color_buffer);
	
	std::copy(img_header.get_data(), img_header.get_data()+img_header.ofset,
		color_buffer+http_header.length());
	
	HttpSocket http_socket;
	if(!http_socket.is_open()) return 1;
	std::cout << "Running...\n";
	for(;;)
	{
		HttpConnection http_con = http_socket.acept();
		
		http_con.http_write(color_buffer, 10917);
	}
	delete[] color_buffer;
	return 0;
}

/**************************************************************
		auto read_color = http_con.http_read_color();
		auto get_color = [](Color color)
		{
			switch(color)
			{
				case Color::red:
				std::cout << "red\n";
				return BMP::RGB{0, 0, 255};
				break;
				case Color::green:
				std::cout << "green\n";
				return BMP::RGB{0, 255, 0};
				break;
				case Color::blue:
				std::cout << "blue\n";
				return BMP::RGB{255, 0, 0};
				break;
				case Color::white:
				std::cout << "white\n";
				return BMP::RGB{255, 255, 255};
				break;
			}
			std::cout << "black\n";
			return BMP::RGB{0, 0, 0};
		};
		BMP::RGB rgb = get_color(read_color);
		BMP::fill(color_buffer+http_header.length()+
			img_header.ofset,
			img_header.width*img_header.height, rgb);

****************************************************************/
