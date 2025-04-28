#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qfiledialog.h"
#include "qstring.h"
#include "pgm.hpp"
#include "ppm.hpp"

QString extencao, log_exec;
ppm ppm_src, ppm_dst, ppm_orig;
pgm pgm_src, pgm_dst, pgm_orig, r, g, b;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_carregarImg_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Carregar imagem"), "../imagens/", tr("Imagens (*.ppm *.pgm)"));
    if (fileName.size() == 0) return;
    QString command = "start ./visualizar.exe ";
    command.append(fileName);
    ui->lineEdit->setText(fileName);
    extencao = fileName.last(3);
    QString img_info;
    if (extencao == "ppm" || extencao == "PPM") {
        carregarImagem(ppm_src, fileName.toStdString().c_str());
        clonarCabecalho(ppm_src, ppm_orig);
        clonarDados(ppm_src, ppm_orig);
        img_info.append("PPM\nTipo: ");
        img_info.append(QString::fromStdString(ppm_src.type));
        img_info.append("\nResolução: ");
        img_info.append(QString::number(ppm_src.w));
        img_info.append(" x ");
        img_info.append(QString::number(ppm_src.h));
        img_info.append("\nBits: ");
        img_info.append(QString::number(ppm_src.bits));
        log_exec.append("Carregado imagem PPM\n");
    }
    if (extencao == "pgm" || extencao == "PGM") {
        carregarImagem(pgm_src, fileName.toStdString().c_str());
        clonarCabecalho(pgm_src, pgm_orig);
        clonarDados(pgm_src, pgm_orig);
        img_info.append("PGM\nTipo: ");
        img_info.append(QString::fromStdString(pgm_src.type));
        img_info.append("\nResolução: ");
        img_info.append(QString::number(pgm_src.w));
        img_info.append(" x ");
        img_info.append(QString::number(pgm_src.h));
        img_info.append("\nBits: ");
        img_info.append(QString::number(pgm_src.bits));
        log_exec.append("Carregado imagem PGM\n");
    }
    ui->imgInfo->setText(img_info);
    ui->log_operacoes->setText(log_exec);
    system(command.toStdString().c_str());
}

void MainWindow::on_plotHist_clicked()
{
    if (extencao == "ppm" || extencao == "PPM") {
        plotHistograma(ppm_src, ppm_dst);
        salvarImagem(ppm_dst, "./temp/temp.tmp");
    }
    if (extencao == "pgm" || extencao == "PGM") {
        plotHistograma(pgm_src, pgm_dst);
        salvarImagem(pgm_dst, "./temp/temp.tmp");
    }
    log_exec.append("Executado plot de histograma\n");
    ui->log_operacoes->setText(log_exec);
    system("start visualizar.exe ./temp/temp.tmp");
}

void MainWindow::on_eqHist_clicked()
{
    if (extencao == "ppm" || extencao == "PPM") {
        equalizarHistograma(ppm_src, ppm_dst);
        salvarImagem(ppm_dst, "./temp/temp.tmp");
    }
    if (extencao == "pgm" || extencao == "PGM") {
        equalizarHistograma(pgm_src, pgm_dst);
        salvarImagem(pgm_dst, "./temp/temp.tmp");
    }
    log_exec.append("Executado equalização de histograma\n");
    ui->log_operacoes->setText(log_exec);
    system("start visualizar.exe ./temp/temp.tmp");
}

void MainWindow::on_separarCanais_clicked()
{
    if (extencao == "ppm" || extencao == "PPM") {
        separarCanais(ppm_src, r, g, b);
        salvarImagem(r, "/Windows/Temp/temp_r.tmp");
        salvarImagem(g, "/Windows/Temp/temp_g.tmp");
        salvarImagem(b, "/Windows/Temp/temp_b.tmp");
        log_exec.append("Separado canais RGB\n");
        ui->log_operacoes->setText(log_exec);
        system("start visualizar.exe /Windows/Temp/temp_r.tmp");
        system("start visualizar.exe /Windows/Temp/temp_g.tmp");
        system("start visualizar.exe /Windows/Temp/temp_b.tmp");
    }
}

void MainWindow::on_media_clicked()
{
    int range = ui->spinBoxMedia->value();
    if (extencao == "ppm" || extencao == "PPM") {
        media(ppm_src, ppm_dst, range);
        salvarImagem(ppm_dst, "./temp/temp.tmp");
    }
    if (extencao == "pgm" || extencao == "PGM") {
        media(pgm_src, pgm_dst, range);
        salvarImagem(pgm_dst, "./temp/temp.tmp");
    }
    log_exec.append("Executado operação de media\n");
    ui->log_operacoes->setText(log_exec);
    system("start visualizar.exe ./temp/temp.tmp");
}

void MainWindow::on_mediana_clicked()
{
    int range = ui->spinBoxMediana->value();
    if (extencao == "ppm" || extencao == "PPM") {
        medianargb(ppm_src, ppm_dst, range);
        salvarImagem(ppm_dst, "./temp/temp.tmp");
    }
    if (extencao == "pgm" || extencao == "PGM") {
        mediana(pgm_src, pgm_dst, range);
        salvarImagem(pgm_dst, "./temp/temp.tmp");
    }
    log_exec.append("Executado operação de mediana\n");
                    ui->log_operacoes->setText(log_exec);
    system("start visualizar.exe ./temp/temp.tmp");
}

void MainWindow::on_laplaciano_clicked()
{
    if (extencao == "ppm" || extencao == "PPM") {
        laplacianorgb(ppm_src, ppm_dst);
        salvarImagem(ppm_dst, "./temp/temp.tmp");
    }
    if (extencao == "pgm" || extencao == "PGM") {
        laplaciano(pgm_src, pgm_dst);
        salvarImagem(pgm_dst, "./temp/temp.tmp");
    }
    log_exec.append("Executado operação de laplaciano\n");
    ui->log_operacoes->setText(log_exec);
    system("start visualizar.exe ./temp/temp.tmp");
}

void MainWindow::on_highBoost_clicked()
{
    float k = ui->spinBoxHighBoost->value();
    int range = ui->spinBoxMedia->value();
    if (extencao == "ppm" || extencao == "PPM") {
        highboostrgb(ppm_src, ppm_dst, range, k);
        salvarImagem(ppm_dst, "./temp/temp.tmp");
    }
    if (extencao == "pgm" || extencao == "PGM") {
        highBoost(pgm_src, pgm_dst, range, k);
        salvarImagem(pgm_dst, "./temp/temp.tmp");
    }
    log_exec.append("Executado operação de high boost\n");
    ui->log_operacoes->setText(log_exec);
    system("start visualizar.exe ./temp/temp.tmp");
}

void MainWindow::on_mesclar_clicked()
{
    separarCanais(ppm_src, r, g, b);
    if (ui->rComboBox->currentIndex() == 0) {
        if (ui->gComboBox->currentIndex() == 0) {
            if (ui->bComboBox->currentIndex() == 0) {
                juntarCanais(ppm_dst, r, r, r);
            }
            else if (ui->bComboBox->currentIndex() == 1) {
                juntarCanais(ppm_dst, r, r, g);
            }
            else if (ui->bComboBox->currentIndex() == 2) {
                juntarCanais(ppm_dst, r, r, b);
            }
        }
        else if (ui->gComboBox->currentIndex() == 1) {
            if (ui->bComboBox->currentIndex() == 0) {
                juntarCanais(ppm_dst, r, g, r);
            }
            else if (ui->bComboBox->currentIndex() == 1) {
                juntarCanais(ppm_dst, r, g, g);
            }
            else if (ui->bComboBox->currentIndex() == 2) {
                juntarCanais(ppm_dst, r, g, b);
            }
        }
        else if (ui->gComboBox->currentIndex() == 2) {
            if (ui->bComboBox->currentIndex() == 0) {
                juntarCanais(ppm_dst, r, b, r);
            }
            else if (ui->bComboBox->currentIndex() == 1) {
                juntarCanais(ppm_dst, r, b, g);
            }
            else if (ui->bComboBox->currentIndex() == 2) {
                juntarCanais(ppm_dst, r, b, b);
            }
        }
    }
    else if (ui->rComboBox->currentIndex() == 1) {
        if (ui->gComboBox->currentIndex() == 0) {
            if (ui->bComboBox->currentIndex() == 0) {
                juntarCanais(ppm_dst, g, r, r);
            }
            else if (ui->bComboBox->currentIndex() == 1) {
                juntarCanais(ppm_dst, g, r, g);
            }
            else if (ui->bComboBox->currentIndex() == 2) {
                juntarCanais(ppm_dst, g, r, b);
            }
        }
        else if (ui->gComboBox->currentIndex() == 1) {
            if (ui->bComboBox->currentIndex() == 0) {
                juntarCanais(ppm_dst, g, g, r);
            }
            else if (ui->bComboBox->currentIndex() == 1) {
                juntarCanais(ppm_dst, g, g, g);
            }
            else if (ui->bComboBox->currentIndex() == 2) {
                juntarCanais(ppm_dst, g, g, b);
            }
        }
        else if (ui->gComboBox->currentIndex() == 2) {
            if (ui->bComboBox->currentIndex() == 0) {
                juntarCanais(ppm_dst, g, b, r);
            }
            else if (ui->bComboBox->currentIndex() == 1) {
                juntarCanais(ppm_dst, g, b, g);
            }
            else if (ui->bComboBox->currentIndex() == 2) {
                juntarCanais(ppm_dst, g, b, b);
            }
        }
    }
    else if (ui->rComboBox->currentIndex() == 2) {
        if (ui->gComboBox->currentIndex() == 0) {
            if (ui->bComboBox->currentIndex() == 0) {
                juntarCanais(ppm_dst, b, r, r);
            }
            else if (ui->bComboBox->currentIndex() == 1) {
                juntarCanais(ppm_dst, b, r, g);
            }
            else if (ui->bComboBox->currentIndex() == 2) {
                juntarCanais(ppm_dst, b, r, b);
            }
        }
        else if (ui->gComboBox->currentIndex() == 1) {
            if (ui->bComboBox->currentIndex() == 0) {
                juntarCanais(ppm_dst, b, g, r);
            }
            else if (ui->bComboBox->currentIndex() == 1) {
                juntarCanais(ppm_dst, b, g, g);
            }
            else if (ui->bComboBox->currentIndex() == 2) {
                juntarCanais(ppm_dst, b, g, b);
            }
        }
        else if (ui->gComboBox->currentIndex() == 2) {
            if (ui->bComboBox->currentIndex() == 0) {
                juntarCanais(ppm_dst, b, b, r);
            }
            else if (ui->bComboBox->currentIndex() == 1) {
                juntarCanais(ppm_dst, b, b, g);
            }
            else if (ui->bComboBox->currentIndex() == 2) {
                juntarCanais(ppm_dst, b, b, b);
            }
        }
    }
    log_exec.append("Mesclado canais RGB\n");
    ui->log_operacoes->setText(log_exec);
    salvarImagem(ppm_dst, "./temp/temp.tmp");
    system("start visualizar.exe ./temp/temp.tmp");
}

void MainWindow::on_usarResult_clicked()
{
    if (extencao == "ppm" || extencao == "PPM") {
        clonarCabecalho(ppm_dst, ppm_src);
        clonarDados(ppm_dst, ppm_src);
    }
    if (extencao == "pgm" || extencao == "PGM") {
        clonarCabecalho(pgm_dst, pgm_src);
        clonarDados(pgm_dst, pgm_src);
    }
    log_exec.append("Usando ultimo resultado como fonte\n");
    ui->log_operacoes->setText(log_exec);
}

void MainWindow::on_restaurar_clicked()
{
    if (extencao == "ppm" || extencao == "PPM") {
        clonarCabecalho(ppm_orig, ppm_src);
        clonarDados(ppm_orig, ppm_src);
    }
    if (extencao == "pgm" || extencao == "PGM") {
        clonarCabecalho(pgm_orig, pgm_src);
        clonarDados(pgm_orig, pgm_src);
    }
    log_exec.append("Restaurado input para imagem original\n");
    ui->log_operacoes->setText(log_exec);
}


void MainWindow::on_aplicar_clicked()
{
    switch(ui->outras->currentIndex()) {
    case 1:
        if (extencao == "ppm" || extencao == "PPM") {
            inverter(ppm_src, ppm_dst);
            salvarImagem(ppm_dst, "./temp/temp.tmp");
        }
        if (extencao == "pgm" || extencao == "PGM") {
            inverter(pgm_src, pgm_dst);
            salvarImagem(pgm_dst, "./temp/temp.tmp");
        }
        break;
    case 2:
        if (extencao == "ppm" || extencao == "PPM") {
            transposta(ppm_src, ppm_dst);
            salvarImagem(ppm_dst, "./temp/temp.tmp");
        }
        if (extencao == "pgm" || extencao == "PGM") {
            transposta(pgm_src, pgm_dst);
            salvarImagem(pgm_dst, "./temp/temp.tmp");
        }
        break;
    case 3:
        if (extencao == "ppm" || extencao == "PPM") {
            flipVertical(ppm_src, ppm_dst);
            salvarImagem(ppm_dst, "./temp/temp.tmp");
        }
        if (extencao == "pgm" || extencao == "PGM") {
            flipVertical(pgm_src, pgm_dst);
            salvarImagem(pgm_dst, "./temp/temp.tmp");
        }
        break;
    case 4:
        if (extencao == "ppm" || extencao == "PPM") {
            flipHorizontal(ppm_src, ppm_dst);
            salvarImagem(ppm_dst, "./temp/temp.tmp");
        }
        if (extencao == "pgm" || extencao == "PGM") {
            flipHorizontal(pgm_src, pgm_dst);
            salvarImagem(pgm_dst, "./temp/temp.tmp");
        }
        break;
    case 5:
        if (extencao == "ppm" || extencao == "PPM") {
            girar90(ppm_src, ppm_dst);
            salvarImagem(ppm_dst, "./temp/temp.tmp");
        }
        if (extencao == "pgm" || extencao == "PGM") {
            girar90(pgm_src, pgm_dst);
            salvarImagem(pgm_dst, "./temp/temp.tmp");
        }
        break;
    case 6:
        if (extencao == "ppm" || extencao == "PPM") {
            girar180(ppm_src, ppm_dst);
            salvarImagem(ppm_dst, "./temp/temp.tmp");
        }
        if (extencao == "pgm" || extencao == "PGM") {
            girar180(pgm_src, pgm_dst);
            salvarImagem(pgm_dst, "./temp/temp.tmp");
        }
        break;
    case 7:
        if (extencao == "ppm" || extencao == "PPM") {
            girar270(ppm_src, ppm_dst);
            salvarImagem(ppm_dst, "./temp/temp.tmp");
        }
        if (extencao == "pgm" || extencao == "PGM") {
            girar270(pgm_src, pgm_dst);
            salvarImagem(pgm_dst, "./temp/temp.tmp");
        }
        break;
    case 8:
        if (extencao == "ppm" || extencao == "PPM") {
            ampliarrgb(ppm_src, ppm_dst);
            salvarImagem(ppm_dst, "./temp/temp.tmp");
        }
        if (extencao == "pgm" || extencao == "PGM") {
            ampliar(pgm_src, pgm_dst);
            salvarImagem(pgm_dst, "./temp/temp.tmp");
        }
        break;
    case 9:
        if (extencao == "ppm" || extencao == "PPM") {
            reduzirrgb(ppm_src, ppm_dst);
            salvarImagem(ppm_dst, "./temp/temp.tmp");
        }
        if (extencao == "pgm" || extencao == "PGM") {
            reduzir(pgm_src, pgm_dst);
            salvarImagem(pgm_dst, "./temp/temp.tmp");
        }
        break;
    case 10:
        if (extencao == "ppm" || extencao == "PPM") {
            normalizarrgb(ppm_src, ppm_dst);
            salvarImagem(ppm_dst, "./temp/temp.tmp");
        }
        if (extencao == "pgm" || extencao == "PGM") {
            normalizar(pgm_src, pgm_dst);
            salvarImagem(pgm_dst, "./temp/temp.tmp");
        }
        break;
    default:
        break;
    }
    system("start visualizar.exe ./temp/temp.tmp");
}


void MainWindow::on_salvar_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Salvar imagem"), "/");
    fileName.append(".");
    fileName.append(extencao);
    if (extencao == "ppm" || extencao == "PPM") {
        salvarImagem(ppm_dst, fileName.toStdString().c_str());
    }
    if (extencao == "pgm" || extencao == "PGM") {
        salvarImagem(pgm_dst, fileName.toStdString().c_str());
    }
}

