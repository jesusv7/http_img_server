#include <fstream>

namespace BMP
{
	struct RGB
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};
	void fill(char* buffer, int size, RGB rgb)
	{
		auto rgb_ptr = reinterpret_cast<RGB*>(buffer);
		std::fill(rgb_ptr, rgb_ptr+size, rgb);
	}
	struct ImageHeader
	{
		//char head[56]; sobran 2!!!
		//char head[54];
		int format{1296171008};// 'B' + 'M' 1296171008
		int file_size{10854};
		int reserved0{};
		int ofset{54};
		int header_size{40};
		int width{80};
		int height{45};
		int planes{1572865};
		int pixel_bits{};
		int compression{10800};
		int image_size{};

		int h_size{};
		int v_size{};
		int colors{};
		int important_color{};
		
		void set_size(int w, int h)
		{
			width = w;
			height = h;
			compression = w*h*3;
			file_size compression + ofset;
		}
		
		char* get_data() noexcept
		{
			auto ptr = reinterpret_cast<char*>(this);
			return ptr+2;
		}
		
		bool save_to_file(const std::string& file_name, char* colors_data)
		{
			auto bmp_header_ptr = get_data();
			std::ofstream file(file_name, std::ios::out | std::ios::binary);
			if(file.is_open())
			{
				file.write(bmp_header_ptr, ofset);
				file.write(colors_data, file_size-ofset);
				file.close();
				return true;
			}
			return false;
		}
		
		class CANT_OPEN_FILE{};
		static ImageHeader load_from_file(const std::string& filename)
		{
			ImageHeader bmp_header;
			auto bmp_header_ptr = bmp_header.get_data();
			std::ifstream file("purple.bmp", std::ios::in | std::ios::binary);
			if(file.is_open())
			{
				file.read(bmp_header_ptr, 54);
				file.close();
				return bmp_header;
			}
			throw CANT_OPEN_FILE{};
		}
	};
}

