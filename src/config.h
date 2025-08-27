// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 zalesyc and the lilac contributors

#pragma once

#include <QObject>
#include <QPoint>

namespace Lilac {

// this class is a singleton
class Config : public QObject {
    Q_OBJECT

   public:
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    static Config& get();

   signals:
    void configChanged();

   private:
    Config();

   private:
    static Config instance;

   private slots:
#if HAS_SETTINGS
    void onSettingsChanged();
#endif
    /*
     * padding and margin are used acording to the css box model:
     *   padding: inside the border
     *   margin: outside the border
     *
     * vertical and horizontal padding/margin:
     *  vertical: top, bottom
     *  horizontal: left, right
     *
     * unless specified othervise, sizes are in logical pixels
     *
     *
     * Constexpr variables cannot be changed, other variables may
     * change trough the configuration system.
     */
   public:
    int cornerRadius = 12;

    static constexpr int smallArrowSize = 10;

    static constexpr int controlsTextVerticalPadding = 10;  // buttons, lineEdits, spinBoxes, comboboxes

    static constexpr int pushButtonMenuArrowPadding = 6;  // length between the menu arrow and the right side

    static constexpr int checkBoxSize = 20;  // all checkbox values are also used for radio buttons
    static constexpr int checkBoxHoverCircleSize = 34;
    static constexpr int checkBoxElementSpacing = 6;  // between the indicator, icon and label; sets PM_CheckBoxLabelSpacing
    bool circleCheckBox = false;

    static constexpr int tabCloseIndicatorSize = 6;          // size of the x, the hover rect size is defined in PM_TabCloseIndicatorWidth/Height
    static constexpr int tabBarStartMargin = 6;              // padding on the left/top side of the tabbar, if the tab alighnment is left, only for QTabWidget
    static constexpr int tabBarMarginAboveTabs = 4;          // there is no rect for the tab position for both QTabBar and QTabWidget so the implemention is not ideal
    static constexpr int tabWidgetPageAreaCornerRadius = 6;  // corner radius for the frame arount the contents of QTabWidget
    static constexpr int tabHorizontalPadding = 8;           // padding between the sides of the tab and the first/last element
    static constexpr int tabElementSpacing = 7;              // spacing between the inner elements of the tab

    static constexpr int sliderHandleHoverCircleDiameter = 32;
    static constexpr int sliderHandleDiameter = 12;
    static constexpr int sliderHandleThicknessMargin = 2;  // margin of the handle in the thickness direction, slider thickness is calculated like: handle + max(handleThicknessMargin, tickmarks)
    static constexpr int sliderTickmarksLen = 8;

    static constexpr int scrollBarThickness = 16;
    static constexpr int scrollBarSliderPadding = 6;  // left and right of the slider -> sets the thickness
    static constexpr int scrollBarSliderPaddingHover = 4;

    static constexpr int lineEditTextHorizontalPadding = 6;
    static constexpr int lineEditMinWidthChars = 14;  // minimal width of lineedit: lineEditMinWidthChars * averageCharWidth

    static constexpr int spinIndicatorWidth = 30;  // the width of the clickable space
    static constexpr int spinIndicatorIconSize = 10;
    static constexpr int spinIndicatorHoverCircleSize = 24;
    static constexpr int spinMinWidthChars = lineEditMinWidthChars;
    static constexpr int spinTextLeftPadding = lineEditTextHorizontalPadding;

    static constexpr int menuMargin = 7;  // transparent margin added to the menu, this margin contains the shadow and creates spacing between submenus
    static constexpr int menuBorderRadius = 10;
    static constexpr int menuItemElementHorizontalSpacing = 8;
    static constexpr int menuItemBorderRadius = 5;
    static constexpr int menuItemHorizontalMargin = 5;
    static constexpr int menuItemHorizontalPadding = 5;
    static constexpr int menuItemVerticalPadding = 5;
    static constexpr int menuSeparatorVerticalMargin = 3;
    static constexpr int menuSeparatorMinLen = 5;
    static constexpr qreal menuShadowBlurRadius = 3.3;
    static constexpr QPointF menuShadowOffset = QPointF(0.3, 0.8);
    static constexpr QPointF menuHighlightDirection = QPointF(0.2, 15);  // the direction and size of the gradient creating the highlight, only used in dark mode
    quint8 menuBgOpacity = 200;                                          // value between 0 and 255, 0: transparent; 255: opaque
    bool menuBlurBehind = true;                                          // whether to blur behind the menu if menuBgOpacity < 255; only if compiled without NO_KWINDOWSYSTEM

    static constexpr int menuBarItemMinHeight = 24;  // without the margin
    static constexpr int menuBarItemBorderRadius = 4;
    static constexpr int menuBarItemMargin = 2;

    static constexpr int comboArrowWidth = 30;  // the part that is clickable, the indicator is smaller
    static constexpr int comboMinWidthChars = lineEditMinWidthChars;
    static constexpr int comboTextLeftPadding = lineEditTextHorizontalPadding;  // used left of icon and left of text
    static constexpr int comboPopupPadding = 3;
    static constexpr int comboPopupMargin = 3;
    static constexpr int comboPopupShadowSize = 5;

    static constexpr int toolBtnMenuArrowSize = 8;              // the small arrow in the bottom left corner
    static constexpr QPoint toolBtnArrowOffset = QPoint(5, 5);  // offset from bottomRight of the widget, for the small arrow in the bottom left
    static constexpr int toolBtnLabelVerticalPadding = controlsTextVerticalPadding;
    static constexpr int toolBtnLabelHorizontalPadding = 6;
    static constexpr int toolbtnLabelSpacing = 4;        // between the icon and text
    static constexpr int toolbtnArrowSectionWidth = 20;  // width of the separated arrow section on the right, poppupMode: MenuButtonPopup
    static constexpr int toolBtnMenuSeparatorVerticalPadding = 4;

    static constexpr int toolBarHandleVerticalPadding = 6;     // vertical when toolbar is horizontal
    static constexpr int toolBarSeparatorVerticalPadding = 3;  // vertical when toolbar is horizontal
    static constexpr int toolBarHandleHorizontalPadding = 2;   // horizontal when toolbar is horizontal
    static constexpr int toolBarHandleLineSpacing = 2;

    static constexpr int progressBarLabelHorizontalPadding = 3;  // around the label, horizontal when the progressbar is horizontal
    static constexpr int progressBarThickness = 6;
    static constexpr qreal progressBarBusyIndicatorLen = 0.25;  // value between 0 and 1 setting how large part of the entire bar should be the indicator

    static constexpr int treeIndicatorArrowSize = 10;
    static constexpr int treeIndicatorArrowLineMargin = 4;   // the distance between the arrow and start of the line
    static constexpr int treeIndicatorLastCornerRadius = 5;  // the radius of the curve on the last indicator

    static constexpr int groupBoxCheckSize = checkBoxSize;
    static constexpr int groupBoxTextCheckSpacing = 8;                                // spacing between the label and the checkbox
    static constexpr int groupBoxHeaderHorizontalPadding = 5;                         // space between the end of the line and the label/check
    static constexpr int groupBoxLabelOffset = groupBoxHeaderHorizontalPadding + 10;  // length between the side of the groupbox and the label/check

    static constexpr int dialHandleDiameter = 12;
    static constexpr int dialHandleHoverCircleDiameter = 32;

    static constexpr int dialRangeNonWaraping = 300;  // degrees

    static constexpr int dockHeaderLabelHorizontalPadding = 6;
    static constexpr int dockHeaderControlsHeight = 12;  // for the close and float buttons

    static constexpr int itemViewItemHorizontalPadding = 5;
    static constexpr int itemViewItemVerticalPadding = 5;
    static constexpr int itemViewItemElementSpacing = 8;
    static constexpr int listViewItemVerticalMargin = 2;       // if item->displayPosition is top or bottom, this is used for all sides
    static constexpr int listViewItemHorizontalMargin = 5;     // only if item->displayPosition is left or right
    static constexpr int kFilePlacesViewHorizontalMargin = 2;  // special case for QListView, if it's KFilePlacesView like in the left dolphin sidebar, where I dont draw vertical margin, because the list items are too small

    static constexpr int tooltipOpacity = 235;

    // animation constants
    /* durations are in miliseconds
     *
     * the durations are only a default value,
     * in the end they may be faster/slower depending on the global animation speed
     */
    double animationSpeed = 1;  // if <=0 then the animations are instant, if settings are enabled this value may be overriden
    static constexpr int progressBarBusyDuration = 1000;
    static constexpr int scrollBarShowDuration = 40;
};

}  // namespace Lilac
