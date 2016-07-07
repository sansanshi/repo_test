#pragma once


class Scene;

class GameMain
{
private:
	GameMain();
	GameMain(const GameMain&);
	GameMain& operator =(const GameMain&);



	Scene* _scene = nullptr;

public:

	~GameMain();
	bool start;

	static GameMain& Instance(){
		static GameMain gameMain;
		return gameMain;
	}

	void GameLoop();

	void Terminate();

	void ChangeScene(Scene* scene);
};

