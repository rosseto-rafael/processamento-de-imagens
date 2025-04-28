#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_plotHist_clicked();

    void on_eqHist_clicked();

    void on_separarCanais_clicked();

    void on_media_clicked();

    void on_mediana_clicked();

    void on_carregarImg_clicked();

    void on_laplaciano_clicked();

    void on_highBoost_clicked();

    void on_mesclar_clicked();

    void on_usarResult_clicked();

    void on_restaurar_clicked();

    void on_aplicar_clicked();

    void on_salvar_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
