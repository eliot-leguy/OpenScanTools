#include "gui/Dialog/DialogRecentProjects.h"

#include "controller/controls/ControlProject.h"
#include "gui/GuiData/GuiDataMessages.h"
#include "gui/GuiData/GuiDataIO.h"

#include "utils/Logger.h"

#include <QtCore/qdir.h>

DialogRecentProjects::DialogRecentProjects(IDataDispatcher& dataDispatcher, QWidget *parent)
	: ADialog(dataDispatcher, parent)
{
	m_ui.setupUi(this);
	this->show();
	setModal(true);
	
	GUI_LOG << "create DialogRecentProjects" << LOGENDL;

	m_dataDispatcher.registerObserverOnKey(this, guiDType::sendRecentProjects);

	this->setAttribute(Qt::WA_DeleteOnClose);

	QObject::connect(m_ui.loadButton, &QPushButton::released, this, [this]() {projectSelect(m_ui.projectsList->currentIndex()); });
	QObject::connect(m_ui.cancelButton, &QPushButton::released, this, &DialogRecentProjects::FinishDialog);
}

DialogRecentProjects::~DialogRecentProjects()
{
	GUI_LOG << "destroy DialogRecentProjects " << LOGENDL;
	m_dataDispatcher.unregisterObserver(this);
}

void DialogRecentProjects::informData(IGuiData *data)
{
	if (data->getType() == guiDType::sendRecentProjects)
		receiveProjectList(data);
}

void DialogRecentProjects::receiveProjectList(IGuiData *data)
{
	GuiDataSendRecentProjects *projectsData = static_cast<GuiDataSendRecentProjects*>(data);

	for (auto pair : projectsData->m_recentProjects)
	{
		m_pathsToProjects.push_back(pair.first);
		QListWidgetItem* item = new QListWidgetItem();
		item->setText(QString::fromStdWString(pair.first.stem().wstring()));
		item->setToolTip(QDir::toNativeSeparators(QString::fromStdWString(pair.first.wstring())));
		m_ui.projectsList->addItem(item);
	}

	QObject::connect(m_ui.projectsList, &QListWidget::doubleClicked, this, &DialogRecentProjects::projectSelect);
}

void DialogRecentProjects::projectSelect(const QModelIndex& selected)
{
	if (!selected.isValid())
	{
		m_dataDispatcher.updateInformation(new GuiDataWarning(tr("You must select a project.")));
		return;
	}
	m_dataDispatcher.sendControl(new control::project::SaveCloseLoad(m_pathsToProjects[selected.row()]));
	close();
}

void DialogRecentProjects::FinishDialog()
{
	close();
}