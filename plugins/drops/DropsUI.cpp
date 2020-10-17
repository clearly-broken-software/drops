/*
license here
*/

#include "DropsUI.hpp"
using namespace artwork;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------

DropsUI::DropsUI()
    : UI(1000, 700)
{
    loadSharedResources();
    plugin = static_cast<DropsPlugin *>(getPluginInstancePointer());
    waveForm = &plugin->waveForm;
    miniMap = &plugin->miniMap;
    sig_sampleLoaded = false;
    showWaveForm = false;
    loopstartDragging = false;
    loopendDragging = false;
    scrollbarDragging = false;
    sampleInDragging = false;
    sampleOutDragging = false;
    display.setSize(display_width, display_height);
    display.setPos(display_left, display_top);
    scrollbarDragging = false;
    mouseX = 0;
    mouseY = 0;

    imgLoopStart = createImageFromMemory((uchar *)artwork::loopstartData, artwork::loopstartDataSize, 1);
    imgLoopEnd = createImageFromMemory((uchar *)artwork::loopendData, artwork::loopendDataSize, 1);
    /* for testing */
    sampleLoopStart = 0;
    sampleLoopEnd = 0;
    sampleIn = 0;
    sampleOut = 0;
    /* ----------- */
    initWidgets();
    if (plugin->loadedSample)
    {
        loadSample();
        std::string filename = plugin->path;
        fFileOpenButton->setText(filename);
    }
}

void DropsUI::initWidgets()
{
    Window &window = getParentWindow();
    fFileOpenButton = new TextButton(window, Size<uint>(40, 40)); // FIXME: remove Size
    fFileOpenButton->setCallback(this);
    fFileOpenButton->setAbsolutePos(238, 0);
    fFileOpenButton->setSize(530, 55);

    fScrollBarHandle = new ScrollBar(window);
    fScrollBarHandle->setId(scrollbarHandle_id);
    fScrollBarHandle->setSize(display_width, minimap_height);
    fScrollBarHandle->setAbsolutePos(display_left, display_bottom);
    fScrollBarHandle->setCallback(this);

    fScrollBarLeft = new ScrollBar(window);
    fScrollBarLeft->setId(scrollbarLeft_id);
    fScrollBarLeft->setSize(0, minimap_height);
    fScrollBarLeft->setAbsolutePos(display_left, display_bottom);
    fScrollBarLeft->setCallback(this);

    fScrollBarRight = new ScrollBar(window);
    fScrollBarRight->setId(scrollbarRight_id);
    fScrollBarRight->setSize(0, minimap_height);
    fScrollBarRight->setAbsolutePos(display_right, display_bottom);
    fScrollBarRight->setCallback(this);

    fLoopStart = new ScrollBar(window);
    fLoopStart->setId(kSampleLoopStart);
    fLoopStart->setSize(32, 32); // FIXME: hardcoded
    fLoopStart->setCallback(this);
    fLoopStart->hide();

    fLoopEnd = new ScrollBar(window);
    fLoopEnd->setId(kSampleLoopEnd);
    fLoopEnd->setSize(32, 32); // FIXME: hardcoded
    fLoopEnd->setCallback(this);
    fLoopEnd->hide();

    fSampleIn = new ScrollBar(window);
    fSampleIn->setId(kSampleIn);
    fSampleIn->setSize(32, 32);
    fSampleIn->setCallback(this);
    fSampleIn->hide();

    fSampleOut = new ScrollBar(window);
    fSampleOut->setId(kSampleOut);
    fSampleOut->setSize(32, 32);
    fSampleOut->setCallback(this);
    fSampleOut->hide();

    const Size<uint> knobSize = Size<uint>(76, 76);
    const uint knobSpacing = knobSize.getWidth() + 15;

    fAmpEgAttack = new Knob(window);
    fAmpEgAttack->setId(kAmpEgAttack);
    fAmpEgAttack->setSize(knobSize);
    fAmpEgAttack->setCallback(this);
    fAmpEgAttack->setAbsolutePos(100, 350);
    fAmpEgAttack->label = "ATTACK";

    fAmpEgDecay = new Knob(window);
    fAmpEgDecay->setId(kAmpEgDecay);
    fAmpEgDecay->setSize(knobSize);
    fAmpEgDecay->setCallback(this);
    fAmpEgDecay->setAbsolutePos(100 + 1 * knobSpacing, 350);
    fAmpEgDecay->label = "DECAY";

    fAmpEgSustain = new Knob(window);
    fAmpEgSustain->setId(kAmpEgSustain);
    fAmpEgSustain->setSize(knobSize);
    fAmpEgSustain->setCallback(this);
    fAmpEgSustain->setAbsolutePos(100 + 2 * knobSpacing, 350);
    fAmpEgSustain->label = "SUSTAIN";

    fAmpEgRelease = new Knob(window);
    fAmpEgRelease->setId(kAmpEgRelease);
    fAmpEgRelease->setSize(knobSize);
    fAmpEgRelease->setCallback(this);
    fAmpEgRelease->setAbsolutePos(100 + 3 * knobSpacing, 350);
    fAmpEgRelease->label = "RELEASE";

    fLoopMode = new DropDown(window);
    fLoopMode->setId(kSamplePlayMode);
    fLoopMode->font_size = 16;
    fLoopMode->setSize(216, fLoopMode->font_size + fLoopMode->margin * 2.0f);
    fLoopMode->setCallback(this);
    fLoopMode->setAbsolutePos(100 + 4 * knobSpacing, 350);
    fLoopMode->label = "LOOP MODE:";
    fLoopMode->item = "NO LOOP";

    fLoopMenu = new Menu(window);
    fLoopMenu->setId(9999); // FIXME: hardcode id
    //fLoopMenu->setSize(200, 120);
    fLoopMenu->setCallback(this);
    const float x = fLoopMode->getMenuOffset() + fLoopMode->getAbsoluteX();
    const float y = fLoopMode->getAbsoluteY() + fLoopMode->getHeight();
    fLoopMenu->setAbsolutePos(x, y);
    fLoopMenu->addItem("NO LOOP");
    fLoopMenu->addItem("ONE SHOT");
    fLoopMenu->addItem("CONTINUOUS");
    fLoopMenu->addItem("SUSTAIN");
    fLoopMenu->font_size = 16;
    fLoopMenu->hide();

    fLoopMode->setMenu(fLoopMenu);
}

void DropsUI::parameterChanged(uint32_t index, float value)
{
    // printf("parameterChanged(%i,%f)\n", index, value);
    switch (index)
    {
    case kSampleLoaded:
    {
        bool oldSampleLoaded = sig_sampleLoaded;
        if (static_cast<bool>(value) == oldSampleLoaded)
        {
            break;
        }
        sig_sampleLoaded = value;
        if (sig_sampleLoaded)
        {
            loadSample();
        }

        break;
    }
    case kSampleIn:
        sampleIn = value * static_cast<float>(sampleLength);
        setMarkers(); // FIXME: all markers are set, only 1 is needed :-/
        break;
    case kSampleOut:
        sampleOut = value * static_cast<float>(sampleLength);
        setMarkers();
        break;
    case kSampleLoopStart:
        sampleLoopStart = value * static_cast<float>(sampleLength);
        setMarkers();
        break;
    case kSampleLoopEnd:
        sampleLoopEnd = value * static_cast<float>(sampleLength);
        setMarkers();
        break;
    case kAmpEgAttack:
        fAmpEgAttack->setValue(value);
        repaint();
        break;
    case kAmpEgDecay:
        fAmpEgDecay->setValue(value);
        repaint();
        break;
    case kAmpEgSustain:
        fAmpEgSustain->setValue(value);
        repaint();
        break;
    case kAmpEgRelease:
        fAmpEgRelease->setValue(value);
        repaint();
        break;

    default:
        break;
    }
}

int DropsUI::loadSample()
{
    sampleLength = static_cast<sf_count_t>(waveForm->size());
    sampleIn = 0;
    sampleOut = sampleLength;
    sampleLoopStart = 0;
    sampleLoopEnd = sampleLength;
    viewStart = 0;
    viewEnd = sampleLength;
    viewZoom = 1.0f;
    viewMaxZoom = float(sampleLength) / float(display_width);

    // make minimap
    /* FIXME: only set this when there are loop points */
    float samples_per_pixel = static_cast<float>(sampleLength) / static_cast<float>(display_width);
    float loopStartPixel = static_cast<float>(sampleLoopStart) / samples_per_pixel + static_cast<float>(display_left);
    float loopEndPixel = static_cast<float>(sampleLoopEnd) / samples_per_pixel + static_cast<float>(display_left);
    float sampleInPixel = static_cast<float>(sampleIn) / samples_per_pixel + static_cast<float>(display_left);
    float sampleOutPixel = static_cast<float>(sampleOut) / samples_per_pixel + static_cast<float>(display_left);

    fLoopStart->setAbsolutePos(loopStartPixel - 32, display_bottom - 32);
    fLoopEnd->setAbsolutePos(loopEndPixel, display_bottom - 32);
    fSampleIn->setAbsolutePos(sampleInPixel - 32, display_top);
    fSampleOut->setAbsolutePos(sampleOutPixel, display_top);
    fLoopStart->show();
    fLoopEnd->show();
    fSampleIn->show();
    fSampleOut->show();
    setState("ui_sample_loaded", "true");
    showWaveForm = true;

    return 0;
}

void DropsUI::onNanoDisplay()
{
    float width = getWidth();
    float height = getHeight();
    beginPath();
    fillColor(121, 121, 121);
    rect(0.0f, 0.0f, width, height);
    fill();
    closePath();

    beginPath();
    fillColor(82, 82, 82);
    rect(display_left, display_top, display_width, display_height);
    fill();
    closePath();

    if (showWaveForm)
    {
        drawWaveform();
        drawMinimap();
        drawLoopMarkers();
        drawInOutMarkers();
    }
}

void DropsUI::drawWaveform()
{
    double view = viewEnd - viewStart; // set these when zooming in
    double samples_per_pixel = view / (double)display_width;
    float fIndex;
    uint iIndex;

    fIndex = float(viewStart) + float(samples_per_pixel);
    iIndex = fIndex;
    beginPath();
    strokeColor(161, 161, 161);
    strokeWidth(1.0f);
    moveTo(display_left, display_center);

    for (uint16_t i = 0; i < display_width; i++)
    {
        fIndex = float(viewStart) + (float(i) * samples_per_pixel);
        iIndex = fIndex;
        auto minmax = std::minmax_element(waveForm->begin() + iIndex, waveForm->begin() + iIndex + int(samples_per_pixel));
        uint16_t min = *minmax.first + display_center;
        uint16_t max = *minmax.second + display_center;
        lineTo(i + display_left, min);
        lineTo(i + display_left, max);
    }
    stroke();
    closePath();
    // center line
    beginPath();
    moveTo(display_left, display_center);
    lineTo(display_right, display_center);
    stroke();
    closePath();
}

void DropsUI::drawMinimap()
{
    beginPath();
    strokeColor(143, 143, 143);
    strokeWidth(1);
    fillColor(107, 107, 107);
    rect(display_left, display_bottom, display_width, minimap_height);
    fill();
    closePath();
    beginPath();

    for (int i = 0; i < display_width; i++)
    {
        moveTo(i + display_left, display_bottom + minimap_height);
        lineTo(i + display_left, display_bottom + minimap_height - miniMap->at(i));
    }
    stroke();
    closePath();

    // draw "handlebar"
    double samples_per_pixel = waveForm->size() / (double)display_width;
    int leftPixel = static_cast<float>(viewStart) / samples_per_pixel + static_cast<float>(display_left);
    int rightPixel = static_cast<float>(viewEnd) / samples_per_pixel + static_cast<float>(display_left);
    // darken left of the bar
    fillColor(0, 0, 0, 64);
    beginPath();
    rect(display_left, display_bottom, leftPixel - display_left, minimap_height);
    fill();
    closePath();
    // darken left of the bar
    beginPath();
    rect(rightPixel, display_bottom, display_right - rightPixel, minimap_height);
    fill();
    closePath();
}

void DropsUI::drawInOutMarkers()
{

    if (sampleIn >= viewStart && sampleIn <= viewEnd)
    {

        // sample to pixel
        double sample_per_pixel = pow(viewMaxZoom, viewZoom);
        float sampleInPixel = (sampleIn / sample_per_pixel) - (viewStart / sample_per_pixel) + static_cast<float>(display_left);
        fillColor(154, 154, 154);
        strokeColor(154, 154, 154);
        strokeWidth(1.0f);
        // line
        beginPath();
        moveTo(sampleInPixel, display_top);
        lineTo(sampleInPixel, display_bottom);
        stroke();
        closePath();
        // handle
        beginPath();
        roundedRect(sampleInPixel - 32.f, display_top, 32.f, 32.f, 32.f);
        fill();
        closePath();
        beginPath();
        rect(sampleInPixel - 16.f, display_top, 16.f, 32.f);
        fill();
        closePath();
        // IN text
        beginPath();
        fillColor(86, 86, 86);
        fontSize(sample_inout_font_size);
        textAlign(ALIGN_CENTER | ALIGN_MIDDLE);
        text(sampleInPixel - 16.f, display_top + 16.f, "IN", NULL);

        closePath();
    }

    if (sampleOut <= viewEnd && sampleOut >= viewStart)
    {
        double sample_per_pixel = pow(viewMaxZoom, viewZoom);
        float sampleOutPixel = (sampleOut / sample_per_pixel) - (viewStart / sample_per_pixel) + static_cast<float>(display_left);

        fillColor(154, 154, 154);
        strokeColor(154, 154, 154);
        strokeWidth(1.0f);
        // line
        beginPath();
        moveTo(sampleOutPixel, display_top);
        lineTo(sampleOutPixel, display_bottom);
        stroke();
        closePath();
        // handle
        beginPath();
        roundedRect(sampleOutPixel, display_top, 32.f, 32.f, 32.f);
        fill();
        closePath();

        beginPath();
        rect(sampleOutPixel, display_top, 16.f, 32.f);
        fill();
        closePath();
        // text OUT
        beginPath();

        fillColor(86, 86, 86);
        fontSize(sample_inout_font_size);
        textAlign(ALIGN_CENTER | ALIGN_MIDDLE);
        text(sampleOutPixel + 16.f, display_top + 16.f, "OUT", NULL);
        closePath();
    }
}

void DropsUI::drawLoopMarkers()
{
    if (sampleLoopStart >= viewStart && sampleLoopStart <= viewEnd)
    {

        // sample to pixel
        double sample_per_pixel = pow(viewMaxZoom, viewZoom);
        float loopStartPixel = (sampleLoopStart / sample_per_pixel) - (viewStart / sample_per_pixel) + static_cast<float>(display_left);
        ;

        fillColor(154, 154, 154);
        strokeColor(154, 154, 154);
        strokeWidth(1.0f);
        // line
        beginPath();
        moveTo(loopStartPixel, display_bottom);
        lineTo(loopStartPixel, display_top);
        stroke();
        closePath();
        // handle
        beginPath();
        roundedRect(loopStartPixel - 32.f, display_bottom - 32.f, 32.f, 32.f, 32.f);
        fill();
        closePath();
        beginPath();
        rect(loopStartPixel - 16.f, display_bottom - 32.f, 16.f, 32.f);
        fill();
        closePath();
        beginPath();
        Paint loopstartpaint = imagePattern(loopStartPixel - 26, display_bottom - 26,
                                            18, 16,
                                            0,
                                            imgLoopStart, 1.0f);
        rect(loopStartPixel - 26, display_bottom - 26, 18, 16);
        fillPaint(loopstartpaint);
        fill();
        closePath();
    }

    if (sampleLoopEnd <= viewEnd && sampleLoopEnd >= viewStart)
    {
        double sample_per_pixel = pow(viewMaxZoom, viewZoom);
        float loopEndPixel = (sampleLoopEnd / sample_per_pixel) - (viewStart / sample_per_pixel) + static_cast<float>(display_left);

        fillColor(154, 154, 154);
        strokeColor(154, 154, 154);
        strokeWidth(1.0f);
        // line
        beginPath();
        moveTo(loopEndPixel, display_bottom);
        lineTo(loopEndPixel, display_top);
        stroke();
        closePath();
        // handle
        beginPath();
        roundedRect(loopEndPixel, display_bottom - 32.f, 32.f, 32.f, 32.f);
        fill();
        closePath();
        beginPath();
        rect(loopEndPixel, display_bottom - 32.f, 16.f, 32.f);
        fill();
        closePath();
        beginPath();
        Paint loopendpaint = imagePattern(loopEndPixel, display_bottom - 26,
                                          18, 16,
                                          0,
                                          imgLoopEnd, 1.0f);
        rect(loopEndPixel, display_bottom - 26, 18, 16);
        fillPaint(loopendpaint);
        fill();
        closePath();
    }

    // do stuff
}

void DropsUI::uiFileBrowserSelected(const char *filename)
{
    // if a file was selected, tell DSP
    if (filename != nullptr)
    {

        fFileOpenButton->setText(filename);
        setState("filepath", filename);
        repaint();
    }
}

void DropsUI::stateChanged(const char *key, const char *)
{
    printf("state changed... do something?\n");
}

bool DropsUI::onMouse(const MouseEvent &ev)
{
    if (ev.press)
        if (scrollbarDragging || loopstartDragging || loopendDragging || sampleInDragging || sampleOutDragging)
        {
            mouseX = ev.pos.getX();
        }

    return false;
}

void DropsUI::scrollWaveform(bool LeftOrRight)
{
    float leftright = LeftOrRight ? -1.0f : 1.0f;
    float samples_per_pixel = pow(viewMaxZoom, viewZoom);
    float length = samples_per_pixel * static_cast<float>(display_width);
    float samples_to_scroll = static_cast<float>(display_width) * samples_per_pixel * .5 * leftright;
    viewStart += static_cast<int>(samples_to_scroll);
    if (viewStart < 0)
    {
        viewStart = 0;
        viewEnd = static_cast<sf_count_t>(length);
        setMarkers();
        setScrollbarWidgets();
        repaint();
        return;
    }
    viewEnd += static_cast<int>(samples_to_scroll);
    if (viewEnd > waveForm->size())
    {
        viewEnd = waveForm->size();
        viewStart = viewEnd - length;
        setMarkers();
        setScrollbarWidgets();
        repaint();
        return;
    }
    setMarkers();
    setScrollbarWidgets();
    repaint();
}

void DropsUI::setMarkers()
{
    float samples_per_pixel = pow(viewMaxZoom, viewZoom);
    float loopStartPixel = static_cast<float>(sampleLoopStart - viewStart) / samples_per_pixel + static_cast<float>(display_left);
    float loopEndPixel = static_cast<float>(sampleLoopEnd - viewStart) / samples_per_pixel + static_cast<float>(display_left);
    fLoopStart->setAbsoluteX(loopStartPixel - 32);
    fLoopEnd->setAbsoluteX(loopEndPixel);

    float sampleInPixel = static_cast<float>(sampleIn - viewStart) / samples_per_pixel + static_cast<float>(display_left);
    float sampleOutPixel = static_cast<float>(sampleOut - viewStart) / samples_per_pixel + static_cast<float>(display_left);
    fSampleIn->setAbsoluteX(sampleInPixel - 32);
    fSampleOut->setAbsoluteX(sampleOutPixel);
}

void DropsUI::setScrollbarWidgets()
{
    double samples_per_pixel = waveForm->size() / (double)display_width;
    int leftPixel = viewStart / samples_per_pixel + display_left;
    int rightPixel = viewEnd / samples_per_pixel + display_left;
    fScrollBarHandle->setWidth(rightPixel - leftPixel);
    fScrollBarHandle->setAbsoluteX(leftPixel);
    fScrollBarLeft->setWidth(leftPixel - display_left);
    fScrollBarRight->setWidth(display_right - rightPixel);
    fScrollBarRight->setAbsoluteX(rightPixel);
}
bool DropsUI::onScroll(const ScrollEvent &ev)
{
    // is the pointer in the display
    int x = ev.pos.getX();
    int y = ev.pos.getY();
    if (!display.contains(x, y))
    {
        return false; // get outta here
    }

    if (waveForm->size() <= display_width)
        return false; // can't zoom anyway

    x -= display_left; // off set in pixels

    // left-right scroll factor
    float scroll_delta = -ev.delta.getX();
    // zoom factor
    float zoom_delta = -ev.delta.getY() * 0.05f;

    // we use a signed int to be able to handle temporary negative starts.
    int start;
    float samples_per_pixel;

    // We either zoom in/out, or ...
    if (zoom_delta != 0.0)
    {
        // old zoom factor
        uint center = int(pow(viewMaxZoom, viewZoom) * (float(x)) + float(viewStart));
        viewZoom += zoom_delta;
        if (viewZoom < 0.0f)
        {
            viewZoom = 0.0f;
        }
        if (viewZoom > 1.0f)
        {
            viewZoom = 1.0f;
        }
        samples_per_pixel = pow(viewMaxZoom, viewZoom);
        start = int(float(center) - (float(x) * samples_per_pixel));
    }
    // ... we scroll left or right.
    else if (scroll_delta != 0.0)
    {
        if ((scroll_delta < 0 && viewStart == 0) ||
            (scroll_delta > 0 && viewEnd == waveForm->size()))
        {
            // can't scroll any further
            return false;
        }
        samples_per_pixel = pow(viewMaxZoom, viewZoom);
        float scroll_distance = 20 * scroll_delta * samples_per_pixel;
        start = viewStart + scroll_distance;
    }
    else
    {
        // this probably shouldn't happen.
        return false;
    }

    // and ensure we stay in view.
    uint length = int(samples_per_pixel * float(display_width));

    viewEnd = start + length;
    if (viewEnd > waveForm->size())
    {
        viewEnd = waveForm->size();
        start = viewEnd - length;
    }
    samples_per_pixel = pow(viewMaxZoom, viewZoom);
    viewStart = start < 0 ? 0 : start;
    /*
    double spp = waveForm->size() / (double)display_width;
    int leftPixel = viewStart / spp + display_left;
    int rightPixel = viewEnd / spp + display_left;
    fScrollBarHandle->setWidth(rightPixel - leftPixel);
    fScrollBarHandle->setAbsoluteX(leftPixel);
    fScrollBarLeft->setWidth(leftPixel - display_left);
    fScrollBarRight->setWidth(display_right - rightPixel);
    fScrollBarRight->setAbsoluteX(rightPixel);
    */
    setScrollbarWidgets();
    setMarkers();
    repaint();
    return true;
}

bool DropsUI::onMotion(const MotionEvent &ev)
{
    if (scrollbarDragging)
    {
        int distance = ev.pos.getX() - mouseX;
        mouseX = ev.pos.getX();
        float samples_per_pixel = waveForm->size() / (double)display_width;
        int underflowcheck = viewStart + (float(distance) * samples_per_pixel);
        if (underflowcheck < 0)
        {
            viewStart = 0;
            int length = display_width * pow(viewMaxZoom, viewZoom);
            viewEnd = viewStart + length;
        }
        else
        {
            viewStart = underflowcheck;
            viewEnd = viewEnd + ((float)distance * samples_per_pixel);
        }

        if (viewEnd > waveForm->size())
        {
            viewEnd = waveForm->size();
            viewStart = (float)waveForm->size() - (float)display_width * pow(viewMaxZoom, viewZoom);
        }
        setScrollbarWidgets();
        setMarkers();
        repaint();
    }

    if (loopstartDragging)
    {
        float distance = ev.pos.getX() - mouseX;
        mouseX = ev.pos.getX();
        float samples_per_pixel = pow(viewMaxZoom, viewZoom);
        sampleLoopStart = static_cast<float>(sampleLoopStart) + distance * samples_per_pixel;
        sampleLoopStart = clamp<sf_count_t>(sampleLoopStart, sampleLoopEnd - 1, sampleIn);
        float loopStartPixel = static_cast<float>(sampleLoopStart - viewStart) / samples_per_pixel + static_cast<float>(display_left);
        fLoopStart->setAbsoluteX(loopStartPixel - 32);
        float value = static_cast<float>(sampleLoopStart) / static_cast<float>(waveForm->size());
        setParameterValue(kSampleLoopStart, value);
        repaint();
    }

    if (loopendDragging)
    {
        float distance = ev.pos.getX() - mouseX;
        mouseX = ev.pos.getX();
        float samples_per_pixel = pow(viewMaxZoom, viewZoom);
        sampleLoopEnd = static_cast<float>(sampleLoopEnd) + distance * samples_per_pixel;
        sampleLoopEnd = clamp<sf_count_t>(sampleLoopEnd, sampleOut, sampleLoopStart + 1);
        float loopEndPixel = static_cast<float>(sampleLoopEnd - viewStart) / samples_per_pixel + static_cast<float>(display_left);
        fLoopEnd->setAbsoluteX(loopEndPixel);
        float value = static_cast<float>(sampleLoopEnd) / static_cast<float>(waveForm->size());
        setParameterValue(kSampleLoopEnd, value);
        repaint();
    }

    if (sampleInDragging)
    {
        float distance = ev.pos.getX() - mouseX;
        mouseX = ev.pos.getX();
        float samples_per_pixel = pow(viewMaxZoom, viewZoom);
        sampleIn = static_cast<float>(sampleIn) + distance * samples_per_pixel;
        sampleIn = clamp<sf_count_t>(sampleIn, sampleOut - 1, 0);
        float sampleInPixel = static_cast<float>(sampleIn - viewStart) / samples_per_pixel + static_cast<float>(display_left);
        fSampleIn->setAbsoluteX(sampleInPixel - 32);
        float value = static_cast<float>(sampleIn) / static_cast<float>(waveForm->size());
        setParameterValue(kSampleIn, value);
        repaint();
    }

    if (sampleOutDragging)
    {
        float distance = ev.pos.getX() - mouseX;
        mouseX = ev.pos.getX();
        float samples_per_pixel = pow(viewMaxZoom, viewZoom);
        sampleOut = static_cast<float>(sampleOut) + distance * samples_per_pixel;
        sampleOut = clamp<sf_count_t>(sampleOut, waveForm->size(), sampleIn + 1);
        float sampleOutPixel = static_cast<float>(sampleOut - viewStart) / samples_per_pixel + static_cast<float>(display_left);
        fSampleOut->setAbsoluteX(sampleOutPixel);
        float value = static_cast<float>(sampleOut) / static_cast<float>(waveForm->size());
        setParameterValue(kSampleOut, value);
        repaint();
    }

    return false;
}

void DropsUI::textButtonClicked(TextButton *textButton)
{
    DGL::Window::FileBrowserOptions opts;
    opts.title = "Load SFZ";
    opts.buttons.showPlaces = 2;
    getParentWindow().openFileBrowser(opts);
}
void DropsUI::dropDownClicked(DropDown *dropDown)
{
    uint id = dropDown->getId();
    switch (id)
    {
    case kSamplePlayMode:
        fLoopMenu->show(); /* code */
        break;

    default:
        break;
    }
    printf("dropdown clicked\n");
}

void DropsUI::knobValueChanged(Knob *knob, float value)
{
    uint id = knob->getId();
    switch (id)
    {
    case kAmpEgAttack:
        setParameterValue(kAmpEgAttack, value);
        break;
    case kAmpEgDecay:
        setParameterValue(kAmpEgDecay, value);
        break;
    case kAmpEgSustain:
        setParameterValue(kAmpEgSustain, value);
        break;
    case kAmpEgRelease:
        setParameterValue(kAmpEgRelease, value);
        break;
    default:
        printf("knob changed: id %i, value %f\n", knob->getId(), value);
        break;
    }

    repaint();
}

void DropsUI::scrollBarClicked(ScrollBar *scrollBar, bool dragging)
{
    uint id = scrollBar->getId();
    switch ((id))
    {
    case scrollbarHandle_id:
        scrollbarDragging = dragging;
        break;
    case scrollbarLeft_id:
        if (!dragging)
        {
            scrollWaveform(true);
        }
        break;
    case scrollbarRight_id:
        if (!dragging)
        {
            scrollWaveform(false);
        }
        break;
    case kSampleLoopStart:
        loopstartDragging = dragging;
        break;
    case kSampleLoopEnd:
        loopendDragging = dragging;
        break;
    case kSampleIn:
        sampleInDragging = dragging;
        break;
    case kSampleOut:
        sampleOutDragging = dragging;
        break;
    default:
        break;
    }
}

void DropsUI::menuClicked(Menu *, uint id, std::string item)
{
    fLoopMode->item = item;
    fLoopMenu->hide();
    setParameterValue(kSamplePlayMode, id);
}

UI *createUI()
{
    return new DropsUI();
}

// -----------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
