// This file contains all the information about which lecture to open when requested & also provided the lecture link for it.

#pragma once
#include <memory>
#include <vector>
#include <stdlib.h>
#include <string>

/// @brief Abstract Class for Lecture to Implement.
class Lecture
{
public:
    Lecture() = default;
    virtual ~Lecture() = default;
    virtual void OpenLecture() = 0;
    virtual void OpenLectureLink() = 0;
    virtual void RenderLecture(bool settingsVisible) = 0;
    virtual void CloseLecture() = 0;
protected:
    const char* m_LectureLink;
};

/// @brief Has information about all the lectures.
class Lectures
{
public:
    Lectures() = default;
    ~Lectures() { delete m_Instance; }
    void OpenLecture(uint32_t lecture_index);
    void OpenLectureLink(uint32_t lecture_index);
    void RenderLecture(uint32_t lecture_index, bool settingsVisible);
    std::vector<Lecture*> m_Lectures;
    static Lectures* m_Instance;
    static void CreateInstance();
private:
    uint32_t m_CurrentLecture = 0;
};

/// @brief Helper Function to Show Label on the Left side of the ImGui Widget.
/// @param label Label
/// @return LabelID
static std::string LabelPrefix(const char* const label)
{
	float width = ImGui::CalcItemWidth();

	float x = ImGui::GetCursorPosX();
	ImGui::Text("%s", label);
	ImGui::SameLine(); 
	ImGui::SetCursorPosX(x + width * 0.6f + ImGui::GetStyle().ItemInnerSpacing.x);
	ImGui::SetNextItemWidth(-1);

	std::string labelID = "##";
	labelID += label;

	return labelID;
}

/// @brief Draws Text in the Center of the Current Window!
/// @param text Text to Draw
static void DrawTextCentered(const char* text)
{
    ImGui::SetCursorPosX( (ImGui::GetWindowWidth() - ImGui::CalcTextSize(text).x) / 2.f);
    ImGui::Text("%s", text);
}

/// @brief Draws Button in the Center of the Current Window!
/// @param label Label for button
static bool DrawButtonCentered(const char* label)
{
    ImGui::SetCursorPosX( (ImGui::GetWindowWidth() - ImGui::CalcTextSize(label).x) / 2.f);
    return ImGui::Button(label);
}