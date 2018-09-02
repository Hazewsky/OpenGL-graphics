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
	//������ ����������� �������������
	m_dimensions = dimensions;
	m_color = color;
	m_texture = texture;
	m_uvRect = uvRect;
	//����� ����
	b2BodyDef bodyDef;
	//������������ ���������(����� ������)
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	//��� ����� �� ���������
	bodyDef.fixedRotation = fixedRotation;
	//������� ����
	m_body = world->CreateBody(&bodyDef);
//fixture
	//�������� ���������
	b2PolygonShape boxShape;
	boxShape.SetAsBox(dimensions.x / 2.0f, dimensions.y / 2.0f);
	//��������� fixture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	//�� ����� ���� 0 ��� ������������ ��������
	fixtureDef.density = 1.0f;
	//����� �������� � ��� �����, ������
	fixtureDef.friction = 0.3f;
	//������
	m_fixture = m_body->CreateFixture(&fixtureDef);
}

void Box::draw(Bengine::SpriteBatch& spriteBatch){
	glm::vec4 destRect;
	//x,y,width,height
	//�������� ��� ������� � ������, � �� � ����
	destRect.x = m_body->GetPosition().x - m_dimensions.x / 2.0f;
	destRect.y = m_body->GetPosition().y - m_dimensions.y / 2.0f;
	destRect.z = m_dimensions.x;
	destRect.w = m_dimensions.y;
	//�.�. ���� �������� ����� ����� ����� glm::vec2
	//������� rotation
	spriteBatch.draw(destRect, m_uvRect, m_texture.id, 0.0f, m_color, m_body->GetAngle());

}