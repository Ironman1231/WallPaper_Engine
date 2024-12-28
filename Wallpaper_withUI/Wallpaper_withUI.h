#pragma once

#include <QtWidgets/QWidget>
#include "ui_Wallpaper_withUI.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <Windows.h>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

class Wallpaper_withUI : public QWidget
{
    Q_OBJECT

public:
    Wallpaper_withUI(QWidget* parent = nullptr);
    ~Wallpaper_withUI();

    //BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM lparam);

protected:
    void closeEvent(QCloseEvent* event) override;    // Rewrite close event

private:
    Ui::Wallpaper_withUIClass ui;

    //Labels for video path and player path
    QLabel* videoPathLabel;
    QLabel* sleepTimeLabel;
    //QLabel* playerPathLabel;

    //LineEdit to show file path
    QLineEdit* videoPathEdit;
    QLineEdit* sleepTimeEdit;
    //QLineEdit* playerPathEdit;

    //Buttons for the selection of pathes
    QPushButton* videoPathButton;
    //QPushButton* playerPathButton;

    //CheckBox Component
    QCheckBox* minimizingCheckBox;

    //Buttons for two functions
    QPushButton* startButton;
    QPushButton* stopButton;

    //Layout
    QWidget* centralWidget;
    QHBoxLayout* videoPathLayout;
    QHBoxLayout* sleepTimeLayout;
    //QHBoxLayout* playerPathLayout;
    QHBoxLayout* buttonLayout;
    QVBoxLayout* mainLayout;

    //Path of video and player
    QString currentVideoPath;    // the video path that is currently playing
    QString newVideoPath;     // the new video path that will be played 
    QString playerPath;

    QSystemTrayIcon* trayIcon;      // Icon for tray
    QMenu* trayMenu;       // Tray Right-Click Menu
    QAction* restoreAction;    // Restore Window Action
    QAction* quitAction;      // Quit Program Action

private:
    void selectVideoPath();
    //void selectPlayerPath();
    void startDisplay();
    void stopDisplay();
    void restoreWindow();
    void quitApplication();
};
