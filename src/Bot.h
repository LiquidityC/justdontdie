#ifndef _BOT_H
#define _BOT_H

#include <flat/flat.h>
#include "GameObjectType.h"

class MapTileObject;
class Rocket;

class Bot : public flat2d::RenderedGameObject
{

	typedef struct {
		int x, y;
	} Clip;

	private:
		bool grounded = false;

		float xvel = 0;
		float yvel = 0;

		bool killed = false;
		flat2d::Timer deathTimer;

		int checkPointX, checkPointY;

		enum ClipIndex { RIGHT,
			LEFT,
			GHOST_RIGHT,
			GHOST_LEFT,
			WALK_RIGHT_1,
			WALK_RIGHT_2,
			WALK_LEFT_1,
			WALK_LEFT_2
		};

		ClipIndex currentClip = RIGHT;
		SDL_Rect clips[8] = {
			{ 	0,	 0, 37, 48 	},
			{ 	37,	 0, 37, 48 	},
			{ 	0,	 48, 37, 48	},
			{ 	37,	 48, 37, 48	},
			{ 	74,	 0, 37, 48	},
			{ 	74,	 48, 37, 48	},
			{ 	111, 0, 37, 48	},
			{ 	111, 48, 37, 48	}
		};

		SDL_Texture* botTexture = NULL;

		void calculateCurrentClip();

		bool handleCollision(flat2d::GameObject*, const flat2d::RenderData*);
		bool handleTileCollision(MapTileObject*, const flat2d::RenderData*);
		bool handleRocketCollision(Rocket*, const flat2d::RenderData*);

		void wasKilled();
		void restoreAtCheckpoint();

	public:

		Bot(unsigned int x, unsigned int y) : RenderedGameObject(x, y, 37, 48), checkPointX(x), checkPointY(y) { };

		~Bot() {
			if (texture != NULL) {
				SDL_DestroyTexture(botTexture);
				botTexture = NULL;
			}
		};

		int getType() {
			return GameObjectType::BOT;
		};

		void init(const flat2d::RenderData*);

		void handle(const SDL_Event& event);
		void postHandle();

		void preRender(const flat2d::RenderData*);
		void render(const flat2d::RenderData*) const;
};

#endif
