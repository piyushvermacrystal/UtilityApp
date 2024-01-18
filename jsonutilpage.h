#ifndef JSONUTILPAGE_H
#define JSONUTILPAGE_H

#include <QWidget>

namespace Ui {
class JsonUtilPage;
}

class JsonUtilPage : public QWidget
{
    Q_OBJECT

public:
    explicit JsonUtilPage(const QString& path, QWidget *parent = nullptr);
    ~JsonUtilPage();
signals:
    void backButtonClicked();
    void closeButtonClicked();

private:
    Ui::JsonUtilPage *ui;

    QString logDirPath;
    QString csvFilePath;

private slots:
    void setCSVFileName();
    void convertJsonDirToCSV();
    void handleBackButton();
    void handleCloseButton();

};

#endif // JSONUTILPAGE_H
