/*
 ISC License

 Copyright (c) 2016-2017, Autonomous Vehicle Systems Lab, University of Colorado at Boulder

 Permission to use, copy, modify, and/or distribute this software for any
 purpose with or without fee is hereby granted, provided that the above
 copyright notice and this permission notice appear in all copies.

 THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

 */
#include "mainwindow.h" // DEF: header file generated by the wizard
#include "ui_mainwindow.h" // DEF: UI header file generated by the uic tool
#include "scenewidget.h"
#include "ipaddressdialog.h"
#include "adcssimdatamanager.h"

#include <QMessageBox>
#include <QPixmap>
#include <QFile>
#include <QFileInfo>
#include <QSettings>
#include <QDoubleSpinBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QSignalMapper>
#include <qkeysequence.h>

extern "C" {
#include "astroFunctions.h"
}

MainWindow::MainWindow()
    : QMainWindow(0)
    , ui(new Ui::MainWindow)

    // NOTE: Change the next line to implement an individual simulation data manager
    , m_simDataManager(new AdcsSimDataManager(this))
    , m_connectionStatus(new QLabel(this))
    , m_modelsDisplayInfo (new ModelsDisplayInfo(this))
    , m_orbitElementsInfo (new OrbitElementsInfo(this))
    , m_reactionWheelsInfo (new ReactionWheelsInfo(this, m_simDataManager))
    , m_simTimeInfo( new SimTimeInfo(this))
    , m_cssInfo(new CssInfo(this))
    , m_angularRatesInfo(new AngularRatesInfo(this))
    , m_controlStateInfo(new ControlStateInfo(this))
    , m_torqueRodInfo(new TorqueRodInfo(this))
    , m_thrustersInfo(new ThrustersInfo(this, m_simDataManager))
    , m_totalPowerInfo(new TotalPowerInfo(this))
    , m_podInfo(new PodInfo(this))
    , m_iruInfo(new IruInfo(this))
    , m_tamInfo(new TamInfo(this))
    , m_starTrackerInfo(new StarTrackerInfo(this))
    , m_cameraTargetCombo(new QComboBox(this))
    , m_cameraModeCombo(new QComboBox(this))
    , m_zoomSpeedLabel(new QLabel(this))
    , m_zoomSpeed(new QDoubleSpinBox(this))
    , m_panSpeedLabel(new QLabel(this))
    , m_panSpeed(new QDoubleSpinBox(this))
    , m_fpvTranslateLabel(new QLabel(this))
    , m_fpvTranslate(new QDoubleSpinBox(this))
    , m_fpvRotateLabel(new QLabel(this))
    , m_fpvRotate(new QDoubleSpinBox(this))
    , m_styleActionGroup(new QActionGroup(this))
    , m_initialCameraMode(-1)
{
    ui->setupUi(this);
    ui->actionOpen_Connection->setIcon(style()->standardIcon(QStyle::SP_DriveNetIcon));
    ui->actionOpen_File->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
    
    ui->menuBar->setEnabled(true);
    ui->menuBar->setFocusPolicy(Qt::StrongFocus);
    ui->menuBar->setNativeMenuBar(false);
    
    // Add action for perturbing rates
    QAction *action = new QAction(tr("Perturb Rates"), this);
    action->setShortcut(QKeySequence(Qt::Key_P));
    this->addAction(action);
//    connect(action, SIGNAL(triggered()), m_simDataManager, SLOT(perturbRates()));
//    connect(m_angularRatesInfo, SIGNAL(perturbRates()), m_simDataManager, SLOT(perturbRates()));
 
    
    // Hook up actions for changing control status
//    connect(m_controlStateInfo, SIGNAL(setControlState(CommandedState_t)), m_simDataManager, SLOT(setControlState(CommandedState_t)));
//    connect(m_controlStateInfo, SIGNAL(setAdcsState(ADCSState_t)), m_simDataManager, SLOT(setAdcsState(ADCSState_t)));
//    connect(m_controlStateInfo, SIGNAL(setThrustingControlMode(int)), m_simDataManager, SLOT(setThrustingControlMode(int)));
//    connect(m_reactionWheelsInfo, SIGNAL(setOnOffState(ComponentState_t, int)), m_simDataManager, SLOT(setOnOffState(ComponentState_t, int)));
//    connect(m_cssInfo, SIGNAL(setStateCSS(ComponentState_t, int)), m_simDataManager, SLOT(setStateCSS(ComponentState_t, int)));
//    connect(m_torqueRodInfo, SIGNAL(setStateTR(ComponentState_t,int)), m_simDataManager, SLOT(setStateTR(ComponentState_t, int)));
//    connect(m_podInfo, SIGNAL(setStatePOD(ComponentState_t)), m_simDataManager, SLOT(setStatePOD(ComponentState_t)));
//    connect(m_iruInfo, SIGNAL(setStateIRU(ComponentState_t)), m_simDataManager, SLOT(setStateIRU(ComponentState_t)));
//    connect(m_tamInfo, SIGNAL(setStateTAM(ComponentState_t)), m_simDataManager, SLOT(setStateTAM(ComponentState_t)));
    connect(m_simDataManager, SIGNAL(setOnOffLegendRW(bool)), m_reactionWheelsInfo, SLOT(setOnOffLegendRW(bool)));
    connect(m_simDataManager, SIGNAL(setOnOffLegendRW(bool)), this, SLOT(openLegendRW(bool)));
    connect(m_simDataManager, SIGNAL(simConnected()), m_reactionWheelsInfo, SLOT(configureLayout()));
    connect(m_simDataManager, SIGNAL(simConnected()), m_thrustersInfo, SLOT(configureLayout()));
//    connect(m_simDataManager, SIGNAL(setOnOffLegendTR(bool)), m_torqueRodInfo, SLOT(setOnOffLegendTR(bool)));
//    connect(m_simDataManager, SIGNAL(setOnOffLegendTR(bool)), this, SLOT(openLegendTR(bool)));
//    connect(m_simDataManager, SIGNAL(setOnOffDefaults(CelestialObject_t)), this, SLOT(setOnOffDefaults(CelestialObject_t)));
//    connect(m_modelsDisplayInfo, SIGNAL(setGravityGradientTorque(int)), m_simDataManager, SLOT(setGravityGradientTorque(int)));
//    connect(m_modelsDisplayInfo, SIGNAL(setReactionWheelJitter(int)), m_simDataManager, SLOT(setReactionWheelJitter(int)));
//    connect(m_modelsDisplayInfo, SIGNAL(setGravityPerturbModel(int)), m_simDataManager, SLOT(setGravityPerturbModel(int)));
//    connect(m_modelsDisplayInfo, SIGNAL(setAtmosDragModel(int)), m_simDataManager, SLOT(setAtmosDragModel(int)));
//    connect(m_modelsDisplayInfo, SIGNAL(setSrpModel(int)), m_simDataManager, SLOT(setSrpModel(int)));
//    connect(m_modelsDisplayInfo, SIGNAL (setAlbedoModel(AlbedoModel_t)), m_simDataManager, SLOT (setAlbedoModel(AlbedoModel_t)));
//    connect(m_modelsDisplayInfo, SIGNAL(setEarthMagField(EarthMagFieldModel_t)), m_simDataManager, SLOT (setEarthMagField(EarthMagFieldModel_t)));
//    connect(m_thrustersInfo, SIGNAL(setNewManeuver(double,double)), m_simDataManager, SLOT (setNewManeuver(double,double)));
    

    

    // Add in default widgets
    // Scene widget must be created before shortcuts widget
    createSceneWidget(); // main scene in MainWindow
    createPlaybackWidget();
    createShortcutsWidget();
    createToggleablesWidget();
    
    ui->actionPlayback_Controls->setChecked(true);
    ui->actionPlayback_Controls->setVisible(true);
    
    // Add in custom dock widgets:
    //addDockWidget(Qt::RightDockWidgetArea, m_modelsDisplayInfo);
    //connect(ui->actionSimInfo, SIGNAL(toggled(bool)), m_modelsDisplayInfo, SLOT(setVisible(bool)));
    ui->actionSimInfo->setChecked(false);
    ui->actionSimInfo->setVisible(false);
    m_modelsDisplayInfo->hide();
    
    addDockWidget(Qt::RightDockWidgetArea, m_orbitElementsInfo);
    connect(ui->actionOE, SIGNAL(toggled(bool)),m_orbitElementsInfo, SLOT(setVisible(bool)));
    ui->actionOE->setChecked(true);
    
    //addDockWidget(Qt::LeftDockWidgetArea,m_controlStateInfo);
    //connect(ui->actionCtrl, SIGNAL(toggled(bool)), m_controlStateInfo, SLOT(setVisible(bool)));
    ui->actionCtrl->setChecked(false);
    ui->actionCtrl->setVisible(false);
    m_controlStateInfo->hide();
    
    addDockWidget(Qt::LeftDockWidgetArea, m_reactionWheelsInfo);
    connect(ui->actionRW, SIGNAL(toggled(bool)),m_reactionWheelsInfo, SLOT(setVisible(bool)));
    ui->actionRW->setChecked(true);
    
    //addDockWidget(Qt::LeftDockWidgetArea, m_cssInfo);
    //connect(ui->actionCSS, SIGNAL(toggled(bool)), m_cssInfo, SLOT(setVisible(bool)));
    ui->actionCSS->setChecked(false);
    ui->actionCSS->setVisible(false);
    m_cssInfo->hide();
    
    //addDockWidget(Qt::LeftDockWidgetArea, m_angularRatesInfo);
    //connect(ui->actionRates, SIGNAL(toggled(bool)), m_angularRatesInfo, SLOT(setVisible(bool)));
    ui->actionRates->setChecked(false);
    ui->actionRates->setVisible(false);
    m_angularRatesInfo->hide();
    
    addDockWidget(Qt::LeftDockWidgetArea, m_simTimeInfo);
    connect(ui->actionSimTime, SIGNAL(toggled(bool)),m_simTimeInfo, SLOT(setVisible(bool)));
    ui->actionSimTime->setChecked(true);
    m_simTimeInfo->hide();
    
    addDockWidget(Qt::RightDockWidgetArea, m_thrustersInfo);
    connect(ui->actionThrusters, SIGNAL(toggled(bool)), m_thrustersInfo, SLOT(setVisible(bool)));
    ui->actionThrusters->setChecked(true);
    m_thrustersInfo->hide();
    
    //addDockWidget(Qt::LeftDockWidgetArea, m_torqueRodInfo);
    //connect(ui->actionTR, SIGNAL(toggled(bool)), m_torqueRodInfo, SLOT(setVisible(bool)));
    ui->actionTR->setChecked(false);
    ui->actionTR->setVisible(false);
    m_torqueRodInfo->hide();
    
    //addDockWidget(Qt::RightDockWidgetArea, m_starTrackerInfo);
    //connect(ui->actionST, SIGNAL(toggled(bool)), m_starTrackerInfo, SLOT(setVisible(bool)));
    ui->actionST->setChecked(false);
    ui->actionST->setVisible(false);
    m_starTrackerInfo->hide();
    
    //addDockWidget(Qt::LeftDockWidgetArea, m_totalPowerInfo);
    //connect(ui->actionTotalPower, SIGNAL(toggled(bool)), m_totalPowerInfo, SLOT(setVisible(bool)));
    ui->actionTotalPower->setChecked(false);
    ui->actionTotalPower->setVisible(false);
    m_totalPowerInfo->hide();
    
    //addDockWidget(Qt::LeftDockWidgetArea, m_podInfo);
    //connect(ui->actionPOD, SIGNAL(toggled(bool)), m_podInfo, SLOT(setVisible(bool)));
    ui->actionPOD->setChecked(false);
    ui->actionPOD->setVisible(false);
    m_podInfo->hide();
    
    //addDockWidget(Qt::RightDockWidgetArea, m_iruInfo);
    //connect(ui->actionIRU, SIGNAL(toggled(bool)), m_iruInfo, SLOT(setVisible(bool)));
    ui->actionIRU->setChecked(false);
    ui->actionIRU->setVisible(false);
    m_iruInfo->hide();
    
    //addDockWidget(Qt::RightDockWidgetArea, m_tamInfo);
    //connect(ui->actionTAM, SIGNAL(toggled(bool)), m_tamInfo, SLOT(setVisible(bool)));
    ui->actionTAM->setChecked(false);
    ui->actionTAM->setVisible(false);
    m_tamInfo->hide();
    
    
    // Add style options
    m_styleActionGroup->setExclusive(true);
    m_styleActionGroup->addAction(ui->actionDefaultStyle);
    m_styleActionGroup->addAction(ui->actionDarkStyle);
    connect(m_styleActionGroup, SIGNAL(triggered(QAction *)), this, SLOT(setWindowStyle(QAction *)));
    ui->actionDarkStyle->setChecked(true);
    setWindowStyle(ui->actionDarkStyle);

    // Add source information to the status bar
    ui->statusBar->addPermanentWidget(m_connectionStatus);
    m_connectionStatus->setText("Source: None");
    connect(m_simDataManager, SIGNAL(showMessage(QString)), ui->statusBar, SLOT(showMessage(QString)));
    connect(m_simDataManager, SIGNAL(showMessage(QString, int)), ui->statusBar, SLOT(showMessage(QString, int)));
    
    loadSettings();

    simDataUpdated();
    
}

MainWindow::~MainWindow()
{
    delete ui;
}


QSize MainWindow::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize MainWindow::sizeHint() const
{
    return QSize(800, 600);
}

/*!
 * Purpose: Set the data path to where the application is to look for supporting libraries.
 *
 *   @param[in] QString *dataPath  path to supporting libraries
 */
void MainWindow::setDataPath(QString &dataPath)
{
    m_dataPath = &dataPath;
}

/*!
 * Purpose: Set the data path to where the application is to look for supporting libraries.
 *
 *   @param[out] QString *dataPath  path to supporting libraries
 */
QString MainWindow::dataPath() // get dataPath
{
    return *m_dataPath;
}

int MainWindow::loadSpiceFiles()
{
    if (m_dataPath) {
        loadSpiceKernels(m_dataPath->toStdString().c_str());
        return 0;
    }
    return 1;
}

int MainWindow::loadStarCatalog()
{
    if (m_dataPath)
    {
        m_starTrackerInfo->parseStarsData(m_dataPath->toStdString().c_str());
        return 0;
    }
    return 1;
}

void MainWindow::openLegendRW(bool open)
{
    if (m_reactionWheelsInfo->isHidden() && open == true)
    {
        m_reactionWheelsInfo->setVisible(true);
        ui->actionRW->setChecked(true);
    }
}

void MainWindow::openLegendTR(bool open)
{
    if (m_torqueRodInfo->isHidden() && open == true)
    {
        m_torqueRodInfo->setVisible(true);
        ui->actionTR->setChecked(true);
    }
}

void MainWindow::setOnOffDefaults(CelestialObject_t mainBody)
{
    switch (mainBody) {
        case CELESTIAL_EARTH:
            
            // Update Toggleable's checkbox
            for (int i=0; i<toggleablesCheckBoxList.size(); i++)
            {
                if (toggleablesCheckBoxList[i]->text() == "Spacecraft Magnetic Field Vector" || toggleablesCheckBoxList[i]->text() == "Torque Rod Pyramid")
                {
                    toggleablesCheckBoxList[i]->show();
                }
            }
            // Update Panels Available
            ui->actionTAM->setVisible(true);
            ui->actionTAM->setCheckable(true);
            
            ui->actionTR->setVisible(true);
            ui->actionTR->setVisible(true);
            
            ui->actionPOD->setVisible(true);
            ui->actionPOD->setVisible(true);
            break;
            
        case CELESTIAL_MARS:
            // Update Toggleable's checkbox
            for (int i=0; i<toggleablesCheckBoxList.size(); i++)
            {
                if (toggleablesCheckBoxList[i]->text() == "Spacecraft Magnetic Field Vector" || toggleablesCheckBoxList[i]->text() == "Torque Rod Pyramid")
                {
                    toggleablesCheckBoxList[i]->hide();
                }
            }
            
            // Update Panels Available
            ui->actionTAM->setVisible(false);
            m_tamInfo->setVisible(false);
            ui->actionTAM->setCheckable(false);
            
            ui->actionTR->setVisible(false);
            m_torqueRodInfo->setVisible(false);
            ui->actionTR->setVisible(false);
            
            ui->actionPOD->setVisible(false);
            m_podInfo->setVisible(false);
            ui->actionPOD->setVisible(false);
            break;
            
        case CELESTIAL_SUN:
            // Update Toggleable's checkbox
            for (int i=0; i<toggleablesCheckBoxList.size(); i++)
            {
                if (toggleablesCheckBoxList[i]->text() == "Spacecraft Magnetic Field Vector" || toggleablesCheckBoxList[i]->text() == "Torque Rod Pyramid")
                {
                    toggleablesCheckBoxList[i]->hide();
                }
            }
            // Update Panels Available
            ui->actionTAM->setVisible(false);
            m_tamInfo->setVisible(false);
            ui->actionTAM->setCheckable(false);
            
            ui->actionTR->setVisible(false);
            m_torqueRodInfo->setVisible(false);
            ui->actionTR->setVisible(false);
            
            ui->actionPOD->setVisible(false);
            m_podInfo->setVisible(false);
            ui->actionPOD->setVisible(false);
            break;
        default:
            break;
    }
}

void MainWindow::simDataUpdated()
{
    
    // Update camera targets (of m_simDataManager and m_sceneWidget according to the box selected)
    // Removing and readding causes issues with selecting camera target
    // so just remove excess and rename things as necessary
    int selectedTargetIndex = m_cameraTargetCombo->currentIndex();
    QString selectedTargetName = m_cameraTargetCombo->currentText();
    QVector<SimObject> simObjects = m_simDataManager->getSimObjects();
    for(int i = m_cameraTargetCombo->count(); i >= simObjects.size(); i--) {
        m_cameraTargetCombo->removeItem(i); // remove excess items fins que nomes quedi el nombre d'obj del SimObject
    }
    for(int i = 0; i < simObjects.size(); i++) {
        if(m_cameraTargetCombo->count() > i) {
            m_cameraTargetCombo->setItemText(i, simObjects.at(i).name);
        } else {
            m_cameraTargetCombo->addItem(simObjects.at(i).name);
        }
        if(simObjects.at(i).name.compare(selectedTargetName) == 0) {
            selectedTargetIndex = i;
            m_initialCameraMode = i;
        }
    }
    if(selectedTargetIndex == -1) {
        selectedTargetIndex = m_simDataManager->getPreferredCameraTarget();
    } // -1 is the value of currentIndex: int when no current item is set in the combo box

    m_cameraTargetCombo->setCurrentIndex(selectedTargetIndex);
    m_simDataManager->setTargetObject(selectedTargetIndex);
    m_sceneWidget->setTargetObject(selectedTargetIndex);
    
    // Update Displays
    //m_modelsDisplayInfo->getModelInfo(m_simDataManager);
    m_orbitElementsInfo->updateOrbitElements(m_simDataManager);
    m_reactionWheelsInfo->updateReactionWheels(m_simDataManager);
    m_simTimeInfo->updateSimTime(m_simDataManager);
    m_cssInfo->updateCSS(m_simDataManager);
    m_angularRatesInfo->updateAngularRates(m_simDataManager);
//    m_controlStateInfo->updateControl(m_simDataManager);
//    m_torqueRodInfo->updateTorqueRods(m_simDataManager);
    m_thrustersInfo->updateThrusters(m_simDataManager);
//    m_totalPowerInfo->updatePower(m_simDataManager);
//    m_podInfo->updatePOD(m_simDataManager);
//    m_iruInfo->updateIRU(m_simDataManager);
//    m_tamInfo->updateTAM(m_simDataManager);
//    m_starTrackerInfo->updateST(m_simDataManager);

    // Update the playbackControls
    SpacecraftSim *scSim = m_simDataManager->getSpacecraftSim();
//    m_playbackControls->setDateTimeBounds(scSim->ics.JD0);
    m_playbackControls->setCurrentDateTime(scSim->spiceTime.J2000Current);
    m_playbackControls->setPlaySpeed(scSim->realTimeSpeedUpFactor);
}

void MainWindow::setWindowStyle(QAction *action)
{
    QString name = action->objectName();
    if(name == "actionDefaultStyle") {
        qApp->setStyleSheet("");
    } else if(name == "actionDarkStyle") {
        QFile file(":/qdarkstyle/style.qss");
        if(file.exists()) {
            file.open(QFile::ReadOnly | QFile::Text);
            QTextStream ts(&file);
            qApp->setStyleSheet(ts.readAll());
        } else {
            qApp->setStyleSheet("");
        }
    }
}

void MainWindow::cycleCameraMode()
{
    m_cameraModeCombo->setCurrentIndex((m_cameraModeCombo->currentIndex() + 1) % m_cameraModeCombo->count());
}

void MainWindow::openConnection()
{
    IpAddressDialog *dialog = new IpAddressDialog(this);
    if(dialog->exec()) {
        QString ipAddress = dialog->getIpAddress();
        QString port = dialog->getPort();
        ui->statusBar->showMessage("Opening " + ipAddress + "::" + port + " and initializing geometries...");
        if(m_simDataManager->openConnection(ipAddress, port)) {
            m_connectionStatus->setText("Source: " + ipAddress + "::" + port);
            ui->actionClose_Connection->setEnabled(true);
        }
        ui->statusBar->clearMessage();
    }
}

void MainWindow::closeConnection()
{
    if(m_simDataManager->closeConnection()) {
        ui->actionClose_Connection->setEnabled(false);
        m_connectionStatus->setText("Source: None");
    }
}

void MainWindow::openFile()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Simulation Data File"), QDir::currentPath());
    if(!filename.isEmpty()) {
        ui->statusBar->showMessage("Opening " + filename + " and initializing geometries...");
        if(m_simDataManager->openFile(filename)) {
            ui->actionClose_File->setEnabled(true);
            m_connectionStatus->setText("Source: " + filename);
        }
        ui->statusBar->clearMessage();
    }
}

void MainWindow::closeFile()
{
    if(m_simDataManager->closeFile()) {
        ui->actionClose_File->setEnabled(false);
        m_connectionStatus->setText("Source: None");
    }
}

void MainWindow::toggleFullScreen()
{
    if(isFullScreen()) {
        showNormal();
        ui->statusBar->showMessage("Fullscreen disabled", 5000);
    } else {
        showFullScreen();
        ui->statusBar->showMessage("Fullscreen enabled, press " + ui->actionFull_Screen->shortcut().toString() + " to exit");
    }
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    QMainWindow::closeEvent(event);
}


void MainWindow::createShortcutsWidget()
{
    // Add in actions
    QList<QAction *> actions = this->findChildren<QAction *>();
    ui->shortcutsTable->setColumnCount(2);
    ui->shortcutsTable->setRowCount(actions.length());
    QStringList headers;
    headers.push_back("Shortcuts");
    headers.push_back("Description");
    ui->shortcutsTable->setHorizontalHeaderLabels(headers);
    ui->shortcutsTable->verticalHeader()->hide();
    int row = 0;
    for(int i = 0; i < actions.size(); i++) {
        QAction *action = actions.at(i);
        QString shortcut = action->shortcut().toString();
        if(!shortcut.isEmpty()) {
            ui->shortcutsTable->setItem(row, 1, new QTableWidgetItem(action->iconText()));
            ui->shortcutsTable->setItem(row, 0, new QTableWidgetItem(shortcut));
            row++;
        }
    }

    // Add in scene shortcuts
        QVector<shortcutPair> sceneShortcuts = m_sceneWidget->getShortcuts();
        for(int i = 0; i < sceneShortcuts.size(); i++) {
            ui->shortcutsTable->setItem(row, 1, new QTableWidgetItem(sceneShortcuts.at(i).first));
            ui->shortcutsTable->setItem(row, 0, new QTableWidgetItem(sceneShortcuts.at(i).second.toString()));
            row++;
        }
    ui->shortcutsTable->setRowCount(row);

    int tot=ui->shortcutsTable->rowCount();
    if(m_initialCameraMode!=2){
        for (int i=tot; i>=(tot-sceneShortcuts.size());i--){
            ui->shortcutsTable->hideRow(i);
        }
    }
    else {
        for (int i=(tot-sceneShortcuts.size()); i<=tot; i++){
            ui->shortcutsTable->showRow(i);
        }
    }
}


void MainWindow::createSceneWidget()
{
    m_sceneWidget = new SceneWidget(ui->centralWidget, m_simDataManager);
    m_sceneWidget->setObjectName(QStringLiteral("sceneWidget"));
    m_sceneWidget->setMinimumSize(QSize(50, 50));
    ui->centralWidgetLayout->addWidget(m_sceneWidget); // adding m_sceneWidget at centralWidgetLayout
    ui->centralWidgetLayout->setStretch(0, 1);
    connect(ui->actionReset_Camera, SIGNAL(triggered()), m_sceneWidget, SLOT(resetAll()));
    connect(ui->actionWireframe, SIGNAL(toggled(bool)), m_sceneWidget, SLOT(setWireframe(bool)));
    connect(ui->actionCamera_Target, SIGNAL(toggled(bool)), m_sceneWidget, SLOT(setCameraTargetVisible(bool)));

    // Setup the simulation data manager
    connect(m_simDataManager, SIGNAL(simDataUpdated()), m_sceneWidget, SLOT(update()));
    connect(m_simDataManager, SIGNAL(simDataUpdated()), this, SLOT(simDataUpdated())); 
    
    // Camera target combobox
    QLabel *label = new QLabel("Camera Target:");
    label->setMargin(5);
    ui->viewToolbar->addWidget(label);
    ui->viewToolbar->addWidget(m_cameraTargetCombo);
    connect(m_cameraTargetCombo, SIGNAL(currentIndexChanged(int)), m_sceneWidget, SLOT(setTargetObject(int)));
    ui->viewToolbar->addSeparator();

    // Camera mode combobox
    label = new QLabel("Camera Mode:");
    label->setMargin(5);
    ui->viewToolbar->addWidget(label);
    ui->viewToolbar->addWidget(m_cameraModeCombo);
    connect(m_cameraModeCombo, SIGNAL(currentIndexChanged(int)), m_sceneWidget, SLOT(setCameraMode(int)));
    connect(m_cameraModeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateViewToolbarStates()));
    connect(m_cameraModeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateShortcutsStates()));
    m_cameraModeCombo->addItem("Arcball");
    m_cameraModeCombo->addItem("Pan");
    m_cameraModeCombo->addItem("FPV");

    // Zoom speed spin box
    m_zoomSpeedLabel->setText("Zoom Speed:");
    m_zoomSpeedLabel->setMargin(5);
    m_zoomActions.push_back(ui->viewToolbar->addWidget(m_zoomSpeedLabel));
    m_zoomActions.push_back(ui->viewToolbar->addWidget(m_zoomSpeed));
    m_zoomSpeed->setDecimals(2);
    m_zoomSpeed->setMinimum(0.01);
    m_zoomSpeed->setValue(0.1);
    m_zoomSpeed->setMinimumWidth(70);
    connect(m_zoomSpeed, SIGNAL(valueChanged(double)), m_sceneWidget, SLOT(setZoomSpeed(double)));

    // Pan speed spin box
    m_panSpeedLabel->setText("Translate Speed:");
    m_panSpeedLabel->setMargin(5);
    m_panActions.push_back(ui->viewToolbar->addWidget(m_panSpeedLabel));
    m_panActions.push_back(ui->viewToolbar->addWidget(m_panSpeed));
    m_panSpeed->setDecimals(2);
    m_panSpeed->setMinimum(0.01);
    m_panSpeed->setValue(0.1);
    m_panSpeed->setMinimumWidth(70);
    connect(m_panSpeed, SIGNAL(valueChanged(double)), m_sceneWidget, SLOT(setPanSpeed(double)));

    // FPV translate spin box
    m_fpvTranslateLabel->setText("Translate Speed:");
    m_fpvTranslateLabel->setMargin(5);
    m_fpvActions.push_back(ui->viewToolbar->addWidget(m_fpvTranslateLabel));
    m_fpvActions.push_back(ui->viewToolbar->addWidget(m_fpvTranslate));
    m_fpvTranslate->setDecimals(2);
    m_fpvTranslate->setMinimum(0.01);
    m_fpvTranslate->setValue(0.1);
    m_fpvTranslate->setMinimumWidth(70);
    connect(m_fpvTranslate, SIGNAL(valueChanged(double)), m_sceneWidget, SLOT(setFpvTranslateSpeed(double)));

    // FPV rotate spin box
    m_fpvRotateLabel->setText("Rotate Speed:");
    m_fpvRotateLabel->setMargin(5);
    m_fpvActions.push_back(ui->viewToolbar->addWidget(m_fpvRotateLabel));
    m_fpvActions.push_back(ui->viewToolbar->addWidget(m_fpvRotate));
    m_fpvRotate->setDecimals(2);
    m_fpvRotate->setMinimum(0.01);
    m_fpvRotate->setValue(1.0);
    m_fpvRotate->setMinimumWidth(70);
    connect(m_fpvRotate, SIGNAL(valueChanged(double)), m_sceneWidget, SLOT(setFpvRotateSpeed(double)));
    
    m_initialCameraMode =m_cameraModeCombo->currentIndex();
    
    updateViewToolbarStates();
    updateShortcutsStates();
}

void MainWindow::createPlaybackWidget()
{
    m_playbackControls = new PlaybackControls(ui->centralWidget);
    m_playbackControls->setObjectName(QStringLiteral("playbackControls"));
    ui->centralWidgetLayout->addWidget(m_playbackControls);

    connect(m_playbackControls, SIGNAL(dateTimeRequested(double)), m_simDataManager, SLOT(setSimTime(double)));
    connect(m_playbackControls, SIGNAL(playSpeedRequested(double)), m_simDataManager, SLOT(setPlaySpeed(double)));
    connect(m_playbackControls, SIGNAL (estPlaySpeedUpdated(double)), m_simTimeInfo, SLOT(setEstPlaySpeed(double)));

    connect(ui->actionPlayback_Controls, SIGNAL(triggered(bool)), this, SLOT(setPlaybackControlsVisible(bool)));
}

void MainWindow::createToggleablesWidget()
{
    toggleableObjectMap toggleableObjects = m_simDataManager->getToggleableObjects();
    QSignalMapper *signalMapper = new QSignalMapper(ui->toggleablesWidget);
    for(toggleableObjectMap::iterator i = toggleableObjects.begin(); i != toggleableObjects.end(); ++i) {
        QCheckBox *checkBox = new QCheckBox;
        checkBox->setObjectName(i.key());
        checkBox->setText(i.key());
        checkBox->setChecked(i.value());
        connect(checkBox, SIGNAL(toggled(bool)), signalMapper, SLOT(map()));
        signalMapper->setMapping(checkBox, checkBox);
        ui->toggleablesLayout->addWidget(checkBox);
        toggleablesCheckBoxList.append(checkBox);
    }
    ui->toggleablesLayout->addStretch();

    connect(signalMapper, SIGNAL(mapped(QWidget *)), this, SLOT(updateToggleables(QWidget *)));
}

void MainWindow::saveSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "AVSLab", "ADCSSimulationTool");

    settings.beginGroup("mainwindow");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.setValue("isMaximized", isMaximized());
    if(!isMaximized()) {
        settings.setValue("pos", pos());
        settings.setValue("size", size());
    }
    settings.setValue("windowStyle", m_styleActionGroup->checkedAction()->objectName());
    settings.endGroup();

    settings.beginGroup("viewtoolbar");
    settings.setValue("cameraMode", m_cameraModeCombo->currentIndex());
    settings.setValue("zoomSpeed", m_zoomSpeed->value());
    settings.setValue("fpvTranslate", m_fpvTranslate->value());
    settings.setValue("fpvRotate", m_fpvRotate->value());
    settings.endGroup();

    settings.beginGroup("camera");
    settings.setValue("wireframe", ui->actionWireframe->isChecked());
    settings.setValue("cameraTarget", ui->actionCamera_Target->isChecked());
    settings.endGroup();
    
    m_modelsDisplayInfo->saveSettings(&settings);
    m_orbitElementsInfo->saveSettings(&settings);
    m_reactionWheelsInfo->saveSettings(&settings);
    m_cssInfo->saveSettings(&settings);
    m_torqueRodInfo->saveSettings(&settings);
    m_thrustersInfo->saveSettings(&settings);
    m_totalPowerInfo->saveSettings(&settings);
    m_angularRatesInfo->saveSettings(&settings);
    m_simTimeInfo->saveSettings(&settings);
    m_playbackControls->saveSettings(&settings);
    m_podInfo->saveSettings(&settings);
    m_iruInfo->saveSettings(&settings);
    m_tamInfo->saveSettings(&settings);
    m_starTrackerInfo->saveSettings(&settings);
    
    settings.beginGroup("view");
    settings.setValue("togWidget", ui->actionScene_Options->isChecked());
    settings.setValue("shortcutsWidget", ui->actionShortcuts->isChecked());
    settings.setValue("statusWidget", ui->actionStatus_Bar->isChecked());
    settings.setValue("toolbarWidget",ui->actionViewToolbar->isChecked());
    settings.setValue("playbackWidget",ui->actionPlayback_Controls->isChecked());
    settings.endGroup();
    
    settings.beginGroup("infoPanels");
    settings.setValue("simInfoDisplay",ui->actionSimInfo->isChecked());
    settings.setValue("simTime",ui->actionSimTime->isChecked());
    settings.setValue("thrusters",ui->actionThrusters->isChecked());
    settings.setValue("TR",ui->actionTR->isChecked());
    settings.setValue("CSS",ui->actionCSS->isChecked());
    settings.setValue("RW",ui->actionRW->isChecked());
    settings.setValue("OE",ui->actionOE->isChecked());
    settings.setValue("ctrl",ui->actionCtrl->isChecked());
    settings.setValue("rates",ui->actionRates->isChecked());
    settings.setValue("power", ui->actionTotalPower->isChecked());
    settings.setValue("POD", ui->actionPOD->isChecked());
    settings.setValue("IRU", ui->actionIRU->isChecked());
    settings.setValue("TAM", ui->actionTAM->isChecked());
    settings.setValue("ST", ui->actionST->isChecked());
    settings.endGroup();
    

    settings.beginGroup("Toggleable Objects");
    toggleableObjectMap toggleableObjects = m_simDataManager->getToggleableObjects();
    for(toggleableObjectMap::iterator iter = toggleableObjects.begin(); iter != toggleableObjects.end(); ++iter) {
        settings.setValue(iter.key(), iter.value());
    }
    settings.endGroup();

    settings.setValue("playbackControlsVisible", ui->actionPlayback_Controls->isChecked());
}

void MainWindow::loadSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "AVSLab", "ADCSSimulationTool");

    settings.beginGroup("mainwindow");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    move(settings.value("pos", pos()).toPoint());
    resize(settings.value("size", size()).toSize());
    if(settings.value("maximized", isMaximized()).toBool()) {
        showMaximized();
    }
    QString currentStyle = settings.value("windowStyle").toString();
    if(!currentStyle.isEmpty()) {
        QAction *action = this->findChild<QAction *>(currentStyle);
        if(action) {
            action->setChecked(true);
            setWindowStyle(action);
        }
    }
    settings.endGroup();

    settings.beginGroup("viewtoolbar");
    m_cameraModeCombo->setCurrentIndex(settings.value("cameraMode").toInt());
    m_zoomSpeed->setValue(settings.value("zoomSpeed").toDouble());
    m_fpvTranslate->setValue(settings.value("fpvTranslate").toDouble());
    m_fpvRotate->setValue(settings.value("fpvRotate").toDouble());
    settings.endGroup();

    settings.beginGroup("camera");
    ui->actionWireframe->setChecked(settings.value("wireframe").toBool());
    ui->actionCamera_Target->setChecked(settings.value("cameraTarget").toBool());
    settings.endGroup();
    
    
    settings.beginGroup("infoPanels");
    ui->actionSimInfo->setChecked(settings.value("simInfoDisplay").toBool());
    ui->actionSimTime->setChecked(settings.value("simTime").toBool());
    ui->actionThrusters->setChecked(settings.value("thrusters").toBool());
    ui->actionTR->setChecked(settings.value("TR").toBool());
    ui->actionCSS->setChecked(settings.value("CSS").toBool());
    ui->actionCSS->setChecked(settings.value("CSS").toBool());
    ui->actionRW->setChecked(settings.value("RW").toBool());
    ui->actionOE->setChecked(settings.value("OE").toBool());
    ui->actionCtrl->setChecked(settings.value("ctrl").toBool());
    ui->actionRates->setChecked(settings.value("rates").toBool());
    ui->actionTotalPower->setChecked(settings.value("power").toBool());
    ui->actionPOD->setChecked(settings.value("POD").toBool());
    ui->actionIRU->setChecked(settings.value("IRU").toBool());
    ui->actionTAM->setChecked(settings.value("TAM").toBool());
    ui->actionST->setChecked(settings.value("ST").toBool());
    settings.endGroup();
    
    settings.beginGroup("view");
    ui->actionScene_Options->setChecked(settings.value("togWidget").toBool());
    ui->actionShortcuts->setChecked(settings.value("shortcutsWidget").toBool());
    ui->actionStatus_Bar->setChecked(settings.value("statusWidget").toBool());
    ui->actionViewToolbar->setChecked(settings.value("toolbarWidget").toBool());
    ui->actionPlayback_Controls->setChecked(settings.value("playbackWidget").toBool());
    settings.endGroup();

    m_modelsDisplayInfo->loadSettings(&settings);
    m_orbitElementsInfo->loadSettings(&settings);
    m_reactionWheelsInfo->loadSettings(&settings);
    m_cssInfo->loadSettings(&settings);
    m_torqueRodInfo->loadSettings(&settings);
    m_thrustersInfo->loadSettings(&settings);
    m_totalPowerInfo->loadSettings(&settings);
    m_angularRatesInfo->loadSettings(&settings);
    m_simTimeInfo->loadSettings(&settings);
    m_playbackControls->loadSettings(&settings);
    m_podInfo->loadSettings(&settings);
    m_iruInfo->loadSettings(&settings);
    m_tamInfo->loadSettings(&settings);
    m_starTrackerInfo->loadSettings(&settings);

    settings.beginGroup("Toggleable Objects");
    QStringList childKeys = settings.childKeys();
    for(int i = 0; i < childKeys.count(); i++) {
        m_simDataManager->setToggleableObjectStatus(childKeys.at(i), settings.value(childKeys.at(i)).toBool());
        QCheckBox *checkBox = ui->toggleablesWidget->findChild<QCheckBox*>(childKeys.at(i));
        if(checkBox) {
            checkBox->setChecked(settings.value(childKeys.at(i)).toBool());
        }
    }
    settings.endGroup();

    setPlaybackControlsVisible(settings.value("playbackControlsVisible").toBool());
    
}

void MainWindow::about()
{
    QString translatedTextAboutCaption;
//    translatedTextAboutCaption = QMessageBox::tr(
//                                     "<h3>ADCS Simulation Tool</h3>"
//                                     "<p>SVN Revision: %1</p>"
//                                     "<p>SVN Status: %2</p>"
//                                     "<p>Built on %3 at %4</p>"
//                                 ).arg(QLatin1String(SVNVERSION),
//                                       QLatin1String(SVNSTATUS),
//                                       QLatin1String(__DATE__),
//                                       QLatin1String(__TIME__));
    QString translatedTextAboutText;
    translatedTextAboutText = QMessageBox::tr(
                                  "<p>The program is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.</p>");

    QMessageBox *about = new QMessageBox(this);
    about->setAttribute(Qt::WA_DeleteOnClose);
    about->setWindowTitle(tr("About ADCS Simulation Tool"));
    about->setText(translatedTextAboutCaption);
    about->setInformativeText(translatedTextAboutText);
    QPixmap logo(":/resources/images/AVSlogo5-Large.png");
    about->setIconPixmap(logo.scaledToHeight(100, Qt::SmoothTransformation));
    about->show();
    about->exec();
}

void MainWindow::updateViewToolbarStates()
{
    for(int i = 0; i < m_zoomActions.size(); i++) {
        m_zoomActions.at(i)->setVisible(m_cameraModeCombo->currentIndex() != 2);
    }
    for(int i = 0; i < m_panActions.size(); i++) {
        m_panActions.at(i)->setVisible(m_cameraModeCombo->currentIndex() == 1);
    }
    for(int i = 0; i < m_fpvActions.size(); i++) {
        m_fpvActions.at(i)->setVisible(m_cameraModeCombo->currentIndex() == 2);
    }
}

void MainWindow::updateShortcutsStates()
{
    QVector<shortcutPair> sceneShortcuts = m_sceneWidget->getShortcuts();
    int tot=ui->shortcutsTable->rowCount();
    if(m_cameraModeCombo->currentIndex() !=2){
        for (int i=tot; i>=(tot-sceneShortcuts.size());i--){
            ui->shortcutsTable->hideRow(i);
        }
    }
    else {
        for (int i=(tot-sceneShortcuts.size()); i<=tot; i++){
            ui->shortcutsTable->showRow(i);
        }
    }
}

void MainWindow::updateToggleables(QWidget *widget)
{
    QCheckBox *checkBox = (QCheckBox *)widget;
    m_simDataManager->setToggleableObjectStatus(checkBox->text(), checkBox->isChecked());
}

void MainWindow::setPlaybackControlsVisible(bool value)
{
    if(value) {
        m_playbackControls->setFixedHeight(QWIDGETSIZE_MAX);
    } else {
        m_playbackControls->setFixedHeight(0);
    }
}