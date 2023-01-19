#include "Lectures.h"
#include <iostream>

Lectures* Lectures::m_Instance = nullptr;

/// @brief Opens the Lecture scene with the given lecture index
/// @param lecture_id Lecture index
void Lectures::OpenLecture(uint32_t lecture_index)
{
    if(m_Instance->m_Lectures.size() < lecture_index + 1) return;
    m_Instance->m_Lectures[lecture_index]->OpenLecture();
}

/// @brief Opens the lecture link of the lecture index
/// @param lecture_id Lecture index
void Lectures::OpenLectureLink(uint32_t lecture_index)
{
    if(m_Instance->m_Lectures.size() < lecture_index + 1) return;
    m_Instance->m_Lectures[lecture_index]->OpenLectureLink();
}

/// @brief Renders the lecture of the lecture index
/// @param lecture_index Lecture Index
/// @param settingsVisible Render ImGui Lecture Specific Settings?
void Lectures::RenderLecture(uint32_t lecture_index, bool settingsVisible)
{
    if(m_Instance->m_Lectures.size() < lecture_index + 1) return;
    m_Instance->m_Lectures[lecture_index]->RenderLecture(settingsVisible);
}

/// @brief Creates Lectures Instance.
void Lectures::CreateInstance()
{
    // Make Lectures Instance.
    Lectures::m_Instance = new Lectures();
}