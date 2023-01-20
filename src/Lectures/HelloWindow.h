// Link -> https://www.learnopengl.com/Getting-started/Hello-Window

#include "../Lectures.h"

class HelloWindow : public Lecture
{
public:
    HelloWindow();
    virtual void OpenLecture() override;
    virtual void OpenLectureLink() override;
    virtual void RenderLecture(bool settingsVisible) override;
    virtual void CloseLecture() override;
private:
    float m_ClearColor[4] = {0.1f, 0.1f, 0.1f, 1.0f};
};