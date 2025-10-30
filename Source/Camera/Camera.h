#pragma once
#include "../precompiled.h"

	class Camera {
	public:
		Camera();
		~Camera();

	private:

		GW::MATH::GMATRIXF camMatrix;
		bool FreeLookEnabled = false;

	public:

		GW::MATH::GMATRIXF FreeLookCamera(GW::SYSTEM::GWindow _window, GW::MATH::GMATRIXF _viewStart = GW::MATH::GIdentityMatrixF);
	};
