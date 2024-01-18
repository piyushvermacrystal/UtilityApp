#ifndef PARSEUTILPAGE_H
#define PARSEUTILPAGE_H

#include <QWidget>

namespace Ui {
class ParseUtilPage;
}

class ParseUtilPage : public QWidget
{
    Q_OBJECT

public:
    explicit ParseUtilPage(const QString& path, QWidget *parent = nullptr);
    ~ParseUtilPage();

signals:
    void backButtonClicked();
    void closeButtonClicked();

private:
    Ui::ParseUtilPage *ui;
    QString logDirPath;

private slots:
    void parseLogFilesToDB();
    void handleBackButton();
    void handleCloseButton();
};

#endif // PARSEUTILPAGE_H
