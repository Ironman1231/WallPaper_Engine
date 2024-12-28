#include "Wallpaper_withUI.h"
#include <QCloseEvent>
#include <QMessageBox>

Wallpaper_withUI::Wallpaper_withUI(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    //Create components
    videoPathLabel = new QLabel("Video Path:", this);
    //playerPathLabel = new QLabel("Player Path:", this);
    sleepTimeLabel = new QLabel("Sleep Time:", this);

    videoPathEdit = new QLineEdit(this);
    //playerPathEdit = new QLineEdit(this);
    sleepTimeEdit = new QLineEdit(this);
    sleepTimeEdit->setText("500");    // set default value
    sleepTimeEdit->setToolTip("If wallpaper cannot be displayed properly, increase this value in steps of 100");

    videoPathButton = new QPushButton("Select video path", this);
    //playerPathButton = new QPushButton("Select player path", this);

    minimizingCheckBox = new QCheckBox("Minimizing to tray", this);
    //Checked by default
    minimizingCheckBox->setChecked(true);      

    startButton = new QPushButton("Start", this);
    stopButton = new QPushButton("Stop", this);

    //Layout
    centralWidget = new QWidget(this);
    videoPathLayout = new QHBoxLayout();
    //playerPathLayout = new QHBoxLayout();
    sleepTimeLayout = new QHBoxLayout();
    buttonLayout = new QHBoxLayout();
    mainLayout = new QVBoxLayout();

    //Add components to Layouts
    videoPathLayout->addWidget(videoPathLabel);
    videoPathLayout->addWidget(videoPathEdit);
    videoPathLayout->addWidget(videoPathButton);

    //playerPathLayout->addWidget(playerPathLabel);
    //playerPathLayout->addWidget(playerPathEdit);
    //playerPathLayout->addWidget(playerPathButton);

    sleepTimeLayout->addWidget(sleepTimeLabel);
    sleepTimeLayout->addWidget(sleepTimeEdit);

    buttonLayout->addWidget(minimizingCheckBox);
    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(stopButton);

    mainLayout->addLayout(videoPathLayout);
    //mainLayout->addLayout(playerPathLayout);
    mainLayout->addLayout(sleepTimeLayout);
    mainLayout->addLayout(buttonLayout);

    centralWidget->setLayout(mainLayout);

    //Create icon of tray
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/Wallpaper_withUI/Icon/ultraman_png_icon.png"));    // Set Tray Icon
    trayIcon->setToolTip("WallPaper Application");    // Set tray tips

    //Create menu of tray
    trayMenu = new QMenu(this);
    restoreAction = new QAction("Restore", this);
    quitAction = new QAction("Quit", this);

    trayMenu->addAction(restoreAction);
    trayMenu->addSeparator();
    trayMenu->addAction(quitAction);

    trayIcon->setContextMenu(trayMenu);    // Set the right-click menu


    //Connecting signals and slots
    connect(videoPathButton, &QPushButton::clicked, this, &Wallpaper_withUI::selectVideoPath);
    //connect(playerPathButton, &QPushButton::clicked, this, &Wallpaper_withUI::selectPlayerPath);

    connect(startButton, &QPushButton::clicked, this, &Wallpaper_withUI::startDisplay);
    connect(stopButton, &QPushButton::clicked, this, &Wallpaper_withUI::stopDisplay);

    connect(restoreAction, &QAction::triggered, this, &Wallpaper_withUI::restoreWindow);
    connect(quitAction, &QAction::triggered, this, &Wallpaper_withUI::quitApplication);
    connect(trayIcon, &QSystemTrayIcon::activated, this, [=](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::Trigger)
        {
            restoreWindow();
        }
        });

    trayIcon->show();
}

Wallpaper_withUI::~Wallpaper_withUI()
{}

void Wallpaper_withUI::closeEvent(QCloseEvent * event)
{
    //check whether checkbox is checked
    bool isChecked = minimizingCheckBox->isChecked();
    
    //Intercept the close eventand minimize to the system tray
    if (trayIcon->isVisible() && isChecked)
    {
        hide();
        trayIcon->showMessage("Application Minimized",
            "The application is still running in the system tray.",
            QSystemTrayIcon::Information, 3000);  // Display a notification
        event->ignore();  // Ignore the close event to prevent the window from closing

        trayIcon->show();
        return;
    }

    stopDisplay();
}

//BOOL QtWidgetsApplication1::EnumWindowsProc(HWND hwnd, LPARAM lparam)
//{
//    HWND hDefView = FindWindowEx(hwnd, 0, L"SHELLDLL_DefView", 0);
//    if (hDefView != 0)
//    {
//        HWND hWorkerw = FindWindowEx(0, hwnd, L"WorkerW", 0);
//        ShowWindow(hWorkerw, SW_HIDE);
//
//        return FALSE;
//    }
//    return TRUE;
//}

void Wallpaper_withUI::selectVideoPath()
{
    QString tempVideoPath = QFileDialog::getOpenFileName(
        this,
        "Video Path",
        "",                     // Initial directory or starting directory that the file dialog will display when it opens
        "Video Files (*.mp4 *.avi *.mkv *.mov *.flv *.wmv);;All Files (*.*)"   // File filters
    );
    if (!tempVideoPath.isEmpty())
    {
        newVideoPath = tempVideoPath;
        newVideoPath = newVideoPath.replace("/", "\\");      // replace "/" with "\\" which is used in command
        videoPathEdit->setText(newVideoPath);
    }
}

//void Wallpaper_withUI::selectPlayerPath()
//{
//    playerPath = QFileDialog::getOpenFileName(
//        this,
//        "Player Path",
//        "",                     // Initial directory or starting directory that the file dialog will display when it opens
//        "Excutable Files (*.exe);;All Files (*.*)"   // File filters
//    );
//    playerPath = playerPath.replace("/", "\\");
//    if (!playerPath.isEmpty())
//    {
//        playerPathEdit->setText(playerPath);
//    }
//}

BOOL EnumWindowsProc(HWND hwnd, LPARAM lparam)
{
    HWND hDefView = FindWindowEx(hwnd, 0, L"SHELLDLL_DefView", 0);
    if (hDefView != 0)
    {
        HWND hWorkerw = FindWindowEx(0, hwnd, L"WorkerW", 0);
        ShowWindow(hWorkerw, SW_HIDE);

        return FALSE;
    }
    return TRUE;
}

void Wallpaper_withUI::startDisplay()
{
    //add some command to  videoPath and convert it from QString to wstring
    QString tempVideoPath = " --fullscreen --loop \"" + newVideoPath + "\"";
    std::wstring wVideoPath = tempVideoPath.toStdWString();
    LPCWSTR lpVideoPath = wVideoPath.c_str();

    //convert playerPath from QString to wstring
    //std::wstring wPlayerPath = playerPath.toStdWString();
    //LPCWSTR lpPlayerPath = wPlayerPath.c_str();

    STARTUPINFO si{ 0 };
    PROCESS_INFORMATION pi{ 0 };

    //if (CreateProcess(lpPlayerPath, (LPWSTR)lpVideoPath, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    if (CreateProcess(L".\\mpv-x86_64-20241204-git-78ee714\\mpv.exe", (LPWSTR)lpVideoPath, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        stopDisplay();   // close the video that is currently playing
        
        int sleepTime = sleepTimeEdit->text().toInt();
        Sleep(sleepTime);

        HWND hProgman = FindWindow(L"Progman", 0);
        SendMessageTimeout(hProgman, 0x52c, 0, 0, 0, 100, 0);

        //Extract video name
        QFileInfo fileinfo(newVideoPath);
        QString videoName = fileinfo.fileName();

        //convert videoName from QString to wstring
        videoName = videoName + " - mpv";
        std::wstring wVideoName = videoName.toStdWString();
        LPCWSTR windowName = wVideoName.c_str();

        // For FindWindow() this function, the first argument is for the hundle of window and the second one is for the name of the window.
        // Not necessarily both arguments need to be provided, but at least one must be explicit. Another one can be NULL or 0.
        //HWND hMpvWindow = FindWindow(L"mpv", L"DynamicWallpaper.mp4 - mpv");
        HWND hMpvWindow = FindWindow(L"mpv", windowName);
        SetParent(hMpvWindow, hProgman);

        EnumWindows(EnumWindowsProc, 0);

        currentVideoPath = newVideoPath;
    }
}

//Define a structure
struct CallbackData {
    QString currentVideoPath; // 目标窗口标题
};

//Close window
BOOL CALLBACK CloseWindows(_In_ HWND hwnd, _In_ LPARAM lparam)
{
    //convert LPARAM to structure pointer
    CallbackData *data = reinterpret_cast<CallbackData*>(lparam);

    std::wstring wVideoName = data->currentVideoPath.toStdWString();
    LPCWSTR windowName = wVideoName.c_str();

    //HWND hDefView = FindWindowEx(hwnd, 0, L"mpv", L"DynamicWallpaper.mp4 - mpv");
    HWND hDefView = FindWindowEx(hwnd, 0, L"mpv", windowName);
    if (hDefView != 0)
    {
        // Send a WM_CLOSE message to close the window
        PostMessage(hDefView, WM_CLOSE, 0, 0);

        return FALSE;
    }
    return TRUE;
}

void Wallpaper_withUI::stopDisplay()
{
    //Extract video name
    QFileInfo fileinfo(currentVideoPath);
    QString videoName = fileinfo.fileName();

    //convert videoName from QString to wstring
    videoName = videoName + " - mpv";
    std::wstring wVideoName = videoName.toStdWString();
    LPCWSTR windowName = wVideoName.c_str();

    //HWND hMpvWindow = FindWindow(L"mpv", L"DynamicWallpaper.mp4 - mpv");
    HWND hMpvWindow = FindWindow(L"mpv", windowName);
    if (hMpvWindow)
    {
        // Send a WM_CLOSE message to close the window
        PostMessage(hMpvWindow, WM_CLOSE, 0, 0);
    }
    else
    {
        CallbackData data;
        data.currentVideoPath = videoName;
        EnumWindows(CloseWindows, reinterpret_cast<LPARAM>(&data));
    }
}

void Wallpaper_withUI::restoreWindow()
{
    showNormal();       // Restore the window
    raise();    // Raise the window (display on top of other windows)
    activateWindow();    // Activate the window
}

void Wallpaper_withUI::quitApplication()
{
    stopDisplay();
    //trayIcon->hide();  // Hide the tray icon (Without this sentence is also working)
    QApplication::quit();  // Exit the application
}
