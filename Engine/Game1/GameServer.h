#pragma once
#include <NetModule\Server.h>
#include "GameState.h"

class GameServer:
	public GameState
{
public:
	GameServer(int port);
	~GameServer();

	virtual void Initialize()final;
	virtual void Update(double elapsed)final;
	virtual void Render(const std::shared_ptr<Engine::GraphicsDevice>& graphics)final;
	virtual void HandleControlls(double elapsed, const std::shared_ptr<Engine::Input>& input, const std::shared_ptr<Engine::Camera>& camera)final;
	virtual void Close()final;

private:
	void UpdateServer();

	int m_port;
	std::vector<uint32_t> m_IDs;
	std::unique_ptr<Character> m_user;
	std::unique_ptr<Character> m_user2;
	Engine::Model m_plane;
	std::unique_ptr<Net::Server> m_server;

};

