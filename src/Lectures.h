// This file contains all the information about which lecture to open when requested & also provided the lecture link for it.
#include <memory>
#include <vector>
#include <stdlib.h>

/// @brief Abstract Class for Lectures to Implement.
class Lecture
{
public:
    Lecture() = default;
    virtual ~Lecture() = default;
    virtual void OpenLecture() = 0;
    virtual void OpenLectureLink() = 0;
    virtual void RenderLecture(bool settingsVisible) = 0;
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
};