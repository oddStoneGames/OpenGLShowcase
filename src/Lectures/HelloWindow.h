#include "../Lectures.h"

class HelloWindow : public Lecture
{
public:
    HelloWindow();
    virtual void OpenLecture() override;
    virtual void OpenLectureLink() override;
    virtual void RenderLecture(bool settingsVisible) override;
private:
    float m_ClearColor[4] = {0.2f, 0.2f, 0.2f, 1.0f};
};