#pragma once
#include <NetModule\Client.h>
#include "GameState.h"

class GameClient :
	public GameState
{
public:
	GameClient(const std::string& ipv4, int port);
	~GameClient();

	virtual void Initialize()final;
	virtual void Update(double elapsed)final;
	virtual void Render(const std::shared_ptr<Engine::GraphicsDevice>& graphics)final;
	virtual void HandleControlls(double elapsed, const std::shared_ptr<Engine::Input>& input, const std::shared_ptr<Engine::Camera>& camera)final;
	virtual void Close()final;

private:

	bool m_loggedIn;
	int m_port;
	std::string m_address;
	std::unique_ptr<Character> m_user;
	std::unique_ptr<Character> m_user2;
	Engine::Model m_plane;
	std::unique_ptr<Net::Client> m_client;

};

