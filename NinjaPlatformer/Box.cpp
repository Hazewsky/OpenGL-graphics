#include "Box.h"

//constructor
Box::Box()
{
}

//destructor
Box::~Box()
{
}
//init function
void Box::init(b2World* world,
	const glm::vec2 &position,
	const glm::vec2 &dimensions,
	Bengine::GLTexture texture,
	Bengine::Color color,
	bool fixedRotation,
	glm::vec4 uvRect /*= glm::vec4(0.0f,0.0f,1.0f,1.0f)*/){
	//ƒќЋ∆Ќќ ќЅЌќ¬Ћя“№—я ј¬“ќћј“»„≈— »
	m_dimensions = dimensions;
	m_color = color;
	m_texture = texture;
	m_uvRect = uvRect;
	//ѕилим тело
	b2BodyDef bodyDef;
	//динамическа€ дефиници€(норма статик)
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	//это чтобы не вращалось
	bodyDef.fixedRotation = fixedRotation;
	//создаем тело
	m_body = world->CreateBody(&bodyDef);
//fixture
	//создание коробочки
	b2PolygonShape boxShape;
	boxShape.SetAsBox(dimensions.x / 2.0f, dimensions.y / 2.0f);
	//создаение fixture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	//не может быть 0 дл€ динимаческих объектов
	fixtureDef.density = 1.0f;
	//такое значение в диз доках, трение
	fixtureDef.friction = 0.3f;
	//храним
	m_fixture = m_body->CreateFixture(&fixtureDef);
}

void Box::draw(Bengine::SpriteBatch& spriteBatch){
	glm::vec4 destRect;
	//x,y,width,height
	//отнимаем ибо позици€ в центре, а не в углу
	destRect.x = m_body->GetPosition().x - m_dimensions.x / 2.0f;
	destRect.y = m_body->GetPosition().y - m_dimensions.y / 2.0f;
	destRect.z = m_dimensions.x;
	destRect.w = m_dimensions.y;
	//т.к. одна текстура можем юзать такой glm::vec2
	//крайнее rotation
	spriteBatch.draw(destRect, m_uvRect, m_texture.id, 0.0f, m_color, m_body->GetAngle());

}