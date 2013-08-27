#include "BubbleCanvas.h"


BubbleParamInfo::~BubbleParamInfo()
{
}


const wxString &BubbleParamInfo::getName() const
{
    return name;
}


const wxString &BubbleParamInfo::getDataType() const
{
    return dataType;
}


const wxImage &BubbleParamInfo::getNotAssignedImage() const
{
    return notAssignedImage;
}


const wxImage &BubbleParamInfo::getDefaultImage() const
{
    return defaultImage;
}


const wxImage &BubbleParamInfo::getHoverImage() const
{
    return hoverImage;
}


const wxImage &BubbleParamInfo::getPressedImage() const
{
    return pressedImage;
}


const wxImage &BubbleParamInfo::getDisabledImage() const
{
    return disabledImage;
}
