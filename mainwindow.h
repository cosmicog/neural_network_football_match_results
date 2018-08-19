#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextStream>
#include <QFile>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        void on_btn_train_released();

        void on_btn_try_released();

        void on_ledt_iter_textChanged(const QString &arg1);

        void on_btn_plot_released();

        void on_btn_train_2_released();

        void on_ledt_alpha_textChanged(const QString &arg1);

        void on_ledt_lambda_textChanged(const QString &arg1);

        void on_btn_train_3_released();

        void on_ledt_hid_neuron_count_textChanged(const QString &arg1);

        void on_btn_train_4_released();

private:
        Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
