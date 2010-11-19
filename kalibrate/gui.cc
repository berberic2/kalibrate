/*
 * Copyright (C) 2010 M G Berberich
 */

#include <iostream>
#include <set>
#include <sstream>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QListView>
#include <QAbstractListModel>
#include <QAbstractItemDelegate>
#include <QComboBox>
#include <QLabel>
#include <QGroupBox>

#include <KPushButton>
#include <KTabWidget>
// #include <KActionCollection>
#include <KMessageBox>
#include <KLocale>
// #include <KMainWindow>
#include <KMenuBar>
#include <KMenu>
// #include <KStandardAction>
// #include <KAction>
// #include <KToggleAction>
// #include <KHelpMenu>
// #include <KStandardDirs>

#include <KFileDialog>

#include <opencv/cv.h>

#include "imagelist.h"
#include "imageviewer.h"

#include "gui.moc"

/** Plugin-System */
#include "kalib_plugin.h"

/**
 * the filepattern / mime-type for module-files
 */
static const char file_mime_type[] = "*.icmodule";
// TODO: define a mim-type "application/x-kalibrate"
static const char image_mime_type[] = "image/png image/jpeg image/gif "
  "image/x-portable-anymap image/tiff";

static struct {
  KStandardAction::StandardAction actionType;
  const char *name;
  const char *slot;
} standard_actions[] = {
  { KStandardAction::Open,      "open",    SLOT(load()) },
  { KStandardAction::Save,      "save",    SLOT(save()) },
  { KStandardAction::SaveAs,    "save",    SLOT(save()) },
  { KStandardAction::Quit,      "quit",    SLOT(close()) },
};

static struct {
  const char *label;
  const char *name;
  const char *slot;
} normal_actions[] = {
  { I18N_NOOP("Last Hour"),        "lastHour",   SLOT(last_hour()) },
};

/**
 * Constructs a Kalibrate-Gui
 *
 * @param parent parent-widget see KMainWindow
 */
KalibrateGui::KalibrateGui(QWidget *parent)
  : KMainWindow(parent), action_collection(parent),
    extractorGui(0), optimizerGui(0)
{
  // standard_actions
  for (size_t i=0; i< sizeof(standard_actions)/sizeof(*standard_actions); ++i)
    actionCollection()->addAction(standard_actions[i].actionType,
	  standard_actions[i].name, this, standard_actions[i].slot);
  // TODO:
#if 0
  // normal actions
  for (size_t i=0; i< sizeof(normal_actions)/sizeof(*normal_actions); ++i) {
    KAction *act = new KAction(i18n(normal_actions[i].label), this);
    connect(act, SIGNAL(triggered()), this, normal_actions[i].slot);
    actionCollection()->addAction(normal_actions[i].name, act);
  }
  // toggle_actions
  auto_action = new KAction(KIcon("chronometer"), i18n("Automatic Update"), this);
  auto_action->setCheckable(true);
  auto_action->setShortcut(KShortcut("f8"));
  actionCollection()->addAction("autoUpdate", auto_action);
  connect(auto_action, SIGNAL(toggled(bool)), this, SLOT(autoUpdate(bool)));

  panel_action = new KAction(i18n("Hide Datasource Tree"), this);
  panel_action->setCheckable(true);
  panel_action->setShortcut(KShortcut("f9"));
  actionCollection()->addAction("hideTree", panel_action);
  connect(panel_action, SIGNAL(toggled(bool)), this, SLOT(hideTree(bool)));
#endif

  // Widgets
  KTabWidget *tabs = new KTabWidget(this);
  tabs->addTab(settingsWidget(), i18n("Settings"));

  // build viewer-widgets
  QSplitter *hsplitter = new QSplitter(this);
  QWidget *vboxw = new QWidget(this);
  QVBoxLayout *vbox = new QVBoxLayout(vboxw);
  theImageList = new ImageListView(&images, this);
  vbox->addWidget(theImageList);
  hsplitter->addWidget(vboxw);
  theImageViewer = new ImageView(hsplitter);
  hsplitter->addWidget(theImageViewer);
  tabs->addTab(hsplitter, "Image View");

  setCentralWidget(tabs);

  // signals
  connect(theImageList, SIGNAL(clicked(const QModelIndex &)),
  	SLOT(imageSelected(const QModelIndex &)));

  // Menu
  KMenu *fileMenu = new KMenu(i18n("&File"));
  menuBar()->addMenu(fileMenu);
  fileMenu->addAction(actionCollection()->action("open"));
  fileMenu->addAction(actionCollection()->action("save"));
  fileMenu->addSeparator();
  fileMenu->addAction(actionCollection()->action("quit"));

  menuBar()->addMenu(helpMenu());
}

KalibrateGui::~KalibrateGui()
{
}

QWidget *KalibrateGui::settingsWidget()
{
  QWidget *settings = new QWidget();
  settingsVBox = new QVBoxLayout(settings);

  // Loading

  QGroupBox *group = new QGroupBox(i18n("Aquire Images."), this);
  QVBoxLayout *groupVBox = new QVBoxLayout(group);
  QComboBox *loaderSelect = new QComboBox(group);
  loaderSelect->addItem("From File");
  groupVBox->addWidget(loaderSelect);
  /**/
  QWidget *loaderGUI = new QWidget(group);
  QHBoxLayout *hbox1 = new QHBoxLayout(loaderGUI);
  KPushButton *bt_load = new KPushButton("Load");
  hbox1->addWidget(bt_load);
  KPushButton *bt_add = new KPushButton("Add");
  hbox1->addWidget(bt_add);
  // signals
  connect(bt_load, SIGNAL(clicked()), SLOT(load_images()));
  /**/
  groupVBox->insertWidget(groupVBox->indexOf(loaderSelect)+1, loaderGUI);
  settingsVBox->addWidget(group);

  // Extract
  group = new QGroupBox(i18n("Extract Points."), this);
  extractorLayout = new QVBoxLayout(group);
  extractorSelector = new QComboBox(group);
  extractorLayout->addWidget(extractorSelector);
  KPushButton *execute = new KPushButton("Find Points", group);
  extractorLayout->addWidget(execute);
  connect(extractorSelector, SIGNAL(currentIndexChanged(int)), 
	SLOT(extractorChanged(int)));
  connect(execute, SIGNAL(clicked()), SLOT(executeExtractor()));
  settingsVBox->addWidget(group);

  // Optimize
  group = new QGroupBox(i18n("Determine Camera-Parameters."), this);
  optimizerLayout = new QVBoxLayout(group);
  optimizerSelector = new QComboBox(group);
  optimizerLayout->addWidget(optimizerSelector);
  execute = new KPushButton("Optimize", group);
  optimizerLayout->addWidget(execute);
  connect(optimizerSelector, SIGNAL(currentIndexChanged(int)), 
	SLOT(optimizerChanged(int)));
  settingsVBox->addWidget(group);

  settingsVBox->addStretch(100);
  return settings;
}


void KalibrateGui::load_images()
{
  QStringList files = KFileDialog::getOpenFileNames(KUrl(),
	image_mime_type, this);
  if (files.isEmpty()) return;
  for(QStringList::iterator i = files.begin(); i != files.end(); ++i) {
    std::cout << qPrintable(*i) << "\n";
    ImageNode node;
    node.set(*i);
    images.push_back(node);
  }
  theImageList->reset();
}

void KalibrateGui::load()
{
  QString file = KFileDialog::getOpenFileName(KUrl(),
	file_mime_type, this);
  if (file.isEmpty()) return;
}

void KalibrateGui::save()
{
  QString file = KFileDialog::getSaveFileName(KUrl(),
	file_mime_type, this);
  if (file.isEmpty()) return;

  QFile out(file);
  if (out.exists()) {
    int answer = KMessageBox::questionYesNo(this,
	  i18n("file ‘%1’ allready exists.\n"
		"Do you want to overwrite it?", file));
    if (answer != KMessageBox::Yes) {
      return;
    }
  }
  out.close();
}

void KalibrateGui::imageSelected(const QModelIndex &index)
{
  const ImageNode *node = index.data(Qt::DisplayRole).value<const ImageNode*>();
  theImageViewer->imageWidget().image(node->image);
  theImageViewer->imageWidget().grid(node->grid);
}

void KalibrateGui::saveProperties(KConfigGroup &conf)
{
#if 0
  conf.writeEntry("hide-navigation", listview_->isHidden());
  conf.writeEntry("auto-update", graph->autoUpdate());
  conf.writeEntry("range", qint64(graph->range()));
  if (!graph->changed() && !filename.isEmpty()) {
    conf.writeEntry("filename", QDir().absoluteFilePath(filename));
    conf.writeEntry("file-is-session", false);
  } else {
    char hostname[100];
    gethostname(hostname, sizeof(hostname));
    QString file = QString("%1session-%2-%3")
      .arg(KGlobal::dirs()->saveLocation("appdata"))
      .arg(hostname)
      .arg(getpid());
    save(file);
    conf.writeEntry("filename", file);
    conf.writeEntry("file-is-session", true);
  }
#endif
}

void KalibrateGui::readProperties(const KConfigGroup &conf)
{
#if 0
  bool nav = conf.readEntry("hide-navigation", false);
  bool aut = conf.readEntry("auto-update", false);
  time_t range = conf.readEntry("range", 24*3600);
  QString file = conf.readEntry("filename", QString());
  bool file_is_session = conf.readEntry("file-is-session", false);
  if (!file.isEmpty()) {
    load(file);
    if (file_is_session) {
      QFile::remove(file);
      graph->changed(true);
    } else {
      filename = file;
    }
  }
  panel_action->setChecked(nav);
  autoUpdate(aut);
  graph->last(range);
#endif
}

/**
 * Add an extractor to the GUI.
 * @param f pointer to a function that returns an Extractor-object.
 */
void KalibrateGui::addExtractor(Extractor *(*f)())
{
  extractors.push_back(f());
  extractorSelector->addItem(f()->getName());
}

void KalibrateGui::extractorChanged(int i)
{
  QWidget *Gui = extractors[i]->getParamGui();
  if (extractorGui) extractorLayout->removeWidget(extractorGui);
  extractorLayout->insertWidget(1, Gui);
  extractorGui = Gui;
}

void KalibrateGui::executeExtractor()
{
  int fails = 0;
  Extractor *cv = extractors[extractorSelector->currentIndex()];
  for(imageIterator i=images.begin(); i!=images.end(); ++i) {
    ImageNode &node = *i;
    node.grid.clear();
    try {
      if((*cv)(node.image, node.grid)){
	node.active = true;
	node.extrinsic = false;
      } else {
	node.active = false;
	node.extrinsic = false;
      }
    }
    catch(...) {
      node.grid.points.clear();
      node.active = false;
      node.extrinsic = false;
      fails++;
    }
  }
  if (fails) {
    KMessageBox::sorry(this,
	  i18n("Could not find a grid or points in %1 of %2 images:", 
		fails, images.size()));
  }
}


/**
 * Add an optimizer to the GUI.
 * @param f pointer to a function that returns an Optimizer-object.
 */
void KalibrateGui::addOptimizer(Optimizer *(*f)())
{
  optimizers.push_back(f());
  optimizerSelector->addItem(f()->getName());
}

void KalibrateGui::optimizerChanged(int i)
{
  QWidget *Gui = optimizers[i]->getParamGui();
  if (optimizerGui) optimizerLayout->removeWidget(optimizerGui);
  optimizerLayout->insertWidget(1, Gui);
  optimizerGui = Gui;
}
