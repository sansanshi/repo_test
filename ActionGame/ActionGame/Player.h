#pragma once
#include "Geometry.h"
#include"Collider.h"
#include"GameObject.h"
#include<map>
#include<list>
#include<memory>
#include"FragmentDrawer.h"

class Camera;
class Enemy;
class GrabMan;
class Stage;
class PlayingScene;

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
	ps_Spell,

};

class Player:public GameObject//:public Collider
{
private:
	Vector2 _pos;//座標
	Vector2 _velocity;//速度
	Vector2 _acceleration;//加速度
	int _handle;//画像ハンドル
	bool _isRight;
	int vy, vx;
	char _key[256];

	float _jumpPower;
	int _shakeCnt;
	bool _isGrabbed;
	bool _prevRejectY;//前フレームでＹ方向（上向き）に押し戻されたか

	int _hpMax;
	int _hp;
	int _attackDamage;
	float _walkSpd;

	void(Player::*_pFunc)();//メンバ関数ポインタ
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
	void DeadUpdate();
	void SpellUpdate();

	int _walkFrame;
	int _kickingFrame;
	int _punchFrame;
	int _kickInterval;

	typedef void(Player::*func_void)();
	std::map<PlayerState, int> _handleMap;
	std::map<PlayerState, int> _stateFrame;
	std::map<PlayerState, func_void> _drawFuncMap;//map作ったらとりあえず全部のsecondは埋めておく
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
	void DrawDead();
	void DrawSpell();

	Rect _attackRect;
	Collider _attackCol;
	Vector2 _attackColOffset;

	Camera& _cameraRef;
	float _cOffsetX;

	std::list<GrabMan*> _grabbingEnemies;//プレイヤーを掴んでいるエネミーのリスト

	Stage& _stageRef;

	FragmentDrawer *_fragDrawer;
	int _deadHandle;
	bool _isDead;

	int _rejectCnt;

	Vector2 _upperVec;//y方向のベクトル　ブロックとの当たり判定に使う 2Dだと下方向（0，1）

	int _modelHandle;
	int _animTime;
	int _animDuration;

	enum AnimState{
		anim_none,
		anim_idle,
		anim_punch,
		anim_kick,
		anim_walk,
		anim_spell,


	};
	//std::map<AnimState, int> _anim;
	int _anim;
	AnimState _animState;

	bool _prevIsRight;//前フレームに右を向いていたか

	int _vertShaderHandle;
	int _pixelShaderHandle;

	PlayingScene& _playingScene;
public:
	Player(Camera&,Stage&,PlayingScene&);
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

	bool IsRight(){ return _isRight; }

	Vector2& GetCenter(){
		return _pos;
	}//return _collider.Center(); }

	MATRIX _scaleMat;
	MATRIX _rotMat;
	MATRIX _transMat;
	void ToRightCulling();//モデルのポリゴンを右回りで表示する
	void ToLeftCulling();
	void TurnModelX();

	void DrawCameraGraph(int x, int y, int srcX, int srcY, int width, int height, int cx, int cy, double extRate,double angle, int handle, int transFlag, int turnFlag);//drawrotagraph2をラップしてずらした位置に表示する　あとは全部の描画をこいつに置き換える

	//Collider& GetCollider(){ return _collider; }
	Collider& GetAttackCol(){ return _attackCol; }

	void Damage(int value);
	void Grabbed(GrabMan*);
	//今の地面の判定ではなく地面という矩形との当たり判定で押し返す

	/*押し戻し処理*/
	void Reject(Vector2);

	float GetPercentageHp(){ return (float)_hp / (float)_hpMax; }

	void Killed();
	int GetAttackDmg(){ return _attackDamage; }
	bool IsDead(){ return _isDead; }

	void Move(Vector2 vec);

	int GetRejectCnt(){ return _rejectCnt; }

	void ChangeAnim(AnimState anim);
};


