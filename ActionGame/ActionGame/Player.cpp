#include "Player.h"
#include<DxLib.h>
#include"Camera.h"
#include"Enemy.h"
#include"GrabMan.h"
//プレイヤーを2000とかかなり右のほうに置いておいて0に向かっていった方が作りやすい
//0からはスクロールしないようにする

Player::Player(Camera& camera, Stage& stage) :_cameraRef(camera), _stageRef(stage)
{
	_collider = Collider(this, ct_player, col_default);
	_collider.ToEnable();

	_jumpPower = 6.0f;
	_pFunc = &Player::JumpUpdate;
	_pos = Vector2(0, 0);
	_velocity = Vector2(0, 0);
	_acceleration = Vector2(0, 0.2f);
	 _collider.SetRect(Rect(_pos,32, 128));
	_handle = LoadGraph("img/neutral.png");
	_isRight = false;
	_walkFrame = 0;
	_kickingFrame = 0;
	_punchFrame = 0;
	_state = ps_Jump;
	_walkFrame = 0;
	_punchFrame = 0;
	_kickInterval = 0;
	_shakeCnt = 0;
	_isGrabbed = false;

	_hpMax = 600;
	_hp = _hpMax;
	_attackDamage = 1;

	_handleMap[ps_Neutral] = LoadGraph("img/neutral.png");
	_handleMap[ps_Jump] = LoadGraph("img/jump.png");
	_handleMap[ps_Walk] = LoadGraph("img/walk_.png");
	_handleMap[ps_Punch] = LoadGraph("img/punch_.png");
	_handleMap[ps_Kick] = LoadGraph("img/kick_.png");
	_handleMap[ps_JumpKick] = LoadGraph("img/jump_kick_.png");
	_handleMap[ps_Crouch] = LoadGraph("img/crouch_.png");
	_handleMap[ps_CrouchKick] = LoadGraph("img/crouch_kick_.png");
	_handleMap[ps_CrouchPunch] = LoadGraph("img/crouch_punch_.png");
	_handleMap[ps_Kamae] = LoadGraph("img/kamae.png");
	_handleMap[ps_CrouchKamae] = LoadGraph("img/crouch_kamae_.png");
	_handleMap[ps_Grabbed] = LoadGraph("img/walk_.png");
	_handleMap[ps_Dead] = LoadGraph("img/damage_bottom.png");

	_pFuncMap[ps_Neutral] = &Player::WalkUpdate;
	_pFuncMap[ps_Walk] = &Player::WalkUpdate;
	_pFuncMap[ps_Punch] = &Player::PunchUpdate;
	_pFuncMap[ps_Jump] = &Player::JumpUpdate;
	_pFuncMap[ps_Kick] = &Player::KickUpdate;
	_pFuncMap[ps_JumpKick] = &Player::JumpKickUpdate;
	_pFuncMap[ps_Crouch] = &Player::CrouchUpdate;
	_pFuncMap[ps_CrouchKick] = &Player::CrouchKickUpdate;
	_pFuncMap[ps_CrouchPunch] = &Player::CrouchPunchUpdate;
	_pFuncMap[ps_Kamae] = &Player::KamaeUpdate;
	_pFuncMap[ps_CrouchKamae] = &Player::CrouchKamaeUpdate;
	_pFuncMap[ps_Grabbed] = &Player::GrabbedUpdate;

	_stateFrame[ps_Neutral] = 0;
	_stateFrame[ps_Walk] = 0;
	_stateFrame[ps_Punch] = 0;
	_stateFrame[ps_Jump] = 0;
	_stateFrame[ps_Kick] = 0;
	_stateFrame[ps_JumpKick] = 0;
	_stateFrame[ps_Crouch] = 0;
	_stateFrame[ps_CrouchKick] = 0;
	_stateFrame[ps_CrouchPunch] = 0;
	_stateFrame[ps_Kamae] = 0;
	_stateFrame[ps_CrouchKamae] = 0;
	_stateFrame[ps_Grabbed] = 0;


	std::map<PlayerState, int>::iterator it = _stateFrame.begin();//mapのfirstはconstらしいので無理っぽい
	for (int i = 0; it != _stateFrame.end();)
	{
		//(*it).first = static_cast<PlayerState>(i);
		(*it).second = 0;
		++it;
	}


	_drawFuncMap[ps_Neutral] = &Player::DrawNeutral;
	_drawFuncMap[ps_Walk] = &Player::DrawWalk;
	_drawFuncMap[ps_Jump] = &Player::DrawJump;
	_drawFuncMap[ps_Punch] = &Player::DrawPunch;
	_drawFuncMap[ps_Kick] = &Player::DrawKick;
	_drawFuncMap[ps_JumpKick] = &Player::DrawJumpKick;
	_drawFuncMap[ps_Crouch] = &Player::DrawCrouch;
	_drawFuncMap[ps_CrouchKick] = &Player::DrawCrouchKick;
	_drawFuncMap[ps_CrouchPunch] = &Player::DrawCrouchPunch;
	_drawFuncMap[ps_Kamae] = &Player::DrawKamae;
	_drawFuncMap[ps_CrouchKamae] = &Player::DrawCrouchKamae;
	_drawFuncMap[ps_Grabbed] = &Player::DrawWalk;

	//attackCol 幅高さ決める　　プレイヤーのセンターからoffset isRightでoffset.xを変える
	_attackCol= Collider(this, ct_player, col_attack);
	_attackCol.ToEnable();
	_attackCol.width = 20;
	_attackCol.height = 20;
	_attackColOffset = _isRight ? Vector2(44, 10) : Vector2(-44, 10);
}


Player::~Player()
{
	DeleteGraph(_handle);

	for (auto& h : _handleMap)
	{
		DeleteGraph(h.second);
	}
}

void
Player::Update()
{
	_cOffsetX = _cameraRef.OffsetX();
	(this->*_pFunc)();//this->pFuncMap[_state]とかにすると毎回mapを見るので無駄？
	//if (_pos.y >= _groundZero&&_velocity.y > 0){}//地面についたかどうか
	_stateFrame[_state]++;
}

void
Player::Draw()
{
	//DrawRectGraph(_pos.x, _pos.y, 0, 0, 32, 32, _handle, true, _isRight);
	//DrawExtendGraph(_pos.x, _pos.y, _pos.x + 32, _pos.y + 128, _handleMap[_state], true);
	//面倒なのでステートで分ける_
	(this->*_drawFuncMap[_state])();
	_collider.Draw();
	_attackCol.Draw(0xff000000);
	//DrawBox(0, 400, 640, 480, 0xffffff,false);
}

void
Player::SetPos(Vector2 pos)
{
	_pos = pos;
	
}

void
Player::OnCollided(Collider* col)
{
	if (col->CharaType() == ct_grabMan)
	{
		
	}
}
void
Player::OnCollided(Enemy* enemy)//上のだと当たってきたenemyの方の関数が呼びづらかったのでエネミー用のOnCollided作った
{
	//enemy->Kill();
}



void 
Player::OnCollided(GameObject*)
{

}
void
Player::OnCollided(std::shared_ptr<GameObject>)
{

}

void
Player::GroundUpdate()
{
	GetHitKeyStateAll(_key);

	vx = vy = 0;
	if (_key[KEY_INPUT_RIGHT])
	{
		vx = 1;
		_isRight = true;
	}
	if (_key[KEY_INPUT_LEFT])
	{
		vx = -1;
		_isRight = false;
	}


	if (_key[KEY_INPUT_SPACE])
	{
		_velocity.y -= _jumpPower;
		_acceleration.y = 0.2f;
		_pFunc = &Player::JumpUpdate;
	}
	

	_velocity.x = vx*2.0f;

	_pos += _velocity;
	_velocity += _acceleration;

	if (_collider.Bottom() >= 400.0f) _pos.y =400.0-_collider.height/2;//_posが真ん中になってるので/2している

	_collider.SetCenter(_pos+Vector2(_cameraRef.OffsetX(),0));
}

void
Player::KickUpdate()
{
	_attackColOffset = _isRight ? Vector2(64+_cOffsetX, -30) : Vector2(-64+_cOffsetX, -30);
	_attackCol.SetCenter(_pos + _attackColOffset);
	if (_stateFrame[ps_Kick] > 10)
	{
		//_pFunc = &Player::WalkUpdate;
		ChangeState(ps_Walk);
		_kickInterval = 30;
	}
}

void
Player::JumpUpdate()
{
	_kickInterval = max(_kickInterval - 1, 0);
	GetHitKeyStateAll(_key);

	_pos += _velocity;
	_velocity += _acceleration;
	_collider.SetCenter(_pos+Vector2(_cameraRef.OffsetX(),0));;

	if (_key[KEY_INPUT_X])
	{
		if (_kickInterval == 0)ChangeState(ps_JumpKick);
	}
	//ここ
	//if (_collider.Bottom() >= 400.0f) {
	//	_pos.y = 400.0 - _collider.height/2;
	//	_collider.SetCenter(_pos+Vector2(_cameraRef.OffsetX(),0));;
	//	//_pFunc = &Player::WalkUpdate;
	//	ChangeState(ps_Walk);
	//	//_stateFrame[ps_Jump] = 0;
	//	_velocity = Vector2(0, 0);
	//	_acceleration = Vector2(0, 0);
	//}
	
}

void
Player::PunchUpdate()
{
	if (_stateFrame[ps_Punch] > 0)_attackCol.ToDisable();
	_attackColOffset = _isRight ? Vector2(42+_cOffsetX, -25) : Vector2(-42+_cOffsetX, -25);
	_attackCol.SetCenter(_pos + _attackColOffset);
	
	if (_stateFrame[ps_Punch] > 10) 
	{
		//_pFunc = &Player::WalkUpdate;
		ChangeState(ps_Walk);
	}
}


void
Player::WalkUpdate()
{
	_velocity.y = 5.0f;//これやってるの危ないかも

	_walkFrame=vx!=0?_walkFrame+1:0;
	_kickInterval = max(_kickInterval-1, 0);
	
	GetHitKeyStateAll(_key);

	vx = vy = 0;
	if (_key[KEY_INPUT_RIGHT])
	{
		vx = 1;
		_isRight = true;
	}
	if (_key[KEY_INPUT_LEFT])
	{
		vx = -1;
		_isRight = false;
	}
	if (_key[KEY_INPUT_DOWN])
	{
		ChangeState(ps_Crouch);
		_collider.width = 48;
		_collider.height = 82;
		_collider.SetCenter(_pos + Vector2(0, 20));
	}


	if (_key[KEY_INPUT_SPACE])
	{
		_velocity.y = -_jumpPower;
		_acceleration.y = 0.3f;
		//_pFunc = &Player::JumpUpdate;
		ChangeState(ps_Jump);
	}
	if (_key[KEY_INPUT_Z])
	{
		ChangeState(ps_Punch);
		
	}
	if (_key[KEY_INPUT_X])
	{
		if(_kickInterval==0)ChangeState(ps_Kamae);
	}

	_velocity.x = vx*2.0f;

	_pos += _velocity;
	_velocity += _acceleration;

	_collider.SetCenter(_pos+Vector2(_cameraRef.OffsetX(),0));
}

void 
Player::NeutralUpdate()
{

}

void
Player::JumpKickUpdate()
{
	_attackColOffset = _isRight ? Vector2(58 + _cOffsetX, -32) : Vector2(-58 + _cOffsetX, -32);//攻撃判定
	_attackCol.SetCenter(_pos + _attackColOffset);
	

	_stateFrame[_state]++;
	_pos += _velocity;
	_velocity += _acceleration;
	_collider.SetCenter(_pos+Vector2(_cameraRef.OffsetX(),0));;
	if (_stateFrame[_state] >= 40)
	{
		ChangeState(ps_Jump);
		_kickInterval = 30;
	}

	if (_collider.Bottom() >= 400.0f) {
		_pos.y = 400.0 - _collider.height / 2;
		_collider.SetCenter(_pos+Vector2(_cameraRef.OffsetX(),0));;
		ChangeState(ps_Walk);
		_velocity = Vector2(0, 0);
		_acceleration = Vector2(0, 0);
	}
}

void
Player::CrouchUpdate()
{
	if (_stateFrame[ps_CrouchPunch] > 0) _attackCol.ToDisable();

	_collider.SetCenter(_pos + Vector2(_cameraRef.OffsetX(), 20));
	_kickInterval = max(_kickInterval - 1, 0);

	
	GetHitKeyStateAll(_key);

	if (_key[KEY_INPUT_LEFT])_isRight = false;
	if (_key[KEY_INPUT_RIGHT])_isRight = true;

	if (_key[KEY_INPUT_Z])
	{
		ChangeState(ps_CrouchPunch);
	}
	if (_key[KEY_INPUT_X])
	{
		if (_kickInterval == 0)ChangeState(ps_CrouchKamae);
	}

	if (!_key[KEY_INPUT_DOWN])
	{
		ChangeState(ps_Walk);
		_collider.SetCenter(_pos+Vector2(_cameraRef.OffsetX(),0));;
		_collider.width = 32;
		_collider.height = 128;
		if (_collider.Bottom() >= 400.0f) {
			_pos.y = 400.0 - _collider.height / 2;
			_collider.SetCenter(_pos+Vector2(_cameraRef.OffsetX(),0));;
		}
	};
}
void 
Player::CrouchKickUpdate()
{
	_attackColOffset = _isRight ? Vector2(64 + _cOffsetX, 52) : Vector2(-64 + _cOffsetX, 52);
	_attackCol.SetCenter(_pos + _attackColOffset);

	_stateFrame[_state]++;
	if (_stateFrame[_state] >= 30)
	{
		ChangeState(ps_Crouch);
		_kickInterval = 30;
	}
}
void
Player::CrouchPunchUpdate()
{
	_attackColOffset = _isRight ? Vector2(48 + _cOffsetX, 0) : Vector2(-48 + _cOffsetX, 0);
	_attackCol.SetCenter(_pos + _attackColOffset);

	_stateFrame[_state]++;
	if (_stateFrame[_state] >= 30)
	{
		ChangeState(ps_Crouch);
	}
}
void 
Player::KamaeUpdate()
{
	_stateFrame[_state]++;
	if (_stateFrame[_state] >= 30)
	{
		ChangeState(ps_Kick);
	}
}
void
Player::CrouchKamaeUpdate()
{
	_stateFrame[_state]++;
	if (_stateFrame[_state] >= 20)
	{
		ChangeState(ps_CrouchKick);
	}
}

void
Player::GrabbedUpdate()
{
	bool isPressedRight = _key[KEY_INPUT_RIGHT];
	bool isPressedLeft = _key[KEY_INPUT_LEFT];
	GetHitKeyStateAll(_key);
	if (_key[KEY_INPUT_LEFT])_isRight = false;
	if (_key[KEY_INPUT_RIGHT])_isRight = true;

	
	if (!isPressedRight&&_key[KEY_INPUT_RIGHT]){
		for (auto grabman : _grabbingEnemies)
		{
			grabman->Shaked();
		}
	}
	if (!isPressedLeft&&_key[KEY_INPUT_LEFT]){
		for (auto grabman : _grabbingEnemies)
		{
			grabman->Shaked();
		}
	}
	int num = _grabbingEnemies.size();
	//不要なもの削除ループ
	_grabbingEnemies.remove_if(
		[](GrabMan* e){//ラムダ式が使われている
		return !e->IsGrabbing();
	});

	if (num > _grabbingEnemies.size())
	{
		std::list<GrabMan*>::iterator it = _grabbingEnemies.begin();
		for (; it != _grabbingEnemies.end();)
		{
			(*it)->GrabRelease();
			++it;
		}
	}

	if (_grabbingEnemies.size() == 0){
		ChangeState(ps_Walk);
	}

}

void
Player::DrawNeutral()
{
	DrawCameraGraph(_pos.x, _pos.y, 16 * 0, 0, 16, 40, 8, 20, 3.0, 0, _handleMap[_state], true, _isRight);
}
void
Player::DrawKick()
{
	DrawCameraGraph(_pos.x, _pos.y, 0,0,50, 40,25,20, 3.0, 0, _handleMap[_state], true, _isRight);
}
void 
Player::DrawPunch()
{
	DrawCameraGraph(_pos.x, _pos.y, 0,0,32, 40,16,20, 3.0, 0, _handleMap[_state], true, _isRight);
}
void
Player::DrawWalk()
{
	//DrawCameraGraph(_pos.x, _pos.y, 16 * ((_walkFrame % 20) / 10), 0, 16, 40, 8, 20, 3.0, 0, _handleMap[_state], true, _isRight);
	DrawCameraGraph(_pos.x, _pos.y, 16 * ((_walkFrame % 20) / 10), 0, 16, 40, 8, 20, 3.0, 0, _handleMap[_state], true, _isRight);//描画場所を変えてるだけなのでrectがついてきてない
}
void
Player::DrawJump()
{
	DrawCameraGraph(_pos.x, _pos.y, 0, 0, 16, 40, 8, 20, 3.0, 0, _handleMap[_state], true, _isRight);
}
void
Player::DrawJumpKick()
{
	DrawCameraGraph(_pos.x, _pos.y, 0, 0, 38, 44, 19, 22, 3.0, 0, _handleMap[_state], true, _isRight);
}
void
Player::DrawCrouch()
{
	//int w, h;//画像のサイズをとってきて自動で下に合わせている
	//GetGraphSize(_handleMap[_state], &w, &h);
	//DrawRotaGraph2(_rect.Center().x, 400 - 3.0*(h / 2), 8, 16, 3.0, 0, _handleMap[_state], true, _isRight);
	DrawCameraGraph(_pos.x, _pos.y, 0, 0, 16, 40, 8, 20, 3.0, 0, _handleMap[_state], true, _isRight);
}
void
Player::DrawCrouchKick()
{
	DrawCameraGraph(_pos.x, _pos.y, 0, 0, 50, 40, 25, 20, 3.0, 0, _handleMap[_state], true, _isRight);
}
void
Player::DrawCrouchPunch()
{
	DrawCameraGraph(_pos.x, _pos.y, 0, 0, 40, 40, 20, 20, 3.0, 0, _handleMap[_state], true, _isRight);
}
void 
Player::DrawKamae()
{
	DrawCameraGraph(_pos.x, _pos.y, 0, 0, 16, 40, 8, 20, 3.0, 0, _handleMap[_state], true, _isRight);
}
void
Player::DrawCrouchKamae()
{
	DrawCameraGraph(_pos.x, _pos.y, 0, 0, 22, 40, 11, 20, 3.0, 0, _handleMap[_state], true, _isRight);
}

void
Player::ChangeState(PlayerState state)
{
	if (state == ps_Grabbed)
	{
		int a;
		a = 1;
	}
	_stateFrame[_state] = 0;//	今までのステートのフレームは0にする
	_state = state;
	_pFunc = _pFuncMap[_state];
	if (_state == ps_CrouchKick || _state == ps_CrouchPunch || _state == ps_JumpKick || _state == ps_Punch || _state == ps_Kick) _attackCol.ToEnable();
	else _attackCol.ToDisable();
}

void
Player::DrawCameraGraph(int x,int y,int srcX,int srcY,int width,int height,int cx,int cy,double extRate,double angle,int handle,int transFlag,int turnFlag)
{
	DrawRectRotaGraph2(x+_cameraRef.OffsetX(),y,srcX,srcY,width,height,cx,cy,extRate,angle,handle,transFlag,turnFlag);
}

void
Player::Damage(int value)
{
	_hp =max(_hp-value,0);
	if (_hp <= 0) _isAvailable = false;
}

void
Player::Grabbed(GrabMan* enemy)
{
	_grabbingEnemies.push_back(enemy);
	ChangeState(ps_Grabbed);
	_pos = Vector2(_pos.x, enemy->GetPos().y);
	_collider.SetCenter(_pos + Vector2(_cameraRef.OffsetX(), 0));;
	_collider.width = 32;
	_collider.height = 128;
	if (_collider.Bottom() >= 400.0f) {
		_pos.y = 400.0 - _collider.height / 2;
		_collider.SetCenter(_pos + Vector2(_cameraRef.OffsetX(), 0));;
	}

}

void 
Player::Reject(Vector2 vec)
{
	_pos += vec;
	_collider.SetCenter(_pos + Vector2(_cameraRef.OffsetX(), 0));
	if (_state==ps_Jump||_state==ps_JumpKick)//上向きに押し返された場合←ここやめたほうがいいかも
	{
		ChangeState(ps_Walk);
		_velocity.Init();
		_acceleration.Init();
	}
}

void
Player::Killed()
{

}