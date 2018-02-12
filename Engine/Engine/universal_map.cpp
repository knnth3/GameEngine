#include "universal_map.h"

#define KEY_SIZE_LIMIT 10

using namespace Engine;
using namespace std;

bool Engine::universal_map::Insert(const std::string & key, const universal_struct & addon)
{
	string upercase = To_upper(key);
	if(upercase.size() > KEY_SIZE_LIMIT)
		upercase.resize(10);

	auto found = m_internal_data.find(upercase);
	if (found == m_internal_data.end())
	{
		m_internal_data.emplace(upercase, addon);
		return true;
	}

	return false;
}

const universal_struct* Engine::universal_map::At(const std::string & key) const
{
	string upercase = To_upper(key);
	if (upercase.size() > KEY_SIZE_LIMIT)
		upercase.resize(10);

	auto found = m_internal_data.find(upercase);
	if (found != m_internal_data.end())
		return &found->second;

	return nullptr;
}

const universal_struct * Engine::universal_map::operator[](const std::string & key) const
{
	return At(key);
}

void Engine::universal_map::Remove(const std::string & key)
{
	string upercase = To_upper(key);
	if (upercase.size() > KEY_SIZE_LIMIT)
		upercase.resize(10);

	auto found = m_internal_data.find(upercase);
	if (found != m_internal_data.end())
		m_internal_data.erase(upercase);
}

void Engine::universal_map::Clear()
{
	m_internal_data.clear();
}

std::vector<std::string> Engine::universal_map::List()
{
	vector<string> out;
	for (auto& x : m_internal_data)
		out.push_back(x.first);

	return out;
}
