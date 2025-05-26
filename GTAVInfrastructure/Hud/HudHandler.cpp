#include "HudHandler.h"
#include "Obj\Rect.h"
#include "Obj\CText.h"
#include "Obj\Scaleform.h"
void CHudHandler::Init()
{
    this->m_ScaleformHandler = new CScaleformHandler();
}
CRect* CHudHandler::CreateRect(CVector3 pos, Color32 col, float width, float height)
{
    CRect* pRect = new CRect(pos, col, width, height);
    this->m_vDrawList.push_back(pRect);
    return pRect;
}

CText* CHudHandler::CreateText(const char* string, CVector3 pos, Color32 col, void* justification, float scale, bool hasOutline, int font)
{
    CText* pText = new CText(string, pos, col, *(CText::sTextJustification*)justification, scale, hasOutline, font); // the dumbest fucking thing ever.
    this->m_vDrawList.push_back(pText);
    return pText;
}

CScaleformHandler* CHudHandler::GetScaleformHandler()
{
    return this->m_ScaleformHandler;
}

void CHudHandler::Update()
{
    for (auto& draws : m_vDrawList) {
        draws->Draw(); 
    }
}

void CHudHandler::Destroy()
{
    
}
