/**
  \file App.h

  The G3D 10.00 default starter app is configured for OpenGL 4.1 and
  relatively recent GPUs.
 */
#pragma once
#include <G3D/G3D.h>
#include "projectsSpecification.h"
#include "RayTracer.h"

#include <string> // Standard header
#include <vector>

/** \brief Application framework. */
class App : public GApp {
protected:
    /** Add default parameters for app */
    shared_ptr<projectsSpecification> proj_spec;
    shared_ptr<RayTracer> my_ray_tracer;

    /** Allocated by expose and render */
    shared_ptr<Texture> m_result;

    /** Used to pass information from rayTraceImage() to trace() */
    shared_ptr<Image3>  m_currentImage;

    /** Position during the previous frame */
    CFrame              m_prevCFrame;

    bool                m_forceRender;

    /** Called from onInit */
    void makeGUI();

public:
    
    App(const GApp::Settings& settings = GApp::Settings());

    virtual void onInit() override;
    virtual void onAI() override;
    virtual void onNetwork() override;
    virtual void onSimulation(RealTime rdt, SimTime sdt, SimTime idt) override;
    virtual void onPose(Array<shared_ptr<Surface> >& posed3D, Array<shared_ptr<Surface2D> >& posed2D) override;

    virtual void onRender();

    // You can override onGraphics if you want more control over the rendering loop.
    // virtual void onGraphics(RenderDevice* rd, Array<shared_ptr<Surface> >& surface, Array<shared_ptr<Surface2D> >& surface2D) override;
    //virtual void onGraphics(RenderDevice* rd, Array<shared_ptr<Surface> >& surface3D, Array<shared_ptr<Surface2D> >& surface2D) override;
    virtual void onGraphics3D(RenderDevice* rd, Array<shared_ptr<Surface> >& surface3D) override;
    virtual void onGraphics2D(RenderDevice* rd, Array<shared_ptr<Surface2D> >& surface2D) override;

    virtual bool onEvent(const GEvent& e) override;
    virtual void onUserInput(UserInput* ui) override;
    virtual void onCleanup() override;
};
