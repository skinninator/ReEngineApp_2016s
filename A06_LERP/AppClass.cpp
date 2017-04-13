#include "AppClass.h"
#include <vector>

void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Assignment  06 - LERP"); // Window Name
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), ZERO_V3, REAXISY);

	// Color of the screen
	m_v4ClearColor = vector4(REBLACK, 1); // Set the clear color to black

	m_pMeshMngr->LoadModel("Sorted\\WallEye.bto", "WallEye");

	fDuration = 1.0f;
}

void AppClass::Update(void)
{
#pragma region Does not change anything here
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
#pragma region

#pragma region Does not need changes but feel free to change anything here
	//Lets us know how much time has passed since the last call
	float fTimeSpan = m_pSystem->LapClock(); //Delta time (between frame calls)

	//cumulative time
	static float fRunTime = 0.0f; //How much time has passed since the program started
	fRunTime += fTimeSpan; 
#pragma endregion

#pragma region Your Code goes here
	//list of locations
	std::vector<vector3> locations;
	locations.push_back(vector3(-4.0f, -2.0f, 5.0f));
	locations.push_back(vector3(1.0f, -2.0f, 5.0f));
	locations.push_back(vector3(-3.0f, -1.0f, 3.0f));
	locations.push_back(vector3(2.0f, -1.0f, 3.0f));
	locations.push_back(vector3(-2.0f, 0.0f, 0.0f));
	locations.push_back(vector3(3.0f, 0.0f, 0.0f));
	locations.push_back(vector3(-1.0f, 1.0f,-3.0f));
	locations.push_back(vector3(4.0f, 1.0f, -3.0f));
	locations.push_back(vector3(0.0f, 2.0f, -5.0f));
	locations.push_back(vector3(5.0f, 2.0f, -5.0f));
	locations.push_back(vector3(1.0f, 3.0f, -5.0f));

	int i = 0;
#pragma endregion

#pragma region Does not need changes but feel free to change anything here
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine("");//Add a line on top
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);

	//timer
	static DWORD startTimeSystem = GetTickCount(); //get system start time
	DWORD timeApplication = GetTickCount() - startTimeSystem; //get current time and substract start time
	float timer = timeApplication / 1000.0f; //change from milliseconds to seconds
	m_pMeshMngr->PrintLine("Time is: " + std::to_string(timer)); //write to screen
	
	//Mapping values
	matrix4 m4WallEye;
	float timerMapped = MapValue(fRunTime, 0 + (i * fTimeSpan), 1.0f, fTimeSpan + (i * fTimeSpan), 1.0f);
	while(timerMapped > i + 1) {
		if (i == 11) {
			i = 0;
			timerMapped = 0;
		}
		i++;
	}
	
	//Lerp Function
	vector3 v3Lerp = glm::lerp(locations[i], locations[i + 1], timerMapped);
	m4WallEye = glm::translate(v3Lerp);

	m_pMeshMngr->PrintLine(std::to_string(fRunTime));
	m_pMeshMngr->PrintLine(std::to_string(timerMapped));
	m_pMeshMngr->PrintLine(std::to_string(i));
	m_pMeshMngr->PrintLine(glm::to_string(locations[i]));
	m_pMeshMngr->SetModelMatrix(m4WallEye, "WallEye");

	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
#pragma endregion
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();
	//Render the grid based on the camera's mode:
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}