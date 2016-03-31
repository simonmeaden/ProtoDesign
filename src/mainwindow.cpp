//#include <qstd.h>

#include "mainwindow.h"

#include "schematicframe.h"

#include "eaglecadschematic.h"
#include "gschemschematic.h"

#include "boards/prototypeboard.h"
#include "boards/stripboard.h"
#include "boards/stripboard_01pitch_60x35.h"
#include "boards/track.h"

#include "optionswidgets.h"

#include "librarymodel.h"

#include "display.h"

#include "detect.h"

QPalette redPalette;
QSettings *pSettings;
Options *pOptions;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

    redPalette.setColor(QPalette::Window, QColor("red"));

    pSettings = new QSettings(QSettings::NativeFormat, QSettings::UserScope,"SM Electronic Components", "ProtoDesign", this);
    pOptions = new Options(this);
//    pDb = new ProtoDesignDB(this);
    // these initialise the various static values in Track;
//    track = new Track(m_settings, this);
//    track->loadSettings();

    pRecentProjects = new QList<QAction*>();
    pRecentFiles = new QList<QAction*>();

    pDlg = NULL;

    initBuild();

    loadSettings();

#if defined(TEST_TRACK)
    setupTest();
#endif

//    pROLibraryBox->addItems(pDb->libraryNames());
//    pLocalLibraryBox->addItems(pDb->localLibraryNames());

}

MainWindow::~MainWindow() {
//    track->deleteLater();
}

#if defined(TEST_TRACK)
void MainWindow::setupTest() {

//#   if defined OPEN_TEST_SCHEMATIC
//    openSchematic();
//#   endif

#   if defined(TRACK_SPLIT)
    pBoard->splitTrack(0, 10);
    pBoard->splitTrackBetween(0, 20, 0, 21);
    pBoard->splitTrackBetween(0, 30, 0, 31);
    pBoard->splitTrackBetween(10, 30, 10, 31);
    pBoard->splitTrack(34, 30);
#   endif

}
#endif

void MainWindow::loadSettings() {

    pSettings->beginGroup("MainWindow");
    resize(pSettings->value("size", QSize(600, 600)).toSize());
    move(pSettings->value("pos", QPoint(200, 200)).toPoint());
    pSettings->endGroup();

    pSettings->beginGroup("root");
    mEagleWorkspace = pSettings->value("eagleWorkspace", "/workspace/eagle").toString();
    pSettings->endGroup();

    mFiles = pSettings->value("recentFileList").toStringList();
    mNumRecentFiles = qMin(mFiles.size(), (int)MaxRecentFiles);
    for (int i = 0; i < mNumRecentFiles; ++i) {

        pRecentFiles->insert(i, new QAction(strippedName(mFiles[i]), this));
        pRecentFiles->at(i)->setVisible(false);
        connect(pRecentFiles->at(i), SIGNAL(triggered()), this, SLOT(openRecentSchematic()));
    }

    mProjects = pSettings->value("recentProjectList").toStringList();
    mNumRecentProjects = qMin(mProjects.size(), (int)MaxRecentProjects);
    for (int i = 0; i < mNumRecentProjects; ++i) {

        pRecentProjects->insert(i, new QAction(strippedName(mProjects[i]), this));
        pRecentProjects->at(i)->setVisible(false);
        connect(pRecentProjects->at(i), SIGNAL(triggered()), this, SLOT(openRecentProject()));
    }

    pSettings->sync();
}

void MainWindow::saveSettings() {
}

QString MainWindow::strippedName(const QString &fullFileName) {
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);

    pSettings->beginGroup("MainWindow");
    pSettings->setValue("size", event->size());
    pSettings->endGroup();
    pSettings->sync();
}

void MainWindow::moveEvent(QMoveEvent *event) {
    QMainWindow::moveEvent(event);

    pSettings->beginGroup("MainWindow");
    pSettings->setValue("pos", event->pos());
    pSettings->endGroup();
    pSettings->sync();
}

void MainWindow::initBuild() {

    initActions();
    initMenus();
    initToolbar();
    initStatusbar();

    pTabs = new QTabWidget(this);
    setCentralWidget(pTabs);

    QFrame *boardFrame = initBoardFrame();
    pTabs->addTab(boardFrame, tr("Board"));

    pSchematicFrame = new SchematicFrame(this);
    pTabs->addTab(pSchematicFrame, tr("Schematic"));

}

QFrame* MainWindow::initBoardFrame() {
    QFrame *boardFrame = new QFrame(this);
    QGridLayout *boardLayout = new QGridLayout;
    boardFrame->setLayout(boardLayout);

    pBoard = new Stripboard_01Pitch_60x35(pOptions, this);
    pBoard->setFrameStyle(QFrame::StyledPanel);
    boardLayout->addWidget(pBoard, 0, 0);

    pComponentList = new QListView(this);
    boardLayout->addWidget(pComponentList, 0, 1);

    return boardFrame;
}

void MainWindow::initMenus() {

    initFileMenu();

    pBoardMenu = menuBar()->addMenu(tr("&Board"));
    pBoardMenu->addAction(pBoardNewPrototypeBoardAct);
    pBoardMenu->addSeparator();
    pBoardMenu->addAction(pBoardChangePackageAct);
    pBoardMenu->addAction(pBoardLockComponent);

    pViewMenu = menuBar()->addMenu(tr("&View"));
    pViewMenu->addAction(pViewZoomIn);
    pViewMenu->addAction(pViewZoomOut);
    pViewMenu->addAction(pViewFitWidth);
    pViewZoom = pViewMenu->addMenu(tr("&Zoom"));

    pOptionsMenu = menuBar()->addMenu(tr("Tools"));

    initLibraryMenu();

    pOptionsMenu->addAction(pToolOptionsAct);

    pHelpMenu = menuBar()->addMenu(tr("&Help"));
}

void MainWindow::initFileMenu() {
    pFileMenu = menuBar()->addMenu(tr("&File"));

    pFileNewProjectAct = new QAction(tr("&New Project"), this);
    pFileNewProjectAct->setShortcuts(QKeySequence::New);
    pFileNewProjectAct->setToolTip(tr("Create a new project"));
    connect(pFileNewProjectAct, SIGNAL(triggered()), this, SLOT(newProject()));
    pFileMenu->addAction(pFileNewProjectAct);

    pFileOpenProjectAct = new QAction(tr("&Open Project"), this);
    pFileOpenProjectAct->setShortcuts(QKeySequence::Open);
    pFileOpenProjectAct->setToolTip(tr("Open an existing project"));
    connect(pFileOpenProjectAct, SIGNAL(triggered()), this, SLOT(openProject()));
    pFileMenu->addAction(pFileOpenProjectAct);

    pFileSaveProjectAct = new QAction(tr("&Save Project"), this);
    pFileSaveProjectAct->setShortcuts(QKeySequence::Save);
    pFileSaveProjectAct->setToolTip(tr("Save project"));
    connect(pFileSaveProjectAct, SIGNAL(triggered()), this, SLOT(saveProject()));
    pFileMenu->addAction(pFileSaveProjectAct);

    pFileSaveAsProjectAct = new QAction(tr("Save &As Project"), this);
    pFileSaveAsProjectAct->setShortcuts(QKeySequence::SaveAs);
    pFileSaveAsProjectAct->setToolTip(tr("Save project under different name"));
    connect(pFileSaveAsProjectAct, SIGNAL(triggered()), this, SLOT(saveAsProject()));
    pFileMenu->addAction(pFileSaveAsProjectAct);

    pFileCloseProjectAct = new QAction(tr("&Close Project"), this);
    pFileCloseProjectAct->setShortcuts(QKeySequence::Close);
    pFileCloseProjectAct->setToolTip(tr("Close working project"));
    connect(pFileCloseProjectAct, SIGNAL(triggered()), this, SLOT(closeProject()));

    pFileSchematicMenu = new QMenu(tr("Open Schematics"), pFileMenu);
    pFileMenu->addMenu(pFileSchematicMenu);

    pFileOpenEagleSchematicAct = new QAction(QIcon(":/icons/schematic"), tr("Open Eagle Schematic"), this);
    pFileOpenEagleSchematicAct->setToolTip(tr("Open Eagle schematic file"));
    connect(pFileOpenEagleSchematicAct, SIGNAL(triggered()), this, SLOT(openSchematic()));
    pFileSchematicMenu->addAction(pFileOpenEagleSchematicAct);

    pFileOpenGedaSchematicAct = new QAction(QIcon(":/icons/schematic"), tr("Open Geda Schematic"), this);
    pFileOpenGedaSchematicAct->setToolTip(tr("Open Eagle schematic file"));
    connect(pFileOpenGedaSchematicAct, SIGNAL(triggered()), this, SLOT(openSchematic()));
    pFileSchematicMenu->addAction(pFileOpenGedaSchematicAct);

    pRecentProjectsMenu = new QMenu(tr("Recent Projects"), this);
    pFileMenu->addMenu(pRecentProjectsMenu);
    pRecentFilesMenu = new QMenu(tr("Recent Schematic Files"), this);
    pFileMenu->addMenu(pRecentFilesMenu);
    pFileMenu->addSeparator();

    pFileExitAct = new QAction(QIcon(":/icons/exit"), tr("E&xit"), this);
    pFileExitAct->setShortcuts(QKeySequence::Quit);
    pFileExitAct->setToolTip(tr("Exit the application"));
    connect(pFileExitAct, SIGNAL(triggered()), this, SLOT(exit()));
    pFileMenu->addAction(pFileExitAct);
}

void MainWindow::initLibraryMenu() {
    pLibMenu = menuBar()->addMenu(tr("&Library"));

    pAccessComponentAct = new QAction(tr("Access component"), this);
    pAccessComponentAct->setToolTip(tr("Access a component from the library."));
    connect(pAccessComponentAct, SIGNAL(triggered(bool)), this, SLOT(accessComponent()));
    pLibMenu->addAction(pAccessComponentAct);

    QMenu *importMenu = new QMenu(tr("&Import"));

    QMenu *eagleMenu = new QMenu(tr("&Eagle Cad"));
    importMenu->addMenu(eagleMenu);

    QAction *eagleSymbolFromFile = new QAction(tr("From EagleCad Library"), this);
    eagleSymbolFromFile->setToolTip(tr("Import a Eagle Cad Symbol/Package"));
    connect(eagleSymbolFromFile, SIGNAL(triggered(bool)), this, SLOT(importEagleLibrary()));
    eagleMenu->addAction(eagleSymbolFromFile);

    QMenu *gschemMenu = new QMenu(tr("&GSchem"));
    importMenu->addMenu(gschemMenu);

    QAction *gschemSymbolFromFile = new QAction(tr("Symbol from file"), this);
    gschemSymbolFromFile->setToolTip(tr("Import a GSchem Symbol from file"));
    connect(gschemSymbolFromFile, SIGNAL(triggered(bool)), this, SLOT(importGSchemSymbol()));
    gschemMenu->addAction(gschemSymbolFromFile);

    QAction *gschemFootprintFromFile = new QAction(tr("Footprint from file"), this);
    gschemFootprintFromFile->setToolTip(tr("Import a GSchem Footprint from file"));
    connect(gschemFootprintFromFile, SIGNAL(triggered(bool)), this, SLOT(importGSchemFootprint()));
    gschemMenu->addAction(gschemFootprintFromFile);

    pLibMenu->addMenu(importMenu);
}

void MainWindow::accessComponent() {

}

void MainWindow::importGSchemSymbol() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Symbol File"), "~", tr("Symbol Files (*.sym)"));

    QFile *file = new QFile(fileName, this);
    if (file->exists()) {
        if (!file->open(QFile::ReadOnly)) {
            qDebug() << "Unable to open file : " << file->fileName();
            return;
        }


    }
}

void MainWindow::importGSchemFootprint() {

}

void MainWindow::importEagleLibrary() {

}

void MainWindow::initActions() {
    // options actions ========================================================================================
    pToolOptionsAct = new QAction(tr("&Options"), this);
    pToolOptionsAct->setToolTip(tr("Set the options."));
    connect(pToolOptionsAct, SIGNAL(triggered()), this, SLOT(options()));

    // view actions ===========================================================================================
    pViewZoomIn = new QAction(QIcon(":/icons/zoomin"), tr("Zoom In"), this);
    connect(pViewZoomIn, SIGNAL(triggered()), this, SLOT(zoomIn()));

    pViewZoomOut = new QAction(QIcon(":/icons/zoomout"), tr("Zoom Out"), this);
    connect(pViewZoomOut, SIGNAL(triggered()), this, SLOT(zoomOut()));

    pViewFitWidth = new QAction(QIcon(":/icons/fitwidth"), tr("Fit to width"), this);
    connect(pViewFitWidth, SIGNAL(triggered()), this, SLOT(fitToWidth()));

    // Board based actions ====================================================================================
    pBoardNewPrototypeBoardAct = new QAction(tr("Prototype board designer"), this);
    pBoardNewPrototypeBoardAct->setToolTip(tr("Design a new prototype board layout"));
    connect(pBoardNewPrototypeBoardAct, SIGNAL(triggered()), this, SLOT(newPrototypeBoard()));

    pBoardChangePackageAct = new QAction(QIcon(":/icons/change"), tr("Change"), this);
    pBoardChangePackageAct->setToolTip(tr("Change package of component"));
    connect(pBoardChangePackageAct, SIGNAL(triggered()), this, SLOT(changePackage()));

    pBoardLockComponent = new QAction(QIcon(":/icons/lock"), tr("Lock"), this);
    pBoardLockComponent->setToolTip(tr("Lock component"));
    connect(pBoardLockComponent, SIGNAL(triggered()), this, SLOT(lock()));

}

void MainWindow::closeEvent(QCloseEvent */*event*/) {
    exit();
}

void MainWindow::zoomIn() {
    // TODO zoom in
}

void MainWindow::zoomOut() {
    // TODO zoom out
}

void MainWindow::fitToWidth() {
    // TODO fit to width
}

void MainWindow::lock() {
    // TODO lock component
}

void MainWindow::changePackage() {
    // TODO change package
}

void MainWindow::options() {
    if (!pDlg)
        pDlg = new OptionsDialog(pOptions, this);

    if (pDlg->exec() == QDialog::Accepted) {
        pOptions->setUnits(pDlg->units());
        pOptions->setHorizontalBorder(pDlg->horizontalBorder());
        pOptions->setVerticalBorder(pDlg->verticalBorder());
        pOptions->setHorizontalOffset(pDlg->horizontalOffset());
        pOptions->setVerticalOffset(pDlg->verticalOffset());
        pOptions->setRowPercent(pDlg->rowPercent());
        pOptions->setGapPercent(pDlg->gapPercent());
        pOptions->setPixelsPerUnit(pDlg->pixelsPerUnit());
    }
}

void MainWindow::initToolbar() {
    pFileToolBar = addToolBar(tr("File"));
    pFileToolBar->addAction(pFileOpenEagleSchematicAct);

    pEditToolBar = addToolBar(tr("edit"));
    pEditToolBar->addAction(pBoardChangePackageAct);
    pEditToolBar->addAction(pBoardLockComponent);

    pViewToolBar = addToolBar(tr("view"));
    pViewToolBar->addAction(pViewZoomIn);
    pViewToolBar->addAction(pViewZoomOut);
    pViewToolBar->addAction(pViewFitWidth);

    pExitToolBar = addToolBar(tr("exit"));
    QWidget *separator = new QWidget(this);
    separator->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pExitToolBar->addWidget(separator);
    pExitToolBar->addAction(pFileExitAct);

}

void MainWindow::initStatusbar() {
    // TODO init status bar
}

void MainWindow::newProject() {
    // TODO new project
}

void MainWindow::openProject() {
    //TODO open project
}

void MainWindow::openRecentProject() {
    // TODO open recent project
}

void MainWindow::openSchematic() {
    QString filename = QFileDialog::getOpenFileName(
                           this,
                           tr("Open Schematic"),
                           "~",
                           tr("Eagle Schematic Files (*.sch);;"
                              "Geda Schematic Files (*.sch)"));

    if (!filename.isEmpty()) {
        QFileInfo info(filename);

        if (info.fileName().endsWith(".sch")) {

            if (Detect::isEagleCadSchematic(info.fileName())) {
                openEagleCadSchematic(info.fileName());
            } else if (Detect::isGedaSchematic(info.fileName())) {
                openGedaSchematic(info.fileName());
            }


        }
    }
}

void MainWindow::openEagleCadSchematic(QString filename) {
    EagleCadSchematic *schematic = new EagleCadSchematic(this);
    schematic->read(filename);
    pBoard->setSchematic(schematic);
}

void MainWindow::openGedaSchematic(QString filename) {
    GSchemSchematic *schematic = new GSchemSchematic(this);
    schematic->read(filename);
    pBoard->setSchematic(schematic);
}

void MainWindow::updateFileList() {
    pSettings->beginWriteArray("recentFileList");
    for (int i = 0; i < mFiles.size(); ++i) {
        pSettings->setArrayIndex(i);
        pSettings->setValue("filename", mFiles.at(i));
    }
    pSettings->endArray();
    pSettings->sync();
}

void MainWindow::information(QString title, QString message) {
    QMessageBox::information(this, title, message);
}

void MainWindow::warning(QString title, QString message) {
    QMessageBox::warning(this, title, message);
}


void MainWindow::updateProjectsList() {
    // TODO open projects list
}

void MainWindow::openRecentSchematic() {
    // TODO open recent schematic
}

void MainWindow::saveProject() {
    // TODO save project
}

void MainWindow::saveAsProject() {
    // TODO save as project
}

void MainWindow::closeProject() {
    // TODO close project
}

void MainWindow::newPrototypeBoard() {
    // TODO new prototype board
}

void MainWindow::exit() {
    pSettings->sync();
//    pDb->write();

    close();
}
