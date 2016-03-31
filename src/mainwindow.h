#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#define OPEN_TEST_SCHEMATIC
#define TEST_TRACK
#define TRACK_SPLIT


#include <QMainWindow>
#include <QSettings>
#include <QTabWidget>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QResizeEvent>
#include <QMoveEvent>
#include <QAction>
#include <QList>
#include <QMap>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QXmlInputSource>
#include <QXmlSimpleReader>
#include <QMessageBox>
#include <QFrame>
#include <QComboBox>
#include <QSortFilterProxyModel>
#include <QListWidget>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QInputDialog>
#include <QStringListModel>
#include <QListView>


class SchematicFrame;
class PrototypeBoard;
//class XmlSchematicReader;
class Track;
class Options;
class OptionsDialog;
class ComponentBox;
class Display;


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    enum { MaxRecentFiles = 10 };
    enum { MaxRecentProjects = 10 };

protected slots:
    virtual void resizeEvent(QResizeEvent* event);
    virtual void moveEvent(QMoveEvent* event);
    virtual void closeEvent (QCloseEvent *);

    void openProject();
    void openRecentProject();
    void newProject();
    void saveProject();
    void saveAsProject();
    void openSchematic();
    void openEagleCadSchematic(QString);
    void openGedaSchematic(QString);
    void openRecentSchematic();
    void closeProject();
    void newPrototypeBoard();
    void exit();
    void options();
    void information(QString title, QString message);
    void warning(QString title, QString message);
    virtual void saveSettings();
    void zoomIn();
    void zoomOut();
    void fitToWidth();
    void changePackage();
    void lock();

    // library methods
    void accessComponent();
    void importGSchemSymbol();
    void importGSchemFootprint();
    void importEagleLibrary();

#if defined(TEST_TRACK)
    void setupTest();
#endif

protected:
    QListWidget *pROPackageBox, *pLocalPackageBox;
    QComboBox *pROLibraryBox, *pLocalLibraryBox;

    OptionsDialog *pDlg;
    QTabWidget *pTabs;

    SchematicFrame *pSchematicFrame;
    PrototypeBoard *pBoard;
    QFrame *pLibraryEditFrame;
    ComponentBox *pComponentBox;
    QListView *pComponentList;
    Display *pRODisplay, *pLocalDisplay;

    QMenu *pFileMenu, *pFileSchematicMenu, *pHelpMenu, *pRecentProjectsMenu, *pRecentFilesMenu;
    QMenu *pViewZoom;
    QMenu *pBoardMenu, *pOptionsMenu, *pViewMenu;

    QAction *pFileNewProjectAct, *pFileOpenProjectAct, *pFileCloseProjectAct;
    QAction *pFileSaveProjectAct, *pFileSaveAsProjectAct;
    QAction *pFileOpenEagleSchematicAct, *pFileOpenGedaSchematicAct;
    QAction *pFileExitAct;
    QAction *pBoardNewPrototypeBoardAct, *pBoardChangePackageAct, *pBoardLockComponent;
    QAction *pViewZoomIn, *pViewZoomOut, *pViewFitWidth;
    QAction *pToolOptionsAct;
    // library actions and menus.
    QMenu *pLibMenu;
    QAction *pAccessComponentAct;

    QToolBar *pFileToolBar, *pExitToolBar, *pEditToolBar, *pViewToolBar;

//    XmlSchematicReader *pSchematicReader;
    QList<QAction*> *pRecentProjects, *pRecentFiles;
    QStringList mFiles, mProjects;

    int mNumRecentFiles, mNumRecentProjects;
    int mRows, mColumns, mRowWidth, mColumnWidth, mBorderWidth;

    QString mEagleWorkspace;

    void initBuild();
    QFrame* initBoardFrame();
    void initActions();
    void initMenus();
    void initFileMenu();
    void initLibraryMenu();
    void initToolbar();
    void initStatusbar();
    void loadSettings();
    void updateFileList();
    void updateProjectsList();
    QString strippedName(const QString &fullFileName);

private:
    Track *track;

};

#endif // MAINWINDOW_H
