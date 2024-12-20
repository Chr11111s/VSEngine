//����Ч����ʾ
#include <VSApplication.h>
#include <VSEngineInput.h>
#include <VSCameraActor.h>
#include <VSWorld.h>
#include <VS1stCameraController.h>
#include "VSStaticActor.h"
#include "VSLightActor.h"
#include "VSSkyLight.h"
#include "VSViewFamily.h"
#include "VSTimer.h"
#include "VSSceneManager.h"
#include "VSSkeletonActor.h"
#include "VSPostEffectFunction.h"
#include "VSReflectCameraActor.h"
#include "VSCaptureTexAllState.h"
using namespace VSEngine2;
class VSDemoWindowsApplication : public VSWindowApplication
{

	DLCARE_APPLICATION(VSDemoWindowsApplication);
public:
	VSDemoWindowsApplication();
	~VSDemoWindowsApplication();
	virtual bool PreInitial();
	virtual bool OnInitial();
	virtual bool OnTerminal();
	virtual bool PostUpdate();
	virtual bool PreUpdate();
	virtual bool OnDraw();
	virtual void OnMove(int xPos, int yPos);
	virtual void OnReSize(int iWidth, int iHeight);
	virtual void OnKeyDown(unsigned int uiKey);
	virtual void OnKeyUp(unsigned int uiKey);
	virtual void OnLButtonDown(int xPos, int yPos);
	virtual void OnLButtonUp(int xPos, int yPos);
	virtual void OnRButtonDown(int xPos, int yPos);
	virtual void OnRButtonUp(int xPos, int yPos);
	virtual void OnMButtonDown(int xPos, int yPos);
	virtual void OnMButtonUp(int xPos, int yPos);
	virtual void OnMouseMove(int xPos, int yPos);
	virtual void OnMouseWheel(int xPos, int yPos, int zDet);
protected:
	VSCameraActor* m_pCameraActor;
	VS1stCameraController* m_p1stCameraController;
	VSSceneMap * m_pTestMap;
	bool m_bLMouseDowning;
	int		m_iDetMouseX;
	int		m_iDetMouseY;
	int		m_iLastMouseX;
	int		m_iLastMouseY;

	VSReflectCameraActor * pReflectCameraActor;
	VSMaterialInstance* pMaterialInstance;

};
IMPLEMENT_APPLICATION(VSDemoWindowsApplication);
VSDemoWindowsApplication::VSDemoWindowsApplication()
{


}
VSDemoWindowsApplication::~VSDemoWindowsApplication()
{

}
bool VSDemoWindowsApplication::PreInitial()
{
	VSWindowApplication::PreInitial();
	m_uiInputAPIType = VSEngineInput::IAT_WINDOWS;
	//m_uiInputAPIType = VSEngineInput::IAT_DX;
	VSResourceManager::ms_bUpdateThread = false;
	VSResourceManager::ms_bRenderThread = false;
	m_bLMouseDowning = false;

	m_iDetMouseX = 0;
	m_iDetMouseY = 0;
	m_iLastMouseX = 0;
	m_iLastMouseY = 0;

	

	return true;
}
bool VSDemoWindowsApplication::OnInitial()
{
	VSWindowApplication::OnInitial();

	m_p1stCameraController = VSObject::GetInstance<VS1stCameraController>();
	m_pCameraActor = (VSCameraActor *)VSWorld::ms_pWorld->CreateActor<VSCameraActor>();
	m_pCameraActor->GetTypeNode()->AddController(m_p1stCameraController);
	VSVector3 CameraPos(0.0f, 100.0f, -300.0f);
	VSVector3 CameraDir(0.0f, 0.0f, 1.0f);
	m_pCameraActor->GetTypeNode()->CreateFromLookDir(CameraPos, CameraDir);
	m_pCameraActor->GetTypeNode()->SetPerspectiveFov(AngleToRadian(90.0f), (m_uiScreenWidth * 1.0f) / (m_uiScreenHeight), 1.0f, 8000.0f);

	m_pTestMap = VSWorld::ms_pWorld->CreateScene(_T("Test"));

	VSSkeletonActor * pSKActor1 = (VSSkeletonActor *)VSWorld::ms_pWorld->CreateActor(_T("NewMonsterWithAnim.SKMODEL"), VSVector3(-600, 0, 600), VSMatrix3X3::ms_Identity, VSVector3::ms_One, m_pTestMap);
	pSKActor1->GetTypeNode()->PlayAnim(_T("Idle"), 1.0f, VSController::RT_WRAP);

	VSSkeletonActor * pSKActor2 = (VSSkeletonActor *)VSWorld::ms_pWorld->CreateActor(_T("NewMonsterWithAnim.SKMODEL"), VSVector3(0, 0, 600), VSMatrix3X3::ms_Identity, VSVector3::ms_One, m_pTestMap);
	pSKActor2->GetTypeNode()->PlayAnim(_T("Walk"), 1.0f, VSController::RT_WRAP);

	VSSkeletonActor * pSKActor3 = (VSSkeletonActor *)VSWorld::ms_pWorld->CreateActor(_T("NewMonsterWithAnim.SKMODEL"), VSVector3(600, 0, 600), VSMatrix3X3::ms_Identity, VSVector3::ms_One, m_pTestMap);
	pSKActor3->GetTypeNode()->PlayAnim(_T("Attack"), 1.0f, VSController::RT_WRAP);

	VSSkeletonActor * pSKActor4 = (VSSkeletonActor *)VSWorld::ms_pWorld->CreateActor(_T("NewMonsterWithAnim.SKMODEL"), VSVector3(-600, 0, 300), VSMatrix3X3::ms_Identity, VSVector3::ms_One, m_pTestMap);
	pSKActor4->GetTypeNode()->PlayAnim(_T("Attack2"), 1.0f, VSController::RT_WRAP);

	VSSkeletonActor * pSKActor5 = (VSSkeletonActor *)VSWorld::ms_pWorld->CreateActor(_T("NewMonsterWithAnim.SKMODEL"), VSVector3(0, 0, 300), VSMatrix3X3::ms_Identity, VSVector3::ms_One, m_pTestMap);
	pSKActor5->GetTypeNode()->PlayAnim(_T("Pos"), 1.0f, VSController::RT_WRAP);

	VSSkeletonActor * pSKActor6 = (VSSkeletonActor *)VSWorld::ms_pWorld->CreateActor(_T("NewMonsterWithAnim.SKMODEL"), VSVector3(600, 0, 300), VSMatrix3X3::ms_Identity, VSVector3::ms_One, m_pTestMap);
	pSKActor6->GetTypeNode()->PlayAnim(_T("AddtiveAnim"), 1.0f, VSController::RT_WRAP);

	VSStaticActor* pStaticActor = (VSStaticActor*)VSWorld::ms_pWorld->CreateActor(_T("ReflectPlane.STMODEL"), VSVector3(0, 0, 0), VSMatrix3X3::ms_Identity, VSVector3(100.0f, 100.f, 100.0f), m_pTestMap);
	pStaticActor->GetTypeNode()->CastShadow(false);

	VSSkyLightActor * pSkyLightActor = (VSSkyLightActor *)VSWorld::ms_pWorld->CreateActor<VSSkyLightActor>(VSVector3::ms_Zero, VSMatrix3X3::ms_Identity, VSVector3::ms_One, m_pTestMap);
	pSkyLightActor->GetTypeNode()->m_DownColor = VSColorRGBA(0.0f, 0.0f, 0.5f, 1.0f);
	pSkyLightActor->GetTypeNode()->m_UpColor = VSColorRGBA(0.2f, 0.2f, 0.2f, 1.0f);

	VSMatrix3X3 Dir;
	Dir.CreateEuler(0, AngleToRadian(45), AngleToRadian(45));
	VSDirectionLightActor * pDirectionLightActor = (VSDirectionLightActor *)VSWorld::ms_pWorld->CreateActor<VSDirectionLightActor>(VSVector3::ms_Zero, Dir, VSVector3::ms_One, m_pTestMap);

	m_pTestMap->GetScene()->Build();

	VSArray<VSString> SceneMap;
	SceneMap.AddElement(_T("Main"));
	SceneMap.AddElement(_T("Test"));


	pReflectCameraActor = (VSReflectCameraActor *)VSWorld::ms_pWorld->CreateActor<VSReflectCameraActor>();
	pReflectCameraActor->SetTargetCameraActor(m_pCameraActor);
	pReflectCameraActor->SetPlane(VSPlane3(VSVector3(0, -1, 0), VSVector3(0, 0.5f, 0)));

	//RT Size > Screen Size when Draw Full Sreen Effect  , this will can't draw .
	VSWorld::ms_pWorld->AttachCaptureViewFamilyToCamera(pReflectCameraActor, 
				VSCaptureViewFamily::VT_CAPTURE_2D_CAPTURE_NORMAL, _T("Reflect"), 
				512, 512, SceneMap, VSSimpleForwardEffectSceneRenderMethod::ms_Type.GetName().GetBuffer());

	VSCaptureTexAllStatePtr pCaptureTex = VSResourceManager::CreateCaptureTexture(_T("Reflect"));
	pMaterialInstance = pStaticActor->GetTypeNode()->GetUseMaterialInstance(0);
	VSTexAllStateRPtr pCaptureTexR = VSTexAllStateR::Create(pCaptureTex);
	pMaterialInstance->SetShaderTexture(_T("EmissiveTexture"), pCaptureTexR);


	VSWorld::ms_pWorld->AttachWindowViewFamilyToCamera(m_pCameraActor, VSWindowViewFamily::VT_WINDOW_NORMAL,
		_T("WindowUse"), SceneMap, VSForwardEffectSceneRenderMethod::ms_Type.GetName().GetBuffer(), -1);




	return true;
}
bool VSDemoWindowsApplication::OnTerminal()
{
	VSWindowApplication::OnTerminal();

	pReflectCameraActor = NULL;
	pMaterialInstance = NULL;
	return true;
}
bool VSDemoWindowsApplication::PostUpdate()
{
	VSWindowApplication::PostUpdate();
	if (m_bLMouseDowning)
	{
		m_p1stCameraController->m_RotXDelta = ((m_iDetMouseY)* 1.0f) * 0.001f/*VSTimer::ms_pTimer->GetDetTime() * 0.5f*/;
		m_p1stCameraController->m_RotYDelta = ((m_iDetMouseX)* 1.0f) * 0.001f/*VSTimer::ms_pTimer->GetDetTime() * 0.5f*/;
		m_iDetMouseX = 0;
		m_iDetMouseY = 0;
	}
	else
	{
		m_p1stCameraController->m_RotXDelta = 0.0f;
		m_p1stCameraController->m_RotYDelta = 0.0f;
	}
	VSMatrix3X3W ReflectM = pReflectCameraActor->GetTypeNode()->GetViewMatrix() * pReflectCameraActor->GetTypeNode()->GetProjMatrix();

	VSVector3W Column[4];
	ReflectM.GetColumnVector(Column);

	pMaterialInstance->SetShaderValue(_T("ReflectViewProject_1 "), &Column[0], sizeof(VSVector3W));
	pMaterialInstance->SetShaderValue(_T("ReflectViewProject_2 "), &Column[1], sizeof(VSVector3W));
	pMaterialInstance->SetShaderValue(_T("ReflectViewProject_3 "), &Column[2], sizeof(VSVector3W));
	pMaterialInstance->SetShaderValue(_T("ReflectViewProject_4 "), &Column[3], sizeof(VSVector3W));
	return true;
}
bool VSDemoWindowsApplication::PreUpdate()
{
	VSWindowApplication::PreUpdate();
	
	return true;
}
bool VSDemoWindowsApplication::OnDraw()
{
	VSWindowApplication::OnDraw();
	return true;
}
void VSDemoWindowsApplication::OnMove(int xPos, int yPos)
{

}
void VSDemoWindowsApplication::OnReSize(int iWidth, int iHeight)
{

}
void VSDemoWindowsApplication::OnKeyDown(unsigned int uiKey)
{
	VSApplication::OnKeyDown(uiKey);
	if (uiKey == VSEngineInput::BK_UP)
	{
		m_p1stCameraController->m_MoveZDelta = 1.0f;
	}
	else if (uiKey == VSEngineInput::BK_DOWN)
	{
		m_p1stCameraController->m_MoveZDelta = -1.0f;
	}
	else if (uiKey == VSEngineInput::BK_LEFT)
	{
		m_p1stCameraController->m_MoveXDelta = -1.0f;
	}
	else if (uiKey == VSEngineInput::BK_RIGHT)
	{
		m_p1stCameraController->m_MoveXDelta = 1.0f;
	}
	else if (uiKey == VSEngineInput::BK_F1)
	{
		ChangeScreenSize(640, 480, true, false);
	}
	else if (uiKey == VSEngineInput::BK_F2)
	{
		ChangeScreenSize(640, 480, true, true);
	}
	else if (uiKey == VSEngineInput::BK_F3)
	{
		ChangeScreenSize(1024, 768, false);
	}

}
void VSDemoWindowsApplication::OnKeyUp(unsigned int uiKey)
{
	VSApplication::OnKeyUp(uiKey);
	if (uiKey == VSEngineInput::BK_UP)
	{
		m_p1stCameraController->m_MoveZDelta = 0.0f;
	}
	else if (uiKey == VSEngineInput::BK_DOWN)
	{
		m_p1stCameraController->m_MoveZDelta = 0.0f;
	}
	else if (uiKey == VSEngineInput::BK_LEFT)
	{
		m_p1stCameraController->m_MoveXDelta = 0.0f;
	}
	else if (uiKey == VSEngineInput::BK_RIGHT)
	{
		m_p1stCameraController->m_MoveXDelta = 0.0f;
	}
}
void VSDemoWindowsApplication::OnLButtonDown(int xPos, int yPos)
{
	VSWindowApplication::OnLButtonDown(xPos, yPos);
	m_bLMouseDowning = true;
	m_iLastMouseX = xPos;
	m_iLastMouseY = yPos;
	m_iDetMouseX = 0;
	m_iDetMouseY = 0;
}
void VSDemoWindowsApplication::OnLButtonUp(int xPos, int yPos)
{
	VSWindowApplication::OnLButtonUp(xPos, yPos);
	m_bLMouseDowning = false;
}
void VSDemoWindowsApplication::OnRButtonDown(int xPos, int yPos)
{
	VSWindowApplication::OnRButtonDown(xPos, yPos);
}
void VSDemoWindowsApplication::OnRButtonUp(int xPos, int yPos)
{
	VSWindowApplication::OnRButtonUp(xPos, yPos);
}
void VSDemoWindowsApplication::OnMButtonDown(int xPos, int yPos)
{
	VSWindowApplication::OnMButtonDown(xPos, yPos);
}
void VSDemoWindowsApplication::OnMButtonUp(int xPos, int yPos)
{
	VSWindowApplication::OnMButtonUp(xPos, yPos);
}
void VSDemoWindowsApplication::OnMouseMove(int xPos, int yPos)
{
	VSWindowApplication::OnMouseMove(xPos, yPos);
	if (m_bLMouseDowning)
	{
		m_iDetMouseX = xPos - m_iLastMouseX;
		m_iDetMouseY = yPos - m_iLastMouseY;
		m_iLastMouseX = xPos;
		m_iLastMouseY = yPos;
	}

}
void VSDemoWindowsApplication::OnMouseWheel(int xPos, int yPos, int zDet)
{
	VSWindowApplication::OnMouseWheel(xPos, yPos, zDet);
}
