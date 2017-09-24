#pragma once

namespace Lime
{
	class Xdata
	{
	public:

		Xdata(const bool intersects, const float distance);

		bool DoesIntersect();

		float GetDistance();

	private:
		bool m_Intersects;
		float m_Distance;
	};
}