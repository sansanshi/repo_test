#include "BatMan.h"
#include"Player.h"
#include"Dxlib.h"
#include<math.h>
#include"Camera.h"
#include"CollisionDetector.h"
#include"EnemyBulletFactory.h"

BatMan::BatMan(Vector2 pos, int& handle, int& deadHandle, Player& player, Camera& camera)
:_playerRef(player), _cameraRef(camera), _fragDrawer(deadHandle)//この時点でFragmentDrawerの引数なしコンストラクタ呼ばれてる
{
	_isAvailable = true;
	_prevRejectY = false;
	_collider = Collider(this, ct_batman, col_default);
	_collider.ToEnable();
	_pos = pos;
	_collider.SetRect(Rect(_pos, 29, 128));
	_collider.SetCenter(_pos + Vector2(_cameraRef.OffsetX(), 0));
	_velocity = Vector2(0, 0);
	_acceleration = Vector2(0, 0.3f);
	_pFunc = &BatMan::ArialUpdate;
	_handle = handle;
	_deadhandle = deadHandle;
	_walkFrame = 0;
	_state = state_arial;

	_hpMax = 2;
	_hp = _hpMax;

	_pfuncMap[BatMan::state_far] = &BatMan::FarUpdate;
	_pfuncMap[BatMan::state_near] = &BatMan::NearUpdate;
	_pfuncMap[BatMan::state_dead] = &BatMan::DeadUpdate;
	_pfuncMap[BatMan::state_underThrow] = &BatMan::UnderThrowUpdate;
	_pfuncMap[BatMan::state_overThrow] = &BatMan::OverThrowUpdate;
	_pfuncMap[BatMan::state_preUnderThrow] = &BatMan::PreUnderThrowUpdate;
	_pfuncMap[BatMan::state_preOverThrow] = &BatMan::PreOverThrowUpdate;
	_pfuncMap[BatMan::state_arial] = &BatMan::ArialUpdate;
	_pfuncMap[BatMan::state_wait] = &BatMan::WaitUpdate;


	_drawFuncMap[BatMan::state_far] = &BatMan::DrawFar;
	_drawFuncMap[BatMan::state_near] = &BatMan::DrawNear;
	_drawFuncMap[BatMan::state_dead] = &BatMan::DrawDead;
	_drawFuncMap[BatMan::state_arial] = &BatMan::DrawArial;
	_drawFuncMap[BatMan::state_underThrow] = &BatMan::DrawUnderThrow;
	_drawFuncMap[BatMan::state_overThrow] = &BatMan::DrawOverThrow;
	_drawFuncMap[BatMan::state_preUnderThrow] = &BatMan::DrawPreUnderThrow;
	_drawFuncMap[BatMan::state_preOverThrow] = &BatMan::DrawPreOverThrow;
	_drawFuncMap[BatMan::state_wait] = &BatMan::DrawWait;


	_stateFrame[BatMan::state_far] = 0;
	_stateFrame[BatMan::state_near] = 0;
	_stateFrame[BatMan::state_dead] = 0;
	_stateFrame[BatMan::state_arial] = 0;
	_stateFrame[BatMan::state_underThrow] = 0;
	_stateFrame[BatMan::state_overThrow] = 0;
	_stateFrame[BatMan::state_preUnderThrow] = 0;
	_stateFrame[BatMan::state_preOverThrow] = 0;
	_stateFrame[BatMan::state_wait] = 0;



	_isGrabbing = false;
	_isNear = false;
	_isDead = false;
	_isLeft = false;

	_overThrowOffset = Vector2(0, -32);
	_underThrowOffset = Vector2(0, 32);

	_attackCol = Collider(this, ct_batman, col_attack);
	_attackCol.width = 40;
	_attackCol.height = 32;
	_attackOffset = _isLeft ? Vector2(-40, -32) : Vector2(40, 32);
	_attackOffset_under = _isLeft ? Vector2(-50, 0) : Vector2(50, 0);
	_attackRange = 40;
	_attackCol.ToDisable();

	//_fragDrawer = new FragmentDrawer(_deadhandle);
}


BatMan::~BatMan()
{
	//DeleteGraph(_handle);//こいつがhandleを解放する必要はない　というかファクトリからの参照なのでここで解放すると_handleは完全に解放されてしまう
}

void
BatMan::OnCollided(Collider* col)
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
BatMan::OnCollided(GameObject* obj)
{
	if (obj->ColType() == col_attack&&obj->CharaType() == ct_player){
		//ChangeState(state_dead);
	}
	if (obj->ColType() == col_default&&obj->CharaType() == ct_player){

	}
}

void
BatMan::OnCollided(std::shared_ptr<GameObject> obj)
{

}



void
BatMan::AliveUpdate()
{
	Vector2 vec = (_playerRef.GetCollider().Center() - _collider.Center());//normalizeで変な値になってる
	vec = vec.Normalize();
	_pos.x += vec.x * 2;
	_collider.pos = _pos + Vector2(_cameraRef.OffsetX(), 0);
	_walkFrame++;
}

void
BatMan::Update()
{
	_cOffsetX = _cameraRef.OffsetX();
	++_stateFrame[_state];
	(this->*_pFunc)();

}

void
BatMan::Draw()
{
	(this->*_drawFuncMap[_state])();
	_collider.Draw();

	_attackCol.Draw();
}

void
BatMan::Kill()
{
	_velocity.x = _isLeft ? 10.0f : -10.0f;
	_velocity.y = -3.0f;
	_collider.ToDisable();//_isCollidable = false;
	_isDead = true;
	ChangeState(state_dead);

	_isLeft ? _fragDrawer.TurnUV() : 0;//uv値のUだけ反転
	_fragDrawer.FragmentScalling(3.0f);//Scallingの方を先にする
	_fragDrawer.FragmentTranslation(Vector2(_collider.Left() - 32, _collider.Top()));
	if (_playerRef.GetCollider().Center().x < _collider.Center().x) { 
		_fragDrawer.Break(Vector2(6.0f, 0.0f));
	} 
	else{
		_fragDrawer.Break(Vector2(-6.0f, 0.f));
	}
}

void
BatMan::NearUpdate()
{


	_velocity.y = 3.0f;
	Vector2 vec = (_playerRef.GetCollider().Center() - _collider.Center());//normalizeで変な値になってる
	_velocity.x = vec.x < 0 ? -2 : 2;
	_pos += _velocity;
	_collider.SetCenter(_pos + Vector2(_cOffsetX, 0));//_collider.pos = _pos;
	float x = _pos.x - 15;
	float colx = _collider.Left();
	_walkFrame++;
	_isLeft = _velocity.x < 0;
	if (fabs(vec.x) <= _attackRange)
	{
		ChangeState(state_wait);
		_isLeft = vec.x < 0;
	}

	if (!_prevRejectY)
	{
		ChangeState(state_arial);
		_velocity.x = 0.f;
	}
	_prevRejectY = false;
}
void
BatMan::FarUpdate()
{

	_velocity.y = 3.0f;
	Vector2 vec = (_playerRef.GetCollider().Center() - _collider.Center());//normalizeで変な値になってる
	_velocity.x = vec.x > 0 ? 2 : -2;
	_pos += _velocity;
	_collider.SetCenter(_pos + Vector2(_cOffsetX, 0));//_collider.pos = _pos;
	_walkFrame++;
	_isLeft = _velocity.x < 0;

	if (fabs(vec.x) <= 200)
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
BatMan::DeadUpdate()//バラバラ描画をしておきたいのでタイマー制にする
{
	if (_stateFrame[state_dead]>120) _isAvailable = false;
	//DrawString(200, 200, "deadupdate", 0xffffff);
	//_pos += _velocity;
	//_velocity.y +=0.5f;
	//if (_pos.y > 500) _isAvailable = false;
}
void
BatMan::OverThrowUpdate()
{
	_attackCol.SetCenter_Cam(_pos + _attackOffset, _cameraRef.OffsetX());

	_velocity.y = 3.0f;
	if (_stateFrame[state_overThrow] > 10)
	{
		fabs((_playerRef.GetCollider().Center() - _collider.Center()).x) > _attackRange ? ChangeState(state_far) : ChangeState(state_near);
	}
	_pos += _velocity;
	_collider.SetCenter_Cam(_pos, _cameraRef.OffsetX());
}
void
BatMan::UnderThrowUpdate()
{
	_attackCol.SetCenter_Cam(_pos + _attackOffset_under, _cameraRef.OffsetX());

	_velocity.y = 3.0f;
	if (_stateFrame[state_underThrow] > 10)
	{
		fabs((_playerRef.GetCollider().Center() - _collider.Center()).x) > _attackRange ? ChangeState(state_far) : ChangeState(state_near);
	}
	_pos += _velocity;
	_collider.SetCenter_Cam(_pos, _cameraRef.OffsetX());
}
void
BatMan::PreOverThrowUpdate()
{
	if (_stateFrame[state_preOverThrow] > 30)
	{
		ChangeState(state_overThrow);
	}
	_velocity.y = 3.0f;
	_pos += _velocity;
	_collider.SetCenter_Cam(_pos, _cameraRef.OffsetX());
}
void
BatMan::PreUnderThrowUpdate()
{
	if (_stateFrame[state_preUnderThrow] > 30)
	{
		ChangeState(state_underThrow);
	}
	_velocity.y = 3.0f;
	_pos += _velocity;
	_collider.SetCenter_Cam(_pos, _cameraRef.OffsetX());
}
void
BatMan::WaitUpdate()
{
	if (_stateFrame[state_wait] > 30)
	{
		rand() % 2 == 0 ? ChangeState(state_preOverThrow) : ChangeState(state_preUnderThrow);
	}
	_velocity.y = 3.0f;
	_pos += _velocity;
	_collider.SetCenter_Cam(_pos, _cameraRef.OffsetX());
}


void
BatMan::ArialUpdate()
{
	_velocity += _acceleration;
	_pos += _velocity;
	_collider.SetCenter_Cam(_pos, _cameraRef.OffsetX());

	if (_pos.y > 500)Kill();
}

void
BatMan::ChangeState(BatMan::State state)
{
	_stateFrame[_state] = 0;
	_state = state;
	_pFunc = _pfuncMap[_state];
	_velocity.x = 0.f;
	if (_state == state_wait) _isLeft = (_playerRef.GetCollider().Center() - _collider.Center()).x < 0;
	if (!(_state == state_overThrow || _state == state_underThrow)) _attackCol.ToDisable();
	else{
		_attackOffset = _isLeft ? Vector2(-40, -32) : Vector2(40, 32);
		_attackOffset_under = _isLeft ? Vector2(-50, 0) : Vector2(50, 0);
		_attackCol.ToEnable();
	}

}

void
BatMan::DrawFar()
{
	DrawCameraGraph(_pos.x, _pos.y, 44 * ((_stateFrame[state_far] % 20) / 10), 0, 44, 48, 22, 24, 3.0, 0, _handle, true, _isLeft);
}
void
BatMan::DrawNear()
{
	DrawCameraGraph(_pos.x, _pos.y, 44 * ((_stateFrame[state_near] % 20) / 10), 0, 44, 48, 22, 24, 3.0, 0, _handle, true, _isLeft);
}
void
BatMan::DrawDead()
{
	//バラバラ描画
	_fragDrawer.Draw();//DrawCameraGraph(_pos.x, _pos.y, 0, 0, 26, 40, 13, 20, 3.0f, 90, _deadhandle, true, _isLeft);
}

void
BatMan::DrawArial()
{
	DrawCameraGraph(_pos.x, _pos.y, 0, 0, 44, 48, 22, 24, 3.0, 0, _handle, true, _isLeft);
}
void
BatMan::DrawOverThrow()
{
	DrawCameraGraph(_pos.x, _pos.y, 44 * 3, 0, 44, 48, 22, 24, 3.0, 0, _handle, true, _isLeft);
}
void
BatMan::DrawUnderThrow()
{
	DrawCameraGraph(_pos.x, _pos.y, 44 * 5, 0, 44, 48, 22, 24, 3.0, 0, _handle, true, _isLeft);
}
void
BatMan::DrawPreOverThrow()
{
	DrawCameraGraph(_pos.x, _pos.y, 44 * 2, 0, 44, 48, 22, 24, 3.0, 0, _handle, true, _isLeft);
}

void
BatMan::DrawPreUnderThrow()
{
	DrawCameraGraph(_pos.x, _pos.y, 44 * 4, 0, 44, 48, 22, 24, 3.0, 0, _handle, true, _isLeft);
}

void
BatMan::DrawWait()
{
	DrawCameraGraph(_pos.x, _pos.y, 0, 0, 44, 48, 22, 24, 3.0, 0, _handle, true, _isLeft);
}

void
BatMan::DrawCameraGraph(int x, int y, int srcX, int srcY, int width, int height, int cx, int cy, double extRate, double angle, int handle, int transFlag, int turnFlag)
{
	DrawRectRotaGraph2(x + _cameraRef.OffsetX(), y, srcX, srcY, width, height, cx, cy, extRate, angle, handle, transFlag, turnFlag);
}

void
BatMan::Reject(Vector2 vec)
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
BatMan::Damage(int value)
{
	_hp = max(_hp - value, 0);
	if (_hp <= 0) Kill();


}

