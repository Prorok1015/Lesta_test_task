/*
  ==============================================================================

    ListBoxData.h
    Created: 24 Jun 2022 1:59:48pm
    Author:  Захар

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Controller.h"
class AttributeListBoxData : public juce::ListBoxModel
{
    std::vector<Attribute> myData;
    std::function<int()> selected_callback;
    int curRow = -1;
public:
    void setSelectedCallBack(std::function<int()> callback)
    {
        selected_callback = callback;
    }

    void selectedRowsChanged(int lastRowselected) override
    {
        curRow = selected_callback();
    }

    Attribute* getAttribute(int row)
    {
        if (row < 0 || row > myData.size()) return nullptr;
        return &(myData[row]);
    }

    Attribute* getCurAttr() { return getAttribute(curRow); }

    void curDelete(Object& obj)
    {
        obj.attrs_.erase(myData[curRow].name_);
        myData.erase(std::find(myData.begin(), myData.end(), myData[curRow].name_));
        curRow = -1;
    }

    void updateCurAttr(const Attribute& at)
    {
        myData[curRow] = at;
    }
    
    void addAttr(Attribute atr)
    {
        myData.push_back(atr);
    }

    void updateData(const std::unordered_map<std::wstring, Attribute>& attrs)
    {
        myData.clear();
        curRow = -1;
        for (const auto& [key, attr] : attrs)
        {
            myData.push_back(attr);
        }
    }

    int getNumRows() override
    {
        return myData.size();
    }

    void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override
    {
        if (rowIsSelected)
            g.fillAll(juce::Colours::lightblue);

        juce::String someData = myData.at(rowNumber).name_.c_str();
        g.setColour(juce::Colours::black);
        g.drawText(someData, 4, 0, width - 4, height, juce::Justification::centredLeft, true);

    }
};

class ListBoxData : public juce::ListBoxModel
{
    std::vector<std::wstring> myData;
    Controller control;
    AttributeListBoxData curAttrModel;
    std::function<int()> selected_callback;
    int curRow = -1;
public:

    void ReloadData()
    {
        myData.clear();
        curAttrModel.updateData({});
        for (const auto& [key, obj] : control.get_all()) {
            myData.push_back(obj.name_);
        }
    }

    void Save(juce::String path)
    {
        control.save(path.toWideCharPointer());
    }

    void Open(juce::String path)
    {
        control.open(path.toWideCharPointer());
    }

    Object* getObject(int row)
    {
        return control.find(myData[row]);
    }

    void Edit(Object& newObj)
    {
        control.edit(myData[curRow], newObj);
        myData[curRow] = newObj.name_;
    }

    void EditCurAttr(Attribute& newAttr)
    {
        auto oldAttr = curAttrModel.getCurAttr();
        if (!oldAttr) return;
        control.edit_attr(myData[curRow], oldAttr->name_, newAttr);
        curAttrModel.updateCurAttr(newAttr);
    }
    void CurDeleteObject()
    {
        auto obj = myData[curRow];
        control.pop(obj);
        myData.erase(std::find(myData.begin(),myData.end(),obj));
        curRow = -1;
        curAttrModel.updateData({});
    }

    void CurDeleteAttr()
    {
        auto obj = control.find(myData[curRow]);
        if (!obj) return;
        curAttrModel.curDelete(*obj);
    }

    void AddNewObject()
    {
        static size_t count = 0;
        Object obj;
        obj.name_ = L"DefaulName" + std::to_wstring(++count);
        obj.img_ = L"default.png";
        control.push(obj);
        myData.push_back(obj.name_);
    }

    void AddNewAttribute()
    {
        static size_t count = 0;
        Attribute atr;
        atr.name_ = L"default" + std::to_wstring(++count);
        atr.value_ = L"false";
        atr.type_ = Type::Bool;
        control.add_attr(myData[curRow], atr);
        curAttrModel.addAttr(atr);
    }

    void setSelectedCallBack(std::function<int()> callback)
    {
        selected_callback = callback;
    }

    void updateAttributeData(int row)
    {
        if (row < 0 || row > myData.size()) return;
        auto obj = control.find(myData[row]);
        if (!obj) return;
        curAttrModel.updateData(obj->attrs_);
    }

    AttributeListBoxData& getAttributeModel()
    {
        return curAttrModel;
    }

    int getNumRows() override
    {
        return myData.size();
    }

    void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override
    {
        juce::String someData = myData[rowNumber].c_str();

        if (rowIsSelected) {
            g.fillAll(juce::Colours::lightblue);
        }

        g.setColour(juce::Colours::black);
        g.drawText(someData, 4, 0, width - 4, height, juce::Justification::centredLeft, true);
    }

    void selectedRowsChanged(int lastRowselected) override
    {
        curRow = selected_callback();
    }
};