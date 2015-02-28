#ifndef _RENDERED_GAME_OBJECT_H
#define _RENDERED_GAME_OBJECT_H

#include "GameObject.h"
#include "LocationProperty.h"

namespace flat2d
{
	class Camera;

	class RenderedGameObject : public GameObject
	{
		protected:
			LocationProperty locationProperty;
			bool dead = false, collidable = false;
			SDL_Texture *texture = nullptr;
			SDL_Rect clip;
			SDL_Rect collider;
			bool fixedPosition = false;

		public:
			RenderedGameObject(int x, int y, int w, int h) : 
				GameObject(),
				locationProperty(x, y, w, h),
				dead(false) {
					clip = { x, y, w, h };
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
			virtual LocationProperty& getLocationProperty();
			virtual const LocationProperty& getLocationProperty() const;

			/* Empty methods */
			virtual void init(const GameData*) { };
			virtual void preHandle(const GameData*) { };
			virtual void handle(const SDL_Event& event) { };
			virtual void postHandle(const GameData*) { };
			virtual void preRender(const GameData*) { };
			virtual void postRender(const GameData*) { };

	};
}

#endif
