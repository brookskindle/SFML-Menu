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


void MenuLayout::apply(std::vector<sf::Text> &options,
		sf::Vector2u windowSize) {
	if(options.size() == 0) { return; } //no options, do nothing

	sf::Vector2f orig; //origin position in pixels, instead of percentages
	orig.x = windowSize.x * this->origin.x;
	orig.y = windowSize.y * this->origin.y;
	//since we have n number of options, calculate where the first one will go
	int n = options.size();
	auto osize = options[0].getGlobalBounds(); //assume all options are the same height
	if(n % 2) { //odd number of options
		orig.y = orig.y - (int)(n / 2) * (osize.height + spacing);
	}
	else { //even number of options
		orig.y = orig.y - (spacing/2.0f) - (osize.height/2.0f) - (n/2 - 1)*(spacing+osize.height);
	}

	for(sf::Text &text : options) { //use reference to update the position
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
	bg.setScale(window.getSize().x / (float)background.getSize().x,
			window.getSize().y / (float)background.getSize().y);
	std::vector<sf::Text> texts;
	sf::Text t = this->templateText;
	for(const auto &o : options) {
		t.setString(o.getText());
		texts.push_back(t);
	}

	window.clear();
	window.draw(bg); //draw background
	layout.apply(texts, window.getSize()); //apply the layout on the options
	for(auto &t : texts) { //draw the options
		window.draw(t);
	}
	window.display();

	bool finished = false;
	int i = 0;
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
					for(auto &t : texts) {
						window.draw(t);
					}
					window.display();
					break;
				case sf::Event::MouseButtonReleased:
					for(i = 0; i < texts.size(); i++) {
						if(texts[i].getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) {
							//this option was clicked
							options[i].select();
							finished = true;
							break;
						}
					}
					break;
				default:
					break;
			}
		}//end while(window.pollEvent(e))
		sf::sleep(sf::milliseconds(100));
		if(timeout > sf::seconds(0.0) && clock.getElapsedTime() >= timeout) {
			//this menu has a timeout and we have reached it
			finished = true;
			if(timeoutCallback) { timeoutCallback(); }
		}
	}//end while(!finished)
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


void Menu::setTemplateText(const sf::Text &t) {
	templateText = t;
}//end setTemplateText
