//--------------------------------------------------------------------------------------------------
//
//  File:       m+mSettingsWindow.cpp
//
//  Project:    m+m
//
//  Contains:   The class definition for the application settings window of the m+m manager
//              application.
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
//  Created:    2015-06-10
//
//--------------------------------------------------------------------------------------------------

#include "m+mSettingsWindow.h"

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
 
 @brief The class definition for the application settings window of the m+m manager application. */
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

SettingsWindow::SettingsWindow(const String &          title,
                               const String &          execType,
                               const ApplicationInfo & appInfo,
                               String &                endpointToUse,
                               String &                tagToUse,
                               String &                portToUse,
                               StringArray &           argsToUse)  :
    inherited1(), inherited2(), inherited3(title, kWindowBackgroundColour, 0), inherited4(),
    _topText("topText"), _cancelButton("Cancel"), _okButton("OK"),
    _descriptors(appInfo._argDescriptions),
    _errorFont(Font::getDefaultMonospacedFontName(), kFontSize, Font::italic + Font::bold),
    _regularFont(Font::getDefaultMonospacedFontName(), kFontSize, Font::plain), _execType(execType),
    _extraArgumentsGroup(nullptr), _addArgumentsButton(nullptr), _removeArgumentsButton(nullptr),
    _endpointEditor(nullptr), _portEditor(nullptr), _tagEditor(nullptr),
    _endpointDescriptor(new Utilities::ChannelArgumentDescriptor(kEndpointFieldName.toStdString(),
                                                                 "", Utilities::kArgModeOptional, "", nullptr)),
    _portDescriptor(new Utilities::PortArgumentDescriptor(kPortFieldName.toStdString(), "", Utilities::kArgModeOptional, 0,
                                                          false, nullptr)), _appInfo(appInfo),
    _endpointToUse(endpointToUse), _portToUse(portToUse), _tagToUse(tagToUse),
    _argsToUse(argsToUse), _canHaveExtraArguments(false), _canSetEndpoint(false),
    _canSetPort(false), _canSetTag(false), _hasFileField(false)
{
    OD_LOG_ENTER(); //####
    OD_LOG_S2s("title = ", title, "execType = ", execType); //####
    OD_LOG_P4("appInfo = ", &appInfo, "endpointToUse = ", &endpointToUse, "tagToUse = ", //####
              &tagToUse, "portToUse = ", &portToUse); //####
    OD_LOG_P1("argsToUse = ", &argsToUse); //####
    _contentArea.setSize(100, 100);
    setContentNonOwned(&_contentArea, true);
    BorderSize<int> bt = getBorderThickness();
    BorderSize<int> cb = getContentComponentBorder();
    int             heightSoFar = 0;
    int             widthSoFar = 0;

    _adjustedEditorHeight = _regularFont.getHeight() + kEditorHeightAdjustment;
    _argsToUse.clear();
    _canSetEndpoint = appInfo._options.contains("e");
    _canSetPort = appInfo._options.contains("p");
    _canSetTag = appInfo._options.contains("t");
    setUpStandardFields(widthSoFar, heightSoFar);
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
} // SettingsWindow::SettingsWindow

SettingsWindow::~SettingsWindow(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // SettingsWindow::~SettingsWindow

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

void SettingsWindow::addAnExtraField(void)
{
    OD_LOG_ENTER(); //####
    Component *             content = _extraArgumentsGroup;
    String                  compCountAsString(static_cast<int>(_extraFieldEditors.size() + 1));
    Point<int>              dimensions;
    TextEditorWithCaption * newEditor = new TextEditorWithCaption(*this, _regularFont, _errorFont,
                                                                  nullptr, _extraArgRootName + "_" +
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
} // SettingsWindow::addAnExtraField

void SettingsWindow::adjustFields(void)
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
    if (_endpointEditor)
    {
        _endpointEditor->setSize(newFieldWidth, _endpointEditor->getHeight());
    }
    if (_portEditor)
    {
        _portEditor->setSize(newFieldWidth, _portEditor->getHeight());
    }
    if (_tagEditor)
    {
        _tagEditor->setSize(newFieldWidth, _tagEditor->getHeight());
    }
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
} // SettingsWindow::adjustFields

void SettingsWindow::buttonClicked(Button * aButton)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("aButton = ", aButton); //####
    int commandId = aButton->getCommandID();
    
    tellAllFieldsToIgnoreNextFocusLoss();
    switch (commandId)
    {
        case kSettingsAddField :
            addAnExtraField();
            break;
            
        case kSettingsRemoveField :
            removeMostRecentlyAddedExtraField();
            break;
            
        case kSettingsOK :
            if (fieldsAreValid())
            {
                exitModalState(commandId);
            }
            break;
            
        case kSettingsFileRequest :
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
} // SettingsWindow::buttonClicked

bool SettingsWindow::fieldsAreValid(void)
{
    OD_LOG_ENTER(); //####
    int    badCount = 0;
    String badArgs;
    String primaryChannel;
    
    // Counterintuitively, we check the values from the descriptors first, before checking the
    // endpoint, port or tag values.
    for (size_t ii = 0, maxf = _standardFieldEditors.size(); maxf > ii; ++ii)
    {
        TextEditorWithCaption * anEditor = _standardFieldEditors[static_cast<int>(ii)];
        
        if (anEditor && (! anEditor->validateField(&_argsToUse)))
        {
            if (0 < badArgs.length())
            {
                badArgs += "\n";
            }
            badArgs += anEditor->getName();
            ++badCount;
        }
    }
    // Add the extra arguments here.
    for (size_t ii = 0, maxf = _extraFieldEditors.size(); maxf > ii; ++ii)
    {
        TextEditorWithCaption * anEditor = _extraFieldEditors[static_cast<int>(ii)];

        _argsToUse.add(anEditor->getText());
    }
    if (_canSetEndpoint)
    {
        if (_endpointEditor->validateField())
        {
            _endpointToUse = _endpointEditor->getText();
        }
        else
        {
            if (0 < badArgs.length())
            {
                badArgs += "\n";
            }
            badArgs += "Endpoint";
            ++badCount;
        }
    }
    if (_canSetPort)
    {
        if (_portEditor->validateField())
        {
            _portToUse = _portEditor->getText();
        }
        else
        {
            if (0 < badArgs.length())
            {
                badArgs += "\n";
            }
            badArgs += "Port";
            ++badCount;
        }
    }
    if (_canSetTag)
    {
        _tagToUse = _tagEditor->getText();
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
    else
    {
        ManagerApplication * ourApp = ManagerApplication::getApp();
        
        if (ourApp->getPrimaryChannelForService(_appInfo, _endpointToUse, _tagToUse, _portToUse,
                                                _argsToUse, primaryChannel))
        {
            if (Utilities::CheckForChannel(primaryChannel.toStdString()))
            {
                AlertWindow::showMessageBox(AlertWindow::WarningIcon, getName(),
                                            String("The primary channel\n") + primaryChannel +
                                            "\nof the " + _execType + " is in use.\n"
                                            "Please correct the arguments to the " + _execType +
                                            " and try again.", String::empty, this);
                badCount = -1; // Flag this as bad.
            }
        }
        else
        {
            AlertWindow::showMessageBox(AlertWindow::WarningIcon, getName(),
                                        "There was a problem retrieving the primary channel for "
                                        "the " + _execType + ".\n"
                                        "Please try again.", String::empty, this);
            badCount = -1; // Flag this as bad.
        }
    }
    OD_LOG_EXIT_B(0 == badCount); //####
    return (0 == badCount);
} // SettingsWindow::fieldsAreValid

#if (! MAC_OR_LINUX_)
# pragma warning(push)
# pragma warning(disable: 4100)
#endif // ! MAC_OR_LINUX_
void SettingsWindow::focusGained(FocusChangeType cause)
{
#if MAC_OR_LINUX_
# pragma unused(cause)
#endif // MAC_OR_LINUX_
	OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // SettingsWindow::focusGained
#if (! MAC_OR_LINUX_)
# pragma warning(pop)
#endif // ! MAC_OR_LINUX_

#if (! MAC_OR_LINUX_)
# pragma warning(push)
# pragma warning(disable: 4100)
#endif // ! MAC_OR_LINUX_
void SettingsWindow::focusLost(FocusChangeType cause)
{
#if MAC_OR_LINUX_
# pragma unused(cause)
#endif // MAC_OR_LINUX_
	OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // SettingsWindow::focusLost
#if (! MAC_OR_LINUX_)
# pragma warning(pop)
#endif // ! MAC_OR_LINUX_

void SettingsWindow::handleAsyncUpdate(void)
{
    OD_LOG_OBJENTER(); //####
    ApplicationCommandManager & commandManager = ManagerWindow::getApplicationCommandManager();

    commandManager.registerAllCommandsForTarget(JUCEApplication::getInstance());
    OD_LOG_OBJEXIT(); //####
} // SettingsWindow::handleAsyncUpdate

bool SettingsWindow::keyPressed(const KeyPress & key)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("key = ", &key); //####
    bool result;
    
    if (key == KeyPress::escapeKey)
    {
        tellAllFieldsToIgnoreNextFocusLoss();
        exitModalState(kSettingsCancel);
        result = true;
    }
    else if (key == KeyPress::returnKey)
    {
        tellAllFieldsToIgnoreNextFocusLoss();
        if (fieldsAreValid())
        {
            exitModalState(kSettingsOK);
        }
        result = true;
    }
    else
    {
        result = inherited3::keyPressed(key);
    }
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // SettingsWindow::keyPressed

void SettingsWindow::recalculateArea(void)
{
    OD_LOG_ENTER(); //####
    int    heightSoFar = 0;
    int    widthSoFar = 0;
    size_t numExtra = _extraFieldEditors.size();
    
    heightSoFar = _topText.getY() + _topText.getHeight() + kButtonGap;
    widthSoFar = jmax(widthSoFar, _topText.getX() + _topText.getWidth());
    if (_canSetEndpoint)
    {
        Label * aCaption = _endpointEditor->getCaption();
        
        widthSoFar = jmax(widthSoFar, aCaption->getX() + aCaption->getWidth());
        heightSoFar = _endpointEditor->getY() + _endpointEditor->getHeight() + (kButtonGap / 2);
    }
    if (_canSetPort)
    {
        Label * aCaption = _portEditor->getCaption();

        widthSoFar = jmax(widthSoFar, aCaption->getX() + aCaption->getWidth());
        heightSoFar = _portEditor->getY() + _portEditor->getHeight() + (kButtonGap / 2);
    }
    if (_canSetTag)
    {
        Label * aCaption = _tagEditor->getCaption();
        
        widthSoFar = jmax(widthSoFar, aCaption->getX() + aCaption->getWidth());
        heightSoFar = _tagEditor->getY() + _tagEditor->getHeight() + (kButtonGap / 2);
    }
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
} // SettingsWindow::recalculateArea

void SettingsWindow::removeMostRecentlyAddedExtraField(void)
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
} // SettingsWindow::removeMostRecentlyAddedExtraField

void SettingsWindow::reportErrorInField(TextEditorWithCaption & fieldOfInterest)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("fieldOfInterest = ", &fieldOfInterest); //####
    String nameToDisplay;
    
    if (&fieldOfInterest == _endpointEditor)
    {
        nameToDisplay = "Endpoint";
    }
    else if (&fieldOfInterest == _portEditor)
    {
        nameToDisplay = "Port";
    }
    else
    {
        nameToDisplay = fieldOfInterest.getName();
    }
    AlertWindow::showMessageBox(AlertWindow::WarningIcon, getName(),
                                String("The ") + nameToDisplay + " argument is invalid.\n"
                                "Please correct the argument and try again.", String::empty,
                                this);
    OD_LOG_OBJEXIT(); //####
} // SettingsWindow::reportErrorInField

void SettingsWindow::resized(void)
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
} // SettingsWindow::resized

void SettingsWindow::setUpStandardFields(int & widthSoFar,
                                         int & heightSoFar)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P2("widthSoFar = ", &widthSoFar, "heightSoFar = ", &heightSoFar); //####
    Component *               content = getContentComponent();
    int                       buttonHeight = getLookAndFeel().getAlertWindowButtonHeight();
    Point<int>                dimensions;
    size_t                    numDescriptors = _descriptors.size();
    ScopedPointer<TextButton> fileButton(new TextButton(kFileButtonText));
    
    fileButton->changeWidthToFitText(buttonHeight);
    _fileButtonWidth = fileButton->getWidth();
    widthSoFar = heightSoFar = 0;
    _topText.setFont(_regularFont);
    if ((0 < numDescriptors) || _canSetEndpoint || _canSetPort || _canSetTag)
    {
        _topText.setText(String("The ") + _execType + " has one or more arguments, that need to be "
                         "provided before it can be launched.", dontSendNotification);
    }
    else
    {
        _topText.setText(String("The ") + _execType + " has no arguments or options, so it can be "
                         "launched right now.", dontSendNotification);
    }
    MPlusM_Manager::CalculateTextArea(dimensions, _regularFont, _topText.getText());
    _topText.setBounds(kButtonGap, kButtonGap + getTitleBarHeight(), dimensions.getX() + kButtonGap,
                       dimensions.getY());
    content->addAndMakeVisible(&_topText, 0);
    heightSoFar = _topText.getY() + _topText.getHeight() + kButtonGap;
    widthSoFar = jmax(widthSoFar, _topText.getX() + _topText.getWidth());
    if (_canSetEndpoint)
    {
        /*! @brief The text field validator for endpoints. */
        _endpointEditor = new TextEditorWithCaption(*this, _regularFont, _errorFont,
                                                    new TextValidator(*_endpointDescriptor),
                                                    kEndpointFieldName);
        Label * aCaption = _endpointEditor->getCaption();
        
        aCaption->setText("(Optional) Endpoint to use", dontSendNotification);
        MPlusM_Manager::CalculateTextArea(dimensions, _regularFont, aCaption->getText());
        aCaption->setBounds(kLabelInset, heightSoFar, dimensions.getX() + kLabelInset,
							dimensions.getY());
        content->addAndMakeVisible(aCaption);
		heightSoFar = aCaption->getY() + aCaption->getHeight() + kLabelToFieldGap;
        widthSoFar = jmax(widthSoFar, aCaption->getX() + aCaption->getWidth());
        _endpointEditor->setBounds(kFieldInset, heightSoFar, widthSoFar - kFieldInset,
                                   static_cast<int>(_adjustedEditorHeight));
        _endpointEditor->setSelectAllWhenFocused(true);
        content->addAndMakeVisible(_endpointEditor);
        heightSoFar = _endpointEditor->getY() + _endpointEditor->getHeight() + (kButtonGap / 2);
    }
    if (_canSetPort)
    {
        /*! @brief The text field validator for ports. */
        _portEditor = new TextEditorWithCaption(*this, _regularFont, _errorFont,
                                                new TextValidator(*_portDescriptor),
                                                kPortFieldName);
        Label * aCaption = _portEditor->getCaption();

        aCaption->setText("(Optional) Network port to use", dontSendNotification);
        MPlusM_Manager::CalculateTextArea(dimensions, _regularFont, aCaption->getText());
        aCaption->setBounds(kLabelInset, heightSoFar, dimensions.getX() + kLabelInset,
                            dimensions.getY());
        content->addAndMakeVisible(aCaption);
		heightSoFar = aCaption->getY() + aCaption->getHeight() + kLabelToFieldGap;
        widthSoFar = jmax(widthSoFar, aCaption->getX() + aCaption->getWidth());
        _portEditor->setBounds(kFieldInset, heightSoFar, widthSoFar - kFieldInset,
                               static_cast<int>(_adjustedEditorHeight));
        _portEditor->setSelectAllWhenFocused(true);
        content->addAndMakeVisible(_portEditor);
        heightSoFar = _portEditor->getY() + _portEditor->getHeight() + (kButtonGap / 2);
    }
    if (_canSetTag)
    {
        _tagEditor = new TextEditorWithCaption(*this, _regularFont, _errorFont, nullptr,
                                               kTagFieldName);
        Label * aCaption = _tagEditor->getCaption();
        
        aCaption->setText(String("(Optional) Tag for the ") + _execType, dontSendNotification);
        MPlusM_Manager::CalculateTextArea(dimensions, _regularFont, aCaption->getText());
		aCaption->setBounds(kLabelInset, heightSoFar, dimensions.getX() + kLabelInset,
                            dimensions.getY());
        content->addAndMakeVisible(aCaption);
		heightSoFar = aCaption->getY() + aCaption->getHeight() + kLabelToFieldGap;
        widthSoFar = jmax(widthSoFar, aCaption->getX() + aCaption->getWidth());
        _tagEditor->setBounds(kFieldInset, heightSoFar, widthSoFar - kFieldInset,
                              static_cast<int>(_adjustedEditorHeight));
        _tagEditor->setSelectAllWhenFocused(true);
        content->addAndMakeVisible(_tagEditor);
        heightSoFar = _tagEditor->getY() + _tagEditor->getHeight() + (kButtonGap / 2);
    }
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
                if (! _canHaveExtraArguments)
                {
                    _canHaveExtraArguments = true;
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
                    _addArgumentsButton->setCommandToTrigger(nullptr, kSettingsAddField, false);
                    _addArgumentsButton->addListener(this);
                    _addArgumentsButton->changeWidthToFitText(buttonHeight);
                    content->addAndMakeVisible(_addArgumentsButton, 0);
                    _removeArgumentsButton = new TextButton(String("- ") + argName);
                    _removeArgumentsButton->setWantsKeyboardFocus(true);
                    _removeArgumentsButton->setMouseClickGrabsKeyboardFocus(false);
                    _removeArgumentsButton->setCommandToTrigger(nullptr, kSettingsRemoveField,
                                                                false);
                    _removeArgumentsButton->addListener(this);
                    _removeArgumentsButton->changeWidthToFitText(buttonHeight);
                    content->addChildComponent(_removeArgumentsButton);
                }
            }
            else
            {
                bool                    forOutput;
                String                  descriptionPrefix;
                TextValidator *         newValidator = new TextValidator(*aDescriptor);
                TextEditorWithCaption * newEditor = new TextEditorWithCaption(*this, _regularFont,
                                                                              _errorFont,
                                                                              newValidator,
                                                                              argName);
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
                newEditor->setText(aDescriptor->getDefaultValue().c_str(), false);
                newEditor->setSelectAllWhenFocused(true);
                content->addAndMakeVisible(newEditor);
                if (aDescriptor->isForFiles(forOutput))
                {
                    TextButton * newButton = new TextButton(kFileButtonText);

                    newButton->setWantsKeyboardFocus(true);
                    newButton->setMouseClickGrabsKeyboardFocus(false);
                    newButton->setCommandToTrigger(nullptr, kSettingsFileRequest, false);
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
    _cancelButton.setCommandToTrigger(nullptr, kSettingsCancel, false);
    _cancelButton.addListener(this);
    _cancelButton.changeWidthToFitText(buttonHeight);
    _cancelButton.setTopLeftPosition(0, heightSoFar + kButtonGap);
    content->addAndMakeVisible(&_cancelButton, 0);
    _okButton.setWantsKeyboardFocus(true);
    _okButton.setMouseClickGrabsKeyboardFocus(false);
    _okButton.setCommandToTrigger(nullptr, kSettingsOK, false);
    _okButton.addListener(this);
    _okButton.changeWidthToFitText(buttonHeight);
    _okButton.setTopLeftPosition(0, heightSoFar + kButtonGap);
    content->addAndMakeVisible(&_okButton, 0);
    heightSoFar += buttonHeight;
    OD_LOG_OBJEXIT(); //####
} // SettingsWindow::setUpStandardFields

void SettingsWindow::tellAllFieldsToIgnoreNextFocusLoss(void)
{
    OD_LOG_ENTER(); //####
    if (_endpointEditor)
    {
        _endpointEditor->ignoreNextFocusLoss();
    }
    if (_portEditor)
    {
        _portEditor->ignoreNextFocusLoss();
    }
    if (_tagEditor)
    {
        _tagEditor->ignoreNextFocusLoss();
    }
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
} // SettingsWindow::tellAllFieldsToIgnoreNextFocusLoss

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
