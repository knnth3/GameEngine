#include "GameClient.h"

using namespace Net;
using namespace Engine;

GameClient::GameClient(const std::string& ipv4, int port)
{
	m_port = port;
	m_address = ipv4;
	m_loggedIn = false;
}

GameClient::~GameClient()
{
}

void GameClient::Initialize()
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

	m_client = std::make_unique<Client>();
	if (!m_client->Initialize(m_address, m_port))
	{
		WindowResources::GetConsole()->Log("Could not initialize the Client.");
		m_client = nullptr;
	}
	else
	{
		WindowResources::GetConsole()->Log("Client initialized with port 1234.");
		if (m_client->Login("Kenneth"))
		{
			WindowResources::GetConsole()->Log("Client login success as Kenneth.");
			m_loggedIn = true;
		}
		else
		{
			m_client = nullptr;
		}
	}
}

void GameClient::Update(double elapsed)
{
	auto mouse = WindowResources::GetInput()->GetMouse();
	if (mouse->ButtonPressed(0))
	{
		auto mousePos = mouse->Get3DPosition();
		m_user->SetDestination(mousePos);

		if (m_loggedIn)
		{
			ByteBuffer packet;
			packet.resize(sizeof(mousePos));
			memcpy(&packet[0], &mousePos, sizeof(mousePos));
			m_client->Send(packet);
		}

	}

	if (m_loggedIn)
	{
		ByteBuffer packet;
		//Data was recieved
		if (m_client->Recieve(packet))
		{
			glm::vec3 position;
			memcpy(&position, packet.data(), sizeof(position));
			m_user2->SetDestination(position);
		}
	}

	m_user->Update(elapsed);
	m_user2->Update(elapsed);

}

void GameClient::Render(const std::shared_ptr<Engine::GraphicsDevice>& graphics)
{
	graphics->Draw(m_plane);
	graphics->Draw(m_user->GetModel());
	graphics->Draw(m_user2->GetModel());
}

void GameClient::HandleControlls(double elapsed, const std::shared_ptr<Engine::Input>& input, const std::shared_ptr<Engine::Camera>& camera)
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

void GameClient::Close()
{
	WindowResources::GetGraphics()->GetTextureLibrary()->DeleteTexture("PLANE_BG");
	if (m_client)
	{
		m_client->Logout();
		m_client.release();
	}
}
