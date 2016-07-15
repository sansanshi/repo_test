#pragma once
#include"GameObject.h"
class Camera;
class EnemyBullet:public GameObject
{
private:
	Vector2 _pos;
	int _handle;
	Vector2 _velocity;

	bool _isLeft;
	Camera& _cameraRef;

public:
	EnemyBullet(Vector2 pos,Vector2 velocity,int handle,Camera& camera);
	~EnemyBullet();
	void Update();

	void OnCollided(Collider*);
	void OnCollided(GameObject*);
	void OnCollided(std::shared_ptr<GameObject>);

	void Draw();
	void Reject(Vector2);
	void DrawCameraGraph(int x, int y, int srcX, int srcY, int width, int height, int cx, int cy, double extRate, double angle, int handle, int transFlag, int turnFlag);
};

/*std::string str;
str[0]='a'
str[1]='b'

&str[0]�Ƃ��ŕ\�������悤�Ƃ���ƁA����str�ɂ͏I�[�L���������̂łǂ��܂ł��\���������悤�Ƃ���

�ꕶ�����\�����������Ȃ�clearString.substr(0,1).c_str
�����ɉe�t�����������炿����Ƃ��炵���ʒu�ɐ^�����œ���������\������
*/
