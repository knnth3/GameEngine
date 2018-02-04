#pragma once
#include "DataTypes.h"

namespace SEF
{
	struct AnimationKey
	{
		float Timestamp;
		Vector3<float> Position;
		Vector4<float> Quartenion;
	};

	//Assumes time in seconds
	class Animation
	{
	public:
		Animation();
		Animation(uint16_t timestamps);
		int JointCount();
		int TimeStampCount();
		float GetDuration();
		const std::vector<std::string> JointNames();
		const std::vector<AnimationKey>* At(const std::string& name)const;
		const std::vector<AnimationKey>* At(const int index)const;

		const std::vector<AnimationKey>* operator[](const std::string& name)const;
		const std::vector<AnimationKey>* operator[](const int index)const;

		bool AddCluster(const std::string& name, const std::vector<AnimationKey>& keyCluster);
		void SetTimestampCount(uint16_t amount);

	private:
		float m_duration;
		uint16_t m_timestampCount;
		std::vector<std::string> m_jointNames;
		std::vector<std::vector<AnimationKey>> m_keyClusters;
	};

	inline Animation::Animation()
	{
		m_timestampCount = 0;
	}

	inline Animation::Animation(uint16_t timestamps)
	{
		m_timestampCount = timestamps;
	}

	inline int Animation::JointCount()
	{
		return (int)m_jointNames.size();
	}

	inline int Animation::TimeStampCount()
	{
		return m_timestampCount;
	}

	inline float Animation::GetDuration()
	{
		return m_duration;
	}

	inline const std::vector<std::string> Animation::JointNames()
	{
		return m_jointNames;
	}

	inline const std::vector<AnimationKey>* Animation::At(const std::string & name) const
	{
		for (int x = 0; x < m_jointNames.size(); x++)
		{
			if (m_jointNames[x].compare(name))
			{
				return &m_keyClusters[x];
			}
		}
		return nullptr;
	}

	inline const std::vector<AnimationKey>* Animation::operator[](const std::string & name) const
	{
		return At(name);
	}

	inline const std::vector<AnimationKey>* Animation::At(const int index) const
	{
		return &m_keyClusters[index];
	}

	inline const std::vector<AnimationKey>* Animation::operator[](const int index) const
	{
		return At(index);
	}

	inline bool Animation::AddCluster(const std::string& name, const std::vector<AnimationKey>& keyCluster)
	{
		if (keyCluster.size() != m_timestampCount)
			return false;

		if (m_keyClusters.empty())
			m_duration = keyCluster.back().Timestamp;

		m_jointNames.push_back(name);
		m_keyClusters.push_back(keyCluster);
		return true;
	}
	inline void Animation::SetTimestampCount(uint16_t amount)
	{
		if (!m_timestampCount)
			m_timestampCount = amount;
	}
}