//--------------------------------------------------------------------------------------------------
//
//  File:       m+mConfigurationWindow.cpp
//
//  Project:    m+m
//
//  Contains:   The class definition for the configuration window of the m+m manager application.
//
//  Written by: Norman Jaffe
//
//  Copyright:  (c) 2015 by H Plus Technologies Ltd. and Simon Fraser University.
//
//              All rights reserved. Redistribution and use in source and binary forms, with or
//              without modification, are permitted provided that the following conditions are met:
//                * Redistributions of source code must retain the above copyright notice, this list
//                  of conditions and the following disclaimer.
//                * Redistributions in binary form must reproduce the above copyright notice, this
//                  list of conditions and the following disclaimer in the documentation and/or
//                  other materials provided with the distribution.
//                * Neither the name of the copyright holders nor the names of its contributors may
//                  be used to endorse or promote products derived from this software without
//                  specific prior written permission.
//
//              THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
//              EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//              OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
//              SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//              INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
//              TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//              BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//              CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//              ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//              DAMAGE.
//
//  Created:    2015-07-20
//
//--------------------------------------------------------------------------------------------------

#include "m+mConfigurationWindow.h"

#include "m+mManagerApplication.h"
#include "m+mTextEditorWithCaption.h"
#include "m+mTextValidator.h"

#include <m+m/m+mChannelArgumentDescriptor.h>
#include <m+m/m+mEndpoint.h>
#include <m+m/m+mPortArgumentDescriptor.h>

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The class definition for the configuration window of the m+m manager application. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Namespace references
#endif // defined(__APPLE__)

using namespace MplusM;
using namespace MPlusM_Manager;
using namespace std;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

/*! @brief The colour to be used for the window background. */
static const Colour & kWindowBackgroundColour(Colours::whitesmoke);

/*! @brief The font size for text. */
static const float kFontSize = 16;

/*! @brief The horizontal gap between buttons. */
static const int kButtonGap = 10;

/*! @brief The amount to add to the height of text editor fields. */
static const int kEditorHeightAdjustment = 4;

/*! @brief The extra space around the content in the window. */
static const int kExtraSpaceInWindow = 20;

/*! @brief The amount to inset text entry fields. */
static const int kFieldInset = (2 * kButtonGap);

/*! @brief The amount to inset labels. */
static const int kLabelInset = (3 * kButtonGap);

/*! @brief The amount of space between a field and its label. */
static const int kLabelToFieldGap = 2;

/*! @brief The overhead for the buttons on the title bar. */
static const int kTitleBarMinWidth = 80;

/*! @brief The internal name for the endpoint text entry field. */
static const String kEndpointFieldName("$$$endpoint$$$");

/*! @brief The text of the file popup invocation button. */
static const String kFileButtonText("...");

/*! @brief The internal name for the port text entry field. */
static const String kPortFieldName("$$$port$$$");

/*! @brief The internal name for the tag text entry field. */
static const String kTagFieldName("$$$tag$$$");

#if defined(__APPLE__)
# pragma mark Global constants and variables
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and Destructors
#endif // defined(__APPLE__)

ConfigurationWindow::ConfigurationWindow(const String &                              title,
                                         const String &                              execType,
                                         const YarpStringVector &                    currentValues,
                                         const MplusM::Utilities::DescriptorVector & descriptors,
                                         yarp::os::Bottle &                          valuesToUse) :
    inherited1(), inherited2(), inherited3(title, kWindowBackgroundColour, 0), inherited4(),
    _topText("topText"), _cancelButton("Cancel"), _okButton("OK"),
    _errorFont(Font::getDefaultMonospacedFontName(), kFontSize, Font::italic + Font::bold),
    _regularFont(Font::getDefaultMonospacedFontName(), kFontSize, Font::plain), _execType(execType),
    _descriptors(descriptors), _extraArgumentsGroup(NULL), _addArgumentsButton(NULL),
    _removeArgumentsButton(NULL), _valuesToUse(valuesToUse), _hasExtraArguments(false),
    _hasFileField(false)
{
    OD_LOG_ENTER(); //####
    OD_LOG_S2s("title = ", title, "execType = ", execType); //####
    OD_LOG_P2("descriptors = ", &descriptors, "argsToUse = ", &argsToUse); //####
    _contentArea.setSize(100, 100);
    setContentNonOwned(&_contentArea, true);
    BorderSize<int> bt = getBorderThickness();
    BorderSize<int> cb = getContentComponentBorder();
    int             heightSoFar = 0;
    int             widthSoFar = 0;

    _adjustedEditorHeight = _regularFont.getHeight() + kEditorHeightAdjustment;
    _valuesToUse.clear();
    setUpStandardFields(widthSoFar, heightSoFar, currentValues);
	int minW = jmax(widthSoFar, _cancelButton.getWidth() + _okButton.getWidth() + (3 * kButtonGap));
    int calcW = minW + bt.getLeftAndRight() + cb.getLeftAndRight();
    int calcH = heightSoFar + bt.getTopAndBottom() + cb.getTopAndBottom();
    
	centreWithSize(calcW + kExtraSpaceInWindow, calcH + kExtraSpaceInWindow);
    adjustFields();
    setOpaque(true);
    setResizable(false, false);
    setVisible(true);
    addKeyListener(ManagerWindow::getApplicationCommandManager().getKeyMappings());
    triggerAsyncUpdate();
    OD_LOG_EXIT_P(this); //####
} // ConfigurationWindow::ConfigurationWindow

ConfigurationWindow::~ConfigurationWindow(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // ConfigurationWindow::~ConfigurationWindow

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

void ConfigurationWindow::addAnExtraField(void)
{
    OD_LOG_ENTER(); //####
    Component *             content = _extraArgumentsGroup;
    String                  compCountAsString(static_cast<int>(_extraFieldEditors.size() + 1));
    Point<int>              dimensions;
    TextEditorWithCaption * newEditor = new TextEditorWithCaption(*this, _regularFont, _errorFont,
                                                                  _extraFieldEditors.size(), NULL,
                                                                  _extraArgRootName + "_" +
                                                                  compCountAsString);
    Label *                 newLabel = newEditor->getCaption();
    
    newLabel->setText(_extraArgRootName + " " + compCountAsString, dontSendNotification);
    _extraFieldEditors.add(newEditor);
    MPlusM_Manager::CalculateTextArea(dimensions, _regularFont, newLabel->getText());
    newLabel->setBounds(kLabelInset, 0, dimensions.getX() + kLabelInset, dimensions.getY());
    content->addAndMakeVisible(newLabel);
    newEditor->setBounds(kFieldInset, 0, 0, static_cast<int>(_adjustedEditorHeight));
    newEditor->setSelectAllWhenFocused(true);
    content->addAndMakeVisible(newEditor);
    recalculateArea();
    adjustFields();
    if (_removeArgumentsButton)
    {
        _removeArgumentsButton->setVisible(true);
    }
    OD_LOG_EXIT(); //####
} // ConfigurationWindow::addAnExtraField

void ConfigurationWindow::adjustFields(void)
{
    OD_LOG_OBJENTER(); //####
    Component * content = getContentComponent();
    int         newButtonTop = content->getHeight() - (_cancelButton.getHeight() + kButtonGap);
    int         newFieldWidth = content->getWidth() - (2 * kFieldInset);
    
    if (_hasFileField)
    {
        newFieldWidth -= (kButtonGap + _fileButtonWidth);
    }
    _cancelButton.setTopLeftPosition(getWidth() - (_cancelButton.getWidth() + kButtonGap),
                                     newButtonTop);
    _okButton.setTopLeftPosition(_cancelButton.getX() - (_okButton.getWidth() + kButtonGap),
                                 newButtonTop);
    if (_addArgumentsButton)
    {
        _addArgumentsButton->setTopLeftPosition(_okButton.getX() -
                                                (_addArgumentsButton->getWidth() + kButtonGap),
                                                newButtonTop);
    }
    if (_removeArgumentsButton)
    {
        _removeArgumentsButton->setTopLeftPosition(_addArgumentsButton->getX() -
                                               (_removeArgumentsButton->getWidth() + kButtonGap),
                                                   newButtonTop);
    }
    for (size_t ii = 0, maxf = _standardFieldEditors.size(); maxf > ii; ++ii)
    {
        TextEditorWithCaption * anEditor = _standardFieldEditors[static_cast<int>(ii)];
        
        if (anEditor)
        {
            TextButton * aButton = anEditor->getButton();
            
            anEditor->setSize(newFieldWidth, anEditor->getHeight());
            if (aButton)
            {
                Label * aLabel = anEditor->getCaption();
                int     span = anEditor->getY() + anEditor->getHeight() - aLabel->getY();
                int     offset = span - aButton->getHeight();
                
                aButton->setTopLeftPosition(anEditor->getX() + newFieldWidth + kButtonGap,
                                            aLabel->getY() + (offset / 2));
            }
        }
    }
    if (_extraArgumentsGroup)
    {
        int groupWidth = _cancelButton.getX() + _cancelButton.getWidth() -
                            (_extraArgumentsGroup->getX() + kButtonGap);
        int innerWidth = groupWidth - (kFieldInset + (2 * kButtonGap));
        
        for (size_t ii = 0, maxf = _extraFieldEditors.size(); maxf > ii; ++ii)
        {
            TextEditorWithCaption * anEditor = _extraFieldEditors[static_cast<int>(ii)];
            
            anEditor->setSize(innerWidth, anEditor->getHeight());
        }
        _extraArgumentsGroup->setSize(groupWidth, _extraArgumentsGroup->getHeight());
    }
    OD_LOG_OBJEXIT(); //####
} // ConfigurationWindow::adjustFields

void ConfigurationWindow::buttonClicked(Button * aButton)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("aButton = ", aButton); //####
    int commandId = aButton->getCommandID();
    
    tellAllFieldsToIgnoreNextFocusLoss();
    switch (commandId)
    {
        case kConfigurationAddField :
            addAnExtraField();
            break;
            
        case kConfigurationRemoveField :
            removeMostRecentlyAddedExtraField();
            break;
            
        case kConfigurationOK :
            if (fieldsAreValid())
            {
                exitModalState(commandId);
            }
            break;
            
        case kConfigurationFileRequest :
            for (size_t ii = 0, maxf = _standardFieldEditors.size(); maxf > ii; ++ii)
            {
                TextEditorWithCaption * anEditor = _standardFieldEditors[static_cast<int>(ii)];
                
                if (anEditor && (anEditor->getButton() == aButton))
                {
                    anEditor->makeFileSelection();
                    break;
                }
                
            }
            break;
            
        default :
            exitModalState(commandId);
            break;
            
    }
    OD_LOG_OBJEXIT(); //####
} // ConfigurationWindow::buttonClicked

bool ConfigurationWindow::fieldsAreValid(void)
{
    OD_LOG_ENTER(); //####
    int    badCount = 0;
    String badArgs;
    
    for (size_t ii = 0, maxf = _standardFieldEditors.size(); maxf > ii; ++ii)
    {
        TextEditorWithCaption * anEditor = _standardFieldEditors[static_cast<int>(ii)];
        
        if (anEditor)
        {
            if (anEditor->validateField())
            {
                size_t jj = anEditor->getIndex();
                
                if (_descriptors.size() > jj)
                {
                    Utilities::BaseArgumentDescriptor * aDescriptor = _descriptors[jj];
                    
                    if (aDescriptor)
                    {
                        aDescriptor->addValueToBottle(_valuesToUse);
                    }
                }
            }
            else
            {
                if (0 < badArgs.length())
                {
                    badArgs += "\n";
                }
                badArgs += anEditor->getName();
                ++badCount;
            }
        }
    }
    if (0 == badCount)
    {
        // Add the extra arguments here.
        for (size_t ii = 0, maxf = _extraFieldEditors.size(); maxf > ii; ++ii)
        {
            TextEditorWithCaption * anEditor = _extraFieldEditors[static_cast<int>(ii)];
            
            _valuesToUse.addString(anEditor->getText().toStdString());
        }
    }
    if (0 < badCount)
    {
        String message1((1 < badCount) ? "arguments are" : "argument is");
        String message2((1 < badCount) ? "arguments" : "argument");
        
        AlertWindow::showMessageBox(AlertWindow::WarningIcon, getName(),
                                    String("The following ") + message1 + " invalid:\n" + badArgs +
                                    "\n" + String("Please correct the ") + message2 + " to the " +
                                    _execType + " and try again.", String::empty, this);
    }
    OD_LOG_EXIT_B(0 == badCount); //####
    return (0 == badCount);
} // ConfigurationWindow::fieldsAreValid

#if (! MAC_OR_LINUX_)
# pragma warning(push)
# pragma warning(disable: 4100)
#endif // ! MAC_OR_LINUX_
void ConfigurationWindow::focusGained(FocusChangeType cause)
{
#if MAC_OR_LINUX_
# pragma unused(cause)
#endif // MAC_OR_LINUX_
	OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // ConfigurationWindow::focusGained
#if (! MAC_OR_LINUX_)
# pragma warning(pop)
#endif // ! MAC_OR_LINUX_

#if (! MAC_OR_LINUX_)
# pragma warning(push)
# pragma warning(disable: 4100)
#endif // ! MAC_OR_LINUX_
void ConfigurationWindow::focusLost(FocusChangeType cause)
{
#if MAC_OR_LINUX_
# pragma unused(cause)
#endif // MAC_OR_LINUX_
	OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // ConfigurationWindow::focusLost
#if (! MAC_OR_LINUX_)
# pragma warning(pop)
#endif // ! MAC_OR_LINUX_

void ConfigurationWindow::handleAsyncUpdate(void)
{
    OD_LOG_OBJENTER(); //####
    ApplicationCommandManager & commandManager = ManagerWindow::getApplicationCommandManager();

    commandManager.registerAllCommandsForTarget(JUCEApplication::getInstance());
    OD_LOG_OBJEXIT(); //####
} // ConfigurationWindow::handleAsyncUpdate

bool ConfigurationWindow::keyPressed(const KeyPress & key)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("key = ", &key); //####
    bool result;
    
    if (key == KeyPress::escapeKey)
    {
        tellAllFieldsToIgnoreNextFocusLoss();
        exitModalState(kConfigurationCancel);
        result = true;
    }
    else if (key == KeyPress::returnKey)
    {
        tellAllFieldsToIgnoreNextFocusLoss();
        if (fieldsAreValid())
        {
            exitModalState(kConfigurationOK);
        }
        result = true;
    }
    else
    {
        result = inherited3::keyPressed(key);
    }
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // ConfigurationWindow::keyPressed

void ConfigurationWindow::recalculateArea(void)
{
    OD_LOG_ENTER(); //####
    int    heightSoFar = 0;
    int    widthSoFar = 0;
    size_t numExtra = _extraFieldEditors.size();
    
    heightSoFar = _topText.getY() + _topText.getHeight() + kButtonGap;
    widthSoFar = jmax(widthSoFar, _topText.getX() + _topText.getWidth());
    for (size_t ii = 0, numDescriptors = _descriptors.size(), jj = 0; numDescriptors > ii; ++ii)
    {
        Utilities::BaseArgumentDescriptor * aDescriptor = _descriptors[ii];
        
        if (aDescriptor)
        {
            String argName(aDescriptor->argumentName().c_str());
            String argDescription(aDescriptor->argumentDescription().c_str());
            
            if (! aDescriptor->isExtra())
            {
                TextEditorWithCaption * anEditor = _standardFieldEditors[static_cast<int>(jj)];
                
                if (anEditor)
                {
                    Label * aLabel = anEditor->getCaption();
                    
                    if (aLabel)
                    {
                        widthSoFar = jmax(widthSoFar, aLabel->getX() + aLabel->getWidth());
                        heightSoFar = anEditor->getY() + anEditor->getHeight() + (kButtonGap / 2);
                        ++jj;
                    }
                }
            }
        }
    }
    if (_extraArgumentsGroup)
    {
        int innerHeight = static_cast<int>(_regularFont.getHeight()) + (kButtonGap / 2);
        int innerWidth = (2 * jmax(kFieldInset, kLabelInset));

        _extraArgumentsGroup->setTopLeftPosition(kFieldInset, heightSoFar);
        for (size_t ii = 0; numExtra > ii; ++ii)
        {
            TextEditorWithCaption * anEditor = _extraFieldEditors[static_cast<int>(ii)];
            Label *                 aLabel = anEditor->getCaption();
            
            aLabel->setTopLeftPosition(kLabelInset, innerHeight);
            innerHeight = aLabel->getY() + aLabel->getHeight() + kLabelToFieldGap;
            innerWidth = jmax(innerWidth, aLabel->getX() + aLabel->getWidth());
            anEditor->setTopLeftPosition(kFieldInset, innerHeight);
            innerHeight = anEditor->getY() + anEditor->getHeight() + (kButtonGap / 2);
        }
        if (0 < numExtra)
        {
            innerHeight += (3 * kButtonGap / 4);
        }
        else
        {
            innerHeight += (kButtonGap / 2);
        }
        _extraArgumentsGroup->setSize(innerWidth, innerHeight);
        heightSoFar = _extraArgumentsGroup->getY() + _extraArgumentsGroup->getHeight();
        widthSoFar = jmax(widthSoFar, _extraArgumentsGroup->getX() +
                          _extraArgumentsGroup->getWidth());
    }
    BorderSize<int> cb = getContentComponentBorder();
    int             minW = jmax(widthSoFar, _cancelButton.getWidth() + _okButton.getWidth() +
                                (3 * kButtonGap));
    
    setContentComponentSize(minW + kExtraSpaceInWindow + cb.getLeftAndRight(),
                            heightSoFar + kExtraSpaceInWindow + cb.getTopAndBottom());
    OD_LOG_EXIT(); //####
} // ConfigurationWindow::recalculateArea

void ConfigurationWindow::removeMostRecentlyAddedExtraField(void)
{
    OD_LOG_ENTER(); //####
    Component *             content = _extraArgumentsGroup;
    TextEditorWithCaption * lastEditor = _extraFieldEditors.getLast();
    Label *                 lastLabel = lastEditor->getCaption();
    
    _extraFieldEditors.removeLast();
    content->removeChildComponent(lastEditor);
    content->removeChildComponent(lastLabel);
    recalculateArea();
    adjustFields();
    if (_removeArgumentsButton)
    {
        _removeArgumentsButton->setVisible(0 < _extraFieldEditors.size());
    }
    OD_LOG_EXIT(); //####
} // ConfigurationWindow::removeMostRecentlyAddedExtraField

void ConfigurationWindow::reportErrorInField(TextEditorWithCaption & fieldOfInterest)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("fieldOfInterest = ", &fieldOfInterest); //####
    AlertWindow::showMessageBox(AlertWindow::WarningIcon, getName(), String("The ") +
                                fieldOfInterest.getName() + " argument is invalid.\n"
                                "Please correct the argument and try again.", String::empty,
                                this);
    OD_LOG_OBJEXIT(); //####
} // ConfigurationWindow::reportErrorInField

void ConfigurationWindow::resized(void)
{
    OD_LOG_OBJENTER(); //####
    Button * close = getCloseButton();
    
    inherited3::resized();
    if (close)
    {
        const KeyPress esc(KeyPress::escapeKey, 0, 0);
        
        if (! close->isRegisteredForShortcut(esc))
        {
            close->addShortcut(esc);
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ConfigurationWindow::resized

void ConfigurationWindow::setUpStandardFields(int &                    widthSoFar,
                                              int &                    heightSoFar,
                                              const YarpStringVector & currentValues)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P3("widthSoFar = ", &widthSoFar, "heightSoFar = ", &heightSoFar, //####
              "currentValues = ", &currentValues); //####
    Component *               content = getContentComponent();
    int                       buttonHeight = getLookAndFeel().getAlertWindowButtonHeight();
    Point<int>                dimensions;
    size_t                    numDescriptors = _descriptors.size();
    ScopedPointer<TextButton> fileButton(new TextButton(kFileButtonText));
    
    fileButton->changeWidthToFitText(buttonHeight);
    _fileButtonWidth = fileButton->getWidth();
    widthSoFar = heightSoFar = 0;
    _topText.setFont(_regularFont);
    if (0 < numDescriptors)
    {
        _topText.setText(String("The ") + _execType +
						 " has one or more values that can be configured.", dontSendNotification);
    }
    else
    {
        _topText.setText(String("The ") + _execType + " has no configurable values.",
						 dontSendNotification);
    }
    MPlusM_Manager::CalculateTextArea(dimensions, _regularFont, _topText.getText());
    _topText.setBounds(kButtonGap, kButtonGap + getTitleBarHeight(), dimensions.getX() + kButtonGap,
                       dimensions.getY());
    content->addAndMakeVisible(&_topText, 0);
    heightSoFar = _topText.getY() + _topText.getHeight() + kButtonGap;
    widthSoFar = jmax(widthSoFar, _topText.getX() + _topText.getWidth());
    // Check for one or more file descriptors
    for (size_t ii = 0; numDescriptors > ii; ++ii)
    {
        bool                                forOutput;
        Utilities::BaseArgumentDescriptor * aDescriptor = _descriptors[ii];
        
        if (aDescriptor && aDescriptor->isForFiles(forOutput))
        {
            _hasFileField = true;
            break;
        }

    }
    for (size_t ii = 0; numDescriptors > ii; ++ii)
    {
        Utilities::BaseArgumentDescriptor * aDescriptor = _descriptors[ii];
        
        if (aDescriptor)
        {
            String argName(aDescriptor->argumentName().c_str());
            String argDescription(aDescriptor->argumentDescription().c_str());
            
            if (aDescriptor->isExtra())
            {
                if (! _hasExtraArguments)
                {
                    _hasExtraArguments = true;
                    _extraArgRootName = argName;
                    _extraArgumentsGroup = new GroupComponent("", argDescription);
                    _extraArgumentsGroup->setBounds(kFieldInset, heightSoFar,
                                                    widthSoFar - (kButtonGap + kFieldInset),
                                                    static_cast<int>(_regularFont.getHeight()) +
                                                    kButtonGap);
                    content->addAndMakeVisible(_extraArgumentsGroup);
                    heightSoFar = _extraArgumentsGroup->getY() + _extraArgumentsGroup->getHeight() +
                                    (kButtonGap / 2);
                    widthSoFar = jmax(widthSoFar, _extraArgumentsGroup->getX() +
                                      _extraArgumentsGroup->getWidth());
                    _addArgumentsButton = new TextButton(String("+ ") + argName);
                    _addArgumentsButton->setWantsKeyboardFocus(true);
                    _addArgumentsButton->setMouseClickGrabsKeyboardFocus(false);
                    _addArgumentsButton->setCommandToTrigger(NULL, kConfigurationAddField, false);
                    _addArgumentsButton->addListener(this);
                    _addArgumentsButton->changeWidthToFitText(buttonHeight);
                    content->addAndMakeVisible(_addArgumentsButton, 0);
                    _removeArgumentsButton = new TextButton(String("- ") + argName);
                    _removeArgumentsButton->setWantsKeyboardFocus(true);
                    _removeArgumentsButton->setMouseClickGrabsKeyboardFocus(false);
                    _removeArgumentsButton->setCommandToTrigger(NULL, kConfigurationRemoveField,
                                                                false);
                    _removeArgumentsButton->addListener(this);
                    _removeArgumentsButton->changeWidthToFitText(buttonHeight);
                    content->addChildComponent(_removeArgumentsButton);
                }
            }
            else if (aDescriptor->isModifiable())
            {
                bool                    forOutput;
                juce_wchar              fillChar = (aDescriptor->isPassword() ?
                                                    CHAR_TO_USE_FOR_PASSWORD_ : 0);
                String                  descriptionPrefix;
                String                  valueToUse;
                TextValidator *         newValidator = new TextValidator(*aDescriptor);
                TextEditorWithCaption * newEditor = new TextEditorWithCaption(*this, _regularFont,
                                                                              _errorFont, ii,
                                                                              newValidator,
                                                                              argName, fillChar);
                Label *                 newLabel = newEditor->getCaption();
                
                if (aDescriptor->isOptional())
                {
                    descriptionPrefix = "(Optional) ";
                }
                newLabel->setText(descriptionPrefix + argDescription, dontSendNotification);
                MPlusM_Manager::CalculateTextArea(dimensions, _regularFont, newLabel->getText());
				newLabel->setBounds(kLabelInset, heightSoFar, dimensions.getX() + kLabelInset,
									dimensions.getY());
                content->addAndMakeVisible(newLabel);
				heightSoFar = newLabel->getY() + newLabel->getHeight() + kLabelToFieldGap;
                widthSoFar = jmax(widthSoFar, newLabel->getX() + newLabel->getWidth());
                _standardFieldEditors.add(newEditor);
                newEditor->setBounds(kFieldInset, heightSoFar, widthSoFar - kFieldInset,
                                     static_cast<int>(_adjustedEditorHeight));
                if (ii < currentValues.size())
                {
                    valueToUse = currentValues[ii].c_str();
                }
                else
                {
                    valueToUse = aDescriptor->getDefaultValue().c_str();
                }
                newEditor->setText(valueToUse, false);
                newEditor->setSelectAllWhenFocused(true);
                content->addAndMakeVisible(newEditor);
                if (aDescriptor->isForFiles(forOutput))
                {
                    TextButton * newButton = new TextButton(kFileButtonText);

                    newButton->setWantsKeyboardFocus(true);
                    newButton->setMouseClickGrabsKeyboardFocus(false);
                    newButton->setCommandToTrigger(NULL, kConfigurationFileRequest, false);
                    newButton->addListener(this);
                    newButton->changeWidthToFitText(buttonHeight);
                    newEditor->setButton(newButton);
                    content->addAndMakeVisible(newEditor->getButton());
                }
                heightSoFar = newEditor->getY() + newEditor->getHeight() + (kButtonGap / 2);
            }
        }
    }
    _cancelButton.setWantsKeyboardFocus(true);
    _cancelButton.setMouseClickGrabsKeyboardFocus(false);
    _cancelButton.setCommandToTrigger(NULL, kConfigurationCancel, false);
    _cancelButton.addListener(this);
    _cancelButton.changeWidthToFitText(buttonHeight);
    _cancelButton.setTopLeftPosition(0, heightSoFar + kButtonGap);
    content->addAndMakeVisible(&_cancelButton, 0);
    _okButton.setWantsKeyboardFocus(true);
    _okButton.setMouseClickGrabsKeyboardFocus(false);
    _okButton.setCommandToTrigger(NULL, kConfigurationOK, false);
    _okButton.addListener(this);
    _okButton.changeWidthToFitText(buttonHeight);
    _okButton.setTopLeftPosition(0, heightSoFar + kButtonGap);
    content->addAndMakeVisible(&_okButton, 0);
    heightSoFar += buttonHeight;
    OD_LOG_OBJEXIT(); //####
} // ConfigurationWindow::setUpStandardFields

void ConfigurationWindow::tellAllFieldsToIgnoreNextFocusLoss(void)
{
    OD_LOG_ENTER(); //####
    for (size_t ii = 0, maxf = _standardFieldEditors.size(); maxf > ii; ++ii)
    {
        TextEditorWithCaption * anEditor = _standardFieldEditors[static_cast<int>(ii)];
        
        if (anEditor)
        {
            anEditor->ignoreNextFocusLoss();
        }
    }
    for (size_t ii = 0, maxf = _extraFieldEditors.size(); maxf > ii; ++ii)
    {
        TextEditorWithCaption * anEditor = _extraFieldEditors[static_cast<int>(ii)];

        if (anEditor)
        {
            anEditor->ignoreNextFocusLoss();
        }
    }
    OD_LOG_EXIT(); //####
} // ConfigurationWindow::tellAllFieldsToIgnoreNextFocusLoss

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
