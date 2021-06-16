#pragma once

// �x�N�g���\����(2D)
struct Vector2
{
	float x, y;

	Vector2();
	Vector2(float inx, float iny);

	//�x�N�g���̑傫����Ԃ�
	float Magnitude()const;		// �傫��

	//�x�N�g���̑傫���̂Q���Ԃ�
	float SQMagnitude()const;

	//�x�N�g���𐳋K������
	void Normalize();

	//�x�N�g���𐳋K�������x�N�g����Ԃ�
	Vector2 Normalized()const;

	//���
	void operator=(const Vector2& rval);
	void operator+=(const Vector2& rval);
	void operator-=(const Vector2& rval);
	void operator*=(const float scale);
	void operator/=(const float div);

};

Vector2 operator+(const Vector2& lval, const Vector2& rval);
Vector2 operator-(const Vector2& lval, const Vector2& rval);
Vector2 operator*(const Vector2& lval, const float scale);
Vector2 operator/(const Vector2& lval, const float div);

// ���W�\����=�x�N�g���ƈꏏ(2D)
using Position2 = Vector2;

// ��`�\����
struct Rect
{
	Position2 center;//��`�̒��S
	float w, h;//���̍����̔���
	float Left() const;//��
	float Top() const;//��
	float Right() const;//�E
	float Bottom() const;//��
	float Width() const;//��
	float Height() const;//����

};

// �~�̍\����
struct Circle {
	Position2 center;//���S�_
	float radius;//���a
};
