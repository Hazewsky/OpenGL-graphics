#include "ParticleBatch2D.h"




Bengine::ParticleBatch2D::ParticleBatch2D()
{
	//�����
}


Bengine::ParticleBatch2D::~ParticleBatch2D()
{
	delete[] p_particles;
}


void Bengine::ParticleBatch2D::init(int maxParticles,
									float decayRate,
									GLTexture texture,//��������� � �������
									std::function<void(Particle2D&, float)>updateFunc /*defaultPartcleUpdate*/){
	p_maxParticles = maxParticles;
	p_particles = new Particle2D[maxParticles];
	p_decayRate = decayRate;
	p_texture = texture;
	p_updateFunc = updateFunc;
}

void Bengine::ParticleBatch2D::update(float deltaTime){
	for (int i = 0; i < p_maxParticles; i++){
		//��������� ������� �� �������
		if (p_particles[i].p_life > 0.0f){
			p_updateFunc(p_particles[i], deltaTime);
			//p_particles[i].update(deltaTime);
			//�������� �������, ���� �� ��� ������� �����
			p_particles[i].p_life -= p_decayRate * deltaTime;
		}
	}
}

void Bengine::ParticleBatch2D::draw(SpriteBatch* spriteBatch){
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	for (int i = 0; i < p_maxParticles; i++){
		auto& it = p_particles[i];
		//��������� ������� �� �������
		if (p_particles[i].p_life > 0.0f){
			glm::vec4 destRect(it.p_position.x, it.p_position.y, it.p_width, it.p_width);

			spriteBatch->draw(destRect, uvRect, p_texture.id, 0.0f, it.p_color);
		}
	}
}
//��������������, ��������, ����, ������(�������)
void Bengine::ParticleBatch2D::addParticle(const glm::vec2& position,
											const glm::vec2& velocity,
											const Color& color,
											float width){

	//������� ������� ����� ������������
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
		//����, ���� ��� �����
		if (p_particles[i].p_life <= 0.0f){
			//������ ��� ������� ��������� �������
			p_lastFreeParticle = i;
			//������ ������ ��������� �������
			return i;
		}
	}
	//���������� ��� ��� ���� � ������� �����
	for (int i = 0; i < p_lastFreeParticle; i++){
		//����, ���� ��� �����
		if (p_particles[i].p_life <= 0.0f){
			//������ ��� ������� ��������� �������
			p_lastFreeParticle = i;
			//������ ������ ��������� �������
			return i;
		}
	}
	//��� ��������� ������, ���������� ������ �������
	return 0;

}