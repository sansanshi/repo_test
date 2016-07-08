#include "KnifeMan.h"
#include"Player.h"
#include"Dxlib.h"
#include<math.h>
#include"Camera.h"

KnifeMan::KnifeMan(Vector2 pos, int handle, int deadHandle, Player& player, Camera& camera) :_playerRef(player), _cameraRef(camera)
{
	_collider = Collider(this, ct_knifeMan, col_default);
	_pos = pos;
	_collider.SetRect(Rect(_pos, 32, 128));
	_velocity = Vector2(0, 0);
	_pFunc = &KnifeMan::FarUpdate;
	_handle = handle;
	_deadhandle = deadHandle;
	_walkFrame = 0;

	_isNear = false;
	_isDead = false;
	_isLeft = false;

}


KnifeMan::~KnifeMan()
{
	DeleteGraph(_handle);
}

void
KnifeMan::OnCollided(Collider* col)
{
	if (col->Type() == col_attack&&col->CharaType() == ct_player){
		_pFunc = &KnifeMan::DeadUpdate;
		_walkFrame = 0;
	}
	if (col->Type() == col_default&&col->CharaType() == ct_player){

		Player* p = dynamic_cast<Player*>(col->_gameObject);
		
		_walkFrame = 0;
		_isCollidable = false;//‚±‚ê‚µ‚Æ‚©‚È‚¢‚Æ“–‚½‚Á‚Ä‚¢‚éŠÔ‰½“x‚àOnCollided‚ªŒÄ‚Î‚ê‚é		
	}
}
void
KnifeMan::OnCollided(GameObject* obj)
{
	if (obj->ColType() == col_attack&&obj->CharaType() == ct_player){
		_pFunc = &KnifeMan::DeadUpdate;
		_walkFrame = 0;
	}
	if (obj->ColType() == col_default&&obj->CharaType() == ct_player){
		Player* p = dynamic_cast<Player*>(obj);
		
	}
}

void
KnifeMan::OnCollided(std::shared_ptr<GameObject> obj)
{

}



void
KnifeMan::Update()
{
	_cOffsetX = _cameraRef.OffsetX();
	(this->*_pFunc)();
}

void
KnifeMan::Draw()
{
	DrawCameraGraph(_pos.x, _pos.y, 0, 0, 26, 40, 13, 20, 3.0f, 90, _deadhandle, true, _isLeft);
}

void
KnifeMan::Kill()
{
	_velocity.x = _isLeft ? 3.0f : -3.0f;
	_velocity.y = -3.0f;
	_isCollidable = false;
	_isDead = true;
	_pFunc = &KnifeMan::DeadUpdate;
}

void
KnifeMan::NearUpdate()
{
	Vector2 vec = (_playerRef.GetCollider().Center() - _collider.Center());//normalize‚Å•Ï‚È’l‚É‚È‚Á‚Ä‚é
	_velocity.x = vec.x > 0 ? 2 : -2;
	_pos += _velocity;
	_collider.SetCenter(_pos + Vector2(_cOffsetX, 0));//_collider.pos = _pos;
	float x = _pos.x - 15;
	float colx = _collider.Left();
	_walkFrame++;
	_isLeft = _velocity.x < 0;


}
void
KnifeMan::FarUpdate()
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
		_pFunc = &KnifeMan::NearUpdate;
		_isNear = true;
	}
}
void
KnifeMan::DeadUpdate()
{
	//DrawString(200, 200, "deadupdate", 0xffffff);
	_pos += _velocity;
	_velocity.y += 0.5f;
}


void
KnifeMan::DrawCameraGraph(int x, int y, int srcX, int srcY, int width, int height, int cx, int cy, double extRate, double angle, int handle, int transFlag, int turnFlag)
{
	DrawRectRotaGraph2(x + _cameraRef.OffsetX(), y, srcX, srcY, width, height, cx, cy, extRate, angle, handle, transFlag, turnFlag);
}

