#include "GrabMan.h"
#include"Player.h"
#include"Dxlib.h"
#include<math.h>
#include"Camera.h"

GrabMan::GrabMan(Vector2 pos, int& handle, int& deadHandle, Player& player, Camera& camera) :_playerRef(player), _cameraRef(camera)
{
	_collider = Collider(this, ct_grabMan, col_default);
	_collider.ToEnable();
	_pos = pos;
	_collider.SetRect(Rect(_pos, 29, 128));
	_collider.SetCenter(_pos + Vector2(_cameraRef.OffsetX(),0));
	_velocity = Vector2(0, 0);
	_pFunc = &GrabMan::FarUpdate;
	_handle = handle;
	_deadhandle = deadHandle;
	_walkFrame = 0;
	_state = state_far;

	_pfuncMap[state_far] = &GrabMan::FarUpdate;
	_pfuncMap[state_near] = &GrabMan::NearUpdate;
	_pfuncMap[state_dead] = &GrabMan::DeadUpdate;
	_pfuncMap[state_grab] = &GrabMan::GrabUpdate;
	_pfuncMap[state_arial] = &GrabMan::ArialUpdate;


	_drawFuncMap[state_far] = &GrabMan::DrawFar;
	_drawFuncMap[state_near] = &GrabMan::DrawNear;
	_drawFuncMap[state_dead] = &GrabMan::DrawDead;
	_drawFuncMap[state_arial] = &GrabMan::DrawArial;
	_drawFuncMap[state_grab] = &GrabMan::DrawGrab;

	_stateFrame[state_far] = 0;
	_stateFrame[state_near] = 0;
	_stateFrame[state_dead] = 0;
	_stateFrame[state_arial ] = 0;
	_stateFrame[state_grab] = 0;



	_isGrabbing = false;
	_isNear = false;
	_isDead = false;
	_isLeft = false;

	_shakedCnt = 0;
}


GrabMan::~GrabMan()
{
	//DeleteGraph(_handle);//������handle���������K�v�͂Ȃ��@�Ƃ������t�@�N�g������̎Q�ƂȂ̂ł����ŉ�������_handle�͊��S�ɉ������Ă��܂�
}

void
GrabMan::OnCollided(Collider* col)
{
	if (col->Type() == col_attack&&col->CharaType()==ct_player){
		ChangeState(state_dead);
	}
	if (col->Type() == col_default&&col->CharaType() == ct_player){
		
		Player* p = dynamic_cast<Player*>(col->_gameObject);
		p->Grabbed(this);
		_isGrabbing = true;
		ChangeState(state_grab);
		_collider.ToDisable();//_isCollidable = false;//���ꂵ�Ƃ��Ȃ��Ɠ������Ă���ԉ��x��OnCollided���Ă΂��		
	}
}
void
GrabMan::OnCollided(GameObject* obj)
{
	if (obj->ColType() == col_attack&&obj->CharaType() == ct_player){
		ChangeState(state_dead);
	}
	if (obj->ColType() == col_default&&obj->CharaType() == ct_player){
		Player* p = dynamic_cast<Player*>(obj);
		p->Grabbed(this);
		_isGrabbing = true;
		ChangeState(state_grab);
		_collider.ToDisable();//_isCollidable = false;
	}
}

void
GrabMan::OnCollided(std::shared_ptr<GameObject> obj)
{

}



void
GrabMan::AliveUpdate()
{
	Vector2 vec = (_playerRef.GetCollider().Center()-_collider.Center());//normalize�ŕςȒl�ɂȂ��Ă�
	vec = vec.Normalize();
	_pos.x += vec.x * 2;
	_collider.pos = _pos+Vector2(_cameraRef.OffsetX(),0);
	_walkFrame++;
}

void
GrabMan::Update()
{
	_cOffsetX = _cameraRef.OffsetX();
	(this->*_pFunc)();
	++_stateFrame[_state];
}

void 
GrabMan::Draw()
{
	(this->*_drawFuncMap[_state])();
	_collider.Draw();
	//if (!_isDead)
	//{
	//	int graphNum = 0;//
	//	if (_isNear) graphNum = 16 * (((_walkFrame % 20) / 10) + 2);
	//	else graphNum = 16 * ((_walkFrame % 20) / 10);

	//	if (_isGrabbing)DrawCameraGraph(_pos.x,_pos.y,16*4,0,16,40,8,20,3.0,0,_handle,true,_isLeft);//DrawRectExtendGraph(_pos.x, _pos.y, _pos.x + 64, _pos.y + 128, 16 * 4, 0, 16, 40, _handle, true);
	//	else DrawCameraGraph(_pos.x,_pos.y,graphNum,0,16,40,8,20,3.0,0,_handle,true,_isLeft);//DrawRectExtendGraph(_pos.x, _pos.y, _pos.x + 64, _pos.y + 128, 16 * ((_walkFrame % 50) / 10), 0, 16, 40, _handle, true);
	//	_collider.Draw();
	//}
	//else DrawCameraGraph(_pos.x, _pos.y,0, 0, 26, 40,13,20, 3.0f, 90, _deadhandle, true,_isLeft);
}

void
GrabMan::Kill()
{
	_velocity.x = _isLeft ? 3.0f : -3.0f;
	_velocity.y = -3.0f;
	_collider.ToDisable();//_isCollidable = false;
	_isDead = true;
	ChangeState(state_dead);
}

void
GrabMan::NearUpdate()
{
	Vector2 vec = (_playerRef.GetCollider().Center() - _collider.Center());//normalize�ŕςȒl�ɂȂ��Ă�
	_velocity.x = vec.x > 0 ? 2 : -2;
	_pos += _velocity;
	_collider.SetCenter(_pos+Vector2(_cOffsetX,0));//_collider.pos = _pos;
	float x = _pos.x - 15;
	float colx = _collider.Left();
	_walkFrame++;
	_isLeft = _velocity.x < 0;
	

}
void 
GrabMan::FarUpdate()
{
	Vector2 vec = (_playerRef.GetCollider().Center() - _collider.Center());//normalize�ŕςȒl�ɂȂ��Ă�
	_velocity.x = vec.x > 0 ? 2 : -2;
	_pos += _velocity;
	_collider.SetCenter(_pos + Vector2(_cOffsetX, 0));//_collider.pos = _pos;
	_walkFrame++;
	_isLeft = _velocity.x < 0;
	
	if (fabs(vec.x) <= 128)
	{
		ChangeState(state_near);
	}
}
void
GrabMan::DeadUpdate()
{
	//DrawString(200, 200, "deadupdate", 0xffffff);
	_pos += _velocity;
	_velocity.y +=0.5f;
	if (_pos.y > 500) _isAvailable = false;
}
void 
GrabMan::GrabUpdate()
{
	
}

void
GrabMan::ArialUpdate()
{

}

void
GrabMan::ChangeState(State state)
{
	_stateFrame[_state] = 0;
	_state = state;
	_pFunc = _pfuncMap[_state];
}

void
GrabMan::DrawFar()
{
	DrawCameraGraph(_pos.x, _pos.y, 16 * ((_stateFrame[state_far] % 20) / 10), 0, 16, 40, 8, 20, 3.0, 0, _handle, true, _isLeft);
}
void 
GrabMan::DrawNear()
{
	DrawCameraGraph(_pos.x, _pos.y, 16 * (((_stateFrame[state_near] % 20) / 10)+2), 0, 16, 40, 8, 20, 3.0, 0, _handle, true, _isLeft);
}
void
GrabMan::DrawDead()
{
	DrawCameraGraph(_pos.x, _pos.y, 0, 0, 26, 40, 13, 20, 3.0f, 90, _deadhandle, true, _isLeft);
}
void 
GrabMan::DrawGrab()
{
	DrawCameraGraph(_pos.x, _pos.y, 16 * 4, 0, 16, 40, 8, 20, 3.0, 0, _handle, true, _isLeft);
}
void
GrabMan::DrawArial()
{
	DrawCameraGraph(_pos.x, _pos.y, 0, 0, 16, 40, 8, 20, 3.0, 0, _handle, true, _isLeft);
}


void
GrabMan::DrawCameraGraph(int x, int y, int srcX, int srcY, int width, int height, int cx, int cy, double extRate, double angle, int handle, int transFlag, int turnFlag)
{
	DrawRectRotaGraph2(x + _cameraRef.OffsetX(), y, srcX, srcY, width, height, cx, cy, extRate, angle, handle, transFlag, turnFlag);
}

void
GrabMan::Shaked()
{
	_shakedCnt++;
	if (_shakedCnt > 5){
		Kill();
		_isGrabbing = false;
	}
}
void
GrabMan::Reject(Vector2 vec)
{
	//_pos += vec;
	//_collider.SetCenter(_pos + Vector2(_cameraRef.OffsetX(), 0));
	//if (_velocity.y > 0.0f&&vec.y < 0.0f)//������ɉ����Ԃ��ꂽ�ꍇ
	//{
	//	ChangeState(ps_Walk);
	//	_velocity.Init();
	//	_acceleration.Init();
	//}
}