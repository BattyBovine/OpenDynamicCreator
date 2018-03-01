#include "src/qt/MusicTreeView.h"

void MusicTreeView::keyPressEvent(QKeyEvent *e)
{
	switch(e->key()) {
	case Qt::Key_Delete:
		this->deleteSelectedItems();
		break;
	}
}

void MusicTreeView::deleteSelectedItems()
{
	QModelIndexList selectedindices = this->selectedIndexes();
	if(selectedindices.size()>0) {
		QStandardItemModel *model = qobject_cast<QStandardItemModel*>(this->model());
		if(model) {
			QStandardItem *selection = model->itemFromIndex(selectedindices[0]);
			if(selection) {
				int retval = QMessageBox::warning(this,"","Are you sure you want to delete this?", QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
				switch(retval) {
				case QMessageBox::Yes:
					if(selection->type()==MusicItemType::MIT_MUSIC)
						model->removeRow(selectedindices[0].row());
					else
						selection->parent()->removeRow(selectedindices[0].row());
					break;
				}
			}
		}
	}
}
