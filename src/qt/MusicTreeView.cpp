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
                if(QMessageBox::warning(this,"","Are you sure you want to delete this?", QMessageBox::Yes|QMessageBox::No, QMessageBox::No)==QMessageBox::Yes) {
                    if(selection->type()==MusicItemType::MIT_TRACK)
						model->removeRow(selectedindices[0].row());
					else
                        selection->parent()->removeRow(selectedindices[0].row());
				}
			}
		}
	}
}



QStringList MusicTreeViewModel::mimeTypes() const
{
	QStringList types;
	types << MIT_MIME;
	types << "text/uri-list";
	return types;
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
	mime->setData(MIT_MIME, mimebytes);
	return mime;
}

bool MusicTreeViewModel::dropMimeData(const QMimeData *data, Qt::DropAction, int row, int, const QModelIndex &parent)
{
	QStandardItem *parentitem = this->itemFromIndex(parent);
	if(data->hasFormat(MIT_MIME)) {
		QByteArray datamime = data->data(MIT_MIME);
        QDataStream ds(&datamime, QIODevice::ReadOnly);
		quint32 itemcount;
		ds >> itemcount;
        int bytelength = itemcount*sizeof(QStandardItem*);
        QStandardItem **items = (QStandardItem**)malloc(bytelength);
        ds.readRawData((char*)items, bytelength);
        for(quint32 i=0; i<itemcount; i++) {
            QStandardItem *item = items[i];
            switch(item->type()) {
            case MusicItemType::MIT_TRACK:
                if(!parentitem) {
                    if(row>=0)          this->insertRow(row, item->clone());
                    else                this->appendRow(item->clone());
                } else {
                    if(item->row()>=0)  this->insertRow(item->row(), item->clone());
                    else                this->appendRow(item->clone());
                }
				break;
            case MusicItemType::MIT_CLIPGROUP:
				if(parentitem && parentitem->type()==MusicItemType::MIT_TRACK) {
                    if(row>=0)          parentitem->insertRow(row, item->clone());
                    else                parentitem->appendRow(item->clone());
                } else {
                    if(item->row()>=0)  item->parent()->insertRow(item->row(), item->clone());
                    else                item->parent()->appendRow(item->clone());
                }
                break;
			case MusicItemType::MIT_CLIP:
				if(parentitem && parentitem->type()!=MusicItemType::MIT_CLIP) {
					if(row>=0)          parentitem->insertRow(row, item->clone());
					else                parentitem->appendRow(item->clone());
				} else {
					if(item->row()>=0)  item->parent()->insertRow(item->row(), item->clone());
					else                item->parent()->appendRow(item->clone());
				}
				break;
            }

		}
		free(items);
        return true;
	}
	if(data->hasFormat("text/uri-list")) {
		// First, make sure the files were dropped into a valid container for clips
		QStandardItem *parentitem = NULL;
		if(parent.isValid())
			parentitem = this->parseMusicItemMimeData(this->itemFromIndex(parent));
		else
			qDebug() << QString("Can't drop onto root item; ignoring drop.");
		if(parentitem==NULL)
			return false;

		// Next, make sure the dropped files are valid audio data, and ignore any that aren't
		QStringList filepaths = QString(data->data("text/uri-list")).split(QRegExp("(\\r\\n?|\\n)+"));
		QStringList validaudiofiles;
		QMediaPlayer *mediatest = new QMediaPlayer();
		foreach(QString fileurl, filepaths) {
			if(fileurl.isEmpty())
				continue;
			mediatest->setMedia(QUrl(fileurl));
			if(mediatest->error()!=QMediaPlayer::NoError) {
				qDebug() << QString("Error loading media file '%1': %2").arg(fileurl).arg(mediatest->errorString());
				continue;
			}
			validaudiofiles << fileurl;
		}
		mediatest->deleteLater();
		emit(audioClipsDropped(this->indexFromItem(parentitem), validaudiofiles));
	}
	return false;
}
QStandardItem *MusicTreeViewModel::parseMusicItemMimeData(QStandardItem *item)
{
	if(!item) {
		qDebug() << QString("Can't drop onto root item; ignoring drop.");
		return NULL;
	}
	switch(item->type()) {
	case MIT_TRACK:
	case MIT_CLIPGROUP:
		return item;
	case MIT_CLIP:
		if(item->parent())
			return this->parseMusicItemMimeData(item->parent());
		else
			qDebug() << QString("Could not find valid parent; ignoring drop.");
		break;
	}
	return NULL;
}
