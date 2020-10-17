/* license */
#include "Menu.hpp"

START_NAMESPACE_DISTRHO

Menu::Menu(Window &parent) noexcept
    : NanoWidget(parent)
{
    parent.addIdleCallback(this);
    loadSharedResources();
    highlighted_item_ = -1;
    font_size = 14;
    margin = 2.f;
    timer = 0; // if >= 0 menu is visible
    time_out = 150;
}

void Menu::idleCallback()
{
    if (!has_mouse_ && !dropdown_has_mouse && timer > -1)
    {
        timer--;
    }

    if (timer < 0)
        hide();
}

bool Menu::onMouse(const MouseEvent &ev)
{
    if (contains(ev.pos) && ev.press && ev.button == 1)
    {
        if (highlighted_item_ >= 0)
            callback_->menuClicked(this, highlighted_item_, items_[highlighted_item_]);
    }
    return true;
}

bool Menu::onMotion(const MotionEvent &ev)
{
    if (contains(ev.pos))
    {
        has_mouse_ = true;
        timer = time_out;
        const int mouse_y = ev.pos.getY();
        highlighted_item_ = mouse_y / font_size;
        printf("highlighted_item_ %i\n", highlighted_item_);
        repaint();
    }
    else
    {
        has_mouse_ = false;
        if (highlighted_item_ >= 0)
        {
            highlighted_item_ = -1;
            repaint();
        }

        if (timer < 0 && isVisible())
        {
            hide();
            repaint();
        }
    }

    return false;
}

void Menu::onNanoDisplay()
{
    int width = getWidth();
    int height = getHeight();
    beginPath();
    fillColor(169, 169, 169);
    rect(0, 0, width, height);
    fill();
    closePath();
    fontSize(font_size);
    if (highlighted_item_ >= 0)
    {
        beginPath();
        rect(0, font_size * highlighted_item_, width, font_size);
        fillColor(0.2f, 0.2f, 0.2f);
        fill();
        closePath();
        ;
    }
    int j = 0;

    fillColor(0, 0, 0);
    j = 0;
    textAlign(ALIGN_TOP);
    for (auto i : items_)
    {
        if (j == highlighted_item_)
        {
            fillColor(0.8f, 0.8f, 0.8f);
        }
        else
        {
            fillColor(0.1f, 0.1f, 0.1f);
        }
        text(0, font_size * j + margin, i.c_str(), nullptr);
        j++;
    }
}

void Menu::setCallback(Callback *cb)
{
    callback_ = cb;
}

void Menu::addItem(std::string item)
{
    items_.push_back(item);
    resize();
}

void Menu::resize()
{
    //float maxX = 0.0f;
    //float maxY = 0.0f;
    float bounds[4];

    std::stringstream buffer;
    for (auto i : items_)
    {
        buffer << i << "\n";
        //    printf("%s\n",i.c_str());
    }
    printf("%s", buffer.str().c_str());
    textAlign(ALIGN_LEFT || ALIGN_TOP);
    textBoxBounds(0.f, 0.f, 200.f, buffer.str().c_str(), nullptr, bounds);
    printf("xmin %f, ymin %f, xmax %f, ymax %f\n", bounds[0], bounds[1], bounds[2], bounds[3]);
    printf("size xmax - xmin %f , ymax - ymin %f\n", bounds[2] - bounds[0], bounds[3] - bounds[1]);
    setSize(bounds[2] - bounds[0], bounds[3] - bounds[1]);
}

END_NAMESPACE_DISTRHO
