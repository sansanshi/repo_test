#include "Player.h"
#include<DxLib.h>
#include"Camera.h"
#include"Enemy.h"
#include"GrabMan.h"
#include"PlayingScene.h"
//プレイヤーを2000とかかなり右のほうに置いておいて0に向かっていった方が作りやすい
//0からはスクロールしないようにする

#define M_PI 3.1415926535

Player::Player(Camera& camera, Stage& stage, PlayingScene& scene) :_cameraRef(camera), _stageRef(stage), _playingScene(scene)
{
	_collider = Collider(this, ct_player, col_default);
	_collider.ToEnable();

	_jumpPower = 10.0f;
	_pFunc = &Player::JumpUpdate;
	_pos = Vector2(640, 0);
	_velocity = Vector2(0, 0);
	_acceleration = Vector2(0, 0.2f);
	 _collider.SetRect(Rect(_pos,32, 128));
	//_handle = LoadGraph("img/neutral.png");
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
	_prevRejectY = false;

	_hpMax = 600;
	_hp = _hpMax;
	_attackDamage = 1;
	_walkSpd = 5.0f;

	_handleMap[ps_Neutral] = LoadGraph("img/sakuya/stand.png");
	_handleMap[ps_Jump] = LoadGraph("img/sakuya/jump.png");
	_handleMap[ps_Walk] = LoadGraph("img/sakuya/walk.png");
	_handleMap[ps_Punch] = LoadGraph("img/sakuya/punch.png");
	_handleMap[ps_Kick] = LoadGraph("img/sakuya/kick.png");
	_handleMap[ps_JumpKick] = LoadGraph("img/sakuya/jump_kick.png");
	_handleMap[ps_Crouch] = LoadGraph("img/sakuya/crouch.png");
	_handleMap[ps_CrouchKick] = LoadGraph("img/sakuya/crouch_kick.png");
	_handleMap[ps_CrouchPunch] = LoadGraph("img/sakuya/crouch_punch.png");
	_handleMap[ps_Kamae] = LoadGraph("img/sakuya/kamae.png");
	_handleMap[ps_CrouchKamae] = LoadGraph("img/sakuya/crouch_kamae.png");
	_handleMap[ps_Grabbed] = LoadGraph("img/sakuya/stand.png");
	_handleMap[ps_Dead] = LoadGraph("img/damage_bottom.png");
	_handleMap[ps_Spell] = LoadGraph("img/sakuya/spell_.png");

	_pFuncMap[ps_Neutral] = &Player::NeutralUpdate;
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
	_pFuncMap[ps_Dead] = &Player::DeadUpdate;
	_pFuncMap[ps_Spell] = &Player::SpellUpdate;

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
	_stateFrame[ps_Dead] = 0;
	_stateFrame[ps_Spell] = 0;


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
	_drawFuncMap[ps_Dead] = &Player::DrawDead;
	_drawFuncMap[ps_Spell] = &Player::DrawSpell;

	//attackCol 幅高さ決める　　プレイヤーのセンターからoffset isRightでoffset.xを変える
	_attackCol= Collider(this, ct_player, col_attack);
	_attackCol.ToEnable();
	_attackCol.width = 40;
	_attackCol.height = 30;
	_attackColOffset = _isRight ? Vector2(44, 10) : Vector2(-44, 10);

	_deadHandle = LoadGraph("img/sakuya/dead.png");
	_fragDrawer = new FragmentDrawer(_deadHandle);
	_fragDrawer->FragmentScalling(1.0f);

	_isAvailable = true;
	_isDead = false;

	_rejectCnt = 0;

	_upperVec = Vector2(0.0f, 1.0f);

	_modelHandle = MV1LoadModel("model/初音ミク.pmd");
	MV1SetScale(_modelHandle, VGet(8.0f, 8.0f, 8.0f));
	int materialNum = MV1GetMaterialNum(_modelHandle);
	for (int i = 0; i < materialNum; i++)
	{
		//MV1SetMaterialOutLineDotWidth(_modelHandle, i, 0.02);
		//MV1SetMaterialDifColor(_modelHandle, 1, GetColorF(0.0f, 0.5f, 0.0f, 1.0f));
	}

	int w, h, depth;
	GetScreenState(&w, &h, &depth);
	SetupCamera_Ortho(h);

	_animState = anim_idle;
	_anim = MV1AttachAnim(_modelHandle, _animState - 1);
	//_anim[anim_idle] = MV1AttachAnim(_modelHandle, 0);
	//_anim[anim_punch] = MV1AttachAnim(_modelHandle, 1);
	_animTime = 0;
	_animDuration = MV1GetAnimTotalTime(_modelHandle, 0);

	_prevIsRight = false;

	_rotMat = MGetRotY(DX_PI_F/2.0f);
	_scaleMat = MGetScale(VGet(8.0f, 8.0f, 8.0f));
	_transMat = MGetTranslate(VGet(100, 0, 0));

	_vertShaderHandle = LoadVertexShader("shader/mmdVS.vso");
	_pixelShaderHandle = LoadPixelShader("shader/mmdPS.pso");

	SetUseVertexShader(_vertShaderHandle);
	SetUsePixelShader(_pixelShaderHandle);
	int i=MV1SetMaterialDifColor(_modelHandle, 1, GetColorF(0.0f, 0.5f, 0.0f, 1.0f));
	int a = 0;
}

void
Player::ToLeftCulling()
{
	int meshNum = MV1GetMeshNum(_modelHandle);
	for (int i = 0; i < meshNum; i++)
	{
		if (MV1GetMeshBackCulling(_modelHandle, i) != DX_CULLING_NONE)
		{
			MV1SetMeshBackCulling(_modelHandle, i, DX_CULLING_LEFT);
		}
	}
	TurnModelX();
}
void
Player::ToRightCulling()
{
	int meshNum = MV1GetMeshNum(_modelHandle);
	for (int i = 0; i < meshNum; i++)
	{
		if (MV1GetMeshBackCulling(_modelHandle, i) != DX_CULLING_NONE)
		{
			MV1SetMeshBackCulling(_modelHandle, i, DX_CULLING_RIGHT);
		}
	}
	TurnModelX();
}

void
Player::TurnModelX()
{
	MATRIX mat;
	mat = MGetIdent();
	mat.m[0][0] = -1.0f;

	_scaleMat = MMult(_scaleMat, mat);

}


Player::~Player()
{
	DeleteGraph(_handle);

	for (auto& h : _handleMap)
	{
		DeleteGraph(h.second);
	}

	// 読み込んだ頂点シェーダーの削除
	DeleteShader(_vertShaderHandle);

	// 読み込んだピクセルシェーダーの削除
	DeleteShader(_pixelShaderHandle);

	// 読み込んだモデルの削除
	MV1DeleteModel(_modelHandle);
}

void
Player::Update() 
{
	_prevIsRight = _isRight;

	_rejectCnt = 0;
	_stateFrame[_state]++;
	_cOffsetX = _cameraRef.OffsetX();
	(this->*_pFunc)();//this->pFuncMap[_state]とかにすると毎回mapを見るので無駄？
	//if (_pos.y >= _groundZero&&_velocity.y > 0){}//地面についたかどうか
	if (_prevIsRight == true && _isRight == false) ToLeftCulling();
	if (_prevIsRight == false && _isRight == true) ToRightCulling();
	
}

void
Player::Draw()
{
	//DrawRectGraph(_pos.x, _pos.y, 0, 0, 32, 32, _handle, true, _isRight);
	//DrawExtendGraph(_pos.x, _pos.y, _pos.x + 32, _pos.y + 128, _handleMap[_state], true);
	//面倒なのでステートで分ける_
	(this->*_drawFuncMap[_state])();
	//_collider.Draw();
	//_attackCol.Draw(0xff000000);
	//DrawBox(0, 400, 640, 480, 0xffffff,false);

	FLOAT4 f4 = { 0.2f, 0.2f, 0.2f, 0.2f };
	
	//MV1SetPosition(_modelHandle, VGet(100.f, 0.f, 0.f));
	SetUseVertexShader(_vertShaderHandle);//ここで設定しとかないと他で設定したシェーダが適用されたりする
	SetUsePixelShader(_pixelShaderHandle);
	//SetVSConstF(1, f4);//渡してみたけど何も変わらない
	
	MATRIX mat = MMult(_rotMat,_scaleMat);
	mat = MMult(mat, _transMat);
	float r = 90 * M_PI / 180;

	
	MV1SetMatrix(_modelHandle,mat);//MV1SetRotationXYZ(_modelHandle, VGet(0,r,0));
	MV1SetUseOrigShader(TRUE);
	_animTime = _animTime > _animDuration ? 0 : _animTime + 1;
	MV1SetAttachAnimTime(_modelHandle, _anim, _animTime);
	MV1DrawModel(_modelHandle);
	MV1SetUseOrigShader(false);
}

void
Player::ChangeAnim(AnimState animState)
{
	_animTime = 0;
	MV1DetachAnim(_modelHandle, _anim);
	_animState = animState;
	_animDuration = MV1GetAnimTotalTime(_modelHandle, _animState-1);
	_anim = MV1AttachAnim(_modelHandle, _animState - 1);
}

void
Player::SetPos(Vector2 pos)
{
	_pos = pos;
	
}

void
Player::OnCollided(Collider* col)
{
	if (col->Type()==col_attack)
	{
		Damage(3);
		col->ToDisable();
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
	_attackColOffset = _isRight ? Vector2(64+_cOffsetX, -10) : Vector2(-64+_cOffsetX, -10);
	_attackCol.SetCenter(_pos + _attackColOffset);
	if (_stateFrame[ps_Kick] > 10)
	{
		//_pFunc = &Player::WalkUpdate;
		ChangeState(ps_Neutral);
		_kickInterval = 30;
		ChangeAnim(anim_idle);
	}
}

void
Player::JumpUpdate()
{
	_kickInterval = max(_kickInterval - 1, 0);
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
	_velocity.x = vx*3.0f;

	_pos += _velocity;
	_velocity += _acceleration;
	_collider.SetCenter(_pos+Vector2(_cameraRef.OffsetX(),0));;

	if (_key[KEY_INPUT_X])
	{
		if (_kickInterval == 0){
			ChangeState(ps_JumpKick);
		}
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
	
	if (_pos.y >= 550) Damage(100);
}

void
Player::PunchUpdate()
{
	_velocity.y = 3.0f;
	if (_stateFrame[ps_Punch] > 0)_attackCol.ToDisable();
	_attackColOffset = _isRight ? Vector2(42+_cOffsetX, -25) : Vector2(-42+_cOffsetX, -25);
	_attackCol.SetCenter(_pos + _attackColOffset);
	
	if (_stateFrame[ps_Punch] > 10) 
	{
		//_pFunc = &Player::WalkUpdate;
		ChangeState(ps_Neutral);
		ChangeAnim(anim_idle);
	}

	_pos += _velocity;
	_collider.SetCenter(_pos + Vector2(_cameraRef.OffsetX(), 0));
}


void
Player::WalkUpdate()
{
	
	_velocity.y = 3.0f;//これやってるの危ないかも

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
	
	if (vx == 0){
		ChangeState(ps_Neutral);
		ChangeAnim(anim_idle);
	}


	if (_key[KEY_INPUT_SPACE])
	{
		_velocity.y = -_jumpPower;
		_acceleration.y = 0.3f;
		//_pFunc = &Player::JumpUpdate;
		ChangeState(ps_Jump);
	}
	

	_velocity.x = vx*_walkSpd;

	_pos += _velocity;
	_velocity += _acceleration;

	_collider.SetCenter(_pos+Vector2(_cameraRef.OffsetX(),0));

	if (_key[KEY_INPUT_DOWN])
	{
		ChangeState(ps_Crouch);
		_collider.width = 48;
		_collider.height = 82;
		_collider.SetCenter(_pos + Vector2(0, 20));
		_velocity.x = 0;
	}
	if (_key[KEY_INPUT_Z])
	{
		_velocity.x = 0;
		ChangeState(ps_Punch);
		ChangeAnim(anim_punch);
	}
	if (_key[KEY_INPUT_X])
	{
		_velocity.x = 0;
		if (_kickInterval == 0)ChangeState(ps_Kamae);
		ChangeAnim(anim_kick);
	}
	if (_key[KEY_INPUT_A])
	{
		_velocity.x = 0;
		ChangeState(ps_Spell);
	}

	if (_prevRejectY == false)//playingSceneの方でUpdate→ステージとの押し戻し　の順で処理していること前提の処理
	{
		ChangeState(ps_Jump);
		_velocity.x = 0.f;
	}
	_prevRejectY = false;
}

void 
Player::NeutralUpdate()
{
	_velocity.y = 3.0f;//これやってるの危ないかも

	_walkFrame = vx != 0 ? _walkFrame + 1 : 0;
	_kickInterval = max(_kickInterval - 1, 0);

	GetHitKeyStateAll(_key);

	vx = vy = 0;
	if (_key[KEY_INPUT_RIGHT])
	{
		vx = 1;
		_isRight = true;
		ChangeState(ps_Walk);
		ChangeAnim(anim_walk);
	}
	if (_key[KEY_INPUT_LEFT])
	{
		vx = -1;
		_isRight = false;
		ChangeState(ps_Walk);
		ChangeAnim(anim_walk);
	}



	if (_key[KEY_INPUT_SPACE])
	{
		_velocity.y = -_jumpPower;
		_acceleration.y = 0.3f;
		//_pFunc = &Player::JumpUpdate;
		ChangeState(ps_Jump);
	}


	_velocity.x = vx*_walkSpd;
	_pos += _velocity;

	_collider.SetCenter(_pos + Vector2(_cameraRef.OffsetX(), 0));

	if (_key[KEY_INPUT_DOWN])
	{
		ChangeState(ps_Crouch);
		_collider.width = 48;
		_collider.height = 82;
		_collider.SetCenter(_pos + Vector2(0, 20));
		_velocity.x = 0;
	}
	if (_key[KEY_INPUT_Z])
	{
		_velocity.x = 0;
		ChangeState(ps_Punch);
		ChangeAnim(anim_punch);
	}
	if (_key[KEY_INPUT_X])
	{
		_velocity.x = 0;
		if (_kickInterval == 0)ChangeState(ps_Kamae);
		ChangeAnim(anim_kick);
	}
	if (_key[KEY_INPUT_A])
	{
		_velocity.x = 0;
		ChangeState(ps_Spell);
	}

	if (_prevRejectY == false)//playingSceneの方でUpdate→ステージとの押し戻し　の順で処理していること前提の処理
	{
		ChangeState(ps_Jump);
		_velocity.x = 0.f;
	}
	_prevRejectY = false;
}

void
Player::JumpKickUpdate()
{
	_attackColOffset = _isRight ? Vector2(58 + _cOffsetX, +16) : Vector2(-58 + _cOffsetX, +16);//攻撃判定
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
		ChangeState(ps_Neutral);
		_velocity = Vector2(0, 0);
		_acceleration = Vector2(0, 0);
	}
}

void
Player::CrouchUpdate()
{
	_velocity.y = 3.0f;
	if (_stateFrame[ps_CrouchPunch] > 0) _attackCol.ToDisable();

	
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
		ChangeState(ps_Neutral);
		_collider.SetCenter(_pos+Vector2(_cameraRef.OffsetX(),0));;
		_collider.width = 32;
		_collider.height = 128;
		/*if (_collider.Bottom() >= 400.0f) {
			_pos.y = 400.0 - _collider.height / 2;
			_collider.SetCenter(_pos+Vector2(_cameraRef.OffsetX(),0));;
		}*/
	};
	_pos += _velocity;
	_collider.SetCenter(_pos + Vector2(_cameraRef.OffsetX(), 0));

}
void
Player::DeadUpdate()
{
	if (_stateFrame[ps_Dead] >= 60)
	{
		_isDead = true;
	}
}

void
Player::SpellUpdate()
{
	_velocity.y = 3.0f;
	if (_stateFrame[ps_Spell] > 20)
	{
		if(_playingScene._isTimeMove)_playingScene.TimeStop();
		else if(_playingScene._isTimeStop) _playingScene.TimeMove();
		ChangeState(ps_Neutral);
	}

	_pos += _velocity;
	_collider.SetCenter(_pos + Vector2(_cameraRef.OffsetX(), 0));
}

void
Player::DrawDead()
{
	_fragDrawer->Draw();
}
void 
Player::CrouchKickUpdate()
{
	_velocity.y = 3.0f;
	_attackColOffset = _isRight ? Vector2(64 + _cOffsetX, 32) : Vector2(-64 + _cOffsetX, 32);
	_attackCol.SetCenter(_pos + _attackColOffset);

	if (_stateFrame[_state] >= 15)
	{
		ChangeState(ps_Crouch);
		_kickInterval = 30;
	}

	_pos += _velocity;
	_collider.SetCenter(_pos + Vector2(_cameraRef.OffsetX(), 0));
}
void
Player::CrouchPunchUpdate()
{
	_velocity.y = 3.0f;
	_attackColOffset = _isRight ? Vector2(64 + _cOffsetX, 0) : Vector2(-64 + _cOffsetX, 0);
	_attackCol.SetCenter(_pos + _attackColOffset);

	_stateFrame[_state]++;
	if (_stateFrame[_state] >= 15)
	{
		ChangeState(ps_Crouch);
	}
	_pos += _velocity;
	_collider.SetCenter(_pos + Vector2(_cameraRef.OffsetX(), 0));
}
void 
Player::KamaeUpdate()
{
	_velocity.y = 3.0f;
	if (_stateFrame[_state] >= 15)
	{
		ChangeState(ps_Kick);
	}
	_pos += _velocity;
	_collider.SetCenter(_pos + Vector2(_cameraRef.OffsetX(), 0));
}
void
Player::CrouchKamaeUpdate()
{
	_velocity.y = 3.0f;
	if (_stateFrame[_state] >= 10)
	{
		ChangeState(ps_CrouchKick);
	}
	_pos += _velocity;
	_collider.SetCenter(_pos + Vector2(_cameraRef.OffsetX(), 0));
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
		ChangeState(ps_Neutral);
	}

}

void
Player::DrawNeutral()
{
	DrawCameraGraph(_pos.x, _pos.y, 64 * ((_stateFrame[ps_Neutral]%60)/10), 0, 64, 128, 32, 64, 1.1, 0, _handleMap[_state], true, !_isRight);
}
void
Player::DrawKick()
{
	DrawCameraGraph(_pos.x, _pos.y, 0,0,128, 128,64,64, 1.1, 0, _handleMap[_state], true, !_isRight);
}
void 
Player::DrawPunch()
{
	DrawCameraGraph(_pos.x, _pos.y, 0,0,64, 128,32,64, 1.1, 0, _handleMap[_state], true, !_isRight);
}
void
Player::DrawWalk()
{
	//DrawCameraGraph(_pos.x, _pos.y, 16 * ((_walkFrame % 20) / 10), 0, 16, 40, 8, 20, 3.0, 0, _handleMap[_state], true, _isRight);
	DrawCameraGraph(_pos.x, _pos.y, 64 * ((_walkFrame % 90) / 10), 0, 64, 128, 32, 64, 1.1, 0, _handleMap[_state], true, !_isRight);//描画場所を変えてるだけなのでrectがついてきてない
}
void
Player::DrawJump()
{
	DrawCameraGraph(_pos.x, _pos.y, 0, 0, 128, 128, 64, 64, 1.1, 0, _handleMap[_state], true, !_isRight);
}
void
Player::DrawJumpKick()
{
	DrawCameraGraph(_pos.x, _pos.y, 0, 0, 128, 128, 64, 64, 1.1, 0, _handleMap[_state], true, !_isRight);
}
void
Player::DrawCrouch()
{
	//int w, h;//画像のサイズをとってきて自動で下に合わせている
	//GetGraphSize(_handleMap[_state], &w, &h);
	//DrawRotaGraph2(_rect.Center().x, 400 - 3.0*(h / 2), 8, 16, 3.0, 0, _handleMap[_state], true, _isRight);
	DrawCameraGraph(_pos.x, _pos.y-16, 0, 0, 128, 128, 64, 64, 1.1, 0, _handleMap[_state], true, !_isRight);
}
void
Player::DrawCrouchKick()
{
	DrawCameraGraph(_pos.x, _pos.y, 0, 0, 128, 64, 64, 32, 1.1, 0, _handleMap[_state], true, !_isRight);
}
void
Player::DrawCrouchPunch()
{
	DrawCameraGraph(_pos.x, _pos.y, 0, 0, 192, 64, 96, 32, 1.1, 0, _handleMap[_state], true, !_isRight);
}
void 
Player::DrawKamae()
{
	DrawCameraGraph(_pos.x, _pos.y, 0, 0, 64, 128, 32, 64, 1.1, 0, _handleMap[_state], true, !_isRight);
}
void
Player::DrawCrouchKamae()
{
	DrawCameraGraph(_pos.x, _pos.y, 0, 0, 128, 64, 64, 32, 1.1, 0, _handleMap[_state], true, !_isRight);
}
void
Player::DrawSpell()
{
	DrawCameraGraph(_pos.x, _pos.y , 64*(min(_stateFrame[ps_Spell],31)/8), 0, 64, 128, 32, 64, 1.1, 0, _handleMap[_state], true, !_isRight);
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
	if (_hp <= 0){
		Killed();
	}
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
	/*if (_collider.Bottom() >= 400.0f) {
		_pos.y = 400.0 - _collider.height / 2;
		_collider.SetCenter(_pos + Vector2(_cameraRef.OffsetX(), 0));;
	}*/

}

void 
Player::Reject(Vector2 vec)
{
	vec.y < 0 ? _prevRejectY = true : 0;//Updateの方で、この関数が前フレームに呼ばれたか
	_pos += vec;
	_collider.SetCenter(_pos + Vector2(_cameraRef.OffsetX(), 0));
	if ((_state==ps_Jump||_state==ps_JumpKick)&&vec.y<0)//ジャンプｏｒジャンプキック状態で上向きに押し返された場合←ここやめたほうがいいかも
	{
		ChangeState(ps_Neutral);
		_velocity.Init();
	}

	++_rejectCnt;
}

void
Player::Killed()
{
	_isAvailable = false;

	ChangeState(ps_Dead);
	_isRight ? _fragDrawer->TurnUV() : 0;//uv値のUだけ反転
	_fragDrawer->FragmentScalling(1.0f);//Scallingの方を先にする
	_fragDrawer->FragmentTranslation(Vector2(_collider.Left() - 64, _collider.Top()));
	_fragDrawer->Break();

	std::list<GrabMan*>::iterator it = _grabbingEnemies.begin();
	for (; it != _grabbingEnemies.end();)
	{
		(*it)->GrabRelease();
		++it;
	}

}

void
Player::Move(Vector2 vec)
{
	_pos += vec;
	_collider.SetCenter_Cam(_pos, _cameraRef.OffsetX());
}