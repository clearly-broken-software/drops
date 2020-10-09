
#include "DropsUI.hpp"
using namespace artwork;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------

DropsUI::DropsUI()
    : UI(1000, 700)
{
    Window &window = getParentWindow();
    loadSharedResources();
    sampleLoaded = false;
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

    fFileOpenButton = new TextButton(window, Size<uint>(40, 40));
    fFileOpenButton->setCallback(this);
    fFileOpenButton->setAbsolutePos(238, 0);
    fFileOpenButton->setSize(530, 55);

    fScrollBar = new ScrollBar(window);
    fScrollBar->setId(scrollbar_id); // fixme harcoded
    fScrollBar->setSize(display_width, minimap_height);
    fScrollBar->setAbsolutePos(display_left, display_bottom);
    fScrollBar->setCallback(this);

    imgLoopStart = createImageFromMemory((uchar *)artwork::loopstartData, artwork::loopstartDataSize, 1);
    imgLoopEnd = createImageFromMemory((uchar *)artwork::loopendData, artwork::loopendDataSize, 1);
    /* for testing */
    sampleLoopStart = 14525;
    /* ----------- */
    fLoopStart = new ScrollBar(window);
    fLoopStart->setId(kSampleLoopStart);
    fLoopStart->setSize(32, 32); // fixme hardcoded
    fLoopStart->setCallback(this);
    fLoopStart->hide();

    sampleLoopEnd = 15525;
    fLoopEnd = new ScrollBar(window);
    fLoopEnd->setId(kSampleLoopEnd);
    fLoopEnd->setSize(32, 32); // fixme hardcoded
    fLoopEnd->setCallback(this);
    fLoopEnd->hide();

    sampleIn = 3000;

    fSampleIn = new ScrollBar(window);
    fSampleIn->setId(kSampleIn);
    fSampleIn->setSize(32, 32);
    fSampleIn->setCallback(this);
    fSampleIn->hide();

    sampleOut = 20000;

    fSampleOut = new ScrollBar(window);
    fSampleOut->setId(kSampleOut);
    fSampleOut->setSize(32, 32);
    fSampleOut->setCallback(this);
    fSampleOut->hide();
}

void DropsUI::parameterChanged(uint32_t index, float value)
{
    //printf("DropsUI::parameterChanged(%i, %f)\n", index, value);
}

int DropsUI::loadSample(const char *fp)
{
    // init waveform andminiMap
    waveForm.resize(0);
    miniMap.resize(0);

    int file_samplerate(0);
    SndfileHandle fileHandle(fp);

    // get the number of frames in the sample
    sampleLength = fileHandle.frames();
    // printf("length in frames %i\n", fileHandle.frames());
    if (sampleLength == 0)
    {
        //file doesn't exist or is of incompatible type, main handles the -1
        printf("Can't load sample %s \n", fp);
        return -1;
    }
    // get some more info of the sample

    sampleChannels = fileHandle.channels();
    file_samplerate = fileHandle.samplerate();

    // get max value
    double max_val;
    fileHandle.command(SFC_CALC_NORM_MAX_ALL_CHANNELS, &max_val, sizeof(max_val));
    // normalize factor
    float ratio = max_val > 1.0f ? 1.0f : 1.0f / max_val;

    // resize vector
    std::vector<float> sample;
    sample.resize(sampleLength * sampleChannels);
    // load sample memory in samplevector
    fileHandle.read(&sample.at(0), sampleLength * sampleChannels);
    // sum to mono if needed
    sf_count_t size = sampleLength;
    if (sampleChannels == 2)
    { // sum to mono

        for (int i = 0, j = 0; i < size; i++)
        {
            float sum_mono = (sample[j] + sample[j + 1]) * 0.5f;
            waveForm.push_back((sum_mono * ratio) * float(display_height / 2));
            j += 2;
        }
    }
    else
    {
        waveForm.resize(size);
        for (int i = 0; i < size; i++)
        {
            waveForm[i] = (sample[i] * ratio) * float(display_height / 2);
        }
    }

    viewStart = 0;
    viewEnd = waveForm.size();
    viewZoom = 1.0f;
    viewMaxZoom = float(waveForm.size()) / float(display_width);
    sampleLoaded = true;

    // make minimap

    miniMap.resize(display_width);
    double view = viewEnd - viewStart;
    double samples_per_pixel = view / (double)display_width;
    float fIndex;
    uint iIndex;
    for (uint16_t i = 0; i < display_width; i++)
    {
        fIndex = float(viewStart) + (float(i) * samples_per_pixel);
        iIndex = fIndex;
        auto minmax = std::minmax_element(waveForm.begin() + iIndex, waveForm.begin() + iIndex + int(samples_per_pixel));

        signed char min = std::abs(*minmax.first);
        signed char max = std::abs(*minmax.second);
        signed char maxValue = std::max(min, max);
        miniMap[i] = (float)maxValue / (float)(display_height / 2) * (float)minimap_height;
    }
    /* FIXME : only set this when there are loop points */
    float loopStartPixel = static_cast<float>(sampleLoopStart) / samples_per_pixel;
    float loopEndPixel = static_cast<float>(sampleLoopEnd) / samples_per_pixel;
    float sampleInPixel = static_cast<float>(sampleIn) / samples_per_pixel;
    float sampleOutPixel = static_cast<float>(sampleOut) / samples_per_pixel;

    fLoopStart->setAbsolutePos(loopStartPixel - 32, display_bottom - 32);
    fLoopEnd->setAbsolutePos(loopEndPixel, display_bottom - 32);
    fSampleIn->setAbsolutePos(sampleInPixel - 32, display_top);
    fSampleOut->setAbsolutePos(sampleOutPixel, display_top);
    fLoopStart->show();
    fLoopEnd->show();
    fSampleIn->show();
    fSampleOut->show();

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

    if (sampleLoaded)
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
        auto minmax = std::minmax_element(waveForm.begin() + iIndex, waveForm.begin() + iIndex + int(samples_per_pixel));
        uint16_t min = *minmax.first + display_center;
        uint16_t max = *minmax.second + display_center;
        lineTo(i + display_left, min);
        lineTo(i + display_left, max);
    }
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
        moveTo(i, display_bottom + minimap_height);
        lineTo(i, display_bottom + minimap_height - miniMap[i]);
    }
    stroke();
    closePath();

    // draw "handlebar"
    double samples_per_pixel = waveForm.size() / (double)display_width;
    int leftPixel = viewStart / samples_per_pixel;
    int rightPixel = viewEnd / samples_per_pixel;

    // darken left of the bar
    fillColor(0, 0, 0, 64);
    beginPath();
    rect(display_left, display_bottom, display_left + leftPixel, minimap_height);
    fill();
    closePath();
    // darken left of the bar
    beginPath();
    rect(display_left + rightPixel, display_bottom, display_width - rightPixel, minimap_height);
    fill();
    closePath();
}

void DropsUI::drawInOutMarkers()
{

    if (sampleIn >= viewStart && sampleIn <= viewEnd)
    {

        // sample to pixel
        double sample_per_pixel = pow(viewMaxZoom, viewZoom);
        float sampleInPixel = (sampleIn / sample_per_pixel) - (viewStart / sample_per_pixel);
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
        float sampleOutPixel = (sampleOut / sample_per_pixel) - (viewStart / sample_per_pixel);

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
        float loopStartPixel = (sampleLoopStart / sample_per_pixel) - (viewStart / sample_per_pixel);

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
        float loopEndPixel = (sampleLoopEnd / sample_per_pixel) - (viewStart / sample_per_pixel);

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
        loadSample(filename);
        repaint();
    }
}

void DropsUI::stateChanged(const char *key, const char *)
{
    if (strcmp(key, "filepath") == 0)
    {
        printf("state changed... do something?\n");
    }
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

bool DropsUI::onScroll(const ScrollEvent &ev)
{
    // is the pointer in the display
    int x = ev.pos.getX();
    int y = ev.pos.getY();
    if (!display.contains(x, y))
    {
        return false; // get outta here
    }

    if (waveForm.size() <= display_width)
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
            (scroll_delta > 0 && viewEnd == waveForm.size()))
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
    if (viewEnd > waveForm.size())
    {
        viewEnd = waveForm.size();
        start = viewEnd - length;
    }
    samples_per_pixel = pow(viewMaxZoom, viewZoom);
    viewStart = start < 0 ? 0 : start;
    double spp = waveForm.size() / (double)display_width;
    int leftPixel = viewStart / spp;
    int rightPixel = viewEnd / spp;
    fScrollBar->setWidth(rightPixel - leftPixel);
    fScrollBar->setAbsoluteX(leftPixel);

    float loopStartPixel = static_cast<float>(sampleLoopStart - viewStart) / samples_per_pixel;
    float loopEndPixel = static_cast<float>(sampleLoopEnd - viewStart) / samples_per_pixel;
    fLoopStart->setAbsoluteX(loopStartPixel - 32);
    fLoopEnd->setAbsoluteX(loopEndPixel);

    float sampleInPixel = static_cast<float>(sampleIn - viewStart) / samples_per_pixel;
    float sampleOutPixel = static_cast<float>(sampleOut - viewStart) / samples_per_pixel;
    fSampleIn->setAbsoluteX(sampleInPixel - 32);
    fSampleOut->setAbsoluteX(sampleOutPixel);

    repaint();
    return true;
}

bool DropsUI::onMotion(const MotionEvent &ev)
{
    if (scrollbarDragging)
    {
        int distance = ev.pos.getX() - mouseX;
        mouseX = ev.pos.getX();
        float samples_per_pixel = waveForm.size() / (double)display_width;
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

        if (viewEnd > waveForm.size())
        {
            viewEnd = waveForm.size();
            viewStart = (float)waveForm.size() - (float)display_width * pow(viewMaxZoom, viewZoom);
        }
        int leftPixel = (float)viewStart / samples_per_pixel;
        fScrollBar->setAbsoluteX(leftPixel);

        // loop points
        float loopStartPixel = static_cast<float>(sampleLoopStart - viewStart) / pow(viewMaxZoom, viewZoom);
        fLoopStart->setAbsoluteX(loopStartPixel - 32);
        float loopEndPixel = static_cast<float>(sampleLoopEnd - viewStart) / pow(viewMaxZoom, viewZoom);
        fLoopEnd->setAbsoluteX(loopEndPixel);
        repaint();
    }

    if (loopstartDragging)
    {
        float distance = ev.pos.getX() - mouseX;
        mouseX = ev.pos.getX();
        float samples_per_pixel = pow(viewMaxZoom, viewZoom);
        sampleLoopStart = static_cast<float>(sampleLoopStart) + distance * samples_per_pixel;
        sampleLoopStart = clamp<sf_count_t>(sampleLoopStart, sampleLoopEnd - 1, sampleIn);
        float loopStartPixel = static_cast<float>(sampleLoopStart - viewStart) / samples_per_pixel;
        fLoopStart->setAbsoluteX(loopStartPixel - 32);
        repaint();
    }

    if (loopendDragging)
    {
        float distance = ev.pos.getX() - mouseX;
        mouseX = ev.pos.getX();
        float samples_per_pixel = pow(viewMaxZoom, viewZoom);
        sampleLoopEnd = static_cast<float>(sampleLoopEnd) + distance * samples_per_pixel;
        sampleLoopEnd = clamp<sf_count_t>(sampleLoopEnd, sampleOut, sampleLoopStart + 1);
        float loopEndPixel = static_cast<float>(sampleLoopEnd - viewStart) / samples_per_pixel;
        fLoopEnd->setAbsoluteX(loopEndPixel);
        repaint();
    }

    if (sampleInDragging)
    {
        float distance = ev.pos.getX() - mouseX;
        mouseX = ev.pos.getX();
        float samples_per_pixel = pow(viewMaxZoom, viewZoom);
        sampleIn = static_cast<float>(sampleIn) + distance * samples_per_pixel;
        sampleIn = clamp<sf_count_t>(sampleIn, sampleOut - 1, 0);
        float sampleInPixel = static_cast<float>(sampleIn - viewStart) / samples_per_pixel;
        fSampleIn->setAbsoluteX(sampleInPixel - 32);
        repaint();
    }

    if (sampleOutDragging)
    {
        float distance = ev.pos.getX() - mouseX;
        mouseX = ev.pos.getX();
        float samples_per_pixel = pow(viewMaxZoom, viewZoom);
        sampleOut = static_cast<float>(sampleOut) + distance * samples_per_pixel;
        sampleOut = clamp<sf_count_t>(sampleOut, waveForm.size(), sampleIn + 1);
        float sampleOutPixel = static_cast<float>(sampleOut - viewStart) / samples_per_pixel;
        fSampleOut->setAbsoluteX(sampleOutPixel);
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

void DropsUI::scrollBarClicked(ScrollBar *scrollBar, bool dragging)
{
    uint id = scrollBar->getId();
    switch ((id))
    {
    case scrollbar_id:
        scrollbarDragging = dragging;
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

    default:
        break;
    }
}

UI *createUI()
{
    return new DropsUI();
}

// -----------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
