#pragma once
#include <vector>
#include <memory>
#include <random>
#include <iostream>
#include <ShlObj_core.h>
#include <functional>
#include "../../dependencies/utilities/render.hpp"
#include "../../dependencies/common_includes.hpp"
/*
	Sippin' On Purple Till Im Lazy
*/


class cMenu : public singleton<cMenu> {
public: 
	void menuRender( );

	bool open;
	Vector2 pos = Vector2(200, 200); // Set our menu starting pos. Used in menu repositioning.
	static Vector2 mousePos;

	Vector2 mouse_pos( ) { // Get out mousepos
		POINT mouse_position;
		GetCursorPos( &mouse_position );
		ScreenToClient( FindWindow ( 0, "Counter-Strike: Global Offensive" ), &mouse_position );
		return { static_cast< float >( mouse_position.x ), static_cast< float >( mouse_position.y ) };
	}
	bool mouse_in_params( int x, int y, int x2, int y2 ) { // Check if mouse is in specified area.
		if ( mouse_pos( ).x > x && mouse_pos( ).y > y && mouse_pos( ).x < x2 + x && mouse_pos( ).y < y2 + y )
			return true;
		return false;
	}
	int menu_alpha;

	void handleInput( Vector2 WindowSize ) {
		/*	
			Very basic and probably terrible window input handler.
			For now Dragging only works. And is really all we need.
		*/

		static bool Dragging = false; bool Click = false;
		static int	dragX = 300; static int dragY = 300;
		static bool _pressed = true;
		Vector2 mousePos = mouse_pos();

		if (!_pressed && GetAsyncKeyState(VK_INSERT))
			_pressed = true;
		else if (_pressed && !GetAsyncKeyState(VK_INSERT)) {
			_pressed = false;
			open = !open;
		}
		interfaces::inputsystem->enable_input(!open);

		if ( GetAsyncKeyState( VK_LBUTTON ) )
			Click = true;

		if (Dragging && !Click)
			Dragging = false;

		if (Dragging && Click) {
			pos.x = mousePos.x - dragX;
			pos.y = mousePos.y - dragY;
		}

		if ( mouse_in_params( pos.x, pos.y, WindowSize.x -200, WindowSize.y ) && !( mouse_in_params( pos.x, pos.y + 20, WindowSize.x - 10, WindowSize.y - 10 ) ) ) {
			Dragging = true;
			dragX = mousePos.x - pos.x;
			dragY = mousePos.y - pos.y;
		}
	}
};

class cBaseObject {
public:
	std::string name, caption;
	Vector2 position, size;
	bool visable = false;
	cBaseObject( ) { }
	cBaseObject( std::string name, std::string caption, Vector2 pos, Vector2 size ) {
		this->name = name;
		this->caption = caption;
		this->position = pos;
		this->size = size;
	}
	virtual void Draw( ) = 0;
};

class cWindow : public cBaseObject {
public:
	std::vector< cBaseObject* > Children;
	cWindow( std::string name, Vector2 pos, Vector2 size );
	void AddChildControl( cBaseObject* Child );
	virtual void Draw( ) override;
};

class cCategory : public cBaseObject
{
public:
	cCategory() {}
	std::vector<cBaseObject*> Children;
	cWindow* parent;
	static cCategory* CurrentCategory;
	bool active = false;
	cCategory(std::string name, Vector2 pos, cWindow* parent);
	void AddChildControl(cBaseObject* Child);
	virtual void Draw() override;
};


class cTab : public cBaseObject {
public:
	cTab( ) { }
	int index;
	cCategory* parent;
	static cTab* CurrentTab;
	bool active = false;
	std::vector< cBaseObject* > Children;
	cTab( std::string name, cCategory* parent, int index );
	void AddChildControl( cBaseObject* child );
	virtual void Draw( ) override;
	cTab( std::string name, cWindow* parent );
};

class cCheckbox : public cBaseObject {
public:
	cCheckbox( ) { }
	bool Var;
	cTab* parent;
	cCheckbox( cTab* parent, std::string name, bool* item, Vector2 pos );
	virtual void Draw( ) override;
};

class cSelector : public cBaseObject {
public:
	cSelector( ) { }
	int* Var;
	cTab* parent;
	std::string elements;
	cSelector( cTab* parent, std::string name, std::string caption, int* variable, Vector2 pos, std::string elements );
	virtual void Draw( ) override;
};

class cSlider : public cBaseObject {
	cSlider( ) { }
	float* Var;
	cTab* parent;
	float min, max;
	int width;
	cSlider( cTab* parent, std::string name, std::string caption, float* variable, float min, float max, int width, Vector2 pos );
	virtual void Draw( ) override;
};
