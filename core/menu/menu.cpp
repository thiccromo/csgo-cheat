#include "menu.hpp"
#include <sstream>
#include "configStuff/config.h"
cCategory* cCategory::CurrentCategory = nullptr;
cTab* cTab::CurrentTab = nullptr;
int testing;
bool test;
/*	-=-=-=-=-=-=-=-=-=- INFO FOR MENU FRAMEWORK -=-=-=-=-=-=-=-=-=-

	Menu framework credits:
	Tomo	- Myself for being myself and writing this all out
	Moveax	- Reference for my parent functions
	Patty	- Helping out throughout the entire development

	Writing down comments so that newer people can understand.
	LOTS of things can be changed or improved and I am hoping to do so.
	Currently I am braindead and I have no idea whats happening lol.
	Current controls and a example on how to initialize them.
	
	Window:		Call inside of your menu render function.
	Params:		cWindow("name", pos, size)
	Initialize:	cWindow* test_window = new cWindow( "name", { x, y }, { w, h } );  
				{ 
					//	other controls 
				} test_window->Draw(); 

	Tab:		Call inside of your window, remember to add tab(s) as a child for the window
				Parent should be a window. Your index should be the tab number. ex. tab1 = 1, tab2 = 2.
	Params:		cTab("name", parent, index)
	Initialize:	cTab* test_tab1 = new cTab( "tab1", test_window, 1 ); 
				{	test_window->AddChildControl(test_tab1); 
					//	items inside of tab 
				}
	-=-=-=-=-=-=-=-=-=- INFO FOR MENU FRAMEWORK -=-=-=-=-=-=-=-=-=-	*/

void cMenu::menuRender() {

	this->handleInput( { 500, 500 } );

	if ( !open ) 
		return;

	cWindow* test_window = new cWindow( "windowTest", { pos.x, pos.y }, { 500, 500 } ); 
	{ 
		cCategory* testCate1 = new cCategory( "testCate1", { pos.x, pos.y }, test_window );
		{ test_window->AddChildControl( testCate1 );

		}
		cCategory* testCate2 = new cCategory("testCate2", { pos.x + 50, pos.y + 100 }, test_window);
		{ test_window->AddChildControl( testCate2 );

		}
	}

	constexpr float fade_factor = 6.666667f;
	float fade_increment = (fade_factor * interfaces::globals->frame_time) * 255.0f;

	if (open) {
		menu_alpha += (int)fade_increment;
	}
	else {
		menu_alpha -= (int)fade_increment;
	}
	menu_alpha = max(0, min(255, menu_alpha));

	if (menu_alpha == 0)
		return;

	test_window->Draw( );
}

cWindow::cWindow(std::string name, Vector2 pos, Vector2 size) {
	this->name = name;
	this->position = pos;
	this->size = size;
	this->visable = true;
}

void cWindow::AddChildControl( cBaseObject* Child ) {
	Children.push_back( Child );
}

void cWindow::Draw() {
	if (cMenu::get().open)
		this->visable;
	else
		!this->visable;

	if (this->visable) {
		render::get().draw_filled_rect(this->position.x, this->position.y, this->size.x, this->size.y, color(44, 44, 44, cMenu::get().menu_alpha)); // Main window background
		render::get().draw_filled_rect(this->position.x + 4, this->position.y + 18, this->size.x - 8, this->size.y - 34, color(20, 19, 19, cMenu::get().menu_alpha)); // inner darker background
		render::get().draw_double_outline(this->position.x, this->position.y, this->size.x, this->size.y, color(13, 13, 13, cMenu::get().menu_alpha));
		render::get().draw_outline(this->position.x + 4, this->position.y + 18, this->size.x - 8, this->size.y - 34, color(54, 54, 54, cMenu::get().menu_alpha));
		// Render all of the window text
		render::get().draw_text(this->position.x + this->size.x / 8 - 28, this->position.y + 4, render::get().main_font, this->name.c_str(), true, color(255, 224, 224));
		render::get().draw_text(this->position.x + this->size.x - 36, this->position.y + this->size.y - 15, render::get().main_font, "user: serrated", true, color(255, 224, 224)); // add actual check later
		render::get().draw_text(this->position.x + this->size.x / 8 - 16, this->position.y + this->size.y - 15, render::get().main_font, "build: " __DATE__, true, color(255, 224, 224));

		for (int i = 0; i < Children.size(); i++)
			Children[i]->Draw();
	}
}

cCategory::cCategory(std::string name, Vector2 pos, cWindow* parent)
{
	this->name = name;
	this->position = pos;
	this->parent = parent;
}

void cCategory::AddChildControl(cBaseObject* Child)
{
	Children.push_back(Child);
}

void cCategory::Draw()
{
	if (GetAsyncKeyState(VK_LBUTTON) && cMenu::get().mouse_in_params(this->position.x, this->position.y, 100, 20))
	{
		cCategory::CurrentCategory->active = false;
		this->active = true;
		cCategory::CurrentCategory = this;
	}

	if (this->active)
	{
		render::get().draw_filled_rect(this->position.x + 1, this->position.y - 3, 125, 25, color(1, 87, 155));
		render::get().draw_text(this->position.x + this->parent->size.x / 8, this->position.y + 1, render::get().main_font, this->name.c_str(), true, color(255, 255, 255));
		for (int i = 0; i < Children.size(); i++) Children[i]->Draw();
	}
	else
	{
		render::get().draw_text(this->position.x + this->parent->size.x / 8, this->position.y + 1, render::get().main_font, this->name.c_str(), true, color(158, 158, 158));
	}
}

cTab::cTab(std::string name, cCategory* parent, int index) {
	this->name = name;
	this->parent = parent;
	this->index = index;
}

void cTab::AddChildControl(cBaseObject* Child) {
	Children.push_back(Child);
}

void cTab::Draw() {

	std::wstring c = std::wstring(name.begin(), name.end());
	int textW, textH;
	interfaces::surface->get_text_size(render::get().main_font, c.c_str(), textW, textH);

	Vector2 TabPos = Vector2(this->parent->position.x + 80 + (50 * this->index), this->parent->position.y + 12);

	if (GetAsyncKeyState(VK_LBUTTON) && cMenu::get().mouse_in_params(TabPos.x - 6, TabPos.y, textW + 12, textH)) {
		cTab::CurrentTab->active = false;
		this->active = true;
		cTab::CurrentTab = this;
	}
	if (this->active) {
		render::get().draw_text(TabPos.x, TabPos.y, render::get().main_font, this->name.c_str(), false, color(1, 87, 155));
		for (int i = 0; i < Children.size(); i++) Children[i]->Draw();
	}
	else
	{
		render::get().draw_text(TabPos.x, TabPos.y, render::get().main_font, this->name.c_str(), false, color(158, 158, 158));
	}

	render::get().draw_outline(TabPos.x - 6, TabPos.y, textW + 12, textH, color(255, 255, 255));
}

cCheckbox::cCheckbox(cTab* parent, std::string name, bool* item, Vector2 pos) {
	this->parent = parent;
	this->name = name;
	this->Var = *item;
	this->position = pos;
}

void cCheckbox::Draw() {
	static bool pressed = false;

	if (!GetAsyncKeyState(VK_LBUTTON) && cMenu::get().mouse_in_params(this->position.x, this->position.y, 15, 15)) {
		if (pressed)
			 Var = !Var;
		pressed = false;
	}

	if (GetAsyncKeyState(VK_LBUTTON) && cMenu::get().mouse_in_params(this->position.x, this->position.y, 15, 15) && !pressed)
		pressed = true;

	std::wstring c = std::wstring(caption.begin(), caption.end());									
	int textW, textH;
	interfaces::surface->get_text_size(render::get().main_font, c.c_str(), textW, textH);

	if (Var == true)
		render::get().draw_filled_rect(this->position.x + 2, this->position.y + 2, 11, 11, color(1, 87, 155));

	render::get().draw_outline(this->position.x, this->position.y, 15, 15, color(158, 158, 158));
	render::get().draw_text(this->position.x + 20, this->position.y + 1, render::get().main_font, this->name.c_str(), false, color(255, 255, 255));
}

/*
cSelector::cSelector(cTab* parent, std::string name, std::string caption, int* variable, Vector2 pos, std::string elements) {
	this->parent = parent;
	this->name = name;
	this->caption = caption;
	this->Var = variable;
	this->position = pos;
	this->elements = elements;
}

void cSelector::Draw() {
	if (Clicked(Vector2(this->position.x, this->position.y + 18), Vector2(10, 10))) {
		if (*this->Var > 0)* this->Var -= 1;
	}
	if (Clicked(Vector2(this->position.x + 150, this->position.y + 18), Vector2(10, 10))) {
		if (*this->Var >= 0 && *this->Var < this->elements.size() - 1 )* this->Var += 1;				
	}

	render::get().draw_text(this->position.x, this->position.y, render::get().main_font, this->name.c_str(), false, color(255, 255, 255));
	render::get().draw_text(this->position.x, this->position.y + 18, render::get().main_font, "<", false, color(255, 255, 255));

	render::get().draw_outline(this->position.x + 15, this->position.y + 18, 125, 16, color(158, 158, 158));
	render::get().draw_text(this->position.x + 15 + (125 / 2), this->position.y + 18, render::get().main_font, this->elements[*this->Var], true, color(255, 255, 255));
	render::get().draw_text(this->position.x + 150, this->position.y + 18, render::get().main_font, ">", false, color(255, 255, 255));

	if (InBounds(Vector2(this->position.x + 15, this->position.y), Vector2(125, 32)))
		render::get().draw_text(this->parent->parent->parent->position.x + 130, this->parent->parent->parent->position.y + this->parent->parent->parent->size.y - 20, render::get().main_font, this->caption.c_str(), false, color(255, 255, 255));
}*/
