#include "MainComponent.h"
#include "ListBoxData.h"
using namespace juce;
//==============================================================================
MainComponent::MainComponent()
{
    
    Savedlg = std::make_unique<juce::FileChooser>("Save File",
        juce::File::getSpecialLocation(juce::File::userDesktopDirectory),
        "*.xml");
    Opendlg = std::make_unique<juce::FileChooser>(L"Open File",
        juce::File::getSpecialLocation(juce::File::userDesktopDirectory),
        "*.xml");

    setSize (1000, 800);

    addAndMakeVisible(imagePreview);
    addAndMakeVisible(ObjectsListBox);
    ObjectsListBox.setTitle("Objects");
    ObjectsListBox.setRowHeight(20);
    ObjectsListBox.setModel(&Model);
    ObjectsListBox.setColour(ListBox::textColourId, Colours::black);
    ObjectsListBox.setColour(ListBox::backgroundColourId, Colours::white);
    Model.setSelectedCallBack([this]() {
            int row = ObjectsListBox.getSelectedRow();
            Model.updateAttributeData(row);
            AttributesListBox.updateContent();
            AttributesListBox.repaint();
            auto* obj = Model.getObject(row);
            if (obj)
                curObject = *obj;
            EditName.setText(curObject.name_.c_str(), false);
            EditImgPath.setText(curObject.img_.c_str(), false);
            EditAttrName.setText(L"", false);
            EditAttrValue.setText(L"", false);
            TypeBox.setSelectedId(0);
            openImage(OpenFilePath.getParentDirectory().getChildFile(juce::String(curObject.img_.c_str())));
            AttributesListBox.deselectAllRows();
            return row;
        });

    addAndMakeVisible(AttributesListBox);
    AttributesListBox.setTitle("Attributes");
    AttributesListBox.setRowHeight(20);
    AttributesListBox.setModel(&Model.getAttributeModel());
    AttributesListBox.setColour(ListBox::textColourId, Colours::black);
    AttributesListBox.setColour(ListBox::backgroundColourId, Colours::white);
    Model.getAttributeModel().setSelectedCallBack([this]() {
        int row = AttributesListBox.getSelectedRow();
        auto atr = Model.getAttributeModel().getAttribute(row);
        if (atr)
            curAttr = *atr;
        EditAttrName.setText(curAttr.name_.c_str(), false);
        EditAttrValue.setText(curAttr.value_.c_str(), false);
        TypeBox.setSelectedId((int)curAttr.type_, false);
        return row;
        });
    addAndMakeVisible(EditName);
    addAndMakeVisible(EditImgPath);
    addAndMakeVisible(EditAttrName);
    addAndMakeVisible(EditAttrValue);
    auto font = EditName.getFont();
    font.setHeight(36.f);
    EditName.setFont(font);
    EditName.onTextChange = [this] {
        curObject.name_ = EditName.getText().toWideCharPointer();
        Model.Edit(curObject);
        EditName.setText(curObject.name_.c_str(), false);
        ObjectsListBox.repaint();
    };
    EditImgPath.setFont(font);
    EditImgPath.onTextChange = [this] {
        curObject.img_ = EditImgPath.getText().toWideCharPointer();
        Model.Edit(curObject);
        openImage(OpenFilePath.getParentDirectory().getChildFile(juce::String(curObject.img_.c_str())), false);
    };

    EditAttrName.setFont(font);
    EditAttrName.onTextChange = [this] {
        curAttr.name_ = EditAttrName.getText().toWideCharPointer();
        Model.EditCurAttr(curAttr);
        EditAttrName.setText(curAttr.name_.c_str(), false);
        AttributesListBox.repaint();
    };
    EditAttrValue.setFont(font);
    EditAttrValue.onTextChange = [this] {
        curAttr.value_ = EditAttrValue.getText().toWideCharPointer();
        Model.EditCurAttr(curAttr);
    };

    addAndMakeVisible(TypeBox);
    TypeBox.addItem(from_type(Type::Bool).c_str(), (int)Type::Bool);
    TypeBox.addItem(from_type(Type::Int).c_str(), (int)Type::Int);
    TypeBox.addItem(from_type(Type::Float).c_str(), (int)Type::Float);
    TypeBox.onChange = [this] { curAttr.type_ = (Type)TypeBox.getSelectedId();
                                Model.EditCurAttr(curAttr); };

    addAndMakeVisible(AddObjBtn);
    addAndMakeVisible(AddAttrBtn);
    AddAttrBtn.onClick = [this] {
        Model.AddNewAttribute();
        AttributesListBox.updateContent();
    };
    AddObjBtn.onClick = [this] {
        Model.AddNewObject();
        ObjectsListBox.updateContent();
    };

    addAndMakeVisible(DeleteObjBtn);
    addAndMakeVisible(DeleteAttrBtn);
    DeleteObjBtn.onClick = [this] {
        Model.CurDeleteObject();
        ObjectsListBox.updateContent();
        AttributesListBox.updateContent();
        ObjectsListBox.repaint();
        AttributesListBox.repaint();
    };
    DeleteAttrBtn.onClick = [this] {
        Model.CurDeleteAttr();
        AttributesListBox.updateContent();
        ObjectsListBox.repaint();
        AttributesListBox.repaint();
    };
    addAndMakeVisible(OpenBtn);
    OpenBtn.onClick = [this] {
        auto folderChooserFlags = FileBrowserComponent::openMode |
            FileBrowserComponent::canSelectFiles;
        Opendlg->launchAsync(folderChooserFlags, [this](const juce::FileChooser& chooser)
            {
                File xmlFile(chooser.getResult());
                OpenFilePath = xmlFile;
                Model.Open(xmlFile.getFullPathName());
                Model.ReloadData();
                ObjectsListBox.updateContent();
                ObjectsListBox.deselectAllRows();
                AttributesListBox.updateContent();
                ObjectsListBox.repaint();
                AttributesListBox.repaint();
            });
    };
    addAndMakeVisible(SaveBtn);
    SaveBtn.onClick = [this] {
        auto folderChooserFlags = FileBrowserComponent::saveMode |
            FileBrowserComponent::canSelectFiles;
        Savedlg->launchAsync(folderChooserFlags, [this](const juce::FileChooser& chooser)
            {
                File xmlFile(chooser.getResult());
                OpenFilePath = xmlFile;
                Model.Save(xmlFile.getFullPathName());
            });
    };
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{

}

void MainComponent::resized()
{
    int oneThirdWidth = getWidth() - (getWidth() / 3)*2;
    int centerHeight = getHeight() - getHeight() / 2;
    ObjectsListBox.setBounds(10, 40, oneThirdWidth - 20, getHeight() - 60);
    AttributesListBox.setBounds(oneThirdWidth + 10, 40, oneThirdWidth - 20, centerHeight);
    EditName.setBounds(oneThirdWidth*2 + 10, 40, oneThirdWidth - 20, 40);
    EditImgPath.setBounds(oneThirdWidth*2 + 10, 90, oneThirdWidth - 20, 40);
    EditAttrName.setBounds(oneThirdWidth * 2 + 10, 160, oneThirdWidth - 20, 40);
    EditAttrValue.setBounds(oneThirdWidth * 2 + 10, 210, oneThirdWidth - 20, 40);
    TypeBox.setBounds(oneThirdWidth * 2 + 10, 260, oneThirdWidth - 20, 40);

    AddAttrBtn.setBounds(oneThirdWidth * 2 + 10, centerHeight - 50, oneThirdWidth - 20, 40);
    DeleteAttrBtn.setBounds(oneThirdWidth * 2 + 10, centerHeight, oneThirdWidth - 20, 40);

    AddObjBtn.setBounds(oneThirdWidth + 10, centerHeight + 50, oneThirdWidth - 20, 40);
    DeleteObjBtn.setBounds(oneThirdWidth + 10, centerHeight + 100, oneThirdWidth - 20, 40);

    OpenBtn.setBounds(oneThirdWidth + 10, getHeight() - 110, oneThirdWidth - 20, 40);
    SaveBtn.setBounds(oneThirdWidth + 10, getHeight() - 60, oneThirdWidth - 20, 40);

    imagePreview.setBounds(oneThirdWidth * 2 + 10, centerHeight + 10, oneThirdWidth - 20, centerHeight -30);
}

void MainComponent::openImage(const juce::File& path, bool clearifnotexist)
{
    if (!path.existsAsFile()) {
        if(clearifnotexist)
            imagePreview.setImage({});
        return;
    }
    imagePreview.setImage(juce::ImageCache::getFromFile(path));

}