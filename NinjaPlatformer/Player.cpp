#include "Player.h"
#include <Bengine/ResourceManager.h>
#include <SDL/SDL.h>
#include <iostream>
Player::Player()
{
}


Player::~Player()
{
}

void Player::init(b2World* world, const glm::vec2& position, const glm::vec2& dimensions, Bengine::Color color){
	Bengine::GLTexture texture = Bengine::ResourceManager::getTexture("Assets/blue_ninja.png");
	//collision box дл€ игрока
	m_collisionBox.init(world, position,dimensions,texture, color, true, glm::vec4(0.0f,1.0f,0.1f,0.5f)); ///< мы берем такой uvRect чтобы вз€ть первую картинку из спрайта. 0.1 ширина ибо их всего 10, 0.5 высота ибо 2 линии 
}

void Player::draw(Bengine::SpriteBatch& spriteBatch){
	m_collisionBox.draw(spriteBatch);
}

void Player::update(Bengine::InputManager& inputManager){
	b2Body* body = m_collisionBox.getBody();
	if (inputManager.isKeyDown(SDLK_a)){
		//примен€ем силу. в alpply нужен вектор, т.к. двигаемс€ влево, надо лева€ сила
		//wake - активирует объект(их физическое взаимодействие)
		body->ApplyForceToCenter(b2Vec2(-100.0f, 0.0),true);
	}
	else if (inputManager.isKeyDown(SDLK_d)){
		//ѕримен€етс€ каждый кадр, каждый раз
		body->ApplyForceToCenter(b2Vec2(100.0f, 0.0), true);
	}
	else{
		//демпинг - замедл€ем его скорость. каждый фрейм уменьшаетс€ в 0.9 раз
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x * 0.95, body->GetLinearVelocity().y));
	}
	//максимальна€ скорость
	float MAX_SPEED = 9.0f;
	if (body->GetLinearVelocity().x < -MAX_SPEED){
		body->SetLinearVelocity(b2Vec2(-MAX_SPEED, body->GetLinearVelocity().y));
	}
	else{
		if (body->GetLinearVelocity().x > MAX_SPEED){
			body->SetLinearVelocity(b2Vec2(MAX_SPEED, body->GetLinearVelocity().y));
		}
	}
	//прыжки. надо пролупить через все точки контакта, чтобы проверить, стоит ли он на земле
	//Edge - рань между 2 точками контакта
	for (b2ContactEdge* ce = body->GetContactList(); ce != nullptr; ce = ce->next){
		//точка соприкосновени€ у игрока
		b2Contact* c = ce->contact;
		//смотрим есть ли вообще контакт
		if (c->IsTouching()){
			//хранит все точки контакта этот класс
			b2WorldManifold manifold;
			//заполн€ем его инфой
			c->GetWorldManifold(&manifold);
			//проверка, что точки внизу(под ногами)
			bool below = false;
			for (int i = 0; i < b2_maxManifoldPoints /*2*/; i++){
				//отнимаем размеры. позици€ в центре, а нам надо проверить низ
				//+ 0,01 дл€ чутка места
				if (manifold.points[i].y < body->GetPosition().y - m_collisionBox.getDimensions().y / 2.0f + 0.01f){
					below = true;
					break;
				}
			}
			if (below){
				//ћожем прыгать
				if (inputManager.isKeyPressed(SDLK_w)){
					//примен€етс€ только один раз
					//вектор силы, центр,
					body->ApplyLinearImpulse(b2Vec2(0.0f, 30.0f), b2Vec2(0.0f, 0.0f), true);
					//предотвращает двойной прыжок, если касаешьс€ 2 коробок
					break;
				}
			}
		}
	}
	//“еперь демпинг ( дает больший контроль над героем. «амедление прыжка
}