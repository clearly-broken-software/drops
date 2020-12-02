/*
license here
*/

#include "DropsUI.hpp"
#include <iostream>

#define NANOSVG_IMPLEMENTATION
#include "nanosvg.h"
#define NANOSVGRAST_IMPLEMENTATION
#include "nanosvgrast.h"

using namespace artwork;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------

DropsUI::DropsUI()
    : UI(UI_W, UI_H)
{
    loadSharedResources();
    plugin = static_cast<DropsPlugin *>(getPluginInstancePointer());

    viewStart = 0;
    viewEnd = 0;
    viewZoom = 0;
    viewMaxZoom = 0;
    mouseX = 0;

    sampleChannels = 0;
    sampleLength = 0;
    filepath = "";
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

    /* for testing */
    sampleLoopStart = 0;
    sampleLoopEnd = 0;
    sampleIn = 0;
    sampleOut = 0;
    /* ----------- */

    scale = 1.0f;
    makeIcons();
    initWidgets();
    if (plugin->loadedSample)
    {
        loadSample();
        std::string filename = plugin->path;
        fileopen_button->setText(filename);
    }
}

DropsUI::~DropsUI()
{
}

void DropsUI::initWidgets()
{
    Window &window = getParentWindow();
    fileopen_button = new FileOpenButton(window);
    fileopen_button->setCallback(this);
    fileopen_button->setAbsolutePos(238, 0);
    fileopen_button->setSize(530, 55);
    fileopen_button->background_color = black_olive;
    fileopen_button->text_color = floral_white;
    fileopen_button->font_size = 24.f;

    fScrollBarHandle = new ScrollBar(window);
    fScrollBarHandle->setId(kScrollbarHandle);
    fScrollBarHandle->setSize(display_width, minimap_height);
    fScrollBarHandle->setAbsolutePos(display_left, display_bottom);
    fScrollBarHandle->setCallback(this);

    fScrollBarLeft = new ScrollBar(window);
    fScrollBarLeft->setId(kScrollbarLeft);
    fScrollBarLeft->setSize(0, minimap_height);
    fScrollBarLeft->setAbsolutePos(display_left, display_bottom);
    fScrollBarLeft->setCallback(this);

    fScrollBarRight = new ScrollBar(window);
    fScrollBarRight->setId(kScrollbarRight);
    fScrollBarRight->setSize(0, minimap_height);
    fScrollBarRight->setAbsolutePos(display_right, display_bottom);
    fScrollBarRight->setCallback(this);

    fLoopStart = new ScrollBar(window);
    fLoopStart->setId(kSampleLoopStart);
    fLoopStart->setSize(35, 35); // FIXME: hardcoded
    fLoopStart->setCallback(this);
    fLoopStart->hide();

    fLoopEnd = new ScrollBar(window);
    fLoopEnd->setId(kSampleLoopEnd);
    fLoopEnd->setSize(35, 35);
    fLoopEnd->setCallback(this);
    fLoopEnd->hide();

    fSampleIn = new ScrollBar(window);
    fSampleIn->setId(kSampleIn);
    fSampleIn->setSize(35, 35);
    fSampleIn->setCallback(this);
    fSampleIn->hide();

    fSampleOut = new ScrollBar(window);
    fSampleOut->setId(kSampleOut);
    fSampleOut->setSize(35, 35);
    fSampleOut->setCallback(this);
    fSampleOut->hide();

    box_tabs = new HBox(window);

    button_sample = new TextButton(box_tabs);
    button_sample->setId(kButtonSample);
    button_sample->setCallback(this);
    button_sample->setSize(100, 40);
    button_sample->setText("SAMPLE");
    button_sample->background_color = black_olive_4;
    button_sample->foreground_color = floral_white;
    button_sample->highlight_color = flame;

    button_amp = new TextButton(box_tabs);
    button_amp->setId(kButtonAmp);
    button_amp->setCallback(this);
    button_amp->setSize(100, 40); //FIXME: harcoded
    button_amp->setText("AMP");
    button_amp->background_color = black_olive_4;
    button_amp->foreground_color = floral_white;
    button_amp->highlight_color = saffron;

    button_pitch = new TextButton(box_tabs);
    button_pitch->setId(kButtonPitch);
    button_pitch->setCallback(this);
    button_pitch->setSize(100, 40);
    button_pitch->setText("PITCH");
    button_pitch->background_color = black_olive_4;
    button_pitch->foreground_color = floral_white;
    button_pitch->highlight_color = shamrock_green;

    button_filter = new TextButton(box_tabs);
    button_filter->setId(kButtonFilter);
    button_filter->setCallback(this);
    button_filter->setSize(100, 40);
    button_filter->setText("FILTER");
    button_filter->background_color = black_olive_4;
    button_filter->foreground_color = floral_white;
    button_filter->highlight_color = blue_pigment;

    box_tabs->setAbsolutePos(0, display_bottom + minimap_height);
    box_tabs->setWidth(getWidth());
    box_tabs->justify_content = HBox::Justify_Content::left;
    box_tabs->addWidget(button_sample);
    box_tabs->addWidget(button_amp);
    box_tabs->addWidget(button_pitch);
    box_tabs->addWidget(button_filter);
    box_tabs->positionWidgets();

    hbox_zoom_icons = new HBox(this);
    hbox_zoom_icons->setId(kHboxZoomIcons);
    hbox_zoom_icons->setAbsolutePos(15, 70);
    hbox_zoom_icons->setWidth(150);

    fZoomOut = new SVGButton(hbox_zoom_icons, zoom_out, zoom_out_hl);
    fZoomOut->setId(kZoomOut);
    fZoomOut->setCallback(this);

    fZoomIn = new SVGButton(hbox_zoom_icons, zoom_in, zoom_in_hl);
    fZoomIn->setId(kZoomIn);
    fZoomIn->setCallback(this);

    fZoomAll = new SVGButton(hbox_zoom_icons, zoom_all, zoom_all_hl);
    fZoomAll->setId(kZoomAll);
    fZoomAll->setCallback(this);

    fZoomInOut = new SVGButton(hbox_zoom_icons, zoom_in_out, zoom_in_out_hl);
    fZoomInOut->setId(kZoomInOut);
    fZoomInOut->setCallback(this);

    hbox_zoom_icons->addWidget(fZoomOut);
    hbox_zoom_icons->addWidget(fZoomIn);
    hbox_zoom_icons->addWidget(fZoomAll);
    hbox_zoom_icons->addWidget(fZoomInOut);

    hbox_zoom_icons->positionWidgets();

    initTabSample();
    initTabAmp();
    initTabPitch();
    initTabFilter();
    showTabSample();
}

void DropsUI::makeIcons()
{
    dropsLogo = new SVGImage(this, drops_logo, 1.0f);
    loopLeft = new SVGImage(this, loop_left, 1.0f);
    loopRight = new SVGImage(this, loop_right, 1.0f);
    clearlyBrokenLogo = new SVGImage(this, artwork::clearly_broken_logo, 0.8f);
}
void DropsUI::parameterChanged(uint32_t index, float value)
{
#ifdef DEBUG
    if (index != kSampleLoaded)
        printf("parameterChanged(%i,%f)\n", index, value);
#endif
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
    case kSamplePitchKeyCenter:
        fSamplePitchKeyCenter->setValue(value);
        break;
    case kSamplePlayMode:
        fSamplePlayMode->setValue(value);
        break;
    case kSamplePlayDirection:
        fSamplePlayDirection->setValue(value);
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
    case kAmpLFOType:
        fAmpLFOType->setValue(value);
        repaint();
        break;
    case kAmpLFOFreq:
        fAmpLFOFreq->setValue(value);
        repaint();
        break;
    case kAmpLFODepth:
        fAmpLFODepth->setValue(value);
        repaint();
        break;
        /*  pitch tab */
    case kPitchEgAttack:
        fPitchEgAttack->setValue(value);
        break;
    case kPitchEgDecay:
        fPitchEgDecay->setValue(value);
        break;
    case kPitchEgSustain:
        fPitchEgSustain->setValue(value);
        break;
    case kPitchEgRelease:
        fPitchEgRelease->setValue(value);
        break;
    // case kPitchEgDepth:
    //     fPitchEgDepth->setValue(value);
    //     break;
    case kPitchLFOType:
        fPitchLFOType->setValue(value);
        break;
    case kPitchLFOFreq:
        fPitchLFOFreq->setValue(value);
        break;
    case kPitchLFODepth:
        fPitchLFODepth->setValue(value);
        break;

    case kActiveTab:
    {
        const uint index = static_cast<uint>(value);

        switch (index)
        {
        case 0:
            button_sample->isActive = true;
            button_amp->isActive = false;
            button_pitch->isActive = false;
            button_filter->isActive = false;
            tabEdge = button_sample->highlight_color;
            showTabSample();
            hideTabAmp();
            hideTabPitch();
            hideTabFilter();

            break;
        case 1:
            button_sample->isActive = false;
            button_amp->isActive = true;
            button_pitch->isActive = false;
            button_filter->isActive = false;
            tabEdge = button_amp->highlight_color;
            hideTabSample();
            showTabAmp();
            hideTabPitch();
            hideTabFilter();
            break;
        case 2:
            button_sample->isActive = false;
            button_amp->isActive = false;
            button_pitch->isActive = true;
            button_filter->isActive = false;
            tabEdge = button_pitch->highlight_color;
            hideTabSample();
            hideTabAmp();
            showTabPitch();
            hideTabFilter();
            break;
        case 3:
            button_sample->isActive = false;
            button_amp->isActive = false;
            button_pitch->isActive = false;
            button_filter->isActive = true;
            tabEdge = button_filter->highlight_color;
            hideTabSample();
            hideTabAmp();
            hideTabPitch();
            showTabFilter();
            break;

        default:
            printf("unexpected tab id\n");
            break;
        }
        repaint();
    }
    break;

    default:
        printf("DropsUI::parameterChanged(%i,%f)\n", index, value);
        break;
    }
}

int DropsUI::loadSample()
{
    sampleLength = static_cast<sf_count_t>(waveForm->size() - 1);
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

    fLoopStart->setAbsolutePos(loopStartPixel - 35 - 1, display_bottom - 35);
    fLoopEnd->setAbsolutePos(loopEndPixel, display_bottom - 35);
    fSampleIn->setAbsolutePos(sampleInPixel - 35 - 1, display_top);
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
    fillColor(eerie_black);
    rect(0.0f, 0.0f, width, height);
    fill();
    closePath();

    beginPath();
    fillColor(black_olive);
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

    // draw logos
    uint w = dropsLogo->getWidth();
    uint h = dropsLogo->getHeight();
    int x = fileopen_button->getAbsoluteX() / 2 - w / 2;
    int y = fileopen_button->getHeight() / 2 - h / 2;
    dropsLogo->drawAt(x, y);
    const int fo_right = fileopen_button->getAbsoluteX() + fileopen_button->getWidth();
    const int half_right_space = (width - fo_right) / 2;
    const int half_cb_logo = clearlyBrokenLogo->getWidth() / 2;
    x = fo_right + half_right_space - half_cb_logo;
    y = fileopen_button->getHeight() / 2 - clearlyBrokenLogo->getHeight() / 2;
    clearlyBrokenLogo->drawAt(x, y);

    // tab background
    beginPath();
    //fillPaint(tab_background);
    fillColor(eerie_black_4);
    strokeColor(saffron);
    rect(tabs_x, tabs_y, tabs_w, tabs_h);
    fill();
    closePath();

    beginPath();
    strokeWidth(2.0f);
    strokeColor(tabEdge);
    rect(tabs_x + 2, tabs_y + 2, tabs_w - 4, tabs_h - 4);
    stroke();
    closePath();
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
    strokeColor(pale_silver);
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
    strokeColor(pale_silver);
    strokeWidth(1);
    float brighten = 1.5f;
    float r = black_olive.red * brighten;
    float g = black_olive.green * brighten;
    float b = black_olive.blue * brighten;
    fillColor(r, g, b);
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
        fillColor(pale_silver);
        strokeColor(flame);
        strokeWidth(1.0f);
        // line
        beginPath();
        moveTo(sampleInPixel, display_top);
        lineTo(sampleInPixel, display_bottom);
        stroke();
        closePath();
        // handle
        beginPath();
        roundedRect(sampleInPixel - 35.f, display_top, 35.f, 35.f, 35.f);
        fill();
        closePath();
        beginPath();
        rect(sampleInPixel - 35.f * .5f, display_top, 35 * .5f, 35.f);
        fill();
        closePath();
        // IN text
        beginPath();
        fillColor(eerie_black);
        fontSize(sample_inout_font_size);
        textAlign(ALIGN_CENTER | ALIGN_MIDDLE);
        text(sampleInPixel - 16.f, display_top + 16.f, "IN", NULL);

        closePath();
    }

    if (sampleOut <= viewEnd && sampleOut >= viewStart)
    {
        double sample_per_pixel = pow(viewMaxZoom, viewZoom);
        float sampleOutPixel = (sampleOut / sample_per_pixel) - (viewStart / sample_per_pixel) + static_cast<float>(display_left);

        fillColor(pale_silver);
        strokeColor(flame);
        strokeWidth(1.0f);
        // line
        beginPath();
        moveTo(sampleOutPixel, display_top);
        lineTo(sampleOutPixel, display_bottom);
        stroke();
        closePath();
        // handle
        beginPath();
        roundedRect(sampleOutPixel, display_top, 35.f, 35.f, 35.f);
        fill();
        closePath();

        beginPath();
        rect(sampleOutPixel, display_top, 35.f * .5f, 35.f);
        fill();
        closePath();
        // text OUT
        beginPath();

        fillColor(eerie_black);
        fontSize(sample_inout_font_size);
        textAlign(ALIGN_CENTER | ALIGN_MIDDLE);
        text(sampleOutPixel + 35.f * 0.5f, display_top + 35.f * .5f, "OUT", NULL);
        closePath();
    }
}

void DropsUI::drawLoopMarkers()
{

    if (sampleLoopStart >= viewStart && sampleLoopStart <= viewEnd)
    {
        fillColor(pale_silver);
        strokeColor(flame);
        strokeWidth(1.0f);
        // sample to pixel
        double sample_per_pixel = pow(viewMaxZoom, viewZoom);
        float loopStartPixel = (sampleLoopStart / sample_per_pixel) - (viewStart / sample_per_pixel) + static_cast<float>(display_left);

        // line
        beginPath();
        moveTo(loopStartPixel, display_bottom);
        lineTo(loopStartPixel, display_top);
        stroke();
        closePath();
        // handle

        loopLeft->drawAt(loopStartPixel - 35, display_bottom - 35);
    }

    if (sampleLoopEnd <= viewEnd && sampleLoopEnd >= viewStart)
    {
        fillColor(pale_silver);
        strokeColor(flame);
        strokeWidth(1.0f);
        double sample_per_pixel = pow(viewMaxZoom, viewZoom);
        float loopEndPixel = (sampleLoopEnd / sample_per_pixel) - (viewStart / sample_per_pixel) + static_cast<float>(display_left);

        // line
        beginPath();
        moveTo(loopEndPixel, display_bottom);
        lineTo(loopEndPixel, display_top);
        stroke();
        closePath();
        // handle
        loopRight->drawAt(loopEndPixel, display_bottom - 35);
    }

    // do stuff
}

void DropsUI::uiFileBrowserSelected(const char *filename)
{
    // if a file was selected, tell DSP
    if (filename != nullptr)
    {

        fileopen_button->setText(filename);
        setState("filepath", filename);
        repaint();
    }
}

void DropsUI::stateChanged(const char *key, const char *value)
{
#ifdef DEBUG
    printf("key = %s, value = %s\n", key, value);
#endif
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
    fLoopStart->setAbsoluteX(loopStartPixel - 35); //FIXME: hardcoded offset
    fLoopEnd->setAbsoluteX(loopEndPixel);

    float sampleInPixel = static_cast<float>(sampleIn - viewStart) / samples_per_pixel + static_cast<float>(display_left);
    float sampleOutPixel = static_cast<float>(sampleOut - viewStart) / samples_per_pixel + static_cast<float>(display_left);
    fSampleIn->setAbsoluteX(sampleInPixel - 35); //FIXME: hardcoded offset
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
    printf("scroll delta x %f\n", scroll_delta);
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
        //sampleLoopStart = clamp<sf_count_t>(sampleLoopStart, sampleLoopEnd - 1, sampleIn);
        float loopStartPixel = static_cast<float>(sampleLoopStart - viewStart) / samples_per_pixel + static_cast<float>(display_left);
        fLoopStart->setAbsoluteX(loopStartPixel - 35);
        /*
        float value = static_cast<float>(sampleLoopStart) / static_cast<float>(waveForm->size());
        setParameterValue(kSampleLoopStart, value);
         */
        repaint();
    }

    if (loopendDragging)
    {
        float distance = ev.pos.getX() - mouseX;
        mouseX = ev.pos.getX();
        float samples_per_pixel = pow(viewMaxZoom, viewZoom);
        sampleLoopEnd = static_cast<float>(sampleLoopEnd) + distance * samples_per_pixel;
        //sampleLoopEnd = clamp<sf_count_t>(sampleLoopEnd, sampleOut, sampleLoopStart + 1);
        float loopEndPixel = static_cast<float>(sampleLoopEnd - viewStart) / samples_per_pixel + static_cast<float>(display_left);
        fLoopEnd->setAbsoluteX(loopEndPixel);
        /* 
        float value = static_cast<float>(sampleLoopEnd) / static_cast<float>(waveForm->size());
        setParameterValue(kSampleLoopEnd, value);
         */
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
        fSampleIn->setAbsoluteX(sampleInPixel - 35);
        // float value = static_cast<float>(sampleIn) / static_cast<float>(waveForm->size());
        // setParameterValue(kSampleIn, value);
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
        // float value = static_cast<float>(sampleOut) / static_cast<float>(waveForm->size());
        // setParameterValue(kSampleOut, value);
        repaint();
    }

    return false;
}

void DropsUI::onFileOpenButtonClicked(FileOpenButton *)
{
    DGL::Window::FileBrowserOptions opts;
    opts.title = "Load SFZ";
    opts.buttons.showPlaces = 2;
    getParentWindow().openFileBrowser(opts);
}

void DropsUI::onTextButtonClicked(TextButton *tb)
{
    tabEdge = tb->highlight_color;
    const uint id = tb->getId();
    switch (id)
    {
    case kButtonSample:
        button_sample->isActive = true;
        button_amp->isActive = false;
        button_pitch->isActive = false;
        button_filter->isActive = false;
        showTabSample();
        hideTabAmp();
        hideTabPitch();
        hideTabFilter();
        setParameterValue(kActiveTab, 0);
        break;
    case kButtonAmp:
        button_sample->isActive = false;
        button_amp->isActive = true;
        button_pitch->isActive = false;
        button_filter->isActive = false;
        hideTabSample();
        showTabAmp();
        hideTabPitch();
        hideTabFilter();
        setParameterValue(kActiveTab, 1);
        break;
    case kButtonPitch:
        button_sample->isActive = false;
        button_amp->isActive = false;
        button_pitch->isActive = true;
        button_filter->isActive = false;
        hideTabSample();
        hideTabAmp();
        showTabPitch();
        hideTabFilter();
        setParameterValue(kActiveTab, 2);
        break;
    case kButtonFilter:
        button_sample->isActive = false;
        button_amp->isActive = false;
        button_pitch->isActive = false;
        button_filter->isActive = true;

        hideTabSample();
        hideTabAmp();
        hideTabPitch();
        showTabFilter();
        setParameterValue(kActiveTab, 3);
        break;
    default:
        printf("unexpected tab id\n");
        break;
    }
}

void DropsUI::onDropDownClicked(DropDown *dropDown)
{
    uint id = dropDown->getId();
    switch (id)
    {
    // case kSampleNormalize:
    //     fNormalizeMenu->show();
    //     break;
    case kSamplePitchKeyCenter:
        fKeyCenterMenu->show();
        break;
    case kSamplePlayMode:
        fPlayModeMenu->show();
        break;
    case kSamplePlayDirection:
        fDirectionMenu->show();
        break;
    case kAmpLFOType:
        fAmpLFOTypeMenu->show();
        break;
    // case kAmpLFOSync:
    //     fAmpLFOSyncMenu->show();
    //     break;
    case kPitchLFOType:
        fPitchLFOTypeMenu->show();
        break;
    case kFilterType:
        fFilterTypeMenu->show();
        break;
    case kFilterLFOType:
        fFilterLFOTypeMenu->show();
        break;

    default:
#ifdef DEBUG
        printf("dropdown %i clicked\n", id);
#endif
        break;
    }
}
void DropsUI::knobDragStarted(Knob *knob)
{
    const uint id = knob->getId();
#ifdef DEBUG
    printf("%i , drag started\n", id);
#endif
}
void DropsUI::knobDragFinished(Knob *knob)
{
    int id = knob->getId();
#ifdef DEBUG
    printf("%i , drag finished\n", id);
#endif
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

    case kPitchEgAttack:
        setParameterValue(kPitchEgAttack, value);
        break;
    case kPitchEgDecay:
        setParameterValue(kPitchEgDecay, value);
        break;
    case kPitchEgSustain:
        setParameterValue(kPitchEgSustain, value);
        break;
    case kPitchEgRelease:
        setParameterValue(kPitchEgRelease, value);
        break;
    case kFilterEgAttack:
        setParameterValue(kFilterEgAttack, value);
        break;
    case kFilterEgDecay:
        setParameterValue(kFilterEgDecay, value);
        break;
    case kFilterEgSustain:
        setParameterValue(kFilterEgSustain, value);
        break;
    case kFilterEgRelease:
        setParameterValue(kFilterEgRelease, value);
        break;
    case kFilterCutOff:
        setParameterValue(kFilterCutOff, value);
        break;

    default:
#ifdef DEBUG
        printf("knobValueChanged(%i, %f)\n", id, value);
#endif
        break;
    }
    repaint();
}

void DropsUI::onSliderValueChanged(Slider *slider, float value)
{
    uint id = slider->getId();

    switch (id)
    {
    case kAmpLFOFreq:
        setParameterValue(kAmpLFOFreq, value);
        break;
    case kAmpLFODepth:
        setParameterValue(kAmpLFODepth, value);
        break;
    case kPitchLFOFreq:
        setParameterValue(kPitchLFOFreq, value);
        break;
    case kPitchLFODepth:
        setParameterValue(kPitchLFODepth, value);
        break;
    case kFilterLFODepth:
        setParameterValue(kFilterLFODepth, value);
        break;
    case kFilterLFOFreq:
        setParameterValue(kFilterLFOFreq, value);
        break;
    default:
#ifdef DEBUG
        printf("slider %i, value %f\n", id, value);
#endif
        break;
    }
}

void DropsUI::onScrollBarClicked(ScrollBar *scrollBar, bool dragging)
{
    uint id = scrollBar->getId();
    switch (id)
    {
    case kScrollbarHandle:
        scrollbarDragging = dragging;
        break;
    case kScrollbarLeft:
        if (!dragging)
        {
            scrollWaveform(true);
        }
        break;
    case kScrollbarRight:
        if (!dragging)
        {
            scrollWaveform(false);
        }
        break;
    case kSampleLoopStart:
    {
        loopstartDragging = dragging;
        if (!loopstartDragging)
        {
            float value = static_cast<float>(sampleLoopStart) / static_cast<float>(waveForm->size());
            setParameterValue(kSampleLoopStart, value);
        }
        break;
    }
    case kSampleLoopEnd:
    {
        loopendDragging = dragging;
        if (!loopendDragging)
        {
            float value = static_cast<float>(sampleLoopEnd) / static_cast<float>(waveForm->size());
            setParameterValue(kSampleLoopEnd, value);
        }
        break;
    }
    case kSampleIn:
    {
        sampleInDragging = dragging;
        if (!sampleInDragging)
        {
            float value = static_cast<float>(sampleIn) / static_cast<float>(waveForm->size());
            setParameterValue(kSampleIn, value);
        }
        break;
    }
    case kSampleOut:
    {
        sampleOutDragging = dragging;
        if (!sampleOutDragging)
        {
            float value = static_cast<float>(sampleOut) / static_cast<float>(waveForm->size());
            setParameterValue(kSampleOut, value);
        }
        break;
    }
    default:
        break;
    }
}

void DropsUI::onMenuClicked(Menu *menu, uint menu_id, std::string item)
{
    const uint id = menu->getId();
#ifdef DEBUG
    printf("menu %i ,menu_id %i, item %s\n", id, menu_id, item.c_str());
#endif
    switch (id)
    {
    case kPlayModeMenu:
        fSamplePlayMode->item = item;
        fPlayModeMenu->hide();
        setParameterValue(kSamplePlayMode, menu_id);
        break;
    // case kNormalizeMenu:
    //     fSampleNormalize->item = item;
    //     fNormalizeMenu->hide();
    //     setParameterValue(kSampleNormalize, menu_id);
    //     break;
    case kKeyCenterMenu:
        fSamplePitchKeyCenter->item = item;
        fKeyCenterMenu->hide();
        setParameterValue(kSamplePitchKeyCenter, menu_id);
        break;
    case kDirectionMenu:
        fSamplePlayDirection->item = item;
        fDirectionMenu->hide();
        setParameterValue(kSamplePlayDirection, menu_id);
        break;
    case kAmpLFOTypeMenu:
        fAmpLFOType->item = item;
        fAmpLFOTypeMenu->hide();
        setParameterValue(kAmpLFOType, menu_id);
        break;
    // case kAmpLFOSyncMenu:
    //     fAmpLFOSync->item = item;
    //     fAmpLFOSyncMenu->hide();
    //     setParameterValue(kAmpLFOSync, menu_id);
    //     break;
    case kPitchLFOTypeMenu:
        fPitchLFOType->item = item;
        fPitchLFOTypeMenu->hide();
        setParameterValue(kPitchLFOType, menu_id);
        break;
    case kFilterTypeMenu:
        fFilterType->item = item;
        fFilterTypeMenu->hide();
        setParameterValue(kFilterType, menu_id);
        break;
    case kFilterLFOTypeMenu:
        fFilterLFOType->item = item;
        fFilterLFOTypeMenu->hide();
        setParameterValue(kFilterLFOType, menu_id);
        break;
    default:

        break;
    }
}

// void DropsUI::onRadioButtonClicked(RadioButton *rb)
// {
//     const uint id = rb->getId();
//     switch (id)
//     {
//     case kAmpLFOFreqBeat:
//         printf("active_option %i\n", rb->active_option);
//         break;

//     default:
//         break;
//     }
// }

void DropsUI::onSVGButtonClicked(SVGButton *svgb)
{
    const uint id = svgb->getId();
    switch (id)
    {
    case kZoomOut:
        zoomButtons(0.05f);
        break;
    case kZoomIn:
        zoomButtons(-0.05f);
        break;
    case kZoomAll:
        viewStart = 0;
        viewEnd = sampleLength;
        viewZoom = 1.0f;
        setScrollbarWidgets();
        setMarkers();
        repaint();
        break;
    case kZoomInOut:
    {
        viewStart = sampleIn;
        viewEnd = sampleOut;
        viewEnd = clamp<sf_count_t>(viewEnd, sampleLength, sampleIn + display_width);
        const float inOutLength = static_cast<float>(sampleOut - sampleIn);
        const float samples_pp = inOutLength / static_cast<float>(display_width);
        const float l2_spp = log2(samples_pp);
        const float l2_vmz = log2(viewMaxZoom);
        viewZoom = l2_spp / l2_vmz;
        viewZoom = clamp<float>(viewZoom, 1.0f, 0.0f);
        setScrollbarWidgets();
        setMarkers();
        repaint();
        break;
    }

    default:
#ifdef DEBUG
        printf("undefined svg button clicked\n");
#endif
        break;
    }
}

void DropsUI::zoomButtons(float zoom_delta)
{
    if (waveForm->size() <= display_width)
        return; // can't zoom anyway

    int x = getWidth() / 2; // off set in pixels, center screen

    // we use a signed int to be able to handle temporary negative starts.
    int start;
    float samples_per_pixel;

    // We either zoom in/out, or ...
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
    setScrollbarWidgets();
    setMarkers();
    repaint();
}

UI *createUI()
{
    return new DropsUI();
}

// -----------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
