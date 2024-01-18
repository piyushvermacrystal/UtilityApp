#ifndef SOURCEPAGE_H
#define SOURCEPAGE_H

#include <QWidget>

namespace Ui {
class SourcePage;
}

class SourcePage : public QWidget
{
    Q_OBJECT

public:
    explicit SourcePage(QWidget *parent = nullptr);
    ~SourcePage();

private:
    Ui::SourcePage *ui;
    QString logDirPath;

private slots:
    void setLogDirectory();
    void handleJsonUtilCalls();
    void handleParseUtilCalls();
};

#endif // SOURCEPAGE_H
