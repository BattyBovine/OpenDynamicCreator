#include "EventTreeView.h"

void EventTreeView::keyPressEvent(QKeyEvent *e)
{
	switch(e->key()) {
	case Qt::Key_Delete:
		this->deleteSelectedItems();
		break;
	}
}

void EventTreeView::deleteSelectedItems()
{
	QModelIndexList selectedindices = this->selectedIndexes();
	if(selectedindices.size()>0) {
		QStandardItemModel *model = qobject_cast<QStandardItemModel*>(this->model());
		if(model) {
			QStandardItem *selection = model->itemFromIndex(selectedindices[0]);
			if(selection) {
                if(QMessageBox::warning(this,"","Are you sure you want to delete this?", QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::Yes) {
					if(selection->type()==EventItemType::EI_EVENT)
                        model->removeRow(selectedindices[0].row());
                    else
                        selection->parent()->removeRow(selectedindices[0].row());
				}
			}
		}
	}
}



QMimeData *EventTreeViewModel::mimeData(const QModelIndexList &indices) const
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
	mime->setData("Qt/StateItemType", mimebytes);
	return mime;
}

bool EventTreeViewModel::dropMimeData(const QMimeData *data, Qt::DropAction, int row, int, const QModelIndex &parent)
{
	QStandardItem *parentitem = this->itemFromIndex(parent);
	if(parentitem && data->hasFormat("Qt/StateItemType")) {
        QByteArray datamime = data->data("Qt/StateItemType");
        QDataStream ds(&datamime, QIODevice::ReadOnly);
		quint32 itemcount;
		ds >> itemcount;
		int bytelength = itemcount*sizeof(QStandardItem*);
		QStandardItem **items = (QStandardItem**)malloc(bytelength);
		ds.readRawData((char*)items, bytelength);
		for(quint32 i=0; i<itemcount; i++) {
			QStandardItem *item = items[i];
			switch(item->type()) {
			case EventItemType::EI_EVENT:
				if(parentitem==this->invisibleRootItem()) {
					if(row>=0)
						parentitem->insertRow(row, item->clone());
					else
						parentitem->appendRow(item->clone());
				}
				break;
			case EventItemType::EI_COMMAND:
				if(parentitem->type()!=EventItemType::EI_EVENT)
					return false;
				if(row>=0)
					parentitem->insertRow(row, item->clone());
				else
					parentitem->appendRow(item->clone());
				break;
			}

		}
		free(items);
		return true;
	}
	return false;
}
