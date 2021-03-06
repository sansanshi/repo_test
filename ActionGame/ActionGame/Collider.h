#pragma once
class GameObject;

enum CharacterType{
	ct_none,
	ct_player,
	ct_grabMan,
	ct_knifeMan,
	ct_batman,
	ct_stage,

};
enum ColliderType{
	col_none,
	col_default,
	col_attack,
	col_stage,
};

#include "Geometry.h"
class Collider:public Rect//この継承よくわからん
{
private:
	bool _isCollidable;
	bool _isAvailable;
protected:
	//Rect _rect;
public:
	Collider();
	~Collider();
	Collider(GameObject* gameObj, CharacterType charaType, ColliderType colType) :_gameObject(gameObj), _charaType(charaType), _colType(colType){};

	//Rect GetRect();
	//virtual void OnCollided() =0;
	//virtual void OnCollided(Collider* col)=0 ;

	void SetRect(Rect rc);

	
	CharacterType _charaType;
	ColliderType _colType;
	GameObject* _gameObject;

	ColliderType Type(){ return _colType; }
	CharacterType CharaType(){ return _charaType; }
	bool IsAvailable(){ return _isAvailable; }
	bool IsCollidable(){ return _isCollidable; }

	void ToDisable();
	void ToEnable();

	void SetCenter_Cam(Vector2, float);//カメラから見た座標（カメラのoffsetを考慮した座標）にセット
};

