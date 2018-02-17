#include "GameServer.h"

using namespace Net;
using namespace Engine;

GameServer::GameServer(int port)
{
	m_port = port;
}

GameServer::~GameServer()
{
}

void GameServer::Initialize()
{
	WindowResources::GetGraphics()->GetTextureLibrary()->CreateTexture("PLANE_BG", "Assets/textures/grid_tile.png");
	int id = MeshLoader::CreatePlane(100, 100, 100, 100);
	m_plane.SetMesh(id);
	m_plane.SetTexture("PLANE_BG");

	//Create main character
	CharacterInfo info;
	info.Name = "Alfie";
	info.Speed = 0.25f;
	m_user = std::make_unique<Character>(info, "Assets/Models/body_plate_silver_epic.sef", "Assets/Models/head1.sef");
	m_user2 = std::make_unique<Character>(info, "Assets/Models/body_plate_silver_epic.sef", "Assets/Models/head1.sef");

	WindowResources::GetGraphics()->GetCamera()->AttachToModel(m_user->GetModel());

	m_server = std::make_unique<Server>();
	if (!m_server->Open(m_port))
	{
		WindowResources::GetConsole()->Log("Could not initialize the Server.");
		m_server = nullptr;
	}
	else
	{
		WindowResources::GetConsole()->Log("Server initialized with port 1234 ");
	}
}

void GameServer::Update(double elapsed)
{
	auto mouse = WindowResources::GetInput()->GetMouse();
	if (mouse->ButtonPressed(0))
	{
		auto mousePos = mouse->Get3DPosition();
		m_user->SetDestination(mousePos);

		if (!m_IDs.empty() && m_server)
		{
			ByteBuffer packet;
			packet.resize(sizeof(mousePos));
			memcpy(&packet[0], &mousePos, sizeof(mousePos));
			m_server->Send(m_IDs.at(0), packet);
		}
	}

	if (!m_IDs.empty() && m_server)
	{
		ByteBuffer packet;
		//Data was recieved
		if (m_server->Recieve(m_IDs.at(0), packet))
		{
			glm::vec3 position;
			memcpy(&position, packet.data(), sizeof(position));
			m_user2->SetDestination(position);
		}
	}

	m_user->Update(elapsed);
	m_user2->Update(elapsed);
	UpdateServer();
}

void GameServer::Render(const std::shared_ptr<Engine::GraphicsDevice>& graphics)
{
	graphics->Draw(m_plane);
	graphics->Draw(m_user->GetModel());
	graphics->Draw(m_user2->GetModel());
}

void GameServer::HandleControlls(double elapsed, const std::shared_ptr<Engine::Input>& input, const std::shared_ptr<Engine::Camera>& camera)
{
	//Keyboard
	auto keyboard = input->GetKeyboard();
	auto mouse = input->GetMouse();

	//Rotation
	float acceleration = 2.45f;
	double seconds = elapsed / 1000.0;
	float velocity = float(acceleration * seconds);

	if (keyboard->ButtonDown('W'))
	{
		camera->Rotate(velocity, 0.0f, 0.0f);
	}
	if (keyboard->ButtonDown('S'))
	{
		camera->Rotate(-velocity, 0.0f, 0.0f);
	}
	if (keyboard->ButtonDown('A'))
	{
		camera->Rotate(0.0f, velocity, 0.0f);
	}
	if (keyboard->ButtonDown('D'))
	{
		camera->Rotate(0.0f, -velocity, 0.0f);
	}
	if (keyboard->ButtonDown('Q'))
	{
		camera->Zoom(velocity * 100.0f);
	}
	if (keyboard->ButtonDown('E'))
	{
		camera->Zoom(-velocity * 100.0f);
	}
}

void GameServer::Close()
{
	WindowResources::GetGraphics()->GetTextureLibrary()->DeleteTexture("PLANE_BG");
	if (m_server)
	{
		m_server->CloseAllConnections();
		m_server.release();
	}
}

void GameServer::UpdateServer()
{
	if (m_server)
	{
		uint32_t ID;
		std::string newUsername;
		if (m_server->GetNewLoginRequest(ID, newUsername))
		{
			WindowResources::GetConsole()->Log(std::to_string(ID) + ": " + newUsername + " connected!");
			m_IDs.push_back(ID);

		}
		if (m_server->GetLogoutRequest(ID))
		{
			WindowResources::GetConsole()->Log(std::to_string(ID) + " disconnected!");

			for (int index = 0; index < m_IDs.size(); index++)
			{
				if (m_IDs.at(index) == ID)
				{
					m_IDs.erase(m_IDs.begin() + index);
				}
			}
		}
	}
}
