#pragma once
#include <glm/glm.hpp>
#include <Bengine/SpriteBatch.h>

class Bullet
{
public:
	Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime);
	~Bullet();
	//��� ����� ��������������� ������(� ������ 1), ��� ����� ���� x ���������� � y �������� �� ����� ������� ����� 2 �������
	//����������, �� ����. � ��� ��� ����� ���������������, (unit) ��������. � glm ���� ����. ������� normalize.

	void draw(Bengine::SpriteBatch &spriteBatch);
	//false ���� ���� ��� �����, true ���� �� _lifeTIme == 0 � �� ���� ����������
	bool update();

private:
	//��� ������ ���� �� 1 ����
	float _speed;
	//������� ������ ����� ���� ����
	int _lifeTime;
	glm::vec2 _direction;
	glm::vec2 _position;
};

