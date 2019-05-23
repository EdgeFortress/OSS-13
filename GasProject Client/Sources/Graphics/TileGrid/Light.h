#pragma once

#include <LightSystem.hpp>

#include <Shared/Types.hpp>
#include <Shared/ErrorHandling.h>

class Light {
public:
	Light(uf::vec2i size) :
		size(size)
	{
		// Create the LightSystem
		ls.create({ -1000.f, -1000.f, 2000.f, 2000.f }, size);
		ls.setAmbientColor(sf::Color::Black);
		

		try {
			EXPECT(pointLightTexture.loadFromFile("External/LTBL2/examples/pointLightTexture.png"));
			pointLightTexture.setSmooth(true);

			mlight = ls.createLightPointEmission();
			mlight->setOrigin(sf::Vector2f(pointLightTexture.getSize().x * 0.5f, pointLightTexture.getSize().y * 0.5f));
			mlight->setTexture(pointLightTexture);
			mlight->setScale(7.f, 7.f);
			mlight->setColor(sf::Color::White);

		} catch (const std::exception &)
		{ }
	}

	void Update(sf::RenderTarget *renderTarget) {
		if (mlight != nullptr)
		{
			//mlight->setPosition(renderTarget->mapPixelToCoords(sf::Mouse::getPosition(render), view));
			mlight->setPosition(uf::vec2f(renderTarget->getSize()) / 2);
		}

		ls.render(*renderTarget);
	}

private:
	uf::vec2i size;

	ltbl::LightSystem ls;

	sf::Texture pointLightTexture;

	ltbl::LightPointEmission* mlight;
};
