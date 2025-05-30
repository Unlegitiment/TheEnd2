#pragma once
#include "LegitProject1\datattypes\color.h"
#include <string>
#include <algorithm>
#include "PauseMenuItem.h"
enum LeftItemType {
	Empty,
	Info,
	Statistics,
	Settings,
	Keymap
};
enum LeftItemBGType {
	Full,
	Masked,
	Resized
};
using IndexChangeEvent = void(*)(SettingsItem* item, int index);
using ActivatedEvent = void(*)(TabLeftItem* item, int index); // yk I hate this right?
class UIMenuItem; // @Todo fwd declare;
class SubmenuTab;
//This class will actually be the death of me. Jesus CHRIST MATE.
class TabLeftItem : public PauseMenuItem{
public:
private:
	UIMenuItem* _internalItem;
	bool enabled = true;
	Color32 mainColor = { 0,0,0,186 };
	Color32 highlightColor = { 240,240,240,255 };
	string textTitle;
	string _label = "";
	string _formatLeftLabel = "";
	string keymapRightLabel_1;
	string keymapRightLabel_2;
	string TextureDict;
	string TextureName;
	LeftItemBGType _LeftItemBGType;
	
	eFont _labelFont = FontChaletLondon;
	eFont _rightlabelFont = FontChaletLondon;
	static std::string ReplaceRstarColorsWith(const std::string& label, const std::string& color) {
		if (label.find('~') == std::string::npos)
			return label;
		std::vector<int> foundIndexes;
		for (size_t i = label.find('~'); i != std::string::npos; i = label.find('~', i + 1)) {
			foundIndexes.push_back(static_cast<int>(i));
		}
		std::string tmp = label;
		for (int i = static_cast<int>(foundIndexes.size()) - 2; i >= 0; i -= 2) {
			int index = foundIndexes[i];
			int length = foundIndexes[i + 1] - index + 1;
			if (index < 0 || length <= 0 || index + length > static_cast<int>(tmp.size()))
				continue;

			std::string segment = tmp.substr(index, length);

			// Skip known non-color tags
			static const std::vector<std::string> notColours = {
				"~s~", "~S~", "~n~", "~h~", "~f~", "~z~", "~u~", "~d~", "~b~", "~i~"
			};

			bool skip = false;
			for (const auto& nc : notColours) {
				if (segment._Starts_with(nc)) {
					skip = true;
					break;
				}
			}
			if (skip) continue;
			std::string tag;
			if (index + 2 < static_cast<int>(tmp.size()) && tmp[index + 2] == '~') {
				tag = segment; // single char format
			}
			else if (tmp.substr(index, 11) == "~HUD_COLOUR") {
				tag = segment;
			}
			else if (tmp.substr(index, 4) == "~HC_") {
				tag = segment;
			}
			if (!tag.empty())
				tmp.replace(index, tag.length(), color);
		}
		return tmp;
	}
	static std::string ReplaceAll(const std::string& input, const std::string& from, const std::string& to) {
		if (from.empty()) return input;

		std::string result = input;
		size_t start_pos = 0;
		while ((start_pos = result.find(from, start_pos)) != std::string::npos) {
			result.replace(start_pos, from.length(), to);
			start_pos += to.length(); // Move past the replaced section
		}
		return result;
	}
public:
	string GetTextureDict() { return this->TextureDict; }
	string GetTextureName() { return this->TextureName; }
	LeftItemBGType GetLeftItemBGType() { return _LeftItemBGType; }
	string GetLabel() { return _label; }
	void SetLabel(string value) {
		std::string v1 = value;
		_label = value;
		_formatLeftLabel = (v1._Starts_with("~") ? value : ("~s~" + v1).c_str());
		if (!enabled) {
			_formatLeftLabel = ReplaceRstarColorsWith(_label, "~c~").c_str();
		}
		else if (Selected) {
			_formatLeftLabel = ReplaceAll(_formatLeftLabel, "~w~", "~l~").c_str();
			_formatLeftLabel = ReplaceAll(_formatLeftLabel, "~s~", "~l~").c_str();
		}
		else {
			_formatLeftLabel = ReplaceAll(_formatLeftLabel, "~l~", "~s~").c_str(); // wtf even is this?
		}
	}
	bool GetEnabled() { return this->enabled; }
	void SetEnabled(bool value) {
		enabled = value;
		if (!value) {
			_formatLeftLabel = ReplaceRstarColorsWith(_formatLeftLabel, "~c~").c_str();
		}
		else {
			Label = _label;
		}
		if (ParentTab != nullptr && ParentTab->IsVisible()) {
			int it = ParentTab->LeftColumn.Items.IndexOf(this);
			ParentTab->UpdateSlot(PM_COLUMNS::LEFT, it);
		}
	}
	ActivatedEvent OnActivated;
	SubmenuTab* ParentTab;
	
	TabLeftItem(string label, LeftItemType type) : PauseMenuItem(label, type) {}
};