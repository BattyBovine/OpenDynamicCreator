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
				switch(QMessageBox::warning(this,"","Are you sure you want to delete this?", QMessageBox::Yes|QMessageBox::No, QMessageBox::No)) {
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



QMimeData *MusicTreeViewModel::mimeData(const QModelIndexList &indices) const
{
	QMimeData *mime = QStandardItemModel::mimeData(indices);
	QByteArray mimebytes;
	{
		QDataStream ds(&mimebytes, QIODevice::WriteOnly);
		ds << quint32(indices.size());
		for(QModelIndexList::const_iterator i=indices.begin(); i<indices.end(); i++) {
			QStandardItem *itemptr = this->itemFromIndex(*i);
			ds.writeRawData((const char*)&itemptr, sizeof(QStandardItem*));
		}
	}
	mime->setData("Qt/MusicItemType", mimebytes);
	return mime;
}

bool MusicTreeViewModel::dropMimeData(const QMimeData *data, Qt::DropAction, int row, int, const QModelIndex &parent)
{
	QStandardItem *parentitem = this->itemFromIndex(parent);
	if(parentitem && data->hasFormat("Qt/MusicItemType")) {
		QDataStream ds(&data->data("Qt/MusicItemType"), QIODevice::ReadOnly);
		quint32 itemcount;
		ds >> itemcount;
		int bytelength = itemcount*sizeof(QStandardItem*);
		QStandardItem **items = (QStandardItem**)malloc(bytelength);
		ds.readRawData((char*)items, bytelength);
		for(quint32 i=0; i<itemcount; i++) {
			QStandardItem *item = items[i];
			switch(item->type()) {
			case MusicItemType::MIT_MUSIC:
				if(parentitem==this->invisibleRootItem()) {
					if(row>=0)
						parentitem->insertRow(row, item->clone());
					else
						parentitem->appendRow(item->clone());
				}
				break;
			case MusicItemType::MIT_CLIP:
				if(parentitem->type()==MusicItemType::MIT_CLIP)
					return false;
			case MusicItemType::MIT_CLIPGROUP:
				if(parentitem->type()==MusicItemType::MIT_CLIPGROUP || parentitem->type()==MusicItemType::MIT_MUSIC) {
					if(row>=0)
						parentitem->insertRow(row, item->clone());
					else
						parentitem->appendRow(item->clone());
				}
				break;
			}

		}
		free(items);
		return true;
	}
	return false;
}
