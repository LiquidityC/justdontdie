#ifndef PARALLAXBACKGROUND_H_
#define	PARALLAXBACKGROUND_H_

#include <flat/flat.h>
#include <vector>
#include <string>

class ParallaxBackground : public flat2d::Entity
{
	private:
		std::vector<flat2d::Texture*> textures;

		void updateSize();

	public:
		ParallaxBackground() : Entity(0, 0, 100, 100) {
			entityProperties.setCollidable(false);
			setFixedPosition(true);
		}
		~ParallaxBackground() {
			for (size_t i = 0; i < textures.size(); i++) {
				delete textures[i];
			}
			textures.clear();
		}

		void addTexture(int x, int y, std::string source, flat2d::RenderData *renderData);

		void render(const flat2d::RenderData*) const;
};

#endif // PARALLAXBACKGROUND_H_
