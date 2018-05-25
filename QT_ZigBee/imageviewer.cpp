#include "imageviewer.h"

#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QScrollArea>
#include <QImageReader>

ImageViewer::ImageViewer(QWidget *parent) : QWidget(parent)
{
    image_label = new QLabel();
    image_label->setBackgroundRole(QPalette::Base);
    image_label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    image_label->setScaledContents(true);

    diy_control = new frmMain();

    //scrollArea = new QScrollArea();
    //scrollArea->setBackgroundRole(QPalette::Dark);
    //scrollArea->setWidget(image_label);
    //scrollArea->setVisible(false);

    main_layout = new QVBoxLayout(this);
    main_layout->addWidget(image_label);

    btn_prev = new QPushButton("上一张");
    //QPushButton* btn_zoom_out = new QPushButton("-");
    //QPushButton* btn_zoom_in = new QPushButton("+");
    btn_next = new QPushButton("下一张");
    btn_close = new QPushButton("退  出");

    diy_control->setBtnQss(btn_prev, "#34495E", "#FFFFFF", "#4E6D8C", "#F0F0F0", "#2D3E50", "#B8C6D1");
    diy_control->setBtnQss(btn_next, "#1ABC9C", "#E6F8F5", "#2EE1C1", "#FFFFFF", "#16A086", "#A7EEE6");
    diy_control->setBtnQss(btn_close, "#3498DB", "#FFFFFF", "#5DACE4", "#E5FEFF", "#2483C7", "#A0DAFB");


    QSpacerItem* item_left = new QSpacerItem(75, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem* item_right = new QSpacerItem(75, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

    tool_layout = new QHBoxLayout();
    tool_layout->addSpacerItem(item_left);
    tool_layout->addWidget(btn_prev);
    //tool_layout->addWidget(btn_zoom_out);
    //tool_layout->addWidget(btn_zoom_in);
    tool_layout->addWidget(btn_next);
    tool_layout->addWidget(btn_close);
    tool_layout->addSpacerItem(item_right);

    main_layout->addLayout(tool_layout);

    this->setLayout(main_layout);
    this->setStyleSheet("background-color: rgb(167, 175, 159);");

    //connect(btn_zoom_out, SIGNAL(clicked(bool)), this, SLOT(on_click_btn_zoom_out()));
    //connect(btn_zoom_in, SIGNAL(clicked(bool)), this, SLOT(on_click_btn_zoom_in()));
    connect(btn_prev, &QPushButton::clicked, this, &ImageViewer::on_click_btn_prev);
    connect(btn_next, &QPushButton::clicked, this, &ImageViewer::on_click_btn_next);
    connect(btn_close, &QPushButton::clicked, this, &ImageViewer::on_click_btn_close);
}

bool ImageViewer::loadFile(const QString &fileName)
{
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();

    setImage(newImage);

    return true;
}

void ImageViewer::setImage(const QImage &newImage)
{
    image = newImage;
    image_label->setPixmap(QPixmap::fromImage(image));

    scaleFactor = 1.0;
}

void ImageViewer::on_click_btn_zoom_out()
{
    zoomOut();
}

void ImageViewer::on_click_btn_zoom_in()
{
    zoomIn();
}

void ImageViewer::on_click_btn_prev()
{
    emit signal_prev();
}

void ImageViewer::on_click_btn_next()
{
    emit signal_next();
}

void ImageViewer::on_click_btn_close()
{
    emit signal_close();
}

void ImageViewer::zoomOut()
{
    scaleImage(0.8);
}

void ImageViewer::zoomIn()
{
    scaleImage(1.2);
}

void ImageViewer::scaleImage(double factor)
{
    Q_ASSERT(image_label->pixmap());
    scaleFactor *= factor;
    image_label->resize(scaleFactor * image_label->pixmap()->size());
}
