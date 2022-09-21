//==============================================================================

#include "MainComponent.h"
#include "MainApplication.h"

MainComponent::MainComponent() {
    addAndMakeVisible(transparencyLabel);
    transparencyLabel.setText("Transparency: ", dontSendNotification);
    addAndMakeVisible(fontSizeLabel);
    fontSizeLabel.setText("Font size: ", dontSendNotification);
    addAndMakeVisible(fontStyleLabel);
    fontStyleLabel.setText("Font style: ", dontSendNotification);
    addAndMakeVisible(textEditor);
    
    
    addAndMakeVisible(clearButton);
    clearButton.addListener(this); //so we can write statement(like line 34)
    
    addAndMakeVisible(boldStyleButton);
    boldStyleButton.addListener(this);
    
    addAndMakeVisible(italicStyleButton);
    italicStyleButton.addListener(this);
    
    addAndMakeVisible(editTextButton);
    editTextButton.addListener(this);
    
    //same thing for slider, also make sure to resize(make sure not 0,0)
    addAndMakeVisible(transparencySlider);
    transparencySlider.addListener(this);
    
    addAndMakeVisible(fontSizeSlider);
    fontSizeSlider.addListener(this);
    
    //For combobox (TBD)
    addAndMakeVisible(colorMenu);
    colorMenu.addListener(this);
    
    //color menu
    colorMenu.setTextWhenNothingSelected("Colors");
    colorMenu.addItemList({"Navy", "Purple", "Brown", "Green", "Black"}, 1);

    
    
    //transparency slider
    transparencySlider.setRange(0.0, 1.0);
    
    //fontSizeSlider
    fontSizeSlider.setSliderStyle(Slider::IncDecButtons);
    fontSizeSlider.setTextBoxStyle(Slider::TextBoxLeft, false, 30, fontSizeSlider.getTextBoxHeight());
    fontSizeSlider.setRange(8.0, 32.0, 1.0);
    fontSizeSlider.setValue(textFont.getHeight(), NotificationType::dontSendNotification);
    //fontSizeSlider.setRange(0.0, 1.0);
    
    //textEditor
    textEditor.setFont(textFont);
    textEditor.setMultiLine(true);
    textEditor.setReturnKeyStartsNewLine(true);
    textEditor.setReadOnly(true);
    
    setVisible(true);
    
}

MainComponent::~MainComponent() {
}

//==============================================================================
// Component overrides

void MainComponent::paint (Graphics& g) {
    //wrong area?
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.fillCheckerBoard(textEditor.getBounds().toFloat(), (float)textEditor.getWidth()/9, (float)textEditor.getHeight()/9, juce::Colours::lightgrey, juce::Colours::white);
    
}

void MainComponent::resized() {
    auto bounds = getLocalBounds(); //the size of the window
    //从左上角移出一个高度为24 * 64的小方块（即那个button）
    auto p = 8;
    bounds.removeFromTop(p);
    bounds.removeFromBottom(p);
    bounds.removeFromLeft(p);
    bounds.removeFromRight(p);
    auto h = 24;
    
    auto line = bounds.removeFromTop(h); //width = 24
    clearButton.setBounds(line.removeFromLeft(120)); //length = 120
    
    line.removeFromLeft(p);
    colorMenu.setBounds(line.removeFromLeft(100));
    
    line.removeFromLeft(p);
    transparencyLabel.setBounds(line.removeFromLeft(100));
    transparencySlider.setBounds(line.removeFromLeft(getWidth() - 2*p - 100*2 - 120));
    
    bounds.removeFromTop(p);
    auto line2 = bounds.removeFromTop(h);
    fontSizeLabel.setBounds(line2.removeFromLeft(70));
    fontSizeSlider.setBounds(line2.removeFromLeft(80));
    line2.removeFromLeft(24);
    fontStyleLabel.setBounds(line2.removeFromLeft(80));
    boldStyleButton.setBounds(line2.removeFromLeft(60));
    italicStyleButton.setBounds(line2.removeFromLeft(60));
    editTextButton.setBounds(line2.removeFromRight(86));
    
    bounds.removeFromTop(p);
    auto line3 = bounds.removeFromTop(getHeight() - 2*h - 2*p);
    textEditor.setBounds(line3.removeFromLeft(getWidth()));
    
    
    
}

//==============================================================================
// Listener overrides

void MainComponent::buttonClicked(Button* button) {
    if(button == &clearButton){
        textEditor.clear();
    }
    
    if(button == &boldStyleButton){
        if(boldStyleButton.getToggleState()){
            if(!textFont.isItalic()){
                textFont.setStyleFlags(Font::bold);
                textEditor.applyFontToAllText(textFont);
            } else {
                textFont.setStyleFlags(Font::bold + Font::italic);
                textEditor.applyFontToAllText(textFont);
            }
        } else {
            if(!textFont.isItalic()){
                textFont.setStyleFlags(Font::plain);
                textEditor.applyFontToAllText(textFont);
            } else {
                textFont.setStyleFlags(Font::italic);
                textEditor.applyFontToAllText(textFont);
            }
        }
    }
    
    if(button == &italicStyleButton){
        if(italicStyleButton.getToggleState()){
            if(textFont.isBold()){
                textFont.setStyleFlags(Font::bold + Font::italic);
                textEditor.applyFontToAllText(textFont);
            } else {
                textFont.setStyleFlags(Font::italic);
                textEditor.applyFontToAllText(textFont);
            }
        } else {
            if(!textFont.isBold()){
                textFont.setStyleFlags(Font::plain);
                textEditor.applyFontToAllText(textFont);
            } else {
                textFont.setStyleFlags(Font::bold);
                textEditor.applyFontToAllText(textFont);
            }
        }
    }
    
    if(button == &editTextButton){
        if(editTextButton.getToggleState()){
            textEditor.setReadOnly(false);
        } else {
            textEditor.setReadOnly(true);
        }
    }
}

void MainComponent::sliderValueChanged(Slider* slider) {
    
    if(slider == &transparencySlider){
        auto opacity = 1 - transparencySlider.getValue();
        textEditor.setAlpha(opacity);
    }
    
    if(slider == &fontSizeSlider){
        textFont.setHeight(fontSizeSlider.getValue());
        textEditor.applyFontToAllText(textFont);
    }
}

void MainComponent::comboBoxChanged(ComboBox* menu) {
    if(menu == &colorMenu){
        int id = colorMenu.getSelectedItemIndex();
        textEditor.setColour(textEditor.backgroundColourId, colors[id]);
    }
    repaint();
}

//==============================================================================
// MainComponent methods

void MainComponent::setTextForId(const int id) {
    if(id == MainApplication::LATIN_TEXT){
        textEditor.setText(latinText);
    } else if (id == MainApplication::CYRILLIC_TEXT){
        textEditor.setText(cyrillicText);
    } else if (id == MainApplication::GREEK_TEXT){
        textEditor.setText(greekText);
    }
}
