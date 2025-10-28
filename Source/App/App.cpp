#include "../precompiled.h"
#include "App.h"

App::App()
{
	if (!+ m_app.Create([]() -> int
		{
			return 0;
		})) {

			std::cout << "GApp created successfully." << std::endl;

		// main app loop
		while (m_state != APP_EXIT)
		{
			if (m_state == APP_RUNNING)
			{
				Run();
			}
		}


	}
}

App::~App()
{
}

// Any app-wide logic goes here
int App::Run()
{
	return 0;
}

// Any per-frame logic goes here
int App::Update()
{
	// are we in background?

	if (m_state == APP_BACKGROUND)
	{
		// sleep for background interval
		//GW::SYSTEM::GSystem::Sleep(static_cast<uint32_t>(m_backgroundInterval * 1000.0f));
	}

	/// TODO: implement per-frame logic

	return HandleUpdate();
}

// Any fixed-timestep logic goes here
int App::FixedUpdate()
{
	// are we in background?
	if (m_state == APP_BACKGROUND)
	{
		// sleep for background interval
		//GW::SYSTEM::GSystem::Sleep(static_cast<uint32_t>(m_backgroundInterval * 1000.0f));
	}

	//conform to fixed timestep
	/// TODO: implement fixed timestep logic
	return HandleFixedUpdate();
}

int App::HandleUpdate()
{
	/// TODO: handle updates and set m_updateStatus accordingly
	return m_updateStatus;
}

int App::HandleFixedUpdate()
{
	/// TODO: handle fixed updates and set m_fixedUpdateStatus accordingly
	return m_fixedUpdateStatus;
}
