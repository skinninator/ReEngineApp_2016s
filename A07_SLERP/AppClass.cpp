#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("SLERP - YOUR USER NAME GOES HERE"); // Window Name

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);
}

void AppClass::InitVariables(void)
{
	//Setting the position in which the camera is looking and its interest point
	m_pCameraMngr->SetPositionTargetAndView(vector3(12.12f, 28.52f, 11.34f), ZERO_V3, REAXISY);

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);

	//Loading the models
	m_pMeshMngr->LoadModel("Planets\\00_Sun.obj", "Sun");
	m_pMeshMngr->LoadModel("Planets\\03_Earth.obj", "Earth");
	m_pMeshMngr->LoadModel("Planets\\03A_Moon.obj", "Moon");

	//Setting the days duration
	m_fDay = 1.0f;
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();

	//Getting the time between calls
	double fCallTime = m_pSystem->LapClock();
	//Counting the cumulative time
	static double fRunTime = 0.0f;
	fRunTime += fCallTime;

	//Earth Orbit
	double fEarthHalfOrbTime = 182.5f * m_fDay; //Earths orbit around the sun lasts 365 days / half the time for 2 stops
	float fEarthHalfRevTime = 0.5f * m_fDay; // Move for Half a day
	float fMoonHalfOrbTime = 14.0f * m_fDay; //Moon's orbit is 28 earth days, so half the time for half a route

	//Setting the matrices
	m_pMeshMngr->SetModelMatrix(IDENTITY_M4, "Sun");
	m_pMeshMngr->SetModelMatrix(IDENTITY_M4, "Earth");
	m_pMeshMngr->SetModelMatrix(IDENTITY_M4, "Moon");

	//set earth and moon distances
	matrix4 distanceEarth = glm::translate(11.0f, 0.0f, 0.0f);
	matrix4 distanceMoon = glm::translate(2.0f, 0.0f, 0.0f);

	//set sun/earth/moon sizes
	matrix4 sizeSun = glm::scale(vector3(5.936f, 5.936f, 5.936f));
	matrix4 sizeEarth = glm::scale(vector3(.524f, .524f, .524f));
	matrix4 sizeMoon = glm::scale(vector3(.27f, .27f, .27f));

	//SLERP Code
	//earth rotation around sun
	glm::quat earthQ1 = glm::angleAxis(0.0f, vector3(0.0f, 1.0f, 0.0f));
	glm::quat earthQ2 = glm::angleAxis(180.0f, vector3(0.0f, 1.0f, 0.0f));
	glm::quat earthQ3;

	//earth rotation on itself
	glm::quat earthQ4 = glm::angleAxis(0.0f, vector3(0.0f, 1.0f, 0.0f));
	glm::quat earthQ5 = glm::angleAxis(180.0f, vector3(0.0f, 1.0f, 0.0f));
	glm::quat earthQ6;

	//moon rotation around earth
	glm::quat moonQ1 = glm::angleAxis(0.0f, vector3(0.0f, 1.0f, 0.0f));
	glm::quat moonQ2 = glm::angleAxis(180.0f, vector3(0.0f, 1.0f, 0.0f));
	glm::quat moonQ3;

	//earth to 365 days
	float earthMapped = MapValue((float)fRunTime, 0.0f, (float)fEarthHalfOrbTime, 0.0f, 1.0f);
	//earth to 1 day
	float earthDayMapped = MapValue((float)fRunTime, 0.0f, fEarthHalfRevTime, 0.0f, 1.0f);
	//moon to 28 days
	float moonMapped = MapValue((float)fRunTime, 0.0f, fMoonHalfOrbTime, 0.0f, 1.0f);

	//mix needed quaternions
	earthQ3 = glm::mix(earthQ1, earthQ2, earthMapped);
	earthQ6 = glm::mix(earthQ4, earthQ5,  earthDayMapped);
	moonQ3 = glm::mix(moonQ1, moonQ2, moonMapped);

	//calculate earth rotation around sun
	matrix4 earthFinal;
	earthFinal = ToMatrix4(earthQ3) * distanceEarth  * sizeEarth;

	//calculate moon location and rotation around earth
	matrix4 moonFinal;
	moonFinal = earthFinal * ToMatrix4(moonQ3) * distanceMoon * sizeMoon;

	//calculate earth rotation on itself
	earthFinal = earthFinal * ToMatrix4(earthQ6);
	
	//set new matrixes
	m_pMeshMngr->SetModelMatrix(sizeSun, "Sun");
	m_pMeshMngr->SetModelMatrix(earthFinal, "Earth");
	m_pMeshMngr->SetModelMatrix(moonFinal, "Moon");

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	static int nEarthOrbits = 0;
	static int nEarthRevolutions = 0;
	static int nMoonOrbits = 0;

	//iterate rotation counts
	if (fRunTime >= (nEarthOrbits + 1) * 365)
		nEarthOrbits++;

	if (fRunTime >= (nEarthRevolutions + 1))
		nEarthRevolutions++;

	if (fRunTime >= (nMoonOrbits + 1) * 28)
		nMoonOrbits++;

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	
	m_pMeshMngr->Print("Time:");
	m_pMeshMngr->PrintLine(std::to_string(fRunTime));

	m_pMeshMngr->Print("Day:");
	m_pMeshMngr->PrintLine(std::to_string(m_fDay));

	m_pMeshMngr->Print("E_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthOrbits));

	m_pMeshMngr->Print("E_Revolutions:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthRevolutions));

	m_pMeshMngr->Print("M_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nMoonOrbits));

	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default:
		break;
	case CAMERAMODE::CAMROTHOX:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOY:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
		break;
	}
	
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}