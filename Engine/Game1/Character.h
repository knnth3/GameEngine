#pragma once
#include "Engine\Model.h"

//uses SI units for measurement
struct CharacterInfo
{
	std::string Name;
	float Speed;
};

class Character
{
public:
	Character(const CharacterInfo& info);
	Character(const CharacterInfo& info, const std::string& sefPath, const std::string& sefPathHead = "");
	~Character();

	void SetInfo(const CharacterInfo& info);
	void SetDestination(const glm::vec3& position);
	void ToggleRun();
	void Update(double elapsed);

	const Engine::Model& GetModel()const;
	const std::string& GetName()const;

private:
	void SetAnimation();
	void Move(double elapsed);

	bool m_bMoving;
	bool m_bRunning;
	glm::vec3 m_destination;
	CharacterInfo m_info;
	std::string m_name;
	Engine::Model m_model;
};

