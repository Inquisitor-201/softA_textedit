#include "videoutils.h"
#include "customedit.h"
#include <QMediaPlayer>
#include <QPainter>

VideoSurface::VideoSurface(QObject *parent)
    : QAbstractVideoSurface(parent)
{
}

VideoSurface::~VideoSurface()
{
}

QList<QVideoFrame::PixelFormat> VideoSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    QList<QVideoFrame::PixelFormat> listPixelFormats;

    listPixelFormats << QVideoFrame::Format_ARGB32
        << QVideoFrame::Format_ARGB32_Premultiplied
        << QVideoFrame::Format_RGB32
        << QVideoFrame::Format_RGB24
        << QVideoFrame::Format_RGB565
        << QVideoFrame::Format_RGB555
        << QVideoFrame::Format_ARGB8565_Premultiplied
        << QVideoFrame::Format_BGRA32
        << QVideoFrame::Format_BGRA32_Premultiplied
        << QVideoFrame::Format_BGR32
        << QVideoFrame::Format_BGR24
        << QVideoFrame::Format_BGR565
        << QVideoFrame::Format_BGR555
        << QVideoFrame::Format_BGRA5658_Premultiplied
        << QVideoFrame::Format_AYUV444
        << QVideoFrame::Format_AYUV444_Premultiplied
        << QVideoFrame::Format_YUV444
        << QVideoFrame::Format_YUV420P
        << QVideoFrame::Format_YV12
        << QVideoFrame::Format_UYVY
        << QVideoFrame::Format_YUYV
        << QVideoFrame::Format_NV12
        << QVideoFrame::Format_NV21
        << QVideoFrame::Format_IMC1
        << QVideoFrame::Format_IMC2
        << QVideoFrame::Format_IMC3
        << QVideoFrame::Format_IMC4
        << QVideoFrame::Format_Y8
        << QVideoFrame::Format_Y16
        << QVideoFrame::Format_Jpeg
        << QVideoFrame::Format_CameraRaw
        << QVideoFrame::Format_AdobeDng;
    //qDebug() << listPixelFormats;
    // Return the formats you will support
    return listPixelFormats;
}

bool VideoSurface::present(const QVideoFrame &frame)
{// Handle the frame and do your processing
    if (frame.isValid())
    {
        QVideoFrame cloneFrame(frame);
        emit frameAvailable(cloneFrame);
        return true;
    }
    return false;
}

CharVideoViewer::CharVideoViewer(QWidget *parent) :
    QFrame(parent)
{
    mediaPlayer = new QMediaPlayer(this);
    videoSurface = new VideoSurface(this);
    mediaPlayer->setVideoOutput(videoSurface);

    connect(videoSurface, SIGNAL(frameAvailable(QVideoFrame &)), this, SLOT(ProcessFrame(QVideoFrame &)));
}

void CharVideoViewer::initialize(QUrl url)
{
    onshown = 0;
    mediaPlayer->setMedia(url);
    mediaPlayer->play();
}

void CharVideoViewer::ProcessFrame(QVideoFrame &f)
{
    if (onshown) return;
    load_complete = 1;
    frame = f;
    if (onshown) return;
    update();
}

void CharVideoViewer::paintEvent(QPaintEvent* e)
{
    if (!load_complete)
        return;
    onshown = 1;

    QFont font;
    font.setPixelSize(10);

    QFontMetrics ft(font);
    int font_w = ft.horizontalAdvance("M");
    int font_h = ft.height() - 2;
    int w = frame.width(),
        h = frame.height();
    int pixw = width() / font_w, pixh = height() / font_h;

    frame.map(QAbstractVideoBuffer::ReadOnly);
    QImage recvImage(frame.bits(), w, h, QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat()));
    QImage res = recvImage.scaled(pixw, pixh, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QPainter painter(this);
    painter.setFont(font);

    for (int i = 0; i < pixh; i++)
    {
        const uchar* line = res.scanLine(i);
        for (int j = 0; j < pixw; j++){
            int b = line[4*j];
            int g = line[4*j+1];
            int r = line[4*j+2];
            int y = 0.2 * r + 0.7 * g + 0.1 * b; //y，亮度
            int index = y * pixels.length() >> 8; //对应哪个pixels
            painter.setPen(QColor(r, g, b));
            painter.drawText(QPoint(j * font_w, i * font_h), pixels.at(index));
//            painter.drawText(QPoint(j * font_w, i * font_h), QString("$"));
        }
    }
    frame.unmap();
    onshown = 0;
}

void CharVideoViewer::keyPressEvent(QKeyEvent* e)
{
    switch (e->key())
    {
        case Qt::Key_Space:
            if (mediaPlayer->state() == QMediaPlayer::PlayingState)
                mediaPlayer->pause();
            else
                mediaPlayer->play();
            break;
        case Qt::Key_Escape:
            mediaPlayer->stop();
            emit videoTerminated();
            break;
        default:
            keyPressEvent(e);
    }
}

CharVideoViewer::~CharVideoViewer()
{

}
