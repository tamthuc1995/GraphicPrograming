/** \file App.cpp */
#include "App.h"

// Tells C++ to invoke command-line main() function even on OS X and Win32.
G3D_START_AT_MAIN();

int main(int argc, const char* argv[]) {
    initGLG3D(G3DSpecification());

    GApp::Settings settings(argc, argv);

    // Change the window and other startup parameters by modifying the
    // settings class.  For example:
    settings.window.caption             = argv[0];

    // Set enable to catch more OpenGL errors
    // settings.window.debugContext     = true;
   
    settings.window.fullScreen          = false;
    if (settings.window.fullScreen) {
        settings.window.width           = 1920;
        settings.window.height          = 1080;
    } else {
        settings.window.height          = int(OSWindow::primaryDisplayWindowSize().y * 0.95f); 
        // Constrain ultra widescreen aspect ratios
        settings.window.width           = min(settings.window.height * 1920 / 1080, int(OSWindow::primaryDisplayWindowSize().x * 0.95f));

        // Make even
        settings.window.width  -= settings.window.width & 1;
        settings.window.height -= settings.window.height & 1;
    }
    settings.window.resizable           = ! settings.window.fullScreen;
    settings.window.framed              = ! settings.window.fullScreen;
    settings.window.defaultIconFilename = "icon.png";

    // Set to true for a significant performance boost if your app can't
    // render at the display frequency, or if you *want* to render faster
    // than the display.
    settings.window.asynchronous        = true;

    // Render slightly larger than the screen so that screen-space refraction, bloom,
    // screen-space AO, and screen-space reflection to look good at screen edges. Set to zero for
    // maximum performance and free memory. Increase the second argument to improve AO without affecting
    // color. The third argument is the resolution. Set to 0.5f to render at half-res and upscale,
    // 2.0f to supersample.
    settings.hdrFramebuffer.setGuardBandsAndSampleRate(64, 0, 1.0f);

    settings.renderer.deferredShading = true;
    settings.renderer.orderIndependentTransparency = true;

    settings.dataDir                       = FileSystem::currentDirectory();

    settings.screenCapture.outputDirectory = FilePath::concat(FileSystem::currentDirectory(), "../journal");
    if (! FileSystem::exists(settings.screenCapture.outputDirectory)) {
        settings.screenCapture.outputDirectory = "";
    }
    settings.screenCapture.includeAppRevision = false;
    settings.screenCapture.includeG3DRevision = false;
    settings.screenCapture.filenamePrefix = "_";
    

    return App(settings).run();
}


App::App(const GApp::Settings& settings) : GApp(settings) {
    shared_ptr<projectsSpecification> temp(new projectsSpecification());
    proj_spec = temp;
    proj_spec->setNumMeshes(600);
    proj_spec->setHeightFieldYScale(50.0f);
    proj_spec->setHeightFieldXZScale(4.0f);

    proj_spec->setGlassParams("upper_inner_h", 3.0f);
    proj_spec->setGlassParams("upper_thickness", 0.05f);
    proj_spec->setGlassParams("mid_h", 5.0f);
    proj_spec->setGlassParams("base_h", 0.2f);
    proj_spec->setGlassParams("base_r", 1.0f);
    proj_spec->setGlassParams("step_size", 0.1f);
    proj_spec->setGlassParams("n_points_per_loop", 3.0f);


    shared_ptr<RayTracer> temp_ray_tracer(new RayTracer());
    my_ray_tracer = temp_ray_tracer;
    my_ray_tracer->setResolution("160 x 160");
    my_ray_tracer->setMultithreading(false);
    my_ray_tracer->setFixedPrimitives(false);
    my_ray_tracer->setNumIndirectRays(0);

}


// Called before the application loop begins.  Load data here and
// not in the constructor so that common exceptions will be
// automatically caught.

/**
This function will create staircases Scene.Any during app init.
*/



/** By default, G3D::GApp creates a G3D::FirstPersonManipulator that allows you to move the 3D camera. 
I'm telling you the name of the class right now so that 
you'll know how to find and configure it later, but right now all that you need to know is...video game controls work. */

void App::onInit() {
    GApp::onInit();

    setFrameDuration(1.0f / 60.0f);

    renderDevice->setSwapBuffersAutomatically(true);
    showRenderingStats = false;
    developerWindow->setVisible(true);
    developerWindow->cameraControlWindow->setVisible(true);
    m_debugCamera->filmSettings().setAntialiasingEnabled(true);
    m_debugCamera->filmSettings().setContrastToneCurve();

    // Starting position
    m_debugCamera->setFrame(CFrame::fromXYZYPRDegrees(24.3f, 0.4f, 2.5f, 68.7f, 1.2f, 0.0f));
    m_debugCamera->frame();


    loadScene("White Cube");

    // Make the GUI after the scene is loaded because loading/rendering/simulation initialize
    // some variables that advanced GUIs may wish to reference with pointers.
    makeGUI();

    // Force re-render on first frame
    m_prevCFrame = CFrame(Matrix3::zero());
    setActiveCamera(debugCamera());
    scene()->setTime(1.0);
    setSimulationTimeScale(0.0f);
}

void App::makeGUI() {
    debugWindow->setVisible(true);
    developerWindow->videoRecordDialog->setEnabled(true);
    GuiPane* infoPane = debugPane->addPane("Info", GuiTheme::ORNATE_PANE_STYLE);
    
    // Example of how to add debugging controls
    infoPane->addLabel("You can add GUI controls");
    infoPane->addLabel("in App::onInit().");
    infoPane->addButton("Exit", [this]() { m_endProgram = true; });
    infoPane->pack();

    GuiPane* rendererPane = debugPane->addPane("DefaultRenderer", GuiTheme::ORNATE_PANE_STYLE);

    // showInTextureBrowser("G3D::GBuffer/CS_NORMAL");

    GuiCheckBox* deferredBox = rendererPane->addCheckBox("Deferred Shading",
        Pointer<bool>([&]() {
                const shared_ptr<DefaultRenderer>& r = dynamic_pointer_cast<DefaultRenderer>(m_renderer);
                return r && r->deferredShading();
            },
            [&](bool b) {
                const shared_ptr<DefaultRenderer>& r = dynamic_pointer_cast<DefaultRenderer>(m_renderer);
                if (r) { r->setDeferredShading(b); }
            }));
    rendererPane->addCheckBox("Order-Independent Transparency",
        Pointer<bool>([&]() {
                const shared_ptr<DefaultRenderer>& r = dynamic_pointer_cast<DefaultRenderer>(m_renderer);
                return r && r->orderIndependentTransparency();
            },
            [&](bool b) {
                const shared_ptr<DefaultRenderer>& r = dynamic_pointer_cast<DefaultRenderer>(m_renderer);
                if (r) { r->setOrderIndependentTransparency(b); }
            }));

    GuiPane* giPane = rendererPane->addPane("Ray Tracing", GuiTheme::SIMPLE_PANE_STYLE);
 

    giPane->addCheckBox("Diffuse",
		Pointer<bool>([&]() {
			const shared_ptr<DefaultRenderer>& r = dynamic_pointer_cast<DefaultRenderer>(m_renderer);
			return r && r->enableDiffuseGI();
			},
			[&](bool b) {
				const shared_ptr<DefaultRenderer>& r = dynamic_pointer_cast<DefaultRenderer>(m_renderer);
				if (r) { r->setEnableDiffuseGI(b); }
			}));
    giPane->addCheckBox("Glossy",
        Pointer<bool>([&]() {
            const shared_ptr<DefaultRenderer>& r = dynamic_pointer_cast<DefaultRenderer>(m_renderer);
            return r && r->enableGlossyGI();
            },
            [&](bool b) {
                const shared_ptr<DefaultRenderer>& r = dynamic_pointer_cast<DefaultRenderer>(m_renderer);
                if (r) { r->setEnableGlossyGI(b); }
            })
    );
    giPane->addCheckBox("Show Probes",
        Pointer<bool>([&]() {
            const shared_ptr<DefaultRenderer>& r = dynamic_pointer_cast<DefaultRenderer>(m_renderer);
            if (notNull(r)) {
                bool allEnabled = r->m_ddgiVolumeArray.size() > 0;
                for (int i = 0; i < r->m_ddgiVolumeArray.size(); ++i) {
                    allEnabled = allEnabled && r->m_showProbeLocations[i];
                }
                return allEnabled;
            }
            return false;
            },
            [&](bool b) {
                const shared_ptr<DefaultRenderer>& r = dynamic_pointer_cast<DefaultRenderer>(m_renderer);
                if (notNull(r)) {
                    for (int i = 0; i < r->m_ddgiVolumeArray.size(); ++i) {
                        r->m_showProbeLocations[i] = b;
                    }
                }
            }), GuiTheme::TOOL_CHECK_BOX_STYLE);
            giPane->pack();

    giPane->moveRightOf(deferredBox);
    giPane->moveBy(100, 0);

    rendererPane->moveRightOf(infoPane);
    rendererPane->moveBy(10, 0);

    shared_ptr<GuiWindow> window2 = GuiWindow::create("Generate Cylinder");
    GuiPane* pane = window2->pane();
    Pointer<int> ptr_num_meshes_cylinder(
        [&]() {
            const shared_ptr<projectsSpecification>& r = dynamic_pointer_cast<projectsSpecification>(proj_spec);
            if (notNull(r)) { return r->getNumMeshes(); }
        },
        [&](int b) {
            const shared_ptr<projectsSpecification>& r = dynamic_pointer_cast<projectsSpecification>(proj_spec);
            if (notNull(r)) { r->setNumMeshes(b); }
        }
    );
    pane->addNumberBox("Num meshes", ptr_num_meshes_cylinder, "", GuiTheme::LINEAR_SLIDER, 20, 1000);
    pane->addButton("Generate", [this]() {
        try {
            projectsSpecification::makeCylinder("model/cylinder.off", proj_spec->getNumMeshes());
            const ModelTable model_table = scene()->modelTable();
            for (ModelTable::Iterator it = model_table.begin(); it.isValid(); ++it) {
                const String name = it->key; 
                const lazy_ptr<ArticulatedModel>& v = ((it->value).resolve());
                v->clearCache();
            }
            scene()->clear();
            scene()->load("Test Scene Cylinder OFF Meshes");
        }
        catch (...) {
            msgBox("Unable create cylinder");
        }
        });
    window2->pack();
    addWidget(window2);



    shared_ptr<GuiWindow> window3 = GuiWindow::create("Generate Heighfield");
    GuiPane* heightfieldPane = window3->pane();

    Pointer<float> ptr_heightfieldYScale(
        [&]() {
            const shared_ptr<projectsSpecification>& r = dynamic_pointer_cast<projectsSpecification>(proj_spec);
            if (notNull(r)) { return r->getHeightFieldYScale(); }
        },
        [&](float b) {
            const shared_ptr<projectsSpecification>& r = dynamic_pointer_cast<projectsSpecification>(proj_spec);
            if (notNull(r)) { r->setHeightFieldYScale(b); }
        }
    );
    heightfieldPane->addNumberBox("Max Y", ptr_heightfieldYScale, "m", GuiTheme::LOG_SLIDER, 0.0f, 100.0f)->setUnitsSize(30);

    Pointer<float> ptr_heightfieldXZScale(
        [&]() {
            const shared_ptr<projectsSpecification>& r = dynamic_pointer_cast<projectsSpecification>(proj_spec);
            if (notNull(r)) { return r->getHeightFieldXZScale(); }
        },
        [&](float b) {
            const shared_ptr<projectsSpecification>& r = dynamic_pointer_cast<projectsSpecification>(proj_spec);
            if (notNull(r)) { r->setHeightFieldXZScale(b); }
        }
    );
    heightfieldPane->addNumberBox("XZ Scale", ptr_heightfieldXZScale, "m/px", GuiTheme::LOG_SLIDER, 0.001f, 10.0f)->setUnitsSize(30);

    Pointer<String> ptr_heightfieldscource(
        [&]() {
            const shared_ptr<projectsSpecification>& r = dynamic_pointer_cast<projectsSpecification>(proj_spec);
            if (notNull(r)) { return r->getHeightFieldSource(); }
        },
        [&](String b) {
            const shared_ptr<projectsSpecification>& r = dynamic_pointer_cast<projectsSpecification>(proj_spec);
            if (notNull(r)) { r->setHeightFieldSource(b); }
        }
    );
    heightfieldPane->beginRow(); {
        heightfieldPane->addTextBox("Input Image", ptr_heightfieldscource)->setWidth(210);
        heightfieldPane->addButton("...", [this]() {
            String filename;
            FileDialog::getFilename(filename, "png", false);
            proj_spec->setHeightFieldSource(filename);
            })->setWidth(30);
    } heightfieldPane->endRow();


    heightfieldPane->addButton("Generate", [this]() {
        shared_ptr<Image> image;
        try {
            image = Image::fromFile(proj_spec->getHeightFieldSource());
            projectsSpecification::makeHeightField(
                "model/heightfield.off", 
                image, 
                proj_spec->getHeightFieldYScale(), 
                proj_spec->getHeightFieldXZScale()
            );

            // Clear cache
            const ModelTable model_table = scene()->modelTable();
            for (ModelTable::Iterator it = model_table.begin(); it.isValid(); ++it) {
                const String name = it->key;
                const lazy_ptr<ArticulatedModel>& v = ((it->value).resolve());
                v->clearCache();
            }
            scene()->clear();
            scene()->load("Test Scene Heightfield OFF Meshes");
        }
        catch (...) {
            String m_heightfieldSource = proj_spec->getHeightFieldSource();
            msgBox("Unable to load the image.", m_heightfieldSource);
        }
        });

    window3->pack();
    addWidget(window3);



    shared_ptr<GuiWindow> window4 = GuiWindow::create("Generate Glass");
    GuiPane* createGlassPane = window4->pane();


    createGlassPane->addNumberBox("upper_inner_h", 
        Pointer<float>(
            [&]() {
                const shared_ptr<projectsSpecification>& r = dynamic_pointer_cast<projectsSpecification>(proj_spec);
                if (notNull(r)) { return r->getGlassParams("upper_inner_h"); }
            },
            [&](float b) {
                const shared_ptr<projectsSpecification>& r = dynamic_pointer_cast<projectsSpecification>(proj_spec);
                if (notNull(r)) { r->setGlassParams("upper_inner_h", b); }
            }),
        "", GuiTheme::LOG_SLIDER, 0.001f, 10.0f
    )->setUnitsSize(30);

    createGlassPane->addNumberBox("upper_thickness",
        Pointer<float>(
            [&]() {
                const shared_ptr<projectsSpecification>& r = dynamic_pointer_cast<projectsSpecification>(proj_spec);
                if (notNull(r)) { return r->getGlassParams("upper_thickness"); }
            },
            [&](float b) {
                const shared_ptr<projectsSpecification>& r = dynamic_pointer_cast<projectsSpecification>(proj_spec);
                if (notNull(r)) { r->setGlassParams("upper_thickness", b); }
            }),
        "", GuiTheme::LOG_SLIDER, 0.001f, 0.5f
    )->setUnitsSize(30);

    createGlassPane->addNumberBox("mid_h",
        Pointer<float>(
            [&]() {
                const shared_ptr<projectsSpecification>& r = dynamic_pointer_cast<projectsSpecification>(proj_spec);
                if (notNull(r)) { return r->getGlassParams("mid_h"); }
            },
            [&](float b) {
                const shared_ptr<projectsSpecification>& r = dynamic_pointer_cast<projectsSpecification>(proj_spec);
                if (notNull(r)) { r->setGlassParams("mid_h", b); }
            }),
        "", GuiTheme::LOG_SLIDER, 0.001f, 10.0f
    )->setUnitsSize(30);

    createGlassPane->addNumberBox("base_h",
        Pointer<float>(
            [&]() {
                const shared_ptr<projectsSpecification>& r = dynamic_pointer_cast<projectsSpecification>(proj_spec);
                if (notNull(r)) { return r->getGlassParams("base_h"); }
            },
            [&](float b) {
                const shared_ptr<projectsSpecification>& r = dynamic_pointer_cast<projectsSpecification>(proj_spec);
                if (notNull(r)) { r->setGlassParams("base_h", b); }
            }),
        "", GuiTheme::LOG_SLIDER, 0.001f, 1.0f
    )->setUnitsSize(30);

    createGlassPane->addNumberBox("base_r",
        Pointer<float>(
            [&]() {
                const shared_ptr<projectsSpecification>& r = dynamic_pointer_cast<projectsSpecification>(proj_spec);
                if (notNull(r)) { return r->getGlassParams("base_r"); }
            },
            [&](float b) {
                const shared_ptr<projectsSpecification>& r = dynamic_pointer_cast<projectsSpecification>(proj_spec);
                if (notNull(r)) { r->setGlassParams("base_r", b); }
            }),
        "", GuiTheme::LOG_SLIDER, 0.001f, 3.0f
    )->setUnitsSize(30);

    createGlassPane->addNumberBox("step_size",
        Pointer<float>(
            [&]() {
                const shared_ptr<projectsSpecification>& r = dynamic_pointer_cast<projectsSpecification>(proj_spec);
                if (notNull(r)) { return r->getGlassParams("step_size"); }
            },
            [&](float b) {
                const shared_ptr<projectsSpecification>& r = dynamic_pointer_cast<projectsSpecification>(proj_spec);
                if (notNull(r)) { r->setGlassParams("step_size", b); }
            }),
        "", GuiTheme::LOG_SLIDER, 0.001f, 1.0f
    )->setUnitsSize(30);

    createGlassPane->addNumberBox("n_points_per_loop",
        Pointer<float>(
            [&]() {
                const shared_ptr<projectsSpecification>& r = dynamic_pointer_cast<projectsSpecification>(proj_spec);
                if (notNull(r)) { return r->getGlassParams("n_points_per_loop"); }
            },
            [&](float b) {
                const shared_ptr<projectsSpecification>& r = dynamic_pointer_cast<projectsSpecification>(proj_spec);
                if (notNull(r)) { r->setGlassParams("n_points_per_loop", b); }
            }),
        "", GuiTheme::LOG_SLIDER, 0.001f, 360.0f
    )->setUnitsSize(30);

    createGlassPane->addButton("Generate", [this]() {
        try {
            projectsSpecification::makeGlass("model/tallglass.off", proj_spec->getGlassParamsTable());
            // Clear cache
            const ModelTable model_table = scene()->modelTable();
            for (ModelTable::Iterator it = model_table.begin(); it.isValid(); ++it) {
                const String name = it->key;
                const lazy_ptr<ArticulatedModel>& v = ((it->value).resolve());
                v->clearCache();
            }
            scene()->clear();
            scene()->load("Test Scene Tall Glass OFF Meshes");
        }
        catch (...) {

            msgBox("Unable to creat tall glass. ");
        }
        });

    window4->pack();
    addWidget(window4);



    debugWindow->pack();
    debugWindow->setRect(Rect2D::xywh(0, 0, (float)window()->width(), debugWindow->rect().height()));
}


// This default implementation is a direct copy of GApp::onGraphics3D to make it easy
// for you to modify. If you aren't changing the hardware rendering strategy, you can
// delete this override entirely.
void App::onGraphics3D(RenderDevice* rd, Array<shared_ptr<Surface> >& allSurfaces) {

    if (! scene()) {
        if ((submitToDisplayMode() == SubmitToDisplayMode::MAXIMIZE_THROUGHPUT) && (!rd->swapBuffersAutomatically())) {
            swapBuffers();
        }
        rd->clear();
        rd->pushState(); {
            rd->setProjectionAndCameraMatrix(activeCamera()->projection(), activeCamera()->frame());
            drawDebugShapes();
        } rd->popState();
        return;
    }

    BEGIN_PROFILER_EVENT("App::onGraphics3D");
    GBuffer::Specification gbufferSpec = m_gbufferSpecification;
    // new
    onRender();
    m_framebuffer = Framebuffer::create(m_result);

    extendGBufferSpecification(gbufferSpec);
    m_gbuffer->setSpecification(gbufferSpec);
    m_gbuffer->resize(m_framebuffer->width(), m_framebuffer->height());
    m_gbuffer->prepare(rd, activeCamera(), 0, -(float)previousSimTimeStep(), m_settings.hdrFramebuffer.depthGuardBandThickness, m_settings.hdrFramebuffer.colorGuardBandThickness);
    debugAssertGLOk();

    //m_renderer->render(rd,
    //    activeCamera(),
    //    m_framebuffer,
    //    scene()->lightingEnvironment().ambientOcclusionSettings.enabled ? m_depthPeelFramebuffer : nullptr,
    //    scene()->lightingEnvironment(), m_gbuffer,
    //    allSurfaces,
    //    [&]() -> decltype(auto) { return scene()->tritree(); }); // decltype(auto) for correct return type deduction in the lambda.


    // Debug visualizations and post-process effects
    rd->pushState(m_framebuffer); {
        // Call to make the App show the output of debugDraw(...)
        rd->setProjectionAndCameraMatrix(activeCamera()->projection(), activeCamera()->frame());
        drawDebugShapes();
        const shared_ptr<Entity>& selectedEntity = (notNull(developerWindow) && notNull(developerWindow->sceneEditorWindow)) ? developerWindow->sceneEditorWindow->selectedEntity() : nullptr;
        scene()->visualize(rd, selectedEntity, allSurfaces, sceneVisualizationSettings(), activeCamera());

        onPostProcessHDR3DEffects(rd);
    } rd->popState();

    // We're about to render to the actual back buffer, so swap the buffers now.
    // This call also allows the screenshot and video recording to capture the
    // previous frame just before it is displayed.
    if (submitToDisplayMode() == SubmitToDisplayMode::MAXIMIZE_THROUGHPUT) {
        swapBuffers();
    }

    // Clear the entire screen (needed even though we'll render over it, since
    // AFR uses clear() to detect that the buffer is not re-used.)
    rd->clear();
    // Perform gamma correction, bloom, and AA, and write to the native window frame buffer
    m_film->exposeAndRender(rd, activeCamera()->filmSettings(), m_framebuffer->texture(0),
        settings().hdrFramebuffer.trimBandThickness().x,
        settings().hdrFramebuffer.depthGuardBandThickness.x,
        Texture::opaqueBlackIfNull(notNull(m_gbuffer) ? m_gbuffer->texture(GBuffer::Field::SS_POSITION_CHANGE) : nullptr),
        activeCamera()->jitterMotion());
    END_PROFILER_EVENT();
}

//void App::onGraphics(RenderDevice* rd, Array<shared_ptr<Surface> >& surface3D, Array<shared_ptr<Surface2D> >& surface2D) {
//    // Show animations by forcing this always to true. Remove to reduce CPU load
//    //m_forceRender = true;
//
//    // Update the preview image only while moving
//    if ((!m_prevCFrame.fuzzyEq(m_debugCamera->frame())) || m_forceRender) {
//        onRender();
//        m_prevCFrame = m_debugCamera->frame();
//        m_forceRender = false;
//    }
//
//    if (m_result) {
//        rd->push2D(); {
//            Draw::rect2D(rd->viewport(), rd, Color3::white(), m_result);
//        } rd->pop2D();
//    }
//
//    Surface2D::sortAndRender(rd, surface2D);
//}

void App::onRender() {
    const String resolution = my_ray_tracer->getResolution();
    const Vector2int32 size = Vector2int32::parseResolution(resolution);

    //const int width = int(window()->width() * 0.1);
    //const int height = int(window()->height() * 0.1);
    const int width = int(size.x);
    const int height = int(size.y);

    if (isNull(m_currentImage) || (m_currentImage->width() != width) || (m_currentImage->height() != height)) {
        m_currentImage = Image3::createEmpty(width, height);
    }

    my_ray_tracer->rayTraceImage(m_currentImage, m_debugCamera);

    // Post-process
    const shared_ptr<PixelTransferBuffer>& ptb = CPUPixelTransferBuffer::fromData(m_currentImage->width(), m_currentImage->height(), ImageFormat::RGB32F(), m_currentImage->getCArray(), 1, 1);
    //const shared_ptr<Texture>& m_result = Texture::fromPixelTransferBuffer("Source", ptb, ImageFormat::RGB32F(), Texture::DIM_2D, false);
    m_result = Texture::fromPixelTransferBuffer("Source", ptb, ImageFormat::RGB32F(), Texture::DIM_2D, false);
    m_result->setName("m_result_");
    //if (m_result) {
    //    m_result->resize(width, height);
    //}

    //m_film->exposeAndRender(renderDevice, m_debugCamera->filmSettings(), src,
    //    settings().hdrFramebuffer.trimBandThickness().x,
    //    settings().hdrFramebuffer.depthGuardBandThickness.x, m_result);

}


void App::onAI() {
    GApp::onAI();
    // Add non-simulation game logic and AI code here
}


void App::onNetwork() {
    GApp::onNetwork();
    // Poll net messages here
}


void App::onSimulation(RealTime rdt, SimTime sdt, SimTime idt) {
    GApp::onSimulation(rdt, sdt, idt);

    // Example GUI dynamic layout code.  Resize the debugWindow to fill
    // the screen horizontally.
    debugWindow->setRect(Rect2D::xywh(0, 0, (float)window()->width(), debugWindow->rect().height()));
}


bool App::onEvent(const GEvent& event) {
    // Handle super-class events
    if (GApp::onEvent(event)) { return true; }

    // If you need to track individual UI events, manage them here.
    // Return true if you want to prevent other parts of the system
    // from observing this specific event.
    //
    // For example,
    // if ((event.type == GEventType::GUI_ACTION) && (event.gui.control == m_button)) { ... return true; }
    // if ((event.type == GEventType::KEY_DOWN) && (event.key.keysym.sym == GKey::TAB)) { ... return true; }
    // if ((event.type == GEventType::KEY_DOWN) && (event.key.keysym.sym == 'p')) { ... return true; }

    if ((event.type == GEventType::KEY_DOWN) && (event.key.keysym.sym == 'p')) { 
        const shared_ptr<DefaultRenderer>& r = dynamic_pointer_cast<DefaultRenderer>(m_renderer);
        r->setDeferredShading(! r->deferredShading());
        return true; 
    }

    return false;
}


void App::onUserInput(UserInput* ui) {
    GApp::onUserInput(ui);
    (void)ui;
    // Add key handling here based on the keys currently held or
    // ones that changed in the last frame.
}


void App::onPose(Array<shared_ptr<Surface> >& surface, Array<shared_ptr<Surface2D> >& surface2D) {
    GApp::onPose(surface, surface2D);

    // Append any models to the arrays that you want to later be rendered by onGraphics()
}


void App::onGraphics2D(RenderDevice* rd, Array<shared_ptr<Surface2D> >& posed2D) {
    // Render 2D objects like Widgets.  These do not receive tone mapping or gamma correction.
    Surface2D::sortAndRender(rd, posed2D);
}


void App::onCleanup() {
    // Called after the application loop ends.  Place a majority of cleanup code
    // here instead of in the constructor so that exceptions can be caught.
}
