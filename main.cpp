#include <iostream>
#include "Menu.h"
using namespace std;

//function prototypes
void splashScreenCallback();
void menuButton1();
void menuButton2();

int main(void) {
	//create the splash screen
	sfm::Menu splash;
	splash.setTimeout(sf::seconds(2.2), splashScreenCallback);
	splash.setBackground("/home/brooks/code/sfml_hotseat/resources/textures/splashscreen.png");
	
	//create the main menu
	sfm::Menu mainmenu;
	sf::Text text;
	text.setString("Say hello");
	sfm::MenuOption opt(text, menuButton1);
	mainmenu.addOption(opt);
	text.setString("Say goodbye!");
	sfm::MenuOption opt2(text, menuButton2);
	mainmenu.addOption(opt2);
	text.setString("Call a lambda function");
	sfm::MenuOption opt3(text, [](){cout << "Lambda functions rock!\n";});
	mainmenu.addOption(opt3);
	mainmenu.setLayout(sfm::MenuLayout::VerticleCentered);
	mainmenu.setBackground("/home/brooks/Pictures/wallpaper/ironman.jpg");
	mainmenu.setFont("/home/brooks/code/sfml_hotseat/resources/fonts/neuropool/NEUROPOL.ttf");

	//now create the window and display the menus
	sf::RenderWindow window;
	window.create(sf::VideoMode(800, 600), "MENU SYSTEM!");
	splash.display(window);
	mainmenu.display(window);
	window.close();
	return 0;
}//end main

void splashScreenCallback() {
	cout << "This is the splash screen callback woot woot!\n";
}

void menuButton1() {
	cout << "HELLO HELLO HELLO! Welcome to sfm - SFML Menus\n";
}

void menuButton2() {
	cout << "Thank you for trying sfm -- SFML Menus. Goodbye!\n";
}
