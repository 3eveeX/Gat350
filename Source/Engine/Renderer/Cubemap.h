#pragma once

namespace neu {
	class Cubemap : public Texture {
	public:
		//CLASS_PROTOTYPE(Cubemap)

		bool Load(const std::string& filename);
	};
}