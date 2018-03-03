/*
	*
	* PBTrashManager.hpp - PiBreeze FolderView Class Header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBButtons.hpp"
#include "PBTrashView.hpp"
#include "PBTrashModel.hpp"
#include "PBTrashNode.hpp"
#include "PBDialog.hpp"
#include "PBMessageDialog.hpp"
#include "PBGuiWidgets.hpp"

class PBTrashManager : public PBDialog {
	Q_OBJECT

	public :
		PBTrashManager( QWidget *parent = 0 );

		QModelIndexList getSelection();
		bool hasSelection();

	private :
		void setupGui();
		void setDialogProperties();
		void createAndSetupActions();

		void updateButtons();

		PBTrashView *TrashView;
		PBTrashModel *tModel;
		QMenu *customMenu;

		PBSegmentButton *restoreButtons, *deleteButtons;

		QClipboard *ClipBoard;
		QAction *reloadAct;
		QAction *restoreAct, *restoreAllAct, *deleteAct, *emptyAct;

	public slots :
		void doReload();

		void handleFailedRestore( QModelIndexList );

		void restoreSelected();
		void restoreAll();

		void deleteSelected();
		void emptyTrash();
};
