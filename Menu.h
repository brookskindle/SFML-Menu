#ifndef MENU_H
#define MENU_H

#include <vector>
#include <string>
#include <functional>
#include <SFML/Graphics.hpp>

namespace sfm {

	class MenuOption {
		public:
			MenuOption(std::string text = "",
					std::function<void(void)> callback = nullptr)
				:text(text), callback(callback) { }
			void select() { if(callback) {callback();} }
			void setText(const std::string &text) { this->text = text; }
			const std::string &getText() const { return text; }
			void setCallback(std::function<void(void)> callback) {
				this->callback = callback;
			}
		private:
			std::string text;
			std::function<void(void)> callback;
	};//end MenuOption

	class MenuLayout {
		friend class Menu;
		public:
			MenuLayout(unsigned int spacing = 50,
					const sf::Vector2f &origin = sf::Vector2f(0.5, 0.5));
			MenuLayout(const MenuLayout &copy);
			void setSpacing(unsigned int spacing);
			void setOrigin(const sf::Vector2f &origin);
			static const MenuLayout VerticleCentered;
			static const MenuLayout VerticleLeft;
			static const MenuLayout VerticleRight;
		private:
			void apply(std::vector<sf::Text> &options,
					sf::Vector2u windowSize);
			unsigned int spacing; //spacing between each option 
			//where on the screen (percentage wise) the MenuOptions revolve
			//around
			sf::Vector2f origin;
	};//end MenuLayout

	class Menu {
		public:
			void display(sf::RenderWindow &window);
			void addOption(const MenuOption &option);
			void clearOptions();
			void setBackground(const std::string &path);
			void setTimeout(sf::Time timeout,
					std::function<void(void)> callback);
			void setExitCallback(std::function<void(void)> callback);
			void setLayout(MenuLayout style);
			void setTemplateText(const sf::Text &t);
		private:
			std::vector<MenuOption> options;
			sf::Texture background;
			sf::Time timeout;
			std::function<void(void)> timeoutCallback;
			std::function<void(void)> exitCallback;
			MenuLayout layout;
			sf::Text templateText;
	};//end Menu

}//end namespace

#endif//MENU_H
