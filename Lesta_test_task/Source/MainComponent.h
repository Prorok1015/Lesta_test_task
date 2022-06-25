#pragma once

#include <JuceHeader.h>
#include "ListBoxData.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void openImage(const juce::File& path, bool clearifnotexist = true);

private:
    //==============================================================================
    // Your private member variables go here...
    juce::ListBox ObjectsListBox;
    juce::ListBox AttributesListBox;
    ListBoxData Model;
    juce::TextEditor EditName;
    juce::TextEditor EditImgPath;
    juce::TextEditor EditAttrName;
    juce::TextEditor EditAttrValue;
    juce::TextButton AddObjBtn{ L"�������� ������" };
    juce::TextButton AddAttrBtn{ L"�������� �������" };
    juce::TextButton DeleteObjBtn{ L"������� ������" };
    juce::TextButton DeleteAttrBtn{ L"������� �������" };
    juce::TextButton OpenBtn{ L"������� ����" };
    juce::TextButton SaveBtn{ L"���������" };
    juce::ComboBox TypeBox;
    juce::ImageComponent imagePreview;

    std::unique_ptr<juce::FileChooser> Savedlg;
    std::unique_ptr<juce::FileChooser> Opendlg;
    Attribute curAttr;
    Object curObject;
    juce::File OpenFilePath;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
