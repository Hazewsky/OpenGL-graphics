#pragma once
#include <Box2D\Box2D.h>
#include <glm\glm.hpp>
#include <Bengine/Vertex.h>
#include <Bengine/SpriteBatch.h>
#include <Bengine/GLTexture.h>
class Box
{
public:
	Box();
	~Box();

	void init(b2World* world,
		const glm::vec2 &position,
		const glm::vec2 &dimensions,
		Bengine::GLTexture texture,
		Bengine::Color color,
		bool fixedRotation = false,
		glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	void draw(Bengine::SpriteBatch& spriteBatch);

	b2Body* getBody() const { return m_body; }
	b2Fixture* getFixture() const { return m_fixture; }
	//немного быстрее
	const glm::vec2& getDimensions() const { return m_dimensions; }
	const Bengine::Color& getColor() const{ return m_color; }
private:
	glm::vec4 m_uvRect;
	b2Body* m_body = nullptr;

	b2Fixture* m_fixture = nullptr;
	Bengine::Color m_color;
	//раним дименсионы
	glm::vec2 m_dimensions = { 0.0f, 0.0f };

	Bengine::GLTexture m_texture;
};

