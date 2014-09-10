#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include <SDL2/SDL.h>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

class GameObject
{
	private:
		boost::uuids::uuid id;

	public:
		GameObject() {

			/* Not providing ran creates valgrind warnings */
			static boost::mt19937 ran;
			id = boost::uuids::random_generator(ran)();
		};

		GameObject(const GameObject& o)	: id(o.id) { };

		virtual ~GameObject() { };

		virtual void preHandle() = 0;
		virtual void handle(const SDL_Event& event) = 0;
		virtual void postHandle() = 0;

		virtual void preRender() = 0;
		virtual void render(SDL_Surface& surface) const = 0;
		virtual void postRender() = 0;

		bool operator==(const GameObject& o) const {
			return id == o.id;
		};

		GameObject& operator=(const GameObject& o) {
			id = o.id;
			return *this;
		};

		std::string getStringId() const {
			return boost::lexical_cast<std::string>(id);
		};
};

#endif
