#include "GrabMan.h"
#include"Player.h"
#include"Dxlib.h"
#include<math.h>
#include"Camera.h"

GrabMan::GrabMan(Vector2 pos, int handle, int deadHandle, Player& player, Camera& camera) :_playerRef(player), _cameraRef(camera)
{
	_collider = Collider(this, ct_grabMan, col_default);
	_pos = pos;
	_collider.SetRect(Rect(_pos, 29, 128));
	_velocity = Vector2(0, 0);
	_pFunc = &GrabMan::FarUpdate;
	_handle = handle;
	_deadhandle = deadHandle;
	_walkFrame = 0;

	_isGrab = false;
	_isNear = false;
	_isDead = false;
	_isLeft = false;

	_shakedCnt = 0;
}


GrabMan::~GrabMan()
{
	DeleteGraph(_handle);
}

void
GrabMan::OnCollided(Collider* col)
{
	if (col->Type() == col_attack&&col->CharaType()==ct_player){
		_pFunc = &GrabMan::DeadUpdate;
		_walkFrame = 0;
	}
	if (col->Type() == col_default&&col->CharaType() == ct_player){
		Player* p=dynamic_cast<Player*>(col->_gameObject);
		p->Grabbed(this);
		_isGrab = true;
		_pFunc = &GrabMan::GrabUpdate;
		_walkFrame = 0;
	}
}
void
GrabMan::OnCollided(GameObject* obj)
{
	if (obj->ColType() == col_attack&&obj->CharaType() == ct_player){
		_pFunc = &GrabMan::DeadUpdate;
		_walkFrame = 0;
	}
	if (obj->ColType() == col_default&&obj->CharaType() == ct_player){
		Player* p = dynamic_cast<Player*>(obj);
		p->Grabbed(this);
		_isGrab = true;
		_pFunc = &GrabMan::GrabUpdate;
		_walkFrame = 0;
	}
}

void
GrabMan::OnCollided(std::shared_ptr<GameObject> obj)
{

}



void
GrabMan::AliveUpdate()
{
	Vector2 vec = (_playerRef.GetCollider().Center()-_collider.Center());//normalize‚Å•Ï‚È’l‚É‚È‚Á‚Ä‚é
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
}

void 
GrabMan::Draw()
{
	if (!_isDead)
	{
		int graphNum = 0;//
		if (_isNear) graphNum = 16 * (((_walkFrame % 20) / 10) + 2);
		else graphNum = 16 * ((_walkFrame % 20) / 10);

		if (_isGrab)DrawCameraGraph(_pos.x,_pos.y,16*4,0,16,40,8,20,3.0,0,_handle,true,_isLeft);//DrawRectExtendGraph(_pos.x, _pos.y, _pos.x + 64, _pos.y + 128, 16 * 4, 0, 16, 40, _handle, true);
		else DrawCameraGraph(_pos.x,_pos.y,graphNum,0,16,40,8,20,3.0,0,_handle,true,_isLeft);//DrawRectExtendGraph(_pos.x, _pos.y, _pos.x + 64, _pos.y + 128, 16 * ((_walkFrame % 50) / 10), 0, 16, 40, _handle, true);
		_collider.Draw();
	}
	else DrawCameraGraph(_pos.x, _pos.y,0, 0, 26, 40,13,20, 3.0f, 90, _deadhandle, true,_isLeft);
}

void
GrabMan::Kill()
{
	_velocity.x = _isLeft ? 3.0f : -3.0f;
	_velocity.y = -3.0f;
	_isCollidable = false;
	_isDead = true;
	_pFunc = &GrabMan::DeadUpdate;
}

void
GrabMan::NearUpdate()
{
	Vector2 vec = (_playerRef.GetCollider().Center() - _collider.Center());//normalize‚Å•Ï‚È’l‚É‚È‚Á‚Ä‚é
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
	Vector2 vec = (_playerRef.GetCollider().Center() - _collider.Center());//normalize‚Å•Ï‚È’l‚É‚È‚Á‚Ä‚é
	_velocity.x = vec.x > 0 ? 2 : -2;
	_pos += _velocity;
	_collider.SetCenter(_pos + Vector2(_cOffsetX, 0));//_collider.pos = _pos;
	_walkFrame++;
	_isLeft = _velocity.x < 0;
	//‚±‚Ì‚Ö‚ñ‚â‚Á‚Ä‚é
	if (fabs(vec.x) <= 128)
	{
		_pFunc = &GrabMan::NearUpdate;
		_isNear = true;
	}
}
void
GrabMan::DeadUpdate()
{
	//DrawString(200, 200, "deadupdate", 0xffffff);
	_pos += _velocity;
	_velocity.y +=0.5f;
}
void 
GrabMan::GrabUpdate()
{
	
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
	}
}