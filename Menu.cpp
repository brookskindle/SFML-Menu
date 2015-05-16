#include "Menu.h"
using namespace sfm;

const MenuLayout MenuLayout::VerticleCentered	=	MenuLayout(50, sf::Vector2f(0.50, 0.5));
const MenuLayout MenuLayout::VerticleLeft		=	MenuLayout(50, sf::Vector2f(0.25, 0.5));
const MenuLayout MenuLayout::VerticleRight		=	MenuLayout(50, sf::Vector2f(0.75, 0.5));


////////////////////
//MenuLayout Methods
////////////////////
MenuLayout::MenuLayout(unsigned int spacing, const sf::Vector2f &origin) {
	setSpacing(spacing);
	setOrigin(origin);
}//end constructor


MenuLayout::MenuLayout(const MenuLayout &other) {
	this->spacing = other.spacing;
	this->origin = other.origin;
}//end copy constructor


void MenuLayout::setSpacing(unsigned int spacing) {
	this->spacing = spacing;
}//end setSpacing


void MenuLayout::setOrigin(const sf::Vector2f &origin) {
	this->origin = origin;
	if(this->origin.x > 1.0) {
		this->origin.x = 1.0;
	}
	if(this->origin.y > 1.0) {
		this->origin.y = 1.0;
	}
}//end setOrigin


void MenuLayout::apply(std::vector<MenuOption> &options,
		sf::Vector2u windowSize) {
	if(options.size() == 0) { return; } //no options, do nothing

	sf::Vector2f orig; //origin position in pixels, instead of percentages
	orig.x = windowSize.x * this->origin.x;
	orig.y = windowSize.y * this->origin.y;
	//since we have n number of options, calculate where the first one will go
	int n = options.size();
	auto osize = options[0].getText().getGlobalBounds(); //assume all options are the same height
	if(n % 2) { //odd number of options
		orig.y = orig.y - (int)(n / 2) * (osize.height + spacing);
	}
	else { //even number of options
		orig.y = orig.y - (spacing/2.0f) - (osize.height/2.0f) - (n/2 - 1)*(spacing+osize.height);
	}

	for(MenuOption &option : options) {
		sf::Text &text = option.getText();
		sf::FloatRect rect = text.getLocalBounds();
		//center text origin on the middle of the text
		text.setOrigin(rect.left + rect.width/2.0f,
				rect.top + rect.height/2.0f);
		text.setPosition(orig); //set option position on the screen
		orig.y += osize.height + spacing; //calculate next position
	}
}//end apply


//////////////
//Menu Methods
//////////////
void Menu::display(sf::RenderWindow &window) {
	sf::Sprite bg;
	bg.setTexture(background);
	for(auto &o : options) { //make all options use the same font
		o.getText().setFont(font);
	}

	window.clear();
	window.draw(bg); //draw background
	layout.apply(options, window.getSize()); //apply the layout on the options
	for(auto &o : options) { //draw the options
		window.draw(o.getText());
	}
	window.display();

	bool finished = false;
	sf::Vector2i mousePos;
	sf::Clock clock;
	while(!finished) {
		sf::Event e;
		while(window.pollEvent(e)) {
			switch(e.type) {
				case sf::Event::Closed:
					finished = true;
					if(exitCallback) { exitCallback(); }
					break;
				case sf::Event::Resized:
				case sf::Event::GainedFocus:
					window.clear();
					window.draw(bg); //draw background
					for(auto &o : options) {
						window.draw(o.getText());
					}
					window.display();
					break;
				case sf::Event::MouseButtonReleased:
					for(auto &o : options) {
						if(o.getText().getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) {
							//this option was clicked
							o.choose();
							finished = true;
							break;
						}
					}
					break;
				default:
					break;
			}
		}
		sf::sleep(sf::milliseconds(100));
		if(timeout > sf::seconds(0.0) && clock.getElapsedTime() >= timeout) {
			//this menu has a timeout and we have reached it
			finished = true;
			if(timeoutCallback) { timeoutCallback(); }
		}
	}
}//end display


void Menu::addOption(const MenuOption &option) {
	options.push_back(option);
}//end addOption


void Menu::clearOptions() {
	options.clear();
}//end clearOptions


void Menu::setBackground(const std::string &path) {
	background.loadFromFile(path);
}//end setBackground


void Menu::setFont(const std::string &path) {
	font.loadFromFile(path);
}//end setFont


void Menu::setTimeout(sf::Time timeout, std::function<void(void)> callback) {
	this->timeout = timeout;
	this->timeoutCallback = callback;
}//end setTimeout


void Menu::setExitCallback(std::function<void(void)> callback) {
	exitCallback = callback;
}//end setExitCallback


void Menu::setLayout(MenuLayout layout) {
	this->layout = layout;
}//end setLayout
