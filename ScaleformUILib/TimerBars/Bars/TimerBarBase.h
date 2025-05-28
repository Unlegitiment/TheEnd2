#pragma once
#include "GTAVInfrastructure\SHV\enums.h"
class TimerBarBase {
public:
	TimerBarBase(const char* label, eFont font = FontChaletLondon):
		m_Label(label),
		m_iFont(font)
	{

	}
	void SetLabel(const char* lb) {
		this->m_Label = lb;
	}
	void SetFont(eFont font) {
		this->m_iFont = font;
	}
	eFont GetFont() {
		return m_iFont;
	}
	const char* GetLabel() {
		return this->m_Label;
	}
	virtual void Draw(int interval) {
		/*
		{
			SizeF res = ScreenTools.ResolutionMaintainRatio;
			PointF safe = ScreenTools.SafezoneBounds;
			new UIResText(Label, new PointF((int)res.Width - safe.X - 180, (int)res.Height - safe.Y - (30 + (4 * interval))), 0.3f, SColor.White.ToColor(), LabelFont, Alignment.Right).Draw();

			new Sprite("timerbars", "all_black_bg", new PointF((int)res.Width - safe.X - 298, (int)res.Height - safe.Y - (40 + (4 * interval))), new SizeF(300, 37), 0f, Color.FromArgb(180, 255, 255, 255)).Draw();

			Screen.Hud.HideComponentThisFrame(HudComponent.AreaName);
			Screen.Hud.HideComponentThisFrame(HudComponent.StreetName);
			Screen.Hud.HideComponentThisFrame(HudComponent.VehicleName);
		}
		*/
	}
private:
	const char* m_Label;
	eFont m_iFont;
};