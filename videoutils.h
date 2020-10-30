#ifndef VIDEOUTILS_H
#define VIDEOUTILS_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QAbstractVideoSurface>
#include <QVideoFrame>
#include <QMediaPlayer>

class VideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    VideoSurface(QObject *parent = Q_NULLPTR);
    ~VideoSurface();
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const override;
    bool present(const QVideoFrame &frame) override;

signals:
    void frameAvailable(QVideoFrame &frame);
};

class CharVideoViewer: public QFrame
{
    Q_OBJECT

public:
    explicit CharVideoViewer(QWidget *parent = nullptr);
    ~CharVideoViewer();
    void initialize(QUrl url);

signals:
    void videoTerminated();

protected slots:
    void paintEvent(QPaintEvent* e) override;
    void keyPressEvent(QKeyEvent* e) override;

private:
    const QString pixels = "$@MOImwoni*+-. ";
    QVideoFrame frame;
    bool load_complete;
    bool onshown;
    int count;
    QMediaPlayer *mediaPlayer;
    VideoSurface *videoSurface;

private slots:
    void ProcessFrame(QVideoFrame &frame);
};

#endif // VIDEOUTILS_H
