#include "ParticleBatch2D.h"




Bengine::ParticleBatch2D::ParticleBatch2D()
{
	//пусто
}


Bengine::ParticleBatch2D::~ParticleBatch2D()
{
	delete[] p_particles;
}


void Bengine::ParticleBatch2D::init(int maxParticles,
									float decayRate,
									GLTexture texture,//указатель к функции
									std::function<void(Particle2D&, float)>updateFunc /*defaultPartcleUpdate*/){
	p_maxParticles = maxParticles;
	p_particles = new Particle2D[maxParticles];
	p_decayRate = decayRate;
	p_texture = texture;
	p_updateFunc = updateFunc;
}

void Bengine::ParticleBatch2D::update(float deltaTime){
	for (int i = 0; i < p_maxParticles; i++){
		//провер€ем активна ли частицы
		if (p_particles[i].p_life > 0.0f){
			p_updateFunc(p_particles[i], deltaTime);
			//p_particles[i].update(deltaTime);
			//угасание эффекта, дабы не был активен ¬≈„Ќќ
			p_particles[i].p_life -= p_decayRate * deltaTime;
		}
	}
}

void Bengine::ParticleBatch2D::draw(SpriteBatch* spriteBatch){
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	for (int i = 0; i < p_maxParticles; i++){
		auto& it = p_particles[i];
		//провер€ем активна ли частицы
		if (p_particles[i].p_life > 0.0f){
			glm::vec4 destRect(it.p_position.x, it.p_position.y, it.p_width, it.p_width);

			spriteBatch->draw(destRect, uvRect, p_texture.id, 0.0f, it.p_color);
		}
	}
}
//местоположение, скорость, цвет, размер(квадрат)
void Bengine::ParticleBatch2D::addParticle(const glm::vec2& position,
											const glm::vec2& velocity,
											const Color& color,
											float width){

	//частица которую будем использовать
	int particleIndex = findFreeParticle();
	auto& it = p_particles[particleIndex];
	it.p_life = 1.0f;
	it.p_position = position;
	it.p_velocity = velocity;
	it.p_color = color;
	it.p_width = width;
}


int Bengine::ParticleBatch2D::findFreeParticle(){
	for (int i = p_lastFreeParticle;i < p_maxParticles; i++){
		//окей, если это верно
		if (p_particles[i].p_life <= 0.0f){
			//значит это крайн€€ свободна€ частица
			p_lastFreeParticle = i;
			//вернет индекс свободной частицы
			return i;
		}
	}
	//убеждаемс€ что луп идет с нужного места
	for (int i = 0; i < p_lastFreeParticle; i++){
		//окей, если это верно
		if (p_particles[i].p_life <= 0.0f){
			//значит это крайн€€ свободна€ частица
			p_lastFreeParticle = i;
			//вернет индекс свободной частицы
			return i;
		}
	}
	//Ќет свободных частиц, перезапись первой частицы
	return 0;

}