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
			SDL_Texture *texture = nullptr;
			SDL_Rect clip;
			SDL_Rect collider;
			bool fixedPosition = false;

		public:
			RenderedGameObject(int x, int y, int w, int h) : 
				GameObject(),
				xpos(x),
				ypos(y),
				width(w),
				height(h),
				dead(false) {
					clip = { xpos, ypos, width, height };
					collider = { 0, 0, 0, 0 };
				}

			virtual ~RenderedGameObject() { };

			/* Own methods */
			void setClip(SDL_Rect&);
			void setDead(bool);
			void setCollidable(bool);
			void setColliderBox(SDL_Rect collider);
			bool isFixedPosition();
			void setFixedPosition(bool);

			/* Implemented override methods */
			virtual bool isDead() const;
			virtual void render(const RenderData*) const;
			virtual bool isCollider() const;
			virtual SDL_Rect getBoundingBox() const;
			virtual const SDL_Texture* getTexture() const;
			virtual void setTexture(SDL_Texture*);

			/* Empty methods */
			virtual void init(const GameData*, const RenderData*) { };
			virtual void preHandle(const GameData*) { };
			virtual void handle(const SDL_Event& event) { };
			virtual void postHandle(const GameData*) { };
			virtual void preRender(const RenderData*) { };
			virtual void postRender(const RenderData*) { };

	};
}

#endif
