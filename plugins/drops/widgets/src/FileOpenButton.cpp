/* license */
#include "FileOpenButton.hpp"

START_NAMESPACE_DISTRHO

FileOpenButton::FileOpenButton(Window &parent) noexcept
    : NanoWidget(parent)
{
    buttonText = "click to load sample";
    background_color = Color(0.8f, 0.8f, 0.8f);
    text_color = Color(0.1f, 0.1f, 0.1f);
    font_size = 16;
    Roboto_ = createFontFromMemory("Roboto",
                                   reinterpret_cast<const uchar *>(fonts::Roboto_RegularData),
                                   fonts::Roboto_RegularDataSize, false);
}

FileOpenButton::FileOpenButton(Widget *widget) noexcept
    : NanoWidget(widget)
{
    buttonText = "click to load sample";
    background_color = Color(0.8f, 0.8f, 0.8f);
    text_color = Color(0.1f, 0.1f, 0.1f);
    font_size = 16;
    Roboto_ = createFontFromMemory("Roboto",
                                   reinterpret_cast<const uchar *>(fonts::Roboto_RegularData),
                                   fonts::Roboto_RegularDataSize, false);
    
}

bool FileOpenButton::onMouse(const MouseEvent &ev)
{
    if (contains(ev.pos) && ev.press && ev.button == 1)
    {
        callback->onFileOpenButtonClicked(this);
        return true;
    }
    else
    {
        return false;
    }
}

void FileOpenButton::onNanoDisplay()
{
    int width = getWidth();
    int height = getHeight();
    beginPath();
    fillColor(background_color);
    rect(0, 0, width, height);
    fill();
    closePath();

    // text
    beginPath();
    fillColor(text_color);
    fontSize(font_size);
    fontFaceId(Roboto_);
    textAlign(ALIGN_LEFT | ALIGN_MIDDLE);
    Rectangle<float> bounds;
    textBounds(0, 0, buttonText.c_str(), NULL, bounds);
    std::string tempText = buttonText;
    for (int i = 0; i < buttonText.size(); i++) // maybe i = 1 ??
    {
        textBounds(0, 0, tempText.c_str(), NULL, bounds);
        // too large ?
        if (bounds.getWidth() > width)
        {
            // remove 1st character
            tempText = buttonText.substr(i);
        }
        else
            break;
    }

    text(0, std::round(height / 2.0f), tempText.c_str(), NULL);
    closePath();
}

void FileOpenButton::setText(std::string str)
{
    buttonText = str;
}
void FileOpenButton::setCallback(Callback *cb)
{
    callback = cb;
}

END_NAMESPACE_DISTRHO
