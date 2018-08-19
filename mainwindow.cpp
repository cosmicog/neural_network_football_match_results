#include "mainwindow.h"
#include "QMessageBox"
#include "ui_mainwindow.h"
#include "dialog1.h"
#include "neuralnetwork.h"
#include "engine.h"

const char * DATA_FILE_NAME = "data.txt";
const char * TAGS_FILE_NAME = "tags.txt";

float ** INPUTS_ARR;
float ** OUTPUTS_ARR;
QMap<QString, float> IN_TAGS_MAP;
QMap<QString, float> OUT_TAGS_MAP;

QString provideStream(QString file_name);
QString getDatas(QString file_name);
QString getTags(QString file_name);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusLabel->setText(getTags(TAGS_FILE_NAME));
    ui->statusLabel->setText(getDatas(DATA_FILE_NAME));
}

bool areEqualRel(float a, float b, float epsilon = FLT_EPSILON)
{
    return (fabs(a - b) <= epsilon * std::max(fabs(a), fabs(b)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool plotAlphaPerformance(float * d0, float * d1, float * d2, float * d3)
{
    Engine * ep = engOpen (NULL);
    if (ep != NULL)
    {
        qApp->processEvents();
        mxArray *a_array = mxCreateDoubleMatrix(1, ITERATION_COUNT, mxREAL);
        mxArray *b_array = mxCreateDoubleMatrix(1, ITERATION_COUNT, mxREAL);
        mxArray *c_array = mxCreateDoubleMatrix(1, ITERATION_COUNT, mxREAL);
        mxArray *d_array = mxCreateDoubleMatrix(1, ITERATION_COUNT, mxREAL);
        double *pa = mxGetPr(a_array);
        double *pb = mxGetPr(b_array);
        double *pc = mxGetPr(c_array);
        double *pd = mxGetPr(d_array);

        for(int i = 0; i < ITERATION_COUNT; i++)
        {
            pa[i] = (double) d0[i];
            pb[i] = (double) d1[i];
            pc[i] = (double) d2[i];
            pd[i] = (double) d3[i];
        }
        engPutVariable(ep,"A",a_array);
        engPutVariable(ep,"B",b_array);
        engPutVariable(ep,"C",c_array);
        engPutVariable(ep,"D",d_array);
        engEvalString(ep,"perfPlotAlpha");
        //engClose(ep);
    return true;
    }
    else return false;
}

bool plotLambdaPerformance(float * d0, float * d1, float * d2, float * d3)
{
    Engine * ep = engOpen (NULL);
    if (ep != NULL)
    {
        qApp->processEvents();
        mxArray *a_array = mxCreateDoubleMatrix(1, ITERATION_COUNT, mxREAL);
        mxArray *b_array = mxCreateDoubleMatrix(1, ITERATION_COUNT, mxREAL);
        mxArray *c_array = mxCreateDoubleMatrix(1, ITERATION_COUNT, mxREAL);
        mxArray *d_array = mxCreateDoubleMatrix(1, ITERATION_COUNT, mxREAL);
        double *pa = mxGetPr(a_array);
        double *pb = mxGetPr(b_array);
        double *pc = mxGetPr(c_array);
        double *pd = mxGetPr(d_array);

        for(int i = 0; i < ITERATION_COUNT; i++)
        {
            pa[i] = (double) d0[i];
            pb[i] = (double) d1[i];
            pc[i] = (double) d2[i];
            pd[i] = (double) d3[i];
        }
        engPutVariable(ep,"A",a_array);
        engPutVariable(ep,"B",b_array);
        engPutVariable(ep,"C",c_array);
        engPutVariable(ep,"D",d_array);
        engEvalString(ep,"perfPlotLambda");
        //engClose(ep);
    return true;
    }
    else return false;
}

bool plotSpecPerformance(float * d0, float * d1)
{
    Engine * ep = engOpen (NULL);
    if (ep != NULL)
    {
        qApp->processEvents();
        mxArray *a_array = mxCreateDoubleMatrix(1, ITERATION_COUNT, mxREAL);
        mxArray *b_array = mxCreateDoubleMatrix(1, ITERATION_COUNT, mxREAL);
        double *pa = mxGetPr(a_array);
        double *pb = mxGetPr(b_array);

        for(int i = 0; i < ITERATION_COUNT; i++)
        {
            pa[i] = (double) d0[i];
            pb[i] = (double) d1[i];
        }
        engPutVariable(ep,"A",a_array);
        engPutVariable(ep,"B",b_array);
        engEvalString(ep,"perfPlotSpec");
        //engClose(ep);
    return true;
    }
    else return false;
}

bool plotPerformance()
{
    Engine * ep = engOpen (NULL);
    if (ep != NULL)
    {
        qApp->processEvents();
        mxArray *x_array = mxCreateDoubleMatrix(1, ITERATION_COUNT, mxREAL);
        double *px = mxGetPr(x_array);
        for(int i = 0; i < ITERATION_COUNT; i++)
        {
            px[i] = (double) deviations[i];
        }
        engPutVariable(ep,"A",x_array);
        engEvalString(ep,"perfPlot");
        //engClose(ep);
    return true;
    }
    else return false;
}

void MainWindow::on_btn_train_released()
{
    ui->statusLabel->setText("Training started.");
    qApp->processEvents();

    setupNetwork(NOT_RANDOM);
    for (int i = 0; i < ITERATION_COUNT; i++)
    {
        for (int j = 0; j < DATA_COUNT; j++) train(INPUTS_ARR[j], OUTPUTS_ARR[j]);
        for (int j = 0; j < DATA_COUNT; j++) calculateDeviations(INPUTS_ARR[j], OUTPUTS_ARR[j], i, j);

        deviations[i] = .0;
        for (int j = 0; j < DATA_COUNT; j++) deviations[i] += dataDeviations[i][j];
        deviations[i] = deviations[i] / DATA_COUNT;
        ui->progressBar->setValue((i*100)/ITERATION_COUNT);
    }
    ui->progressBar->setValue(100);
    ui->statusLabel->setText("Training successful.");

    QMap<QString, float> tahminlerM;
    QMap<QString, float> gerceklerM;

    for (int j = 0; j < DATA_COUNT; j++)
    {
        char * chr = new char[32];
        sprintf(chr, "%f-%f",INPUTS_ARR[j][0], INPUTS_ARR[j][1]);
        tahminlerM[QString(chr)] = .0;
    }
    for (int j = 0; j < DATA_COUNT; j++)
    {
        char * chr = new char[32];
        calculateOuts(INPUTS_ARR[j]);
        sprintf(chr, "%f-%f",INPUTS_ARR[j][0], INPUTS_ARR[j][1]);
        tahminlerM[QString(chr)] += nOuts[0];
    }

    for (int j = 0; j < DATA_COUNT; j++)
    {
        char * chr = new char[32];
        sprintf(chr, "%f-%f",INPUTS_ARR[j][0], INPUTS_ARR[j][1]);
        gerceklerM[QString(chr)] = .0;
    }

    for (int j = 0; j < DATA_COUNT; j++)
    {
        char * chr = new char[32];
        sprintf(chr, "%f-%f",INPUTS_ARR[j][0], INPUTS_ARR[j][1]);
        gerceklerM[QString(chr)] += OUTPUTS_ARR[j][0];
    }

    foreach( QString key, tahminlerM.keys() )
    {
        tahminlerM[key] = tahminlerM[key]/7;
    }

    foreach( QString key, gerceklerM.keys() )
    {
        gerceklerM[key] = gerceklerM[key]/7;
    }

    printf("DEV_OR = [");
    foreach( QString key, gerceklerM.keys() )
    {
        printf("%f ", gerceklerM.value(key));
    }
    printf(" ];\n");

    printf("DEV_SPEC = [");
    foreach( QString key, tahminlerM.keys() )
    {
        printf("%f ", tahminlerM.value(key));
    }
    printf(" ];\n");
}

void MainWindow::on_btn_try_released()
{
    QStringList words;
    words = ui->ledt_try->text().split(",");
    words[0] = words[0].trimmed();
    words[1] = words[1].trimmed();
    float * trying = new float[2];
    printf("%f<->", IN_TAGS_MAP[words[0]]);
    printf("%f=", IN_TAGS_MAP[words[1]]);
    trying[0] = IN_TAGS_MAP[words[0]];
    trying[1] = IN_TAGS_MAP[words[1]];
    calculateOuts(trying);
    printf("%f\n", nOuts[0]);

    Dialog1 * dia1 = new Dialog1();
    int val = (int)(nOuts[0] * 100);
    dia1->setValue(val);
    dia1->setFirst(IN_TAGS_MAP.key(trying[0]));
    dia1->setSecond(IN_TAGS_MAP.key(trying[1]));
    dia1->exec();
    delete dia1;
}

QString provideStream(QString file_name)
{
    QFile file1(file_name);
    if(!file1.open(QFile::ReadOnly | QFile::Text)) return "X";

    QTextStream qts1(&file1);
    qts1.setCodec("ISO-8859-9");
    QString str = qts1.readAll();
    file1.close();

    if(str.count("  ") > 0 || str.count("   ") > 0 || str.count("    ") > 0) return "X";

    return str;
}

QString getDatas(QString file_name)
{
    QString string_all = provideStream(file_name);
    if (string_all == "X") return (file_name + ": Can't open or corrupted file.");

    QStringList rows = string_all.split('\n');
    int row_count = rows.count()-1; // last row is empty
    DATA_COUNT = row_count;
    QStringList words;

    INPUTS_ARR = new float * [DATA_COUNT];
    OUTPUTS_ARR = new float * [DATA_COUNT];
    for(int i = 0; i < DATA_COUNT; i++)
    {
        INPUTS_ARR[i] = new float [IN_NEURON_COUNT];
        OUTPUTS_ARR[i] = new float [OUT_NEURON_COUNT];
    }

    for(int i = 0; i < row_count; i++)
    {
        words = rows[i].split(",");
        //kelime_sayisi = kelimeler.count();
        for(int j = 0; j < words.count(); j++)
        {
            if(j < IN_NEURON_COUNT)INPUTS_ARR[i][j] = words[j].toFloat();
            else OUTPUTS_ARR[i][j - IN_NEURON_COUNT] = words[j].toFloat();
        }
    }
    return "Getting Data Operation Success.";
}

QString getTags(QString file_name)
{
    QString string_all = provideStream(file_name);
    if (string_all == "X") return (file_name + ": Can't open or corrupted file.");

    QStringList rows = string_all.split('\n');
    int row_count = rows.count()-1; // last row is empty
    QStringList words;

    for(int i = 0; i < row_count; i++)
    {
        words = rows[i].split(",");
        if(i < IN_POP_COUNT)IN_TAGS_MAP [  words[1].toStdString().c_str()  ] = words[0].toFloat();
        else                OUT_TAGS_MAP[  words[1].toStdString().c_str()  ] = words[0].toFloat();
    }
    return "Getting Tags Operation Success.";
}

void MainWindow::on_btn_train_2_released()
{
    ui->statusLabel->setText("Training started.");
    qApp->processEvents();

    ALPHA = .5;
    setupNetwork(NOT_RANDOM);
    for (int i = 0; i < ITERATION_COUNT; i++)
    {
        for (int j = 0; j < DATA_COUNT; j++) train(INPUTS_ARR[j], OUTPUTS_ARR[j]);
        for (int j = 0; j < DATA_COUNT; j++) calculateDeviations(INPUTS_ARR[j], OUTPUTS_ARR[j], i, j);

        deviations[i] = .0;
        for (int j = 0; j < DATA_COUNT; j++) deviations[i] += dataDeviations[i][j];
        deviations[i] = deviations[i] / DATA_COUNT;
        ui->progressBar->setValue((i*100)/ITERATION_COUNT);
    }
    ui->progressBar->setValue(100);
    ui->statusLabel->setText("Training successful.");

    float * d0 = new float [ITERATION_COUNT];
    for(int i = 0; i < ITERATION_COUNT; i++) d0[i] = deviations[i];

    ui->statusLabel->setText("Training started.");
    qApp->processEvents();

    ALPHA = .6;
    setupNetwork(NOT_RANDOM);
    for (int i = 0; i < ITERATION_COUNT; i++)
    {
        for (int j = 0; j < DATA_COUNT; j++) train(INPUTS_ARR[j], OUTPUTS_ARR[j]);
        for (int j = 0; j < DATA_COUNT; j++) calculateDeviations(INPUTS_ARR[j], OUTPUTS_ARR[j], i, j);

        deviations[i] = .0;
        for (int j = 0; j < DATA_COUNT; j++) deviations[i] += dataDeviations[i][j];
        deviations[i] = deviations[i] / DATA_COUNT;
        ui->progressBar->setValue((i*100)/ITERATION_COUNT);
    }
    ui->progressBar->setValue(100);
    ui->statusLabel->setText("Training successful.");

    float * d1 = new float [ITERATION_COUNT];
    for(int i = 0; i < ITERATION_COUNT; i++) d1[i] = deviations[i];

    ui->statusLabel->setText("Training started.");
    qApp->processEvents();

    ALPHA = .7;
    setupNetwork(NOT_RANDOM);
    for (int i = 0; i < ITERATION_COUNT; i++)
    {
        for (int j = 0; j < DATA_COUNT; j++) train(INPUTS_ARR[j], OUTPUTS_ARR[j]);
        for (int j = 0; j < DATA_COUNT; j++) calculateDeviations(INPUTS_ARR[j], OUTPUTS_ARR[j], i, j);

        deviations[i] = .0;
        for (int j = 0; j < DATA_COUNT; j++) deviations[i] += dataDeviations[i][j];
        deviations[i] = deviations[i] / DATA_COUNT;
        ui->progressBar->setValue((i*100)/ITERATION_COUNT);
    }
    ui->progressBar->setValue(100);
    ui->statusLabel->setText("Training successful.");

    float * d2 = new float [ITERATION_COUNT];
    for(int i = 0; i < ITERATION_COUNT; i++) d2[i] = deviations[i];

    ui->statusLabel->setText("Training started.");
    qApp->processEvents();

    ALPHA = .8;
    setupNetwork(NOT_RANDOM);
    for (int i = 0; i < ITERATION_COUNT; i++)
    {
        for (int j = 0; j < DATA_COUNT; j++) train(INPUTS_ARR[j], OUTPUTS_ARR[j]);
        for (int j = 0; j < DATA_COUNT; j++) calculateDeviations(INPUTS_ARR[j], OUTPUTS_ARR[j], i, j);

        deviations[i] = .0;
        for (int j = 0; j < DATA_COUNT; j++) deviations[i] += dataDeviations[i][j];
        deviations[i] = deviations[i] / DATA_COUNT;
        ui->progressBar->setValue((i*100)/ITERATION_COUNT);
    }
    ui->progressBar->setValue(100);
    ui->statusLabel->setText("Training successful.");

    plotAlphaPerformance(d0, d1, d2, deviations);
}


void MainWindow::on_btn_train_4_released()
{
    ui->statusLabel->setText("Training started.");
    qApp->processEvents();

    LAMBDA = .1;
    setupNetwork(NOT_RANDOM);
    for (int i = 0; i < ITERATION_COUNT; i++)
    {
        for (int j = 0; j < DATA_COUNT; j++) train(INPUTS_ARR[j], OUTPUTS_ARR[j]);
        for (int j = 0; j < DATA_COUNT; j++) calculateDeviations(INPUTS_ARR[j], OUTPUTS_ARR[j], i, j);

        deviations[i] = .0;
        for (int j = 0; j < DATA_COUNT; j++) deviations[i] += dataDeviations[i][j];
        deviations[i] = deviations[i] / DATA_COUNT;
        ui->progressBar->setValue((i*100)/ITERATION_COUNT);
    }
    ui->progressBar->setValue(100);
    ui->statusLabel->setText("Training successful.");

    float * d0 = new float [ITERATION_COUNT];
    for(int i = 0; i < ITERATION_COUNT; i++) d0[i] = deviations[i];

    ui->statusLabel->setText("Training started.");
    qApp->processEvents();

    LAMBDA = .4;
    setupNetwork(NOT_RANDOM);
    for (int i = 0; i < ITERATION_COUNT; i++)
    {
        for (int j = 0; j < DATA_COUNT; j++) train(INPUTS_ARR[j], OUTPUTS_ARR[j]);
        for (int j = 0; j < DATA_COUNT; j++) calculateDeviations(INPUTS_ARR[j], OUTPUTS_ARR[j], i, j);

        deviations[i] = .0;
        for (int j = 0; j < DATA_COUNT; j++) deviations[i] += dataDeviations[i][j];
        deviations[i] = deviations[i] / DATA_COUNT;
        ui->progressBar->setValue((i*100)/ITERATION_COUNT);
    }
    ui->progressBar->setValue(100);
    ui->statusLabel->setText("Training successful.");

    float * d1 = new float [ITERATION_COUNT];
    for(int i = 0; i < ITERATION_COUNT; i++) d1[i] = deviations[i];

    ui->statusLabel->setText("Training started.");
    qApp->processEvents();

    LAMBDA = .7;
    setupNetwork(NOT_RANDOM);
    for (int i = 0; i < ITERATION_COUNT; i++)
    {
        for (int j = 0; j < DATA_COUNT; j++) train(INPUTS_ARR[j], OUTPUTS_ARR[j]);
        for (int j = 0; j < DATA_COUNT; j++) calculateDeviations(INPUTS_ARR[j], OUTPUTS_ARR[j], i, j);

        deviations[i] = .0;
        for (int j = 0; j < DATA_COUNT; j++) deviations[i] += dataDeviations[i][j];
        deviations[i] = deviations[i] / DATA_COUNT;
        ui->progressBar->setValue((i*100)/ITERATION_COUNT);
    }
    ui->progressBar->setValue(100);
    ui->statusLabel->setText("Training successful.");

    float * d2 = new float [ITERATION_COUNT];
    for(int i = 0; i < ITERATION_COUNT; i++) d2[i] = deviations[i];

    ui->statusLabel->setText("Training started.");
    qApp->processEvents();

    LAMBDA = 1.0;
    setupNetwork(NOT_RANDOM);
    for (int i = 0; i < ITERATION_COUNT; i++)
    {
        for (int j = 0; j < DATA_COUNT; j++) train(INPUTS_ARR[j], OUTPUTS_ARR[j]);
        for (int j = 0; j < DATA_COUNT; j++) calculateDeviations(INPUTS_ARR[j], OUTPUTS_ARR[j], i, j);

        deviations[i] = .0;
        for (int j = 0; j < DATA_COUNT; j++) deviations[i] += dataDeviations[i][j];
        deviations[i] = deviations[i] / DATA_COUNT;
        ui->progressBar->setValue((i*100)/ITERATION_COUNT);
    }
    ui->progressBar->setValue(100);
    ui->statusLabel->setText("Training successful.");

    plotLambdaPerformance(d0, d1, d2, deviations);
}

void MainWindow::on_btn_train_3_released()
{
    ui->statusLabel->setText("Normal Training started.");
    qApp->processEvents();

    setupNetwork(NOT_RANDOM);
    for (int i = 0; i < ITERATION_COUNT; i++)
    {
        for (int j = 0; j < DATA_COUNT; j++) train(INPUTS_ARR[j], OUTPUTS_ARR[j]);
        for (int j = 0; j < DATA_COUNT; j++) calculateDeviations(INPUTS_ARR[j], OUTPUTS_ARR[j], i, j);

        deviations[i] = .0;
        for (int j = 0; j < DATA_COUNT; j++) deviations[i] += dataDeviations[i][j];
        deviations[i] = deviations[i] / DATA_COUNT;
        ui->progressBar->setValue((i*100)/ITERATION_COUNT);
    }
    ui->progressBar->setValue(100);
    ui->statusLabel->setText("Normal Training successful.");

    float * d0 = new float [ITERATION_COUNT];
    for(int i = 0; i < ITERATION_COUNT; i++) d0[i] = deviations[i];

    ui->statusLabel->setText("Special Training started.");
    qApp->processEvents();

    setupNetwork(NOT_RANDOM);
    for (int i = 0; i < ITERATION_COUNT; i++)
    {
        for (int j = 0; j < DATA_COUNT; j++)
        {
            float f0 = INPUTS_ARR[j][0];
            float f1 = INPUTS_ARR[j][1];
            if (areEqualRel(f0, 0.1f) && areEqualRel(f1, 0.7f)) {continue;}
            if (areEqualRel(f0, 0.7f) && areEqualRel(f1, 0.1f)) {continue;}
            //printf("%f, %f = %f \n", INPUTS_ARR[j][0], INPUTS_ARR[j][1], OUTPUTS_ARR[j][0]);
            train(INPUTS_ARR[j], OUTPUTS_ARR[j]);
        }
        for (int j = 0; j < DATA_COUNT; j++)
        {
            calculateDeviations(INPUTS_ARR[j], OUTPUTS_ARR[j], i, j);
        }

        deviations[i] = .0;
        for (int j = 0; j < DATA_COUNT; j++) deviations[i] += dataDeviations[i][j];
        deviations[i] = deviations[i] / DATA_COUNT;
        ui->progressBar->setValue((i*100)/ITERATION_COUNT);
        qApp->processEvents();
    }

    ui->progressBar->setValue(100);
    ui->statusLabel->setText("Special Training successful.");
    qApp->processEvents();

    plotSpecPerformance(d0, deviations);
}

void MainWindow::on_btn_plot_released()
{
    plotPerformance();
}

void MainWindow::on_ledt_alpha_textChanged(const QString &arg1)
{
    ALPHA = ui->ledt_alpha->text().toFloat();
}

void MainWindow::on_ledt_iter_textChanged(const QString &arg1)
{
    ITERATION_COUNT = ui->ledt_iter->text().toInt();
}

void MainWindow::on_ledt_lambda_textChanged(const QString &arg1)
{
    LAMBDA = ui->ledt_lambda->text().toFloat();
}

void MainWindow::on_ledt_hid_neuron_count_textChanged(const QString &arg1)
{
    HID_NEURON_COUNT = ui->ledt_hid_neuron_count->text().toInt();
}
