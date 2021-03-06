#include "imageglview.h"

/**
 * @brief ImageGLView::ImageGLView
 */
ImageGLView::ImageGLView(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
    isMouseClicked(false),
    redMax(255),
    redMin(100),
    greenMax(255),
    greenMin(100),
    blueMax(255),
    blueMin(100)
{

}

/**
 * @brief ImageGLView::~ImageGLView
 */
ImageGLView::~ImageGLView()
{

}

/**
 * @brief ImageGLView::abs
 * @param n
 */
int ImageGLView::abs(int n)
{
    if( n < 0 )
        n *= -1;
    return n;
}

/**
 * @brief ImageGLView::slotImageLoad
 * @param image
 */
void ImageGLView::slotImageLoad(QPixmap image)
{
    frameImage = image;
    this->repaint();
}

int dir[4][2] = { {-1, 0},
                  {0, -1}, {0, 1},
                  {1, 0}};

/**
 * @brief ImageGLView::paintEvent
 * @param event
 */
void ImageGLView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    if( frameImage.isNull() )   return;

    QPainter painter;

    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painterWidth  = 180;
    painterHeight = 120;

    painter.fillRect(0, 0, painterWidth, painterHeight, Qt::black);

    QImage image = frameImage.toImage();
    bool check[180][120] = {0};
    bool preCheck[180][120] = {0};
    QRect rectDrawArea(0, 0, painterWidth, painterHeight);

    int len = 86400;    // 180 * 120 * 4
    unsigned char *imageData = image.bits();

    int red, green, blue;
    int i, j, k;
    int n_i, n_j;
    int loc[180] = {0};

    // 카메라 비율때문에 가로 0 ~ 179가 아닌 16 ~ 163 범위 처리

    painter.setPen(QColor(0, 255, 0));

    // filter
    /////////////////////////////////////////
    for(i=1; i<painterHeight; i++)    {
        for(j=16; j<164; j++)    {
            red = imageData[(i*720) + (j*4)];
            green = imageData[(i*720) + (j*4) + 1];
            blue = imageData[(i*720) + (j*4) + 2];

            for(k=0; k<4; k++)  {
                n_i = i + dir[k][0];
                n_j = j + dir[k][1];

                red += imageData[(n_i * 720) + (n_j * 4)];
                green += imageData[(n_i * 720) + (n_j * 4) + 1];
                blue += imageData[(n_i * 720) + (n_j * 4) + 2];
            }

            imageData[(i * 720) + (j * 4)] = red / 5;
            imageData[(i * 720) + (j * 4) + 1] = green / 5;
            imageData[(i * 720) + (j * 4) + 2] = blue / 5;
        }
    }
    /////////////////////////////////////////

    painter.drawImage(rectDrawArea, image);

    for(j=16; j<164; j++)    {
        for(i=painterHeight-1; i>=0; i--)   {
            int t = (i * 720) + (j * 4);

            int blue = imageData[t+0];
            int green = imageData[t+1];
            int red = imageData[t+2];

            if( (red >= redMin) &&
                (green >= greenMin) &&
                (blue >= blueMin) )  {
                // white detected
            }
            else    {
                loc[j] = i;
                if( abs(loc[j-1] - loc[j]) < 5 )
                    loc[j] = loc[j-1];

                painter.drawEllipse(QPoint(j, loc[j]), 1, 1);
                break;
            }
        }
    }


    /*
    painter.drawImage(rectDrawArea, image);

    for(i=len-4; i>=0; i--) {
        int blue = imageData[i+0];
        int green = imageData[i+1];
        int red = imageData[i+2];

        if( (red <= redMax && red >= redMin) &&
            (green <= greenMax && green >= greenMin) &&
            (blue <= blueMax && blue >= blueMin) )
        {
            int t = i/4;
            int x = t%180, y = t/180;
            painter.setPen(QColor(255, 0, 0));
            painter.drawEllipse(QPoint(x, y), 5, 5);
            if( x < 80 )
                qDebug("Right turn    x : %d,  y : %d", t%180, t/180);
            else if( x > 100 )
                qDebug("Left turn     x : %d,  y : %d", t%180, t/180);
            else    {
                if( y > 60 )
                    qDebug("Attack        x : %d,  y : %d", t%180, t/180);
                else
                    qDebug("Forward       x : %d,  y : %d", t%180, t/180);
            }

            break;
        }
    }
    */


    /*
    // detection
    /////////////////////////////////////////
    for(int i=0; i<len; i+=4)   {
        int blue = imageData[i+0];
        int green = imageData[i+1];
        int red = imageData[i+2];

        if( (red <= redMax && red >= redMin) &&
            (green <= greenMax && green >= greenMin) &&
            (blue <= blueMax && blue >= blueMin) )
        {
            check[i/720][(i%720)/4] = true;
        }
    }
    /////////////////////////////////////////


    // 로봇 팽창
    /////////////////////////////////////////
    for(i=1; i<119; i++)    {
        for(j=1; j<179; j++)    {
            if( check[i][j] == false )  {
                for(k=0; k<4; k++)  {
                    n_i = i + dir[k][0];
                    n_j = j + dir[k][1];

                    preCheck[n_i][n_j] = true;
                }
            }
        }
    }

    for(i=0; i<120; i++)    {
        for(j=0; j<180; j++)    {
            if( preCheck[i][j] == true )  {
                preCheck[i][j] = false;
                check[i][j] = false;
            }
        }
    }

    /////////////////////////////////////////


    // 로봇 팽창
    /////////////////////////////////////////
    for(i=1; i<119; i++)    {
        for(j=1; j<179; j++)    {
            if( check[i][j] == false )  {
                for(k=0; k<4; k++)  {
                    n_i = i + dir[k][0];
                    n_j = j + dir[k][1];

                    preCheck[n_i][n_j] = true;
                }
            }
        }
    }

    for(i=0; i<120; i++)    {
        for(j=0; j<180; j++)    {
            if( preCheck[i][j] == true )  {
                preCheck[i][j] = false;
                check[i][j] = false;
            }

            if( check[i][j] == true )   {
                int t = (i * 720) + (j * 4);
                imageData[t] = 0;
                imageData[t+1] = 255;
                imageData[t+2] = 0;
            }
        }
    }
    /////////////////////////////////////////
    */

//    painter.drawImage(rectDrawArea, image);
}

/**
 * @brief ImageGLView::mousePressEvent
 * @param event
 */
void ImageGLView::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)

    isMouseClicked = true;
}

/**
 * @brief ImageGLView::mouseReleaseEvent
 * @param event
 */
void ImageGLView::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)

    isMouseClicked = false;
}

/**
 * @brief ImageGLView::mouseMoveEvent
 * @param event
 */
void ImageGLView::mouseMoveEvent(QMouseEvent *event)
{
    if( isMouseClicked )    {
        mousePosX = event->x();
        mousePosY = event->y();

        QColor maskColor = QColor::fromRgb(frameImage.toImage().pixel(mousePosX, mousePosY));

        int red = maskColor.red();
        int green = maskColor.green();
        int blue = maskColor.blue();

        redMax   = (redMax<red) ? red : redMax;
        redMin   = (redMin>red) ? red : redMin;
        greenMax = (greenMax<green) ? green : greenMax;
        greenMin = (greenMin>green) ? green : greenMin;
        blueMax  = (blueMax<blue) ? blue : blueMax;
        blueMin  = (blueMin>blue) ? blue : blueMin;

    }

    qDebug("%d %d", event->x(), event->y());

    qDebug("%d %d    %d %d    %d %d", redMax, redMin, greenMax, greenMin, blueMax, blueMin);
}
