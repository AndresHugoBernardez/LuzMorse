#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
//#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>



using namespace cv;
using namespace std;



class localizacion {




public:
    int x1, x2, y1, y2,cx,cy,xIzq,xDer,yAbj,yArr;
    int width, height,diametro;
    float radio;
    bool vacio;

    localizacion() {
        x1 = 0;
        x2 = 0;
        y1 = 0;
        y2 = 0;
        xIzq = 0;
        xDer = 0;
        yAbj = 0;
        yArr = 0;
        cx = 0;
        cy = 0;
        diametro = 0;
        radio = 0;
        width = 0;
        height = 0;
        vacio = true; 
    }
    localizacion(int x,int y) {
        x1 = x;
        x2 = x;
        y1 = y;
        y2 = y;
        xIzq = x;
        xDer = x;
        yAbj = y;
        yArr = y;
        cx = x;
        cy = y;
        diametro = 2;
        radio = 1;
        width = 0;
        height = 0;
        vacio = false;
    }

    int getDiametro() {
        

        width = y2 - y1;
        height = x2 - x1;
        if (width < height) diametro = height;
        else diametro = width;

        
        return(diametro);
    }
    float getRadio() {
        radio = (float)getDiametro() / 2;
        return(radio);
    }
    



    void centro() {
        if (x2 > x1)cx = (x2 + x1) / 2;
        else cx = x1;
        if (y2 > y1)cy = (y2 + y1) / 2;
        else cy = y1;

    }
    


    void actualizarValores() {
        getRadio();
        centro();
    }

    float distancia(int x, int y) {

        float xx, yy;
        xx = (float)(x - cx);
        yy = (float)(y - cy);

        return (sqrt((xx*xx + yy*yy)));

    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="x"> posicion x</param>
    /// <param name="y"> posicion y</param>
    /// <returns>devuelve la distancia del centro al cuadrado evitando la raíz cuadrada</returns>
    int distanciaCuadrada(int x, int y) {
       

        int xx, yy;
        xx = (x - cx);
        yy = (y - cy);

        return (xx * xx + yy * yy);
    }

    /// <summary>
    /// 
    /// 
    /// </summary>
    /// <param name="x"> posicion x</param>
    /// <param name="y"> posicion y</param>
    /// <param name="separacion"> cantidad de veces de radio al cuadrado que se separa y</param>
    /// <returns>Si esta fuera de alcance devuelve 1, si esta dentro del alcance devuelve 0</returns>
    int fueraDeAlcance(int x, int y,float separacion=2.25) {
        float distanciaP2,RR;
 

        if (radio < 5) RR = 100;
        else RR = radio*radio*separacion;
        
       

            distanciaP2 = (float)distanciaCuadrada(x, y);
            if ((distanciaP2 - RR) > 0) return(1);
            else return(0);


    }

    int Sector(float separacion=1.5) {
        int RR;
        RR = (int)(radio*separacion);
        xIzq = cx - RR;
        xDer = cx + RR;
        yArr = cy - RR;
        yAbj = cy + RR;

    }




};







int main() {
	Mat A;
	char entrada[100], direccion[15] = "D:/luz1.png", Letra;
    int i, j, k, N, encontrado = 0, actualizar = 0;;
    uchar pixelValue,* rowPtr;

    localizacion localizaciones[30];
    


	A = imread(direccion,  CV_8UC1);

    

	
	namedWindow("hola", WINDOW_FREERATIO);
	//resizeWindow("hola", Size( A.size().width/7, A.size().height/7 ));
	imshow("hola", A);
    Letra = waitKey(1);
	cout << " AS Andy's Softwares" << endl << endl;
	cout << " Luz Morse" << endl << "-------------";

    k = 0;
    N = 0;
    
    for (i = 0; i < A.rows; i++) {

        rowPtr = A.ptr<uchar>(i);
        for (j = 0; j < A.cols; j++) {

            pixelValue = rowPtr[j];

            k = 0;
            if (pixelValue >200 ) {
                encontrado = 0;
                while (k < N && !encontrado) {

                    if(localizaciones[k].fueraDeAlcance(i, j)) k++;
                    else {
                        encontrado = 1;
                        actualizar = 0;
                        if (i < localizaciones[k].x1) {
                            localizaciones[k].x1 = i;
                            actualizar = 1;
                        }
                        else if(i > localizaciones[k].x2) {
                            localizaciones[k].x2 = i;
                            actualizar = 1;
                        }
                        if (j < localizaciones[k].y1) {
                            localizaciones[k].y1 = j;
                            actualizar = 1;
                        }
                        else if(j> localizaciones[k].y2) {
                            localizaciones[k].y2 = j;
                            actualizar = 1;
                        }
                        if (actualizar) localizaciones[k].actualizarValores();

                    }

                }
                
                if (!encontrado) {
                    if (N < 30) { 
                        localizaciones[k] = localizacion(i, j); 
                        N++;
                    }

                }



                }


        }
    }

    for (k = 0; k < N; k++) {
        rectangle(A,
            Rect(Point(localizaciones[k].y1, localizaciones[k].x1),Point(localizaciones[k].y2+1, localizaciones[k].x2+1)),
            Scalar(100),
            1,
            LINE_8,
            0
        );

        cout << "1(" << localizaciones[k].x1 << "|" << localizaciones[k].y1 << ")2(" << localizaciones[k].x2 << "|" << localizaciones[k].y2 << ");;";
  
    }
    cout << "Cantidad: " << N<<endl;
    imshow("hola", A);

	Letra=waitKey(0);
	
	destroyWindow("hola");



    /*
	
    Mat frame;
    //--- INITIALIZE VIDEOCAPTURE
    VideoCapture cap;
    // open the default camera using default API
    // cap.open(0);
    // OR advance usage: select any API backend
    int deviceID = 0;             // 0 = open default camera
    int apiID = cv::CAP_ANY;      // 0 = autodetect default API
    // open selected camera using selected API
    cap.open(deviceID, apiID);
    // check if we succeeded
    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }

    //--- GRAB AND WRITE LOOP
    cout << "Start grabbing" << endl
        << "Press any key to terminate" << endl;
    for (;;)
    {
        // wait for a new frame from camera and store it into 'frame'
        cap.read(frame);
        // check if we succeeded
        if (frame.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }
        // show live and wait for a key with timeout long enough to show images
        imshow("Live", frame);
        if (waitKey(5) >= 0)
            break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
	
    */



	cout << " Gracias por usar este programa. AS" << endl;
	system("pause");



	return(0);
}