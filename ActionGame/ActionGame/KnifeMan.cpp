#include "KnifeMan.h"
#include"Player.h"
#include"Dxlib.h"
#include<math.h>
#include"Camera.h"
#include"CollisionDetector.h"
#include"EnemyBulletFactory.h"

KnifeMan::KnifeMan(Vector2 pos, int& handle, int& deadHandle, Player& player, Camera& camera,EnemyBulletFactory& ebulletFac)
:_playerRef(player), _cameraRef(camera), _fragDrawer(deadHandle),_ebulletFac(ebulletFac)//この時点でFragmentDrawerの引数なしコンストラクタ呼ばれてる
{
	_isAvailable = true;
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

	_hpMax = 5;
	_hp = _hpMax;

	_pfuncMap[KnifeMan::state_far] = &KnifeMan::FarUpdate;
	_pfuncMap[KnifeMan::state_near] = &KnifeMan::NearUpdate;
	_pfuncMap[KnifeMan::state_dead] = &KnifeMan::DeadUpdate;
	_pfuncMap[KnifeMan::state_underThrow] = &KnifeMan::UnderThrowUpdate;
	_pfuncMap[KnifeMan::state_overThrow] = &KnifeMan::OverThrowUpdate;
	_pfuncMap[KnifeMan::state_preUnderThrow] = &KnifeMan::PreUnderThrowUpdate;
	_pfuncMap[KnifeMan::state_preOverThrow] = &KnifeMan::PreOverThrowUpdate;
	_pfuncMap[KnifeMan::state_arial] = &KnifeMan::ArialUpdate;
	_pfuncMap[KnifeMan::state_wait] = &KnifeMan::WaitUpdate;


	_drawFuncMap[KnifeMan::state_far] = &KnifeMan::DrawFar;
	_drawFuncMap[KnifeMan::state_near] = &KnifeMan::DrawNear;
	_drawFuncMap[KnifeMan::state_dead] = &KnifeMan::DrawDead;
	_drawFuncMap[KnifeMan::state_arial] = &KnifeMan::DrawArial;
	_drawFuncMap[KnifeMan::state_underThrow] = &KnifeMan::DrawUnderThrow;
	_drawFuncMap[KnifeMan::state_overThrow] = &KnifeMan::DrawOverThrow;
	_drawFuncMap[KnifeMan::state_preUnderThrow] = &KnifeMan::DrawPreUnderThrow;
	_drawFuncMap[KnifeMan::state_preOverThrow] = &KnifeMan::DrawPreOverThrow;
	_drawFuncMap[KnifeMan::state_wait] = &KnifeMan::DrawWait;


	_stateFrame[KnifeMan::state_far] = 0;
	_stateFrame[KnifeMan::state_near] = 0;
	_stateFrame[KnifeMan::state_dead] = 0;
	_stateFrame[KnifeMan::state_arial] = 0;
	_stateFrame[KnifeMan::state_underThrow] = 0;
	_stateFrame[KnifeMan::state_overThrow] = 0;
	_stateFrame[KnifeMan::state_preUnderThrow] = 0;
	_stateFrame[KnifeMan::state_preOverThrow] = 0;
	_stateFrame[KnifeMan::state_wait] = 0;



	_isGrabbing = false;
	_isNear = false;
	_isDead = false;
	_isLeft = false;

	_overThrowOffset = Vector2(0, -32);
	_underThrowOffset = Vector2(0, 32);

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
	_playerRef.GetCenter().x<_collider.Center().x ? _fragDrawer.Break(Vector2(6.0f, 0.0f)) : _fragDrawer.Break(Vector2(-6.0f, 0.f));
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
	if (fabs(vec.x) >= 200)
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
KnifeMan::FarUpdate()
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
		ChangeState(state_wait);
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
KnifeMan::OverThrowUpdate()
{
	_velocity.y = 3.0f;
	if (_stateFrame[state_overThrow] > 40)
	{
		fabs((_playerRef.GetCollider().Center() - _collider.Center()).x) > 200.0f ? ChangeState(state_far): ChangeState(state_near);
	}
	_pos += _velocity;
	_collider.SetCenter_Cam(_pos, _cameraRef.OffsetX());
}
void
KnifeMan::UnderThrowUpdate()
{
	_velocity.y = 3.0f;
	if (_stateFrame[state_underThrow] > 40)
	{
		fabs((_playerRef.GetCollider().Center() - _collider.Center()).x) > 200.0f ? ChangeState(state_far) : ChangeState(state_near);
	}
	_pos += _velocity;
	_collider.SetCenter_Cam(_pos, _cameraRef.OffsetX());
}
void
KnifeMan::PreOverThrowUpdate()
{
	if (_stateFrame[state_preOverThrow] > 30)
	{
		ChangeState(state_overThrow);
		!_isLeft ? _ebulletFac.Create(_pos + _overThrowOffset, Vector2(4.0f, 0.f)) : _ebulletFac.Create(_pos + _overThrowOffset, Vector2(-4.0f, 0.f));
	}
	_velocity.y = 3.0f;
	_pos += _velocity;
	_collider.SetCenter_Cam(_pos, _cameraRef.OffsetX());
}
void
KnifeMan::PreUnderThrowUpdate()
{
	if (_stateFrame[state_preUnderThrow] > 30)
	{
		ChangeState(state_underThrow);
		!_isLeft ? _ebulletFac.Create(_pos + _underThrowOffset, Vector2(4.0f, 0.f)) : _ebulletFac.Create(_pos + _underThrowOffset, Vector2(-4.0f, 0.f));
	}
	_velocity.y = 3.0f;
	_pos += _velocity;
	_collider.SetCenter_Cam(_pos, _cameraRef.OffsetX());
}
void
KnifeMan::WaitUpdate()
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
KnifeMan::ArialUpdate()
{
	_velocity += _acceleration;
	_pos += _velocity;
	_collider.SetCenter_Cam(_pos, _cameraRef.OffsetX());

	if (_pos.y > 500)Kill();
}

void
KnifeMan::ChangeState(KnifeMan::State state)
{
	_stateFrame[_state] = 0;
	_state = state;
	_pFunc = _pfuncMap[_state];
	_velocity.x = 0.f;
	if(_state == state_wait) _isLeft = (_playerRef.GetCollider().Center()-_collider.Center()).x < 0;
}

void
KnifeMan::DrawFar()
{
	DrawCameraGraph(_pos.x, _pos.y, 32 * ((_stateFrame[state_far] % 20) / 10), 0, 32, 48, 16, 24, 3.0, 0, _handle, true, _isLeft);
}
void
KnifeMan::DrawNear()
{
	DrawCameraGraph(_pos.x, _pos.y, 32 * ((_stateFrame[state_near] % 20) / 10), 0, 32, 48, 16, 24, 3.0, 0, _handle, true, _isLeft);
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
	DrawCameraGraph(_pos.x, _pos.y, 0, 0, 32, 48, 16, 24, 3.0, 0, _handle, true, _isLeft);
}
void
KnifeMan::DrawOverThrow()
{
	DrawCameraGraph(_pos.x, _pos.y, 32*3, 0, 32, 48, 16, 24, 3.0, 0, _handle, true, _isLeft);
}
void
KnifeMan::DrawUnderThrow()
{
	DrawCameraGraph(_pos.x, _pos.y, 32 * 5, 0, 32, 48, 16, 24, 3.0, 0, _handle, true, _isLeft);
}
void
KnifeMan::DrawPreOverThrow()
{
	DrawCameraGraph(_pos.x, _pos.y, 32 * 2, 0, 32, 48, 16, 24, 3.0, 0, _handle, true, _isLeft);
}

void
KnifeMan::DrawPreUnderThrow()
{
	DrawCameraGraph(_pos.x, _pos.y, 32 * 4, 0, 32, 48, 16, 24, 3.0, 0, _handle, true, _isLeft);
}

void
KnifeMan::DrawWait()
{
	DrawCameraGraph(_pos.x, _pos.y, 0, 0, 32, 48, 16, 24, 3.0, 0, _handle, true, _isLeft);
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

