#pragma once
#include <string>
#include "singleton.hpp"
#include "../interfaces/interfaces.hpp"

enum font_flags {
	fontflag_none,
	fontflag_italic = 0x001,
	fontflag_underline = 0x002,
	fontflag_strikeout = 0x004,
	fontflag_symbol = 0x008,
	fontflag_antialias = 0x010,
	fontflag_gaussianblur = 0x020,
	fontflag_rotary = 0x040,
	fontflag_dropshadow = 0x080,
	fontflag_additive = 0x100,
	fontflag_outline = 0x200,
	fontflag_custom = 0x400,
	fontflag_bitmap = 0x800,
};

class render : public singleton<render> {
public: // fonts
	DWORD main_font;

public: // basic renderer
	void setup_fonts( ) {
		static bool _o = false;
		if ( !_o ) {
			main_font = interfaces::surface->font_create( );

			interfaces::surface->set_font_glyph( main_font, "Tahoma", 12, 500, 0, 0, font_flags::fontflag_outline );			
			_o = true;
		}
	}
	void draw_line( int x1, int y1, int x2, int y2, color colour ) {
		interfaces::surface->set_drawing_color( colour.r, colour.g, colour.b, colour.a = 255 );
		interfaces::surface->draw_line( x1, y1, x2, y2 );
	}
	void draw_text( int x, int y, unsigned long font, const char* string, bool text_centered, color colour ) {
		va_list va_alist;
		char buf[ 1024 ];
		va_start( va_alist, string );
		_vsnprintf( buf, sizeof( buf ), string, va_alist );
		va_end( va_alist );
		wchar_t wbuf[ 1024 ];
		MultiByteToWideChar( CP_UTF8, 0, buf, 256, wbuf, 256 );

		int width, height;
		interfaces::surface->get_text_size( font, wbuf, width, height );

		interfaces::surface->set_text_color( colour.r, colour.g, colour.b, colour.a = 255 );
		interfaces::surface->draw_text_font( font );
		if ( text_centered )
			interfaces::surface->draw_text_pos( x - ( width / 2 ), y );
		else
			interfaces::surface->draw_text_pos( x, y );
		interfaces::surface->draw_render_text( wbuf, wcslen( wbuf ) );
	}
	void draw_text( int x, int y, unsigned long font, std::string string, bool text_centered, color colour ) {
		std::wstring text = std::wstring( string.begin( ), string.end( ) );
		const wchar_t* converted_text = text.c_str( );

		int width, height;
		interfaces::surface->get_text_size( font, converted_text, width, height );

		interfaces::surface->set_text_color( colour.r, colour.g, colour.b, colour.a = 255 );
		interfaces::surface->draw_text_font( font );
		if ( text_centered )
			interfaces::surface->draw_text_pos( x - ( width / 2 ), y );
		else
			interfaces::surface->draw_text_pos( x, y );
		interfaces::surface->draw_render_text( converted_text, wcslen( converted_text ) );
	}
	void draw_filled_rect( int x, int y, int w, int h, color colour ) {
		interfaces::surface->set_drawing_color( colour.r, colour.g, colour.b, colour.a = 255 );
		interfaces::surface->draw_filled_rectangle( x, y, w, h );
	}
	void draw_double_outline( int x, int y, int w, int h, color colour ) {
		interfaces::surface->set_drawing_color( colour.r, colour.g, colour.b, colour.a = 255 );
		interfaces::surface->draw_outlined_rect( x + 1, y + 1, w - 2, h - 2 );
		interfaces::surface->draw_outlined_rect( x - 1, y - 1, w + 2, h + 2 );
	}
	void draw_outline(int x, int y, int w, int h, color colour) {
		interfaces::surface->set_drawing_color(colour.r, colour.g, colour.b, colour.a = 255);
		interfaces::surface->draw_outlined_rect(x, y , w , h );
	}
	void get_text_size( unsigned long font, const char* string, int w, int h ) {
		va_list va_alist;
		char buf[ 1024 ];
		va_start( va_alist, string );
		_vsnprintf( buf, sizeof( buf ), string, va_alist );
		va_end( va_alist );
		wchar_t out[ 1024 ];
		MultiByteToWideChar( CP_UTF8, 0, buf, 256, out, 256 );

		interfaces::surface->get_text_size( font, out, w, h );
	}
	void get_text_size( unsigned long font, std::string string, int w, int h ) {
		std::wstring text = std::wstring( string.begin( ), string.end( ) );
		const wchar_t* out = text.c_str( );

		interfaces::surface->get_text_size( font, out, w, h );
	}
	Vector2 get_screen_size(Vector2 area)
	{
		static int old_w, old_h;
		interfaces::engine->get_screen_size((int&)area.x, (int&) area.y);

		if ((int&) area.x != old_w || (int&) area.y != old_h)
		{
			old_w = (int&) area.x;
			old_h = (int&) area.y;
			return Vector2(old_w, old_h);
		}
		return area;
	}

	//--------- new ---------\\

	RECT GetTextSize2(DWORD font, const char* text, ...)
	{
		size_t origsize = strlen(text) + 1;
		const size_t newsize = 100;
		size_t convertedChars = 0;
		wchar_t wcstring[newsize];
		mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

		RECT rect; int x, y;
		interfaces::surface->get_text_size(font, wcstring, x, y);
		rect.left = x; rect.bottom = y;
		rect.right = x;
		return rect;
	}
	RECT GetTextSize3(DWORD font, std::string string, ...)
	{
		std::wstring text = std::wstring(string.begin(), string.end());
		const wchar_t* out = text.c_str();
		
		RECT rect; int x, y;
		interfaces::surface->get_text_size(font, out, x, y);
		rect.left = x; rect.bottom = y;
		rect.right = x;
		return rect;
	}

	void color_grad(int x, int y, int w, int h) {
		for (int i = 0; i < w; i++) {
			int red, green, blue;
			int div = w / 6;
			int phase = i / div;
			float t = (i % div) / (float)div;

			switch (phase)
			{
			case(0):
				red = 255;
				green = 255 * t;
				blue = 0;
				break;
			case(1):
				red = 255 * (1.f - t);
				green = 255;
				blue = 0;
				break;
			case(2):
				red = 0;
				green = 255;
				blue = 255 * t;
				break;
			case(3):
				red = 0;
				green = 255 * (1.f - t);
				blue = 255;
				break;
			case(4):
				red = 255 * t;
				green = 0;
				blue = 255;
				break;
			case(5):
				red = 255;
				green = 0;
				blue = 255 * (1.f - t);
				break;
			}

			for (int k = 0; k < h; k++)
			{
				float sat = k / (float)h;
				int new_red = red + sat * (128 - red);  int new_green = green + sat * (128 - green);  int new_blue = blue + sat * (128 - blue);
				interfaces::surface->set_drawing_color(new_red, new_green, new_blue, 255);
				interfaces::surface->draw_filled_rectangle(x + 10 + i, y + 10 + k, 1, 1);
			}
		}
	}
	void gradient(int x, int y, int w, int h, color c1, color c2) {
		render::get().draw_filled_rect(x, y, w, h, c1);

		BYTE first = c2.r;
		BYTE second = c2.g;
		BYTE third = c2.b;
		int a;
		if (GetKeyState(VK_INSERT) & 1)
			a = 255; else a = 0;

		for (int i = 0; i < h; i++)
		{
			float fi = i, fh = h;
			float a = fi / fh;
			DWORD ia = a * a;
			render::get().draw_filled_rect(x, y + i, w, 1, color(first, second, third, ia));
		}
	}

	void draw_filled_outlined_rect(int x, int y, int w, int h, color colour, color out_colour) {
		interfaces::surface->set_drawing_color(out_colour.r, out_colour.g, out_colour.b, out_colour.a);
		interfaces::surface->draw_filled_rectangle(x - 1, y - 1, w + 2, h + 2);
		interfaces::surface->set_drawing_color(colour.r, colour.g, colour.b, colour.a);
		interfaces::surface->draw_filled_rectangle(x, y, w, h);
	}
	void draw_filled_double_outlined_rect(int x, int y, int w, int h, color colour, color out_colour, color do_out_colour) {
		interfaces::surface->set_drawing_color(out_colour.r, out_colour.g, out_colour.b, out_colour.a);
		interfaces::surface->draw_filled_rectangle(x - 2, y - 2, w + 3, h + 3);
		interfaces::surface->set_drawing_color(out_colour.r, out_colour.g, out_colour.b, out_colour.a);
		interfaces::surface->draw_filled_rectangle(x - 1, y - 1, w + 2, h + 2);
		interfaces::surface->set_drawing_color(colour.r, colour.g, colour.b, colour.a);
		interfaces::surface->draw_filled_rectangle(x, y, w, h);
	}

	void gradient_verticle(int x, int y, int w, int h, color c1, color c2)
	{
		render::get().draw_filled_rect(x, y, w, h, c1);
		BYTE first = c2.r;
		BYTE second = c2.g;
		BYTE third = c2.b;
		BYTE alpha = c2.a;
		for (int i = 0; i < h; i++)
		{
			float fi = i, fh = h;
			float a = fi / fh;
			DWORD ia = a * 255;
			if (c1.a < 255)
				ia = c1.a;
			render::get().draw_filled_rect(x, y + i, w, 1, color(first, second, third, ia));
		}
	}
	void gradient_horz(int x, int y, int w, int h, color c1, color c2)
	{
		render::get().draw_filled_rect(x, y, w, h, c1);
		BYTE first = c2.r;
		BYTE second = c2.g;
		BYTE third = c2.b;
		BYTE alpha = c2.a;
		for (int i = 0; i < w; i++)
		{
			float fi = i, fh = w;
			float alpha = fi / fh;
			DWORD ia = alpha * 255;
			if (c1.a < 255)
				ia = c1.a;
			render::get().draw_filled_rect(x + i, y, 1, h, color(first, second, third, ia));
		}
	}
};