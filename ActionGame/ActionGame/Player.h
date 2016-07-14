#pragma once
#include "Geometry.h"
#include"Collider.h"
#include"GameObject.h"
#include<map>
#include<list>
#include<memory>

class Camera;
class Enemy;
class GrabMan;
class Stage;

enum PlayerState{
	ps_Neutral,
	ps_Walk,
	ps_Kick,
	ps_Punch,
	ps_Jump,
	ps_JumpKick,
	ps_Crouch,
	ps_CrouchKick,
	ps_CrouchPunch,
	ps_Kamae,
	ps_CrouchKamae,
	ps_Grabbed,
	ps_Dead,
};

class Player:public GameObject//:public Collider
{
private:
	Vector2 _pos;//���W
	Vector2 _velocity;//���x
	Vector2 _acceleration;//�����x
	int _handle;//�摜�n���h��
	bool _isRight;
	int vy, vx;
	char _key[256];

	float _jumpPower;
	int _shakeCnt;
	bool _isGrabbed;
	bool _prevRejectY;//�O�t���[���łx�����i������j�ɉ����߂��ꂽ��

	int _hpMax;
	int _hp;
	int _attackDamage;

	void(Player::*_pFunc)();//�����o�֐��|�C���^
	void KickUpdate();
	void PunchUpdate();
	void JumpUpdate();
	void GroundUpdate();
	void NeutralUpdate();
	void WalkUpdate();
	void JumpKickUpdate();
	void CrouchUpdate();
	void CrouchKickUpdate();
	void CrouchPunchUpdate();
	void KamaeUpdate();
	void CrouchKamaeUpdate();
	void GrabbedUpdate();

	int _walkFrame;
	int _kickingFrame;
	int _punchFrame;
	int _kickInterval;

	typedef void(Player::*func_void)();
	std::map<PlayerState, int> _handleMap;
	std::map<PlayerState, int> _stateFrame;
	std::map<PlayerState, func_void> _drawFuncMap;//map�������Ƃ肠�����S����second�͖��߂Ă���
	std::map<PlayerState, func_void> _pFuncMap;

	PlayerState _state;

	void DrawNeutral();
	void DrawWalk();
	void DrawKick();
	void DrawPunch();
	void DrawJump();
	void DrawJumpKick();
	void DrawCrouch();
	void DrawCrouchKick();
	void DrawCrouchPunch();
	void DrawKamae();
	void DrawCrouchKamae();

	Rect _attackRect;
	Collider _attackCol;
	Vector2 _attackColOffset;

	Camera& _cameraRef;
	float _cOffsetX;

	std::list<GrabMan*> _grabbingEnemies;//�v���C���[��͂�ł���G�l�~�[�̃��X�g

	Stage& _stageRef;

public:
	Player(Camera&,Stage&);
	~Player();
	Vector2& GetPos(){ return _pos; }
	void Update();
	void Draw();
	void SetPos(Vector2 pos);

	void OnCollided(Collider*);
	void OnCollided(Enemy*);
	void OnCollided(GameObject*);
	void OnCollided(std::shared_ptr<GameObject>);

	void SetAcceleration(Vector2 acceleration){ _acceleration = acceleration; }
	void SetVelocity(Vector2 vel){ _velocity = vel; }
	
	void ChangeState(PlayerState state);

	Rect& GetAttackRect(){ return _attackRect; }

	Vector2& GetCenter(){
		return _pos;
	}//return _collider.Center(); }


	void DrawCameraGraph(int x, int y, int srcX, int srcY, int width, int height, int cx, int cy, double extRate,double angle, int handle, int transFlag, int turnFlag);//drawrotagraph2�����b�v���Ă��炵���ʒu�ɕ\������@���Ƃ͑S���̕`��������ɒu��������

	//Collider& GetCollider(){ return _collider; }
	Collider& GetAttackCol(){ return _attackCol; }

	void Damage(int value);
	void Grabbed(GrabMan*);
	//���̒n�ʂ̔���ł͂Ȃ��n�ʂƂ�����`�Ƃ̓����蔻��ŉ����Ԃ�

	/*�����߂�����*/
	void Reject(Vector2);

	float GetPercentageHp(){ return (float)_hp / (float)_hpMax; }

	void Killed();
	int GetAttackDmg(){ return _attackDamage; }
};


