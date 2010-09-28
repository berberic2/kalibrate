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
#include <KPushButton>
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
#include "opencv.h"

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
  : KMainWindow(parent), action_collection(parent)
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

  // build widgets
  QSplitter *hsplitter = new QSplitter(this);
  QWidget *vboxw = new QWidget(this);
  QVBoxLayout *vbox = new QVBoxLayout(vboxw);
  theImageList = new ImageListView(&images, this);
  vbox->addWidget(theImageList);
  QHBoxLayout *hbox2 = new QHBoxLayout();
  KPushButton *bt_load = new KPushButton("Load");
  hbox2->addWidget(bt_load);
  KPushButton *bt_add = new KPushButton("Add");
  hbox2->addWidget(bt_add);
  vbox->addLayout(hbox2);
  hsplitter->addWidget(vboxw);
  theImageViewer = new ImageView(hsplitter);
  hsplitter->addWidget(theImageViewer);
  setCentralWidget(hsplitter);

  // signals
  connect(bt_load, SIGNAL(clicked()), SLOT(load_images()));
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

void KalibrateGui::load_images()
{
  QStringList files = KFileDialog::getOpenFileNames(KUrl(), 
	image_mime_type, this);
  if (files.isEmpty()) return;
  for(QStringList::iterator i = files.begin(); i != files.end(); ++i) {
    std::cout << qPrintable(*i) << "\n";
    ImageNode node;
    node.set(*i);
    extract(node, 10, 10);
    images.push_back(node);
    //    theImageViewer->imageWidget().image(node.image);
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
