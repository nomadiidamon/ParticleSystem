#pragma once
#include "../../precompiled.h"

	class Camera {
	public:
		Camera();
		Camera(GW::MATH::GMATRIXF initialMatrix);
		~Camera();

		GW::MATH::GMATRIXF camMatrix;
	private:

		bool FreeLookEnabled = false;

	public:
		void EnableFreeLook(bool _enable);
		GW::MATH::GMATRIXF FreeLookCamera(GW::SYSTEM::GWindow _window, GW::MATH::GMATRIXF _viewStart = GW::MATH::GIdentityMatrixF);
	};
