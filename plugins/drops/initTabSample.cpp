/* license */
#include "DropsUI.hpp"

START_NAMESPACE_DISTRHO

void DropsUI::initTabSample()
{
    Window &window = getParentWindow();
    box_sample = new VBox(window);
    box_sample->setId(kVBoxSample);
    box_sample_row_1 = new HBox(box_sample);
    box_sample_row_1->setId(kHBoxSampleRow1);

    fSampleXFade = new Slider(box_sample_row_1);
    fSampleXFade->setId(kSampleXFade);
    fSampleXFade->setCallback(this);
    fSampleXFade->setSize(300, 20);
    fSampleXFade->setLabel("X-FADE :");
    fSampleXFade->background_color = pale_silver;
    fSampleXFade->foreground_color = floral_white;
    fSampleXFade->highlight_color = flame;
    fSampleXFade->text_color = floral_white;
    fSampleXFade->unit = "MS";
    fSampleXFade->right_padding = slider_right_padding;
    fSampleXFade->format_str = "%.2f";

    fSampleNormalize = new DropDown(box_sample_row_1);
    fSampleNormalize->setId(kSampleNormalize);
    fSampleNormalize->font_size = 16;
    fSampleNormalize->setSize(300,
                              fSampleNormalize->font_size + fSampleNormalize->margin * 2.0f);
    fSampleNormalize->setCallback(this);
    fSampleNormalize->label = "NORMALIZE :";
    fSampleNormalize->item = "NONE";
    fSampleNormalize->foreground_color = floral_white;
    fSampleNormalize->background_color = black_olive;
    fSampleNormalize->text_color = floral_white;

    fNormalizeMenu = new Menu(box_sample_row_1);
    fNormalizeMenu->setId(kNormalizeMenu);
    fNormalizeMenu->setCallback(this);
    fNormalizeMenu->addItem("NONE");
    fNormalizeMenu->addItem("SAMPLE");
    fNormalizeMenu->addItem("ACTIVE REGION");
    fNormalizeMenu->font_size = 16;
    fNormalizeMenu->hide();
    fNormalizeMenu->background_color = black_olive;
    fNormalizeMenu->foreground_color = floral_white;
    fNormalizeMenu->highlight_color = flame;
    fNormalizeMenu->text_color = floral_white;

    fSamplePitchKeyCenter = new DropDown(box_sample_row_1);
    fSamplePitchKeyCenter->setId(kSamplePitchKeyCenter);
    fSamplePitchKeyCenter->font_size = 16;
    fSamplePitchKeyCenter->setSize(300,
                                   fSamplePitchKeyCenter->font_size +
                                       fSamplePitchKeyCenter->margin * 2.0f);
    fSamplePitchKeyCenter->setCallback(this);
    fSamplePitchKeyCenter->label = "PITCH KEY-CENTER :";
    fSamplePitchKeyCenter->item = "C4";
    fSamplePitchKeyCenter->foreground_color = floral_white;
    fSamplePitchKeyCenter->background_color = black_olive;
    fSamplePitchKeyCenter->text_color = floral_white;

    fKeyCenterMenu = new Menu(box_sample_row_1);
    fKeyCenterMenu->setId(kKeyCenterMenu);
    fKeyCenterMenu->setCallback(this);
    fKeyCenterMenu->addItem("C4");
    fKeyCenterMenu->addItem("D4");
    fKeyCenterMenu->addItem("E4");
    fKeyCenterMenu->font_size = 16;
    fKeyCenterMenu->hide();
    fKeyCenterMenu->background_color = black_olive;
    fKeyCenterMenu->foreground_color = floral_white;
    fKeyCenterMenu->highlight_color = flame;
    fKeyCenterMenu->text_color = floral_white;

    box_sample->setAbsolutePos(tabs_x, tabs_y);
    box_sample->setWidth(getWidth());
    box_sample->setHeight(getHeight() - tabs_y);

    box_sample_row_1->setWidth(box_sample->getWidth());
    // box_sample_row_1->setHeight(100);
    box_sample->addWidget(box_sample_row_1);
    box_sample->setWidgetResize(kHBoxSampleRow1, true);

    box_sample_row_1->addWidget(fSampleXFade);
    box_sample_row_1->addWidget(fSampleNormalize);
    box_sample_row_1->addWidget(fSamplePitchKeyCenter);

    // row 2
    box_sample_row_2 = new HBox(box_sample);
    box_sample_row_2->setId(kHBoxSampleRow2);

    fSamplePlayMode = new DropDown(box_sample_row_2);
    fSamplePlayMode->setId(kSamplePlayMode);
    fSamplePlayMode->font_size = 16;
    fSamplePlayMode->setSize(216,
                             fSamplePlayMode->font_size +
                                 fSamplePlayMode->margin * 2.0f);
    fSamplePlayMode->setCallback(this);
    fSamplePlayMode->label = "LOOP MODE:";
    fSamplePlayMode->item = "NO LOOP";
    fSamplePlayMode->foreground_color = floral_white;
    fSamplePlayMode->background_color = black_olive;
    fSamplePlayMode->text_color = floral_white;

    fPlayModeMenu = new Menu(box_sample_row_2);
    fPlayModeMenu->setId(kPlayModeMenu);
    fPlayModeMenu->setCallback(this);
    fPlayModeMenu->addItem("NO LOOP");
    fPlayModeMenu->addItem("ONE SHOT");
    fPlayModeMenu->addItem("CONTINUOUS");
    fPlayModeMenu->addItem("SUSTAIN");
    fPlayModeMenu->font_size = 16;
    fPlayModeMenu->hide();
    fPlayModeMenu->background_color = black_olive;
    fPlayModeMenu->foreground_color = floral_white;
    fPlayModeMenu->highlight_color = flame;
    fPlayModeMenu->text_color = floral_white;

    fSamplePlayDirection = new DropDown(box_sample_row_2);
    fSamplePlayDirection->setId(kSamplePlayDirection);
    fSamplePlayDirection->font_size = 16;
    fSamplePlayDirection->setSize(216,
                                  fSamplePlayDirection->font_size +
                                      fSamplePlayDirection->margin * 2.0f);
    fSamplePlayDirection->setCallback(this);
    fSamplePlayDirection->label = "DIRECTION: ";
    fSamplePlayDirection->item = "FORWARD";
    fSamplePlayDirection->foreground_color = floral_white;
    fSamplePlayDirection->background_color = black_olive;
    fSamplePlayDirection->text_color = floral_white;

    fDirectionMenu = new Menu(box_sample_row_2);
    fDirectionMenu->setId(kDirectionMenu);
    fDirectionMenu->setCallback(this);
    fDirectionMenu->addItem("FORWARD");
    fDirectionMenu->addItem("REVERSE");
    fDirectionMenu->font_size = 16;
    fDirectionMenu->hide();
    fDirectionMenu->background_color = black_olive;
    fDirectionMenu->foreground_color = floral_white;
    fDirectionMenu->highlight_color = flame;
    fDirectionMenu->text_color = floral_white;

    box_sample_row_2->setWidth(box_sample->getWidth() * 2 / 3);
    box_sample_row_2->align_items = HBox::Align_Items::top;

    box_sample->addWidget(box_sample_row_2);
    box_sample->setWidgetResize(kHBoxSampleRow2, true);

    box_sample_row_2->addWidget(fSamplePlayMode);
    box_sample_row_2->addWidget(fSamplePlayDirection);

    box_sample->positionWidgets();
    box_sample_row_1->positionWidgets();
    box_sample_row_2->positionWidgets();

    fSamplePlayDirection->setMenu(fDirectionMenu);
    fSamplePlayMode->setMenu(fPlayModeMenu);
    fSamplePitchKeyCenter->setMenu(fKeyCenterMenu);
    fSampleNormalize->setMenu(fNormalizeMenu);
}

void DropsUI::hideTabSample()
{
    box_sample->hide();
    box_sample_row_1->hide();
    box_sample_row_2->hide();
    fSampleXFade->hide();
    //fSampleTune->hide(); FIXME:
    fSampleNormalize->hide();
    fSamplePitchKeyCenter->hide();
    fSamplePlayMode->hide();
    fSamplePlayDirection->hide();
}

void DropsUI::showTabSample()
{
    box_sample->show();
    box_sample_row_1->show();
    box_sample_row_2->show();
    fSampleXFade->show();
    //fSampleTune->show();
    fSampleNormalize->show();
    fSamplePitchKeyCenter->show();
    fSamplePlayMode->show();
    fSamplePlayDirection->show();
}

END_NAMESPACE_DISTRHO