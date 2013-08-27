#include "BubbleCanvas.h"


WX_DEFINE_OBJARRAY(arrayOfParamInfo);


//##Esto tiene luego que ser parte de alguna interfaz común, para que la herede también BubbleBlock:
void BubbleBlockInfo::setName(const wxString& value)
{
    name = value;
}


const wxString &BubbleBlockInfo::getName() const
{
    return name;
}


void BubbleBlockInfo::setCode(const wxArrayString& value)
{
    code = value;
}


const wxArrayString &BubbleBlockInfo::getCode() const
{
    return code;
}

/*
void BubbleBlockInfo::setHasOperationButton(const bool value)
{
    hasOperationButton = value;
}
*/


/*
bool BubbleBlockInfo::getHasOperationButton() const
{
    return hasOperationButton;
}
*/


void BubbleBlockInfo::setHasAddParamsButton(const bool value)
{
    hasAddParamsButton = value;
}


bool BubbleBlockInfo::getHasAddParamsButton() const
{
    return hasAddParamsButton;
}


bool BubbleBlockInfo::hasBlockHoverImage() const
{
    return _hasBlockHoverImage;
}


bool BubbleBlockInfo::hasBlockPressedImage() const
{
    return _hasBlockPressedImage;
}


bool BubbleBlockInfo::hasMethodHoverImage() const
{
    return _hasMethodHoverImage;
}


bool BubbleBlockInfo::hasMethodPressedImage() const
{
    return _hasMethodPressedImage;
}


void BubbleBlockInfo::setBlockDefaultImage(const wxImage& image)
{
    blockDefaultImage = image;
}

////Picker images:
void BubbleBlockInfo::setPickerDefaultImage(const wxImage& image)
{
    pickerDefaultImage = image;
}

const wxImage &BubbleBlockInfo::getPickerDefaultImage() const
{
    return pickerDefaultImage;
}


void BubbleBlockInfo::setPickerHoverImage(const wxImage& image)
{
    pickerHoverImage = image;
}


const wxImage &BubbleBlockInfo::getPickerHoverImage() const
{
    return pickerHoverImage;
}


void BubbleBlockInfo::setPickerPressedImage(const wxImage& image)
{
    pickerPressedImage = image;
}


const wxImage &BubbleBlockInfo::getPickerPressedImage() const
{
    return pickerPressedImage;
}


void BubbleBlockInfo::setPickerDisabledImage(const wxImage& image)
{
    pickerDisabledImage = image;
}


const wxImage &BubbleBlockInfo::getPickerDisabledImage() const
{
    return pickerDisabledImage;
}
////


const wxImage &BubbleBlockInfo::getBlockDefaultImage() const
{
    return blockDefaultImage;
}


void BubbleBlockInfo::setBlockHoverImage(const wxImage& image)
{
    blockHoverImage = image;
    _hasBlockHoverImage = true;
}


const wxImage &BubbleBlockInfo::getBlockHoverImage() const
{
    return blockHoverImage;
}


void BubbleBlockInfo::setBlockPressedImage(const wxImage& image)
{
    blockPressedImage = image;
    _hasBlockPressedImage = true;
}


const wxImage &BubbleBlockInfo::getBlockPressedImage() const
{
    return blockPressedImage;
}


void BubbleBlockInfo::setBlockDisabledImage(const wxImage& image)
{
    blockDisabledImage = image;
}


const wxImage &BubbleBlockInfo::getBlockDisabledImage() const
{
    return blockDisabledImage;
}


void BubbleBlockInfo::setMethodDefaultImage(const wxImage& image)
{
    methodDefaultImage = image;
}


const wxImage &BubbleBlockInfo::getMethodDefaultImage() const
{
    return methodDefaultImage;
}


void BubbleBlockInfo::setMethodHoverImage(const wxImage& image)
{
    methodHoverImage = image;
    _hasMethodHoverImage = true;
}


const wxImage &BubbleBlockInfo::getMethodHoverImage() const
{
    return methodHoverImage;
}


void BubbleBlockInfo::setMethodPressedImage(const wxImage& image)
{
    methodPressedImage = image;
    _hasMethodPressedImage = true;
}


const wxImage &BubbleBlockInfo::getMethodPressedImage() const
{
    return methodPressedImage;
}


void BubbleBlockInfo::setMethodDisabledImage(const wxImage& image)
{
    methodDisabledImage = image;
}


const wxImage &BubbleBlockInfo::getMethodDisabledImage() const
{
    return methodDisabledImage;
}


/*
void BubbleBlockInfo::setOperationDefaultImage(const wxImage& image)
{
    operationDefaultImage = image;
}
*/


/*
const wxImage &BubbleBlockInfo::getOperationDefaultImage() const
{
    return operationDefaultImage;
}
*/


/*
void BubbleBlockInfo::setOperationHoverImage(const wxImage& image)
{
    operationHoverImage = image;
}
*/


/*
const wxImage &BubbleBlockInfo::getOperationHoverImage() const
{
    return operationHoverImage;
}
*/


/*
void BubbleBlockInfo::setOperationPressedImage(const wxImage& image)
{
    operationPressedImage = image;
}
*/


/*
const wxImage &BubbleBlockInfo::getOperationPressedImage() const
{
    return operationPressedImage;
}
*/


/*
void BubbleBlockInfo::setOperationDisabledImage(const wxImage& image)
{
    operationDisabledImage = image;
}
*/


/*
const wxImage &BubbleBlockInfo::getOperationDisabledImage() const
{
    return operationDisabledImage;
}
*/


//##The return button is not visible, is just to be copied in the left-block's param button:
void BubbleBlockInfo::setReturnDefaultImage(const wxImage& image)
{
    returnDefaultImage = image;
}


const wxImage &BubbleBlockInfo::getReturnDefaultImage() const
{
    return returnDefaultImage;
}


void BubbleBlockInfo::setReturnHoverImage(const wxImage& image)
{
    returnHoverImage = image;
}


const wxImage &BubbleBlockInfo::getReturnHoverImage() const
{
    return returnHoverImage;
}


void BubbleBlockInfo::setReturnPressedImage(const wxImage& image)
{
    returnPressedImage = image;
}


const wxImage &BubbleBlockInfo::getReturnPressedImage() const
{
    return returnPressedImage;
}


void BubbleBlockInfo::setReturnDisabledImage(const wxImage& image)
{
    returnDisabledImage = image;
}


const wxImage &BubbleBlockInfo::getReturnDisabledImage() const
{
    return returnDisabledImage;
}


void BubbleBlockInfo::setAddParamsDefaultImage(const wxImage& image)
{
    addParamsDefaultImage = image;
}


const wxImage &BubbleBlockInfo::getAddParamsDefaultImage() const
{
    return addParamsDefaultImage;
}


void BubbleBlockInfo::setAddParamsHoverImage(const wxImage& image)
{
    addParamsHoverImage = image;
}


const wxImage &BubbleBlockInfo::getAddParamsHoverImage() const
{
    return addParamsHoverImage;
}


void BubbleBlockInfo::setAddParamsPressedImage(const wxImage& image)
{
    addParamsPressedImage = image;
}


const wxImage &BubbleBlockInfo::getAddParamsPressedImage() const
{
    return addParamsPressedImage;
}


void BubbleBlockInfo::setAddParamsDisabledImage(const wxImage& image)
{
    addParamsDisabledImage = image;
}


const wxImage &BubbleBlockInfo::getAddParamsDisabledImage() const
{
    return addParamsDisabledImage;
}


void BubbleBlockInfo::setRemoveParamsDefaultImage(const wxImage& image)
{
    removeParamsDefaultImage = image;
}


const wxImage &BubbleBlockInfo::getRemoveParamsDefaultImage() const
{
    return removeParamsDefaultImage;
}


void BubbleBlockInfo::setRemoveParamsHoverImage(const wxImage& image)
{
    removeParamsHoverImage = image;
}


const wxImage &BubbleBlockInfo::getRemoveParamsHoverImage() const
{
    return removeParamsHoverImage;
}


void BubbleBlockInfo::setRemoveParamsPressedImage(const wxImage& image)
{
    removeParamsPressedImage = image;
}


const wxImage &BubbleBlockInfo::getRemoveParamsPressedImage() const
{
    return removeParamsPressedImage;
}


void BubbleBlockInfo::setRemoveParamsDisabledImage(const wxImage& image)
{
    removeParamsDisabledImage = image;
}


const wxImage &BubbleBlockInfo::getRemoveParamsDisabledImage() const
{
    return removeParamsDisabledImage;
}


//##No sé si esto quedará en el futuro: los nombres por defecto saldrán de un sistema más sofisticado donde
//se toma el tipo de dato y se mira cuál es el nombre por defecto disponible más "bajo" (ej: number1, number2,
//bool1, text1, etc....)
void BubbleBlockInfo::setInstanceNameFieldDefaultValue(const wxString& value)
{
    instanceNameFieldDefaultValue = value;
}


const wxString &BubbleBlockInfo::getInstanceNameFieldDefaultValue() const
{
    return instanceNameFieldDefaultValue;
}


void BubbleBlockInfo::addParam( const wxString& name,
                                const wxString& dataType,
                                const wxImage& notAssignedImage,
                                const wxImage& defaultImage,
                                const wxImage& hoverImage,
                                const wxImage& pressedImage,
                                const wxImage& disabledImage
                                )
{
    BubbleParamInfo *newParam = new BubbleParamInfo(name,
                                                    dataType,
                                                    notAssignedImage,
                                                    defaultImage,
                                                    hoverImage,
                                                    pressedImage,
                                                    disabledImage
                                                   );
    if (newParam)
    {
        params.Add(newParam);
    }
}


wxString BubbleBlockInfo::getParamName(const unsigned int index) const
{
    BubbleParamInfo *iteratorParamInfo = NULL;
    while (index < params.GetCount())
    {
        iteratorParamInfo = &(params.Item(index));
        if (iteratorParamInfo)
            return iteratorParamInfo->getName();
    }
    return wxString(""); //##: See how to improve this.
}

//##const wxString &?
wxString BubbleBlockInfo::getParamDataType(const unsigned int index) const
{
    BubbleParamInfo *iteratorParamInfo = NULL;
    while (index < params.GetCount())
    {
        iteratorParamInfo = &(params.Item(index));
        if (iteratorParamInfo)
            return iteratorParamInfo->getDataType();
    }
    return wxString(""); //##Error: See how to improve this.
}


//##Convertir estos 5 gettters en la misma función, con un callback o algo así:

//##Ver si está bien retornar esto así:
const wxImage &BubbleBlockInfo::getParamNotAssignedImage(const unsigned int index) const
{
    BubbleParamInfo *iteratorParamInfo = NULL;
    while (index < params.GetCount())
    {
        iteratorParamInfo = &(params.Item(index));
        if (iteratorParamInfo)
            return iteratorParamInfo->getNotAssignedImage();
    }
    //##Horrible, but works nice!
    return emptyDummyImage;
}


const wxImage &BubbleBlockInfo::getParamDefaultImage(const unsigned int index) const
{
    BubbleParamInfo *iteratorParamInfo = NULL;
    while (index < params.GetCount())
    {
        iteratorParamInfo = &(params.Item(index));
        if (iteratorParamInfo)
            return iteratorParamInfo->getDefaultImage();
    }
    //##Horrible, but works nice!
    return emptyDummyImage;
}


const wxImage &BubbleBlockInfo::getParamHoverImage(const unsigned int index) const
{
    BubbleParamInfo *iteratorParamInfo = NULL;
    while (index < params.GetCount())
    {
        iteratorParamInfo = &(params.Item(index));
        if (iteratorParamInfo)
            return iteratorParamInfo->getHoverImage();
    }
    //##Horrible, but works nice!
    return emptyDummyImage;
}


const wxImage &BubbleBlockInfo::getParamPressedImage(const unsigned int index) const
{
    BubbleParamInfo *iteratorParamInfo = NULL;
    while (index < params.GetCount())
    {
        iteratorParamInfo = &(params.Item(index));
        if (iteratorParamInfo)
            return iteratorParamInfo->getPressedImage();
    }
    //##Horrible, but works nice!
    return emptyDummyImage;
}


const wxImage &BubbleBlockInfo::getParamDisabledImage(const unsigned int index) const
{
    BubbleParamInfo *iteratorParamInfo = NULL;
    while (index < params.GetCount())
    {
        iteratorParamInfo = &(params.Item(index));
        if (iteratorParamInfo)
            return iteratorParamInfo->getDisabledImage();
    }
    //##Horrible, but works nice!
    return emptyDummyImage;
}


unsigned int BubbleBlockInfo::getParamsCount() const
{
    return params.GetCount();
}


void BubbleBlockInfo::setBrother(BubbleBlockInfo *const brother)
{
    brotherBlockInfo = brother;
}


BubbleBlockInfo *const BubbleBlockInfo::getBrother() const
{
    return brotherBlockInfo;
}


void BubbleBlockInfo::setNextBlockIndentationTabs(const unsigned int value)
{
    nextBlockIndentationTabs = value;
}


int BubbleBlockInfo::getNextBlockIndentationTabs() const
{
    return nextBlockIndentationTabs;
}
