#pragma once
#include "ScaleformUILib\Scaleform\common.h"
#include <LegitProject1\datattypes\color.h>
#include <string>
#include <algorithm>
#include <sstream>
#include <iomanip>
enum CrewHierarchy
{
	Leader = 0,
	Commissioner,
	Liutenant = 3, // 2 looks exactly like 1.. so dunno.. let's start on 3
	Representative,
	Muscle,
	Generic
};
class CrewTag {
public:
	static inline std::string ColorToHex(int r, int g, int b) {
		std::stringstream ss;
		ss << '#' << std::uppercase << std::setfill('0') << std::hex
			<< std::setw(2) << r
			<< std::setw(2) << g
			<< std::setw(2) << b;
		return ss.str();
	}
	CrewTag(string tag, bool isPrivate, bool crewContainRSG, CrewHierarchy level, Color32 crewColor) {
		std::string tag_cpp = tag;
		std::string res = "";
		res += isPrivate ? "(" : ".";
		res += crewContainRSG ? "*" : ".";
		res += level; // its not the actual string rep its the number
		std::transform(tag_cpp.begin(), tag_cpp.end(), tag_cpp.begin(), [](unsigned char c) {return std::toupper(c); });
		res += tag_cpp;
		res += ColorToHex(crewColor.GetR(), crewColor.GetG(), crewColor.GetB());
		Tag = res.c_str();
	}
	const char* GetTag() { return this->Tag; }
private:
	void SetTag(const char* tag) {
		this->Tag = tag;
	}
	string Tag;

};
