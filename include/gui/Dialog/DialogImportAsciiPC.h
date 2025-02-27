#ifndef IMPORT_ASCII_PC_H_
#define IMPORT_ASCII_PC_H_

#include <QtWidgets/qdialog.h>
#include "ui_DialogImportAsciiPC.h"
#include "io/imports/ImportTypes.h"

#include <vector>
#include <filesystem>

class QComboBox;

class DialogImportAsciiPC : public QDialog
{
	Q_OBJECT

public:
	explicit DialogImportAsciiPC(QWidget* parent = 0);
	~DialogImportAsciiPC();

	bool setInfoAsciiPC(std::vector<std::filesystem::path> filePaths);

	bool isFinished();
	std::map<std::filesystem::path, Import::AsciiInfo> getFileImportInfo();

	void onOk(bool all);
	void changeSeparator(char sep);
	void onCancel();
private:
	bool updateTable();
	char getSeparator();

	QComboBox* createChoiceBox(QWidget* parent, int index = 0);
	void updateComboBox(int colInd);

private:
	Ui::ImportAsciiPC m_ui;

	std::vector<std::pair<std::filesystem::path, Import::AsciiInfo>> m_filesValuesRoles;

	int m_fileIndex;
	bool m_isFinished;
};

#endif // !DELETE_TYPE_DIALOG_H_