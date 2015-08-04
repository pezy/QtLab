#include <QApplication>
#include <QTableView>
#include <QAbstractListModel>

class StringListModel : public QAbstractListModel
 {
 public:
	 StringListModel(const QStringList &strings, QObject *parent = 0)
		 : QAbstractListModel(parent), stringList(strings) {}

	 int rowCount(const QModelIndex &) const
	 {
		return stringList.count();
	 }
	 QVariant data(const QModelIndex &index, int role) const
	 {
		if (role == Qt::DisplayRole)
			return stringList.at(index.row());
		if (role == Qt::BackgroundColorRole && index.row() == 0)
			return QBrush(Qt::red);
		else
		  return QVariant();
	 }

 private:
	 QStringList stringList;
 };

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTableView tableView;

	QStringList list;
	list << "a" << "b" << "c";
	StringListModel model(list);

	tableView.setModel(&model);
	tableView.show();
	
	return a.exec();
}
