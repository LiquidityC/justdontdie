#ifndef _RENDERED_GAME_OBJECT_H
#define _RENDERED_GAME_OBJECT_H

#include "GameObject.h"

namespace flat2d
{
	class Camera;

	class RenderedGameObject : public GameObject
	{
		protected:
			int xpos, ypos, width, height;
			bool dead = false, collidable = false;
			SDL_Texture *texture = NULL;
			SDL_Rect clip;
			Camera* camera = NULL;

			void setCamera(Camera*);

		public:
			RenderedGameObject(int x, int y, int w, int h) : 
				GameObject(),
				xpos(x),
				ypos(y),
				width(w),
				height(h),
				dead(false) {
					texture = NULL;
					camera = NULL;
					clip = { xpos, ypos, width, height };
				}

			virtual ~RenderedGameObject() { };

			/* Own methods */
			void setClip(SDL_Rect&);
			void setDead(bool);
			void setCollidable(bool);

			/* Implemented override methods */
			virtual void init(SDL_Renderer*, Camera*);
			virtual bool isDead() const;
			virtual void render(SDL_Renderer* renderer) const;
			virtual bool isCollider() const;
			virtual SDL_Rect getBoundingBox() const;
			virtual const SDL_Texture* getTexture() const;
			virtual void setTexture(SDL_Texture*);

			/* Empty methods */
			virtual void preHandle() { };
			virtual void handle(const SDL_Event& event) { };
			virtual void postHandle() { };
			virtual void preRender() { };
			virtual void postRender() { };

	};
}

#endif
