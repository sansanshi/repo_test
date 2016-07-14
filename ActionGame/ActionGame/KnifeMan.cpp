#include "KnifeMan.h"
#include"Player.h"
#include"Dxlib.h"
#include<math.h>
#include"Camera.h"
#include"CollisionDetector.h"

KnifeMan::KnifeMan(Vector2 pos, int& handle, int& deadHandle, Player& player, Camera& camera)
:_playerRef(player), _cameraRef(camera), _fragDrawer(deadHandle)//この時点でFragmentDrawerの引数なしコンストラクタ呼ばれてる
{
	_prevRejectY = false;
	_collider = Collider(this, ct_knifeMan, col_default);
	_collider.ToEnable();
	_pos = pos;
	_collider.SetRect(Rect(_pos, 29, 128));
	_collider.SetCenter(_pos + Vector2(_cameraRef.OffsetX(), 0));
	_velocity = Vector2(0, 0);
	_acceleration = Vector2(0, 0.3f);
	_pFunc = &KnifeMan::ArialUpdate;
	_handle = handle;
	_deadhandle = deadHandle;
	_walkFrame = 0;
	_state = state_arial;

	_hpMax = 2;
	_hp = _hpMax;

	_pfuncMap[state_far] = &KnifeMan::FarUpdate;
	_pfuncMap[state_near] = &KnifeMan::NearUpdate;
	_pfuncMap[state_dead] = &KnifeMan::DeadUpdate;
	_pfuncMap[state_underThrow] = &KnifeMan::UnderThrowUpdate;
	_pfuncMap[state_overThrow] = &KnifeMan::OverThrowUpdate;
	_pfuncMap[state_preUnderThrow] = &KnifeMan::PreUnderThrowUpdate;
	_pfuncMap[state_preOverThrow] = &KnifeMan::PreOverThrowUpdate;
	_pfuncMap[state_arial] = &KnifeMan::ArialUpdate;


	_drawFuncMap[state_far] = &KnifeMan::DrawFar;
	_drawFuncMap[state_near] = &KnifeMan::DrawNear;
	_drawFuncMap[state_dead] = &KnifeMan::DrawDead;
	_drawFuncMap[state_arial] = &KnifeMan::DrawArial;
	_drawFuncMap[state_underThrow] = &KnifeMan::DrawUnderThrow;
	_drawFuncMap[state_overThrow] = &KnifeMan::DrawOverThrow;
	_drawFuncMap[state_preUnderThrow] = &KnifeMan::DrawPreUnderThrow;
	_drawFuncMap[state_preOverThrow] = &KnifeMan::DrawPreOverThrow;


	_stateFrame[state_far] = 0;
	_stateFrame[state_near] = 0;
	_stateFrame[state_dead] = 0;
	_stateFrame[state_arial] = 0;
	_stateFrame[state_underThrow] = 0;
	_stateFrame[state_overThrow] = 0;
	_stateFrame[state_preUnderThrow] = 0;
	_stateFrame[state_preOverThrow] = 0;



	_isGrabbing = false;
	_isNear = false;
	_isDead = false;
	_isLeft = false;


	//_fragDrawer = new FragmentDrawer(_deadhandle);
}


KnifeMan::~KnifeMan()
{
	//DeleteGraph(_handle);//こいつがhandleを解放する必要はない　というかファクトリからの参照なのでここで解放すると_handleは完全に解放されてしまう
}

void
KnifeMan::OnCollided(Collider* col)
{
	if (col->Type() == col_attack&&col->CharaType() == ct_player){
		Vector2 vec = (_pos - _playerRef.GetCenter()).Normalize();
		Reject(Vector2(vec.x * 20, 0));
		Player* p = dynamic_cast<Player*>(col->_gameObject);
		Damage(p->GetAttackDmg());
	}
	if (col->Type() == col_default&&col->CharaType() == ct_player){

	}
	
}
void
KnifeMan::OnCollided(GameObject* obj)
{
	if (obj->ColType() == col_attack&&obj->CharaType() == ct_player){
		//ChangeState(state_dead);
	}
	if (obj->ColType() == col_default&&obj->CharaType() == ct_player){
		
	}
}

void
KnifeMan::OnCollided(std::shared_ptr<GameObject> obj)
{

}



void
KnifeMan::AliveUpdate()
{
	Vector2 vec = (_playerRef.GetCollider().Center() - _collider.Center());//normalizeで変な値になってる
	vec = vec.Normalize();
	_pos.x += vec.x * 2;
	_collider.pos = _pos + Vector2(_cameraRef.OffsetX(), 0);
	_walkFrame++;
}

void
KnifeMan::Update()
{
	_cOffsetX = _cameraRef.OffsetX();
	++_stateFrame[_state];
	(this->*_pFunc)();

}

void
KnifeMan::Draw()
{
	(this->*_drawFuncMap[_state])();
	_collider.Draw();


}

void
KnifeMan::Kill()
{
	_velocity.x = _isLeft ? 10.0f : -10.0f;
	_velocity.y = -3.0f;
	_collider.ToDisable();//_isCollidable = false;
	_isDead = true;
	ChangeState(state_dead);

	_isLeft ? _fragDrawer.TurnUV() : 0;//uv値のUだけ反転
	_fragDrawer.FragmentScalling(3.0f);//Scallingの方を先にする
	_fragDrawer.FragmentTranslation(Vector2(_collider.Left() - 32, _collider.Top()));
	_fragDrawer.Break(Vector2(6.0f, 0.0f));
}

void
KnifeMan::NearUpdate()
{


	_velocity.y = 3.0f;
	Vector2 vec = (_playerRef.GetCollider().Center() - _collider.Center());//normalizeで変な値になってる
	_velocity.x = vec.x > 0 ? -2 : 2;
	_pos += _velocity;
	_collider.SetCenter(_pos + Vector2(_cOffsetX, 0));//_collider.pos = _pos;
	float x = _pos.x - 15;
	float colx = _collider.Left();
	_walkFrame++;
	_isLeft = _velocity.x < 0;

	if (!_prevRejectY)
	{
		ChangeState(state_arial);
		_velocity.x = 0.f;
	}
	_prevRejectY = false;
}
void
KnifeMan::FarUpdate()
{

	_velocity.y = 3.0f;
	Vector2 vec = (_playerRef.GetCollider().Center() - _collider.Center());//normalizeで変な値になってる
	_velocity.x = vec.x > 0 ? 2 : -2;
	_pos += _velocity;
	_collider.SetCenter(_pos + Vector2(_cOffsetX, 0));//_collider.pos = _pos;
	_walkFrame++;
	_isLeft = _velocity.x < 0;

	if (fabs(vec.x) <= 128)
	{
		ChangeState(state_near);
	}

	if (!_prevRejectY)
	{
		ChangeState(state_arial);
		_velocity.x = 0.f;
	}
	_prevRejectY = false;
}
void
KnifeMan::DeadUpdate()//バラバラ描画をしておきたいのでタイマー制にする
{
	if (_stateFrame[state_dead]>120) _isAvailable = false;
	//DrawString(200, 200, "deadupdate", 0xffffff);
	//_pos += _velocity;
	//_velocity.y +=0.5f;
	//if (_pos.y > 500) _isAvailable = false;
}


void
KnifeMan::ArialUpdate()
{
	_velocity += _acceleration;
	_pos += _velocity;
	_collider.SetCenter_Cam(_pos, _cameraRef.OffsetX());
}

void
KnifeMan::ChangeState(State state)
{
	_stateFrame[_state] = 0;
	_state = state;
	_pFunc = _pfuncMap[_state];
}

void
KnifeMan::DrawFar()
{
	DrawCameraGraph(_pos.x, _pos.y, 16 * ((_stateFrame[state_far] % 20) / 10), 0, 16, 40, 8, 20, 3.0, 0, _handle, true, _isLeft);
}
void
KnifeMan::DrawNear()
{
	DrawCameraGraph(_pos.x, _pos.y, 16 * (((_stateFrame[state_near] % 20) / 10) + 2), 0, 16, 40, 8, 20, 3.0, 0, _handle, true, _isLeft);
}
void
KnifeMan::DrawDead()
{
	//バラバラ描画
	_fragDrawer.Draw();//DrawCameraGraph(_pos.x, _pos.y, 0, 0, 26, 40, 13, 20, 3.0f, 90, _deadhandle, true, _isLeft);
}

void
KnifeMan::DrawArial()
{
	DrawCameraGraph(_pos.x, _pos.y, 0, 0, 16, 40, 8, 20, 3.0, 0, _handle, true, _isLeft);
}


void
KnifeMan::DrawCameraGraph(int x, int y, int srcX, int srcY, int width, int height, int cx, int cy, double extRate, double angle, int handle, int transFlag, int turnFlag)
{
	DrawRectRotaGraph2(x + _cameraRef.OffsetX(), y, srcX, srcY, width, height, cx, cy, extRate, angle, handle, transFlag, turnFlag);
}

void
KnifeMan::Reject(Vector2 vec)
{
	vec.y<0 ? _prevRejectY = true : 0;
	_pos += vec;
	_collider.SetCenter(_pos + Vector2(_cameraRef.OffsetX(), 0));
	if (_state == state_arial&&_velocity.y > 0.0f&&vec.y < 0.0f)//空中状態から上向きに押し返された場合
	{
		ChangeState(state_far);
		_velocity.Init();
		_acceleration.Init();
	}
}

void
KnifeMan::Damage(int value)
{
	_hp = max(_hp - value, 0);
	if (_hp <= 0) Kill();


}

